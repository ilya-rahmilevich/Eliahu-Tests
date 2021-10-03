#include "StdAfx.h"
#include "JDsClipboardStringObject.h"
#include "..\AutoResource.h"
#include "..\JDsClipboardException.h"

CJDsClipboardStringObject::CJDsClipboardStringObject(void)
	:CJDsClipboardObject(CF_UNICODETEXT)
{
	_sText = _T("");
}

CJDsClipboardStringObject::CJDsClipboardStringObject(UINT type)
	:CJDsClipboardObject(type)
{
	_sText = _T("");
}

CJDsClipboardStringObject::~CJDsClipboardStringObject(void)
{
}

void CJDsClipboardStringObject::Reset()
{
	CJDsClipboardObject::Reset();

	_sText = _T("");
}