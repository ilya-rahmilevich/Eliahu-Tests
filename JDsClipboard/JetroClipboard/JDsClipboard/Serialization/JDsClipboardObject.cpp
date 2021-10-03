#include "StdAfx.h"
#include "JDsClipboardObject.h"


CJDsClipboardObject::CJDsClipboardObject(void)
{
}

CJDsClipboardObject::CJDsClipboardObject(UINT type)
{
	_uType = type;
}


CJDsClipboardObject::~CJDsClipboardObject(void)
{
}


void CJDsClipboardObject::Serialize(CXArchive& ar, BOOL fSaveMemCopy)
{
	if (ar.IsStoring())
	{
		ar << _uType;
	}
	else
	{
		ar >> _uType;
	}
}