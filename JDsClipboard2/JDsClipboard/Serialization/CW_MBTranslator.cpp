
/**************************************************************************
*   CW_MBTranslator.cpp                                                   *
*   Implementation file                                                   *
*   Sabelnikov Andrey N. mail-to: de_kastro@mail.ru                       *
*   25.04.2003 17:19:18                                                   *
*   Description: Helper class to translate strings from ascii             *
*                to wide and back                                         *
**************************************************************************/

#include "stdafx.h"
#include "CW_MBTranslator.h"


CW_MBTranslator::CW_MBTranslator()
{
}
//------------------------------------------------------------------------------------
CW_MBTranslator::~CW_MBTranslator()
{
}
//------------------------------------------------------------------------------------
const TCHAR* CW_MBTranslator::TranslateTO_T(const wchar_t* str)
{
	if(!str) return NULL;
	size_t size = wcslen(str);
	if(0 >= size) return _T("");
#ifdef UNICODE
	m_TString = str;
#else
	TCHAR* pBuf = new TCHAR[size+1];
	BOOL defaultChUsed = FALSE;
	//wcstombs(pBuf, str, size+1);
	WideCharToMultiByte(CP_ACP, NULL, str, -1, pBuf, (int)size+1, NULL, &defaultChUsed);
	m_TString = pBuf;
	delete pBuf;
#endif
	
	return m_TString.c_str();
}

//------------------------------------------------------------------------------------
const TCHAR* CW_MBTranslator::TranslateTO_T(const char* str)
{
	if(!str) return NULL;
	size_t size = strlen(str);
	if(0 >= size) return _T("");
#ifdef UNICODE
	//TCHAR* pBuf = new TCHAR[size+1];
	//mbstowcs(pBuf, str, size+1);
	//MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, str, -1, pBuf, size+1);
	m_TString = std::wstring(str, str + strlen(str));
	//delete pBuf;
#else
	m_TString = str;
#endif	

	return m_TString.c_str();
}
//------------------------------------------------------------------------------------
const wchar_t* CW_MBTranslator::TranslateFrom_T_TO_W(const TCHAR* str)
{
	if(!str) return NULL;
	size_t size = _tcslen(str);
	if(0 >= size) return L"";
#ifdef UNICODE
	m_WString = str;
#else
	wchar_t* pBuf = new wchar_t[size+1];
	//mbstowcs(pBuf, str, size+1);
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, str, -1, pBuf, (int)size+1);
	m_WString = pBuf;
	delete pBuf;
#endif
	return m_WString.c_str();
}
//------------------------------------------------------------------------------------
const  char* CW_MBTranslator::TranslateFrom_T_TO_A(const TCHAR* str)
{
	if(!str) return NULL;
	size_t size = _tcslen(str);
	if(0 >= size) return "";
#ifdef UNICODE
	char* pBuf = new char[size+1];
	BOOL defaultChUsed = FALSE;
	//wcstombs(pBuf, str, size+1);
	WideCharToMultiByte(CP_ACP, NULL, str, -1, pBuf, size+1, NULL, &defaultChUsed);
	m_AString = pBuf;
	delete pBuf;
#else
	m_AString = str;
#endif
	return m_AString.c_str();
}
//------------------------------------------------------------------------------------







