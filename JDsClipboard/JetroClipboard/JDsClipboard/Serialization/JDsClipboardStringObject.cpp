#include "StdAfx.h"
#include "JDsClipboardStringObject.h"

CJDsClipboardStringObject::CJDsClipboardStringObject(void)
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

void CJDsClipboardStringObject::Serialize(CXArchive& ar, BOOL fSaveMemCopy)
{
	CJDsClipboardObject::Serialize(ar, fSaveMemCopy);

	if (ar.IsStoring())
	{
		ar.WriteString(_sText);
	}
	else
	{
		ar.ReadString(_sText);
	}
}
