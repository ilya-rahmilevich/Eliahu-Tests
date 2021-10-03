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

 BEGIN_SERIALIZE_MAP(CJDsClipboardStringObject)
	SERIALIZE_NATIVE(_uType)
    SERIALIZE_CSTRING_AS_UNICODE(_sText)
  END_SERIALIZE_MAP()

    void Reset();
private:
	CString _sText; 
};

