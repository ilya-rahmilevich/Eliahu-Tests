
/**************************************************************************
*   SerializeBase.cpp                                                     *
*   Implementation file                                                   *
*   Sabelnikov Andrey N. mail-to: de_kastro@mail.ru                       *
*   25.04.2003 17:19:18                                                   *
*   Description: Make you easy to serialize any compound data objects     *
*                                                                         *
**************************************************************************/

#include "stdafx.h"
#include "SerializeBase.h"

//-------------------------------------------------------------------------------------------------------------------
void CSerializedDownContainer::Clear()
{
	for(intrenalIterator it = m_SerializedObjects.begin(); it != m_SerializedObjects.end(); it++)
		delete (*it);
	m_SerializedObjects.clear();
}
//-------------------------------------------------------------------------------------------------------------------
void CSerializedDownContainer::PushBack(const CSerializedObject* pObject)
{
	ATLASSERT(pObject);
	PushBack(pObject->Get(), pObject->GetLength());
}
//-------------------------------------------------------------------------------------------------------------------
void CSerializedDownContainer::PushBack(const BYTE* pBuf, UINT lenght)
{
	CSerializedObject* pSerializedObject = new CSerializedObject;
	pSerializedObject->Set(pBuf, lenght);
	m_SerializedObjects.push_back(pSerializedObject);
}
//-------------------------------------------------------------------------------------------------------------------
bool CSerializedDownContainer::SerializeAllTo(CSerializedObject* pObject)
{
	UINT size_summary = 0;
	for(intrenalIterator it = m_SerializedObjects.begin(); it != m_SerializedObjects.end(); it++)
		size_summary += (*it)->GetLength();
	if(!size_summary) return false;
	BYTE* pNewBuf = new BYTE[size_summary];
	ATLASSERT(pNewBuf);
	BYTE* pRunning = pNewBuf;
	for(intrenalIterator it = m_SerializedObjects.begin(); it != m_SerializedObjects.end(); it++)
	{
		memcpy(pRunning, (*it)->Get(), (*it)->GetLength());
		pRunning += (*it)->GetLength();
	}
	pObject->Attach(pNewBuf, size_summary);
	return true;
}
//-------------------------------------------------------------------------------------------------------------------
/********************************************************************************************************************/
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::Serialize(CSerializedDownContainer* pObjectToSerialize)
{
	return SerializeMap(pObjectToSerialize, NULL, true);
}
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::UnSerialize(CSerializedUpContainer* pObjectToSerialize)
{
	return SerializeMap(NULL, pObjectToSerialize, false);
}
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::SerializeStringAsAscii(CString str, CSerializedDownContainer* pObjectToSerialize)
{
	const char* pChar = NULL;
#ifdef UNICODE
	CW_MBTranslator tr;
	pChar = tr.TranslateFrom_T_TO_A(str);
#else
	pChar = str;
#endif
	UINT len = (strlen(pChar)+1)*sizeof(char);
	pObjectToSerialize->PushBack((BYTE*)pChar, len);
	return len;
}
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::UnSerializeStringAsAscii(CString &str, CSerializedUpContainer* pObjectToSerialize)
{
#ifdef UNICODE
	CW_MBTranslator tr;
	str = tr.TranslateTO_T((char*)pObjectToSerialize->GetCurrent());
#else
	str = (char*)pObjectToSerialize->GetCurrent();
#endif
	UINT len = (str.GetLength()+1)*sizeof(char);
	if(!pObjectToSerialize->IncreaseCurrent(len))
		return -1;
	return len;
}
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::SerializeStringAsUnicode(CString str, CSerializedDownContainer* pObjectToSerialize)
{
	const wchar_t* pChar = NULL;
#ifndef UNICODE
	CW_MBTranslator tr;
	pChar = tr.TranslateFrom_T_TO_W(str);
#else
	pChar = str;
#endif
	UINT len = (wcslen(pChar)+1)*sizeof(wchar_t);
	pObjectToSerialize->PushBack((BYTE*)pChar, len);
	return len;
}
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::UnSerializeStringAsUnicode(CString &str, CSerializedUpContainer* pObjectToSerialize)
{
#ifndef UNICODE
	CW_MBTranslator tr;
	str = tr.TranslateTO_T((wchar_t*)pObjectToSerialize->GetCurrent());
#else
	str = (wchar_t*)pObjectToSerialize->GetCurrent();
#endif
	UINT len = (str.GetLength()+1)*sizeof(wchar_t);
	if(!pObjectToSerialize->IncreaseCurrent(len))
		return -1;
	return len;
}
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::SerializeT(ISerialize* pObject, CSerializedDownContainer* pObjectToSerialize)
{
	return pObject->Serialize(pObjectToSerialize);
}
//-------------------------------------------------------------------------------------------------------------------
int CSerializeBase::UnSerializeT(ISerialize* pObject, CSerializedUpContainer* pObjectToSerialize)
{
	return pObject->UnSerialize(pObjectToSerialize);
}
//-------------------------------------------------------------------------------------------------------------------

