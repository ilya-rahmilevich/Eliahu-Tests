#pragma once

#include "..\JDsClipArchive\Archive.h"

class CJDsClipboardObject
{
public:
	CJDsClipboardObject(void);
	CJDsClipboardObject(UINT type);
	virtual ~CJDsClipboardObject(void);

	void SetType(UINT type) { _uType = type; }
	UINT GetType() { return _uType; }

	virtual void Serialize(CXArchive& ar, BOOL fSaveMemCopy = FALSE);
protected:
	UINT _uType;
};


