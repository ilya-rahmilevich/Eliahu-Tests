
// MFC_TFS.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// For ChangeSet 1

// For ChangeSet 2


// CMFC_TFSApp:
// See MFC_TFS.cpp for the implementation of this class
//

class CMFC_TFSApp : public CWinApp
{
public:
	CMFC_TFSApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFC_TFSApp theApp;