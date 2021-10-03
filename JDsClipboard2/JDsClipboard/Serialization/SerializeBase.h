
/**************************************************************************
*   SerializeBase.h                                                       *
*   Header File                                                           *
*   Sabelnikov Andrey N. mail-to: de_kastro@mail.ru                       *
*   25.04.2003 17:19:18                                                   *
*   Description: Make you easy to serialize any compound data objects     *
*                                                                         *
**************************************************************************/

/*
Usage: Decleare some classes inherited from CSerializeBase and insert in it serialize map like this:

	class AAA: public CSerializeBase
	{
	public:
		AAA()
		{}
		virtual ~AAA(){}
		
		BEGIN_SERIALIZE_MAP(AAA)
			SERIALIZE_NATIVE(m_int_23)
			SERIALIZE_NATIVE(m_DWORD_66)
			SERIALIZE_CSTRING_AS_ASCII(m_str_xxx)
			SERIALIZE_CSTRING_AS_UNICODE(m_str_zzzzz)
		END_SERIALIZE_MAP()

		void SetTestVal()
		{
			m_int_23 = 23;
			m_DWORD_66 = 66;
			m_str_xxx = _T("xxx");
			m_str_zzzzz =_T("zzzzz");
		}

	protected:
		CString m_str_xxx;
	private:
		int m_int_23;
		DWORD m_DWORD_66;
		CString m_str_zzzzz;
	};


	class BBB: public CSerializeBase
	{
	public:
		BBB()
		{}
		virtual ~BBB(){}

		BEGIN_SERIALIZE_MAP(BBB)
			SERIALIZE_NATIVE(m_253)
			SERIALIZE_NATIVE(m_DWORD__22)
			SERIALIZE_CSTRING_AS_ASCII(m_str_someText)
			SERIALIZE_CSTRING_AS_UNICODE(m_str_ZZZ)
			SERIALIZE_T(&m_aaa)
		END_SERIALIZE_MAP()


		void SetTestVal()
		{
			m_str_someText = _T("someText");
			m_253 = 253;
			m_DWORD__22 = 22;
			m_str_ZZZ = _T("ZZZ");
			m_bbb.SetTestVal();
		}

	protected:
		CString m_str_someText;
	private:
		int m_253;
		DWORD m_DWORD__22;
		CString m_str_ZZZ;
		AAA m_aaa;
	};

And use it like this:
	
	void Test()
	{
		BBB bbb_1, bbb_2;

		bbb_1.SetTestVal();

		CSerializedDownContainer down_container;
		CSerializedUpContainer upContainer;
		int count = bbb_1.Serialize(&down_container);
		down_container.SerializeAllTo(&upContainer);
		//Here you can do everthing with data in upContainer, for example write to file, or send through network
		//	BYTE* pBuf = upContainer.Get(); //pointer to buffer with data 
		//  UINT bufferLen = upContainer.GetLength();//len of buffer
        //  --------------------------------------------------------- 
		//  use 
		//     upContainer.Set(const BYTE* pBuf, UINT lenght);
		//  to set data from external source
		//When you have the buffer you can unserialize
		bbb_2.UnSerialize(&upContainer);
	}

*/



#pragma once
#include <list>
#include <vector>
#include "CW_MBTranslator.h"


/*****************************************************************************/
/* CSerializedObject - just wrap any in-memory serialized abstract object    */
/*****************************************************************************/
class CSerializedObject
{
public:
	CSerializedObject():m_pObject(NULL),m_ObjLength(0)
	{}

	CSerializedObject(const CSerializedObject& object):m_pObject(NULL),m_ObjLength(0)
	{
		Set(object.Get(), object.GetLength());
	}

	CSerializedObject& operator=(const CSerializedObject& object)
	{
		ReleaseBuffer();
		new(this)CSerializedObject(object);
		return *this;
	}

	virtual ~CSerializedObject()
	{
		ReleaseBuffer();
	}

	void Clear()
	{
		ReleaseBuffer();
	}

	const BYTE* Get()const{return m_pObject;}
	UINT  GetLength()const{return m_ObjLength;}
	bool  Set(const BYTE* pBuf, UINT lenght)
	{
		ReleaseBuffer();
		m_pObject = new BYTE[lenght];
		memcpy(m_pObject, pBuf, lenght);
		m_ObjLength = lenght;
		NotifyPointerChanged();
		return true;
	}
	//------------------------------------------------------------------------
	bool Attach(BYTE* pBuf, UINT lenght)
	{
		ReleaseBuffer();
		m_pObject = pBuf;
		m_ObjLength = lenght;
		NotifyPointerChanged();
		return true;
	}
	
	void Detach()
	{
		m_pObject = NULL;
		m_ObjLength = 0;
		NotifyPointerChanged();
	}
	//------------------------------------------------------------------------

protected:
private:
	virtual void NotifyPointerChanged(){}
	bool ReleaseBuffer()
	{
		if(m_pObject) 
		{
			delete [] m_pObject;
			m_pObject = NULL;
			m_ObjLength = 0;
			NotifyPointerChanged();
		}
		return true;
	}

	BYTE* m_pObject;
	UINT  m_ObjLength;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class CRunningCSerializedObject: public CSerializedObject
{
public:
	CRunningCSerializedObject(){}
	virtual ~CRunningCSerializedObject(){}

	BYTE* GetCurrent(){return m_pCurrent;}
	int GetLenRemaning()
	{
		return (this->GetLength() - (m_pCurrent - this->Get()));
	}
	bool  IncreaseCurrent(int offset)
	{
		if((m_pCurrent+offset)>(this->Get()+this->GetLength()) ) return false;
		else m_pCurrent += offset;
		return true;
	}
protected:
private:
	virtual void NotifyPointerChanged(){m_pCurrent = (BYTE*)this->Get();}
	BYTE* m_pCurrent;
};
/*********************************************************************************************************/

/************************************************************************/
/*                                                                      */
/************************************************************************/
class CSerializedDownContainer
{
public:
	CSerializedDownContainer(){}
	virtual ~CSerializedDownContainer(){Clear();}

	void Clear();
	void PushBack(const CSerializedObject* pObject);
	void PushBack(const BYTE* pBuf, UINT lenght);
	bool SerializeAllTo(CSerializedObject* pObject);
protected:
	
private:
	typedef std::list<CSerializedObject*>::iterator intrenalIterator;
	std::list<CSerializedObject*> m_SerializedObjects;
};


/************************************************************************/
/*                                                                      */
/************************************************************************/
class CSerializedUpContainer: public CRunningCSerializedObject
{
public:
	CSerializedUpContainer(){}
	virtual ~CSerializedUpContainer(){}

protected:
private:
};



/************************************************************************/
/* ISerialize - serialisation interface class                            */
/************************************************************************/
class ISerialize
{
public:
	ISerialize(){}
	virtual ~ISerialize(){}

	virtual int Serialize(CSerializedDownContainer* pObjectToSerialize)=0;//must pack object to buffer
	virtual int UnSerialize(CSerializedUpContainer* pObjectToSerialize)=0;//must read object from buffer

protected:
private:
};

/************************************************************************/
/* Serialize map declarations                                                                     */
/************************************************************************/
#define BEGIN_SERIALIZE_MAP(theClass) \
public: \
	virtual int SerializeMap(CSerializedDownContainer* pObjectToSerializeDown, CSerializedUpContainer* pObjectToSerializeUp, bool bSerialze) \
		{ \
			int count = 0; \
			int	res = 0; \
			if(bSerialze) OnPreSerialize(); else OnPreUnSerialize();			

#define SERIALIZE_NATIVE(varialble) \
	if(bSerialze) \
		res = SerializeNative(varialble, pObjectToSerializeDown); \
	else \
		res = UnSerializeNative(varialble, pObjectToSerializeUp); \
	if(0>res) return res; \
	else count+=res;

#define SERIALIZE_CSTRING_AS_ASCII(varialble) \
	if(bSerialze) \
		res = SerializeStringAsAscii(varialble, pObjectToSerializeDown); \
	else \
		res	= UnSerializeStringAsAscii(varialble, pObjectToSerializeUp); \
	if(0>res) return res; \
	else count+=res;

#define SERIALIZE_CSTRING_AS_UNICODE(varialble) \
	if(bSerialze) \
		res = SerializeStringAsUnicode(varialble, pObjectToSerializeDown); \
	else \
		res = UnSerializeStringAsUnicode(varialble, pObjectToSerializeUp); \
	if(0>res) {return res;} \
	else {count+=res;}

#define SERIALIZE_T(varialble) \
	if(bSerialze) \
		res = SerializeT(varialble, pObjectToSerializeDown); \
	else \
		res = UnSerializeT(varialble, pObjectToSerializeUp); \
	if(0>res) return res; \
	else count+=res;

#define SERIALIZE_BASE() \
	if(bSerialze) \
		res = SerializeBase(pObjectToSerializeDown); \
	else \
		res = UnSerializeBase(pObjectToSerializeUp); \
	if(0>res) return res; \
	else count+=res;

#define SERIALIZE_IMAGE() \
	if(bSerialze) \
		res = SerializeImage(pObjectToSerializeDown); \
	else \
		res = UnSerializeImage(pObjectToSerializeUp); \
	if(0>res) return res; \
	else count+=res;

#define END_SERIALIZE_MAP() \
	if(bSerialze) OnEndSerialize(); else OnEndUnSerialize(); \
	return count;}

/************************************************************************/
/*  CSerializeBase -                                                    */
/************************************************************************/
class CSerializeBase: public ISerialize
{
public:
	CSerializeBase(){}
	virtual ~CSerializeBase(){}

	BEGIN_SERIALIZE_MAP(CSerializeBase)
	END_SERIALIZE_MAP()

	virtual int Serialize(CSerializedDownContainer* pObjectToSerialize);  //must pack object to buffer
	virtual int UnSerialize(CSerializedUpContainer* pObjectToSerialize);//must read object from buffer

protected:
	template<class Native>
	int SerializeNative(Native nativeData, CSerializedDownContainer* pObjectToSerialize);
	template<class Native>
	int UnSerializeNative(Native &nativeData, CSerializedUpContainer* pObjectToSerialize);
	int SerializeStringAsAscii(CString str, CSerializedDownContainer* pObjectToSerialize);
	int UnSerializeStringAsAscii(CString &str, CSerializedUpContainer* pObjectToSerialize);
	int SerializeStringAsUnicode(CString str, CSerializedDownContainer* pObjectToSerialize);
	int UnSerializeStringAsUnicode(CString &str, CSerializedUpContainer* pObjectToSerialize);
	int SerializeT(ISerialize* pObject, CSerializedDownContainer* pObjectToSerialize);
	int UnSerializeT(ISerialize* pObject, CSerializedUpContainer* pObjectToSerialize);
	int SerializeImage(CSerializedDownContainer* pObjectToSerialize) {}
	int UnSerializeImage(CSerializedUpContainer* pObjectToSerialize) {}
	//virtual call-backs
	virtual void OnPreSerialize(){}
	virtual void OnEndSerialize(){}
	virtual void OnPreUnSerialize(){}
	virtual void OnEndUnSerialize(){}
private:

};

template<class Native>
int CSerializeBase::SerializeNative(Native nativeData, CSerializedDownContainer* pObjectToSerialize)
{
	pObjectToSerialize->PushBack((BYTE*)&nativeData, sizeof(nativeData));
	return sizeof(nativeData);
}
//-------------------------------------------------------------------------------------------------------------------
template<class Native>
int CSerializeBase::UnSerializeNative(Native &nativeData, CSerializedUpContainer* pObjectToSerialize)
{
	if(pObjectToSerialize->GetLenRemaning() >= sizeof(nativeData)) 
	{
		memcpy(&nativeData, pObjectToSerialize->GetCurrent(), sizeof(nativeData));
		pObjectToSerialize->IncreaseCurrent(sizeof(nativeData));
		return sizeof(nativeData);

	}else
		return -1;
}
//-------------------------------------------------------------------------------------------------------------------
//for std::list<> std::vector<>
/************************************************************************/
/*                                                                      */
/************************************************************************/
template<class T>
class CStdContainerInTSerializeWrapp: public T, 
	                                  public ISerialize
{
public:
	CStdContainerInTSerializeWrapp(){}
	~CStdContainerInTSerializeWrapp(){}

	virtual int Serialize(CSerializedDownContainer* pObjectToSerialize)  //must pack object to buffer
	{
		UINT size = T::size();
		pObjectToSerialize->PushBack((BYTE*)&size, sizeof(size));
		int count = sizeof(size);
		int res;
		for(T::iterator it = T::begin(); it != T::end(); it++)
		{
			if(0 > (res = it->Serialize(pObjectToSerialize)))
				return res;
			else 
				count += res;
		}
		return count;
	}

	virtual int UnSerialize(CSerializedUpContainer* pObjectToSerialize) //must read object from buffer
	{
		T::clear();
		UINT size = 0;
		if(pObjectToSerialize->GetLenRemaning()< sizeof(size)) return -1;
		memcpy(&size, pObjectToSerialize->GetCurrent(), sizeof(size));
		if(!pObjectToSerialize->IncreaseCurrent(sizeof(size))) return -1;
		T::iterator it;
		int count = 0;
		int res;
		for(UINT i = 0; i < size ; i++)
		{
			T::value_type val;
			T::push_back(val);
			it = --T::end();
			if(0 >  (res = it->UnSerialize(pObjectToSerialize)))
				return res;
			else
				count += res;
		}
		return count;
	}
};
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//Some helper implementations
/************************************************************************/
/* CStdContainerInNativeSerializeWrapp -                                                                      */
/************************************************************************/
template<class T>
class CStdContainerInNativeSerializeWrapp: public T, 
										   public CSerializeBase
{
public:
	CStdContainerInNativeSerializeWrapp(){}
	~CStdContainerInNativeSerializeWrapp(){}

	virtual int Serialize(CSerializedDownContainer* pObjectToSerialize)  //must pack object to buffer
	{
		UINT size = T::size();
		pObjectToSerialize->PushBack((BYTE*)&size, sizeof(size));
		int count = sizeof(size);
		int res;
		for(T::iterator it = T::begin(); it != T::end(); it++)
		{
			if(0 > (res = CSerializeBase::SerializeNative((*it), pObjectToSerialize)))
				return res;
			else 
				count += res;
		}
		return count;
	}

	virtual int UnSerialize(CSerializedUpContainer* pObjectToSerialize) //must read object from buffer
	{
		T::clear();
		UINT size = 0;
		if(pObjectToSerialize->GetLenRemaning()< sizeof(size)) return -1;
		memcpy(&size, pObjectToSerialize->GetCurrent(), sizeof(size));
		if(!pObjectToSerialize->IncreaseCurrent(sizeof(size))) return -1;
		T::iterator it;
		int count = 0;
		int res;
		for(UINT i = 0; i < size ; i++)
		{
			T::value_type val;
			T::push_back(val);
			it = --T::end();
			if(0 >  (res = CSerializeBase::UnSerializeNative((*it), pObjectToSerialize)))
				return res;
			else
				count += res;
		}
		return count;
	}
};


/************************************************************************/
/*                                                                      */
/************************************************************************/
template<class T>
class list_in_T_serializable: public CStdContainerInTSerializeWrapp<std::list<T> >
{
public:
	list_in_T_serializable(){}
	~list_in_T_serializable(){}
protected:
private:
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
template<class T>
class list_in_native_serializable: public CStdContainerInNativeSerializeWrapp<std::list<T> >
{
public:
	list_in_native_serializable(){}
	~list_in_native_serializable(){}
protected:
private:
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
template<class T>
class vector_in_T_serializable: public CStdContainerInTSerializeWrapp<std::vector<T> >
{
public:
	vector_in_T_serializable(){}
	~vector_in_T_serializable(){}

	bool IsExist(T* pObj)
	{
		std::vector<T>::iterator it = begin();
		for(;it != end(); it++)
			if(&(*it) == pObj) return true;
		return false;
	}

	bool IsExist(T Obj)
	{
		std::vector<T>::iterator it = begin();
		for(;it != end(); it++)
			if((*it) == Obj) return true;
		return false;
	}

	bool Remove(T* pObj)
	{
		std::vector<T>::iterator it = begin();
		for(;it != end(); it++)
			if(&(*it) == pObj) 
			{
				erase(it);
				return true;
			}
		return false;

	}
protected:
private:
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
template<class T>
class vector_in_native_serializable: public CStdContainerInNativeSerializeWrapp<std::vector<T> >
{
public:
	vector_in_native_serializable(){}
	~vector_in_native_serializable(){}
protected:
private:
};


/************************************************************************/
/*                                                                      */
/************************************************************************/
class CSerATString: public CString
{
public:
	CSerATString(const CString& stringSrc):CString(stringSrc){Init();}
	CSerATString(TCHAR ch, int nRepeat = 1):CString(ch, nRepeat){Init();}
	CSerATString(LPCSTR lpsz):CString(lpsz){Init();}
	CSerATString(LPCWSTR lpsz):CString(lpsz){Init();}
	CSerATString(LPCSTR lpch, int nLength):CString(lpch, nLength){Init();}
	CSerATString(LPCWSTR lpch, int nLength):CString(lpch, nLength){Init();}
	CSerATString(const unsigned char* psz):CString(psz){Init();}
	CSerATString(void){}
	virtual ~CSerATString(void){}

	//-------------------------------------------------------------------------------------------
	int Serialize(CSerializedDownContainer* pObjectToSerialize)
	{
		const char* pChar = NULL;
#ifdef UNICODE
		CW_MBTranslator tr;
		pChar = tr.TranslateFrom_T_TO_A(this->operator PCXSTR());
#else
		pChar = this->operator PCXSTR();
#endif
		UINT len = (strlen(pChar)+1)*sizeof(char);
		pObjectToSerialize->PushBack((BYTE*)pChar, len);
		return len;
	}
	//-------------------------------------------------------------------------------------------
	int UnSerialize(CSerializedUpContainer* pObjectToSerialize)
	{
#ifdef UNICODE
		CW_MBTranslator tr;
		this->operator =(tr.TranslateTO_T((char*)pObjectToSerialize->GetCurrent()));
#else
		this->operator =((char*)pObjectToSerialize->GetCurrent());
#endif
		UINT len = (GetLength()+1)*sizeof(char);
		if(!pObjectToSerialize->IncreaseCurrent(len))
			return -1;
		return len;
	}

	//-------------------------------------------------------------------------------------------
private:
	void Init(){}

}; 
/************************************************************************/
/*                                                                      */
/************************************************************************/
class CSerUTString: public CString
{
public:
	CSerUTString(const CString& stringSrc):CString(stringSrc){Init();}
	CSerUTString(TCHAR ch, int nRepeat = 1):CString(ch, nRepeat){Init();}
	CSerUTString(LPCSTR lpsz):CString(lpsz){Init();}
	CSerUTString(LPCWSTR lpsz):CString(lpsz){Init();}
	CSerUTString(LPCSTR lpch, int nLength):CString(lpch, nLength){Init();}
	CSerUTString(LPCWSTR lpch, int nLength):CString(lpch, nLength){Init();}
	CSerUTString(const unsigned char* psz):CString(psz){Init();}
	CSerUTString(void){}
	virtual ~CSerUTString(void){}

	//-------------------------------------------------------------------------------------------
	int Serialize(CSerializedDownContainer* pObjectToSerialize)
	{
		const wchar_t* pChar = NULL;
#ifndef UNICODE
		CW_MBTranslator tr;
		pChar = tr.TranslateFrom_T_TO_W(this->operator PCXSTR());
#else
		pChar = this->operator PCXSTR();
#endif
		UINT len = (wcslen(pChar)+1)*sizeof(wchar_t);
		pObjectToSerialize->PushBack((BYTE*)pChar, len);
		return len;
	}
	//-------------------------------------------------------------------------------------------
	int UnSerialize(CSerializedUpContainer* pObjectToSerialize)
	{
#ifndef UNICODE
		CW_MBTranslator tr;
		this->operator =(tr.TranslateTO_T((wchar_t*)pObjectToSerialize->GetCurrent()));
#else
		this->operator =((wchar_t*)pObjectToSerialize->GetCurrent());
#endif
		UINT len = (GetLength()+1)*sizeof(wchar_t);
		if(!pObjectToSerialize->IncreaseCurrent(len))
			return -1;
		return len;
	}
	//-------------------------------------------------------------------------------------------
private:
	void Init(){}

}; 
