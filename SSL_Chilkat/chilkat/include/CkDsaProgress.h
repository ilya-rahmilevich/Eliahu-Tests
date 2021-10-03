// CkDsaProgress.h: interface for the CkDsaProgress class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CKDSAPROGRESS_H
#define _CKDSAPROGRESS_H



#ifndef __sun__
#pragma pack (push, 8)
#endif
 

class CkDsaProgress  
{
    public:
	CkDsaProgress() { }
	virtual ~CkDsaProgress() { }

	// Called periodically to check to see if the method call should be aborted.
	virtual void AbortCheck(bool *abort) { }

};
#ifndef __sun__
#pragma pack (pop)
#endif


#endif
