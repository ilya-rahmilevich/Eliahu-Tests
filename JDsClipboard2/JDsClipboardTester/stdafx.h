// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

#pragma warning(disable: 4290)

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#define STRICT

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#include <atlstr.h>

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;
//#include /**/ <atlstr.h>
#include <atlwin.h>
#include <atlframe.h>

#include <atlctrls.h>
#include <atlctrlw.h>
#include <atldlgs.h>
#include <atlscrl.h>
#include <atlsplit.h>
#include <atlctrlx.h>

#define _WTL_NO_CSTRING
#include <atlmisc.h>
#include <atlcrack.h>
using namespace ATL;
