#ifndef _CkXmpW_H
#define _CkXmpW_H
#pragma once

#include "CkWideCharBase.h"
#include "CkString.h"
#include "CkXmp.h"

class CkXmlW;



#ifndef __sun__
#pragma pack (push, 8) 
#endif


class CkXmpW : public CkWideCharBase
{
    private:
	CkXmp *m_impl;

        // Disallow assignment or copying this object.
	CkXmpW(const CkXmpW &) { }
	CkXmpW &operator=(const CkXmpW &) { return *this; }


    public:
	CkXmpW()
	    {
	    m_impl = new CkXmp;
	    m_impl->put_Utf8(true);
	    }
	CkXmpW(CkXmp *p) : m_impl(p)
	    {
	    if (!m_impl) m_impl = new CkXmp;
	    m_impl->put_Utf8(true);
	    }
	void dispose(void) { m_impl->dispose();  }
	virtual ~CkXmpW() { delete m_impl; m_impl = 0; }

void get_DebugLogFilePath(CkString &propVal);
void put_DebugLogFilePath(const wchar_t *w1);
void LastErrorHtml(CkString &propVal);
void LastErrorText(CkString &propVal);
void LastErrorXml(CkString &propVal);
long get_NumEmbedded(void);
bool get_StructInnerDescrip(void);
void put_StructInnerDescrip(bool propVal);
bool get_VerboseLogging(void);
void put_VerboseLogging(bool propVal);
void get_Version(CkString &propVal);
bool AddArray(CkXml &xml,const wchar_t *w1,const wchar_t *w2,CkStringArray &values);
void AddNsMapping(const wchar_t *w1,const wchar_t *w2);
bool AddSimpleDate(CkXml &xml,const wchar_t *w1,SYSTEMTIME &sysTime);
bool AddSimpleInt(CkXml &xml,const wchar_t *w1,int propVal);
bool AddSimpleStr(CkXml &xml,const wchar_t *w1,const wchar_t *w2);
bool AddStructProp(CkXml &xml,const wchar_t *w1,const wchar_t *w2,const wchar_t *w3);
bool Append(CkXml &xml);
bool DateToString(SYSTEMTIME &sysTime,CkString &outStr);
bool GetArray(CkXml &xml,const wchar_t *w1,CkStringArray &sa);
CkXmlW *GetEmbedded(int index);
CkXmlW *GetProperty(CkXml &xml,const wchar_t *w1);
bool GetSimpleDate(CkXml &xml,const wchar_t *w1,SYSTEMTIME &outSysTime);
int GetSimpleInt(CkXml &xml,const wchar_t *w1);
bool GetSimpleStr(CkXml &xml,const wchar_t *w1,CkString &outStr);
bool GetStructPropNames(CkXml &xml,const wchar_t *w1,CkStringArray &sa);
bool GetStructValue(CkXml &xml,const wchar_t *w1,const wchar_t *w2,CkString &outStr);
bool LoadAppFile(const wchar_t *w1);
bool LoadFromBuffer(CkByteData &byteData,const wchar_t *w1);
void NewXmp(CkXml &xmlOut);
void RemoveAllEmbedded(void);
bool RemoveArray(CkXml &xml,const wchar_t *w1);
void RemoveEmbedded(int index);
void RemoveNsMapping(const wchar_t *w1);
bool RemoveSimple(CkXml &xml,const wchar_t *w1);
bool RemoveStruct(CkXml &xml,const wchar_t *w1);
bool RemoveStructProp(CkXml &xml,const wchar_t *w1,const wchar_t *w2);
bool SaveAppFile(const wchar_t *w1);
bool SaveLastError(const wchar_t *w1);
bool SaveToBuffer(CkByteData &byteData);
bool StringToDate(const wchar_t *w1,SYSTEMTIME &outSysTime);
bool UnlockComponent(const wchar_t *w1);
const wchar_t *dateToString(SYSTEMTIME &sysTime);
const wchar_t *debugLogFilePath(void);
const wchar_t *getSimpleStr(CkXml &xml,const wchar_t *w1);
const wchar_t *getStructValue(CkXml &xml,const wchar_t *w1,const wchar_t *w2);
const wchar_t *lastErrorHtml(void);
const wchar_t *lastErrorText(void);
const wchar_t *lastErrorXml(void);
const wchar_t *simpleStr(CkXml &xml,const wchar_t *w1);
const wchar_t *structValue(CkXml &xml,const wchar_t *w1,const wchar_t *w2);
const wchar_t *version(void);

};
#endif
