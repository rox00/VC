
// SVCBoxTest.h : main header file for the SVCBoxTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSVCBoxTestApp:
// See SVCBoxTest.cpp for the implementation of this class
//

class CSVCBoxTestApp : public CWinApp
{
public:
	CSVCBoxTestApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSVCBoxTestApp theApp;
