
// HotKeyer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHotKeyerApp:
// See HotKeyer.cpp for the implementation of this class
//

class CHotKeyerApp : public CWinApp
{
public:
	CHotKeyerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
//  afx_msg void OnAfxIdPreviewClose();
};

extern CHotKeyerApp theApp;
