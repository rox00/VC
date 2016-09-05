#pragma once

// SVCBox.h : main header file for SVCBox.DLL

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols

extern CString g_szEXEPath;

// CSVCBoxApp : See SVCBox.cpp for implementation.

class CSVCBoxApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

