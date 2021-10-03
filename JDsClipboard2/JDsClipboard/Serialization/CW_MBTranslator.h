
/**************************************************************************
*   CW_MBTranslator.h                                                     *
*   Header File                                                           *
*   Sabelnikov Andrey N. mail-to: de_kastro@mail.ru                       *
*   25.04.2003 17:19:18                                                   *
*   Description: Helper class to translate strings from ascii             *
*                to wide and back                                         *
**************************************************************************/
#ifndef _CW_MBTRANSLATOR_H_
#define _CW_MBTRANSLATOR_H_

#include <string>


#ifndef tstring
#ifdef UNICODE
	#define tstring std::wstring
#else
	#define tstring std::string
#endif//UNICODE
#endif//!tstring


class CW_MBTranslator
{
public:
	CW_MBTranslator();
	virtual ~CW_MBTranslator();

	const TCHAR*   TranslateTO_T(const wchar_t* str);
	const TCHAR*   TranslateTO_T(const char*    str);
	const wchar_t* TranslateFrom_T_TO_W(const TCHAR* str);
	const char*    TranslateFrom_T_TO_A(const TCHAR* str);

private:
	tstring      m_TString;
	std::string  m_AString;
	std::wstring m_WString;

};


#endif//_CW_MBTRANSLATOR_H_
