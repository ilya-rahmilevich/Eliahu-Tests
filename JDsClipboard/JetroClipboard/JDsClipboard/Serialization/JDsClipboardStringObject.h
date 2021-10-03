#pragma once
#include "JDsClipboardObject.h"
class CJDsClipboardStringObject : public CJDsClipboardObject
{
public:
	CJDsClipboardStringObject(void);
	CJDsClipboardStringObject(UINT type);
	virtual ~CJDsClipboardStringObject(void);

	void SetText(CString &sT) { _sText = sT; }
	CString GetText() { return _sText; }

	void Serialize(CXArchive& ar, BOOL fSaveMemCopy = FALSE);
private:
	CString _sText; 
};

