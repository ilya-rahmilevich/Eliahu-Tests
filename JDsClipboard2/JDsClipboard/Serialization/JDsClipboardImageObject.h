#pragma once
#include "JDsClipboardObject.h"
#include "..\JDsClipboardException.h"

class CJDsClipboardImageObject : public CJDsClipboardObject
{
public:
	CJDsClipboardImageObject(void);
	CJDsClipboardImageObject(UINT type);
	virtual ~CJDsClipboardImageObject(void);

	BOOL CopyBuffer(LPVOID pBuffer, long length);

	int SerializeImage(CSerializedDownContainer* pObjectToSerialize)  throw(CJDsClipboardException);
	int UnSerializeImage(CSerializedUpContainer* pObjectToSerialize)  throw(CJDsClipboardException);

	LPVOID GetBuffer() { return _pBuffer; }

	long GetLength() { return _length; }
	void SetLength(long iL) { _length = iL; }

	BEGIN_SERIALIZE_MAP(A)
		SERIALIZE_IMAGE()
	END_SERIALIZE_MAP()

private:
	CComHeapPtr<BYTE> _pBuffer;
	long   _length;
};

