#include "StdAfx.h"
#include "JDsClipboardImageObject.h"
#include "..\AutoResource.h"
#include "..\JDsClipboard.h"

CJDsClipboardImageObject::CJDsClipboardImageObject(void)
	:CJDsClipboardObject(CF_BITMAP)
{
	_length		= 0;
}

CJDsClipboardImageObject::CJDsClipboardImageObject(UINT type)
		:CJDsClipboardObject(type)
{
	_length	  = 0;
}

CJDsClipboardImageObject::~CJDsClipboardImageObject(void)
{
}


int CJDsClipboardImageObject::SerializeImage(CSerializedDownContainer* pObjectToSerialize)
{
	BOOL bResult = FALSE;

	if ( _pBuffer == NULL )
		throw CJDsClipboardException (CJDsClipboardException::enSerEmptyBuffer);

	// copy the picture out to the memory buffer
	if ( _length > 0)
	{
		pObjectToSerialize->PushBack((BYTE*)_pBuffer.m_pData, _length);
	}

//	if ( _pBuffer != NULL  )
//		_pBuffer.Free();

	return _length;
}


int CJDsClipboardImageObject::UnSerializeImage(CSerializedUpContainer* pObjectToSerialize)
{
	BYTE *pData = (BYTE *)pObjectToSerialize->GetCurrent();

	UINT size = 0;
	if(pObjectToSerialize->GetLenRemaning()< sizeof(size)) 
		throw CJDsClipboardException (CJDsClipboardException::enSerAbnormalLength);

	memcpy(&size, pObjectToSerialize->GetCurrent(), sizeof(size));

	if(!pObjectToSerialize->IncreaseCurrent(sizeof(size))) 
		throw CJDsClipboardException (CJDsClipboardException::enSerIncreaseCurrFailed);

	DWORD dwBytesToWrite = pObjectToSerialize->GetLength();

	_pBuffer.Reallocate(dwBytesToWrite);
	
	::CopyMemory( _pBuffer, pData, dwBytesToWrite );

	_length = dwBytesToWrite;

	return _length;
}


BOOL CJDsClipboardImageObject::CopyBuffer(LPVOID pData, long length)
{
	// test
	try
	{
		CString sTextMD5;
		CreateMD5HashCode((BYTE *)pData, length, sTextMD5);

		_pBuffer.Reallocate(length);

		::CopyMemory( _pBuffer, pData, length );

		_length = length;


	}
	catch(CJDsClipboardException& Ex)
	{
		ATLTRACE(Ex.GetErrorDesc().GetString());
	}
	catch(...)
	{
		ATLTRACE(_T("Unknown hash error"));
	}

	return TRUE;
}


