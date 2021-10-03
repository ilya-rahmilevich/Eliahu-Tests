#pragma once

#include "SerializeBase.h"

class CJDsClipboardObject : public CSerializeBase
{
public:
	CJDsClipboardObject(void);
	CJDsClipboardObject(UINT type);
	virtual ~CJDsClipboardObject(void);

 BEGIN_SERIALIZE_MAP(CJDsClipboardObject)
	SERIALIZE_NATIVE(_uType)
  END_SERIALIZE_MAP()

  virtual void Reset() { _uType = -1; }
protected:
	UINT		_uType;
};


