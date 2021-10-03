#include "StdAfx.h"
#include "JDsClipboardObject.h"


CJDsClipboardObject::CJDsClipboardObject(void)
{
	_uType		= -1;
}

CJDsClipboardObject::CJDsClipboardObject(UINT type)
{
	_uType = type;
}


CJDsClipboardObject::~CJDsClipboardObject(void)
{
}

