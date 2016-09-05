#pragma once

// SVCBoxMonitor.h : main header file for SVCBoxMonitor.DLL

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols

extern CString g_szEXEPath;

// CSVCBoxMonitorApp : See SVCBoxMonitor.cpp for implementation.

class CSVCBoxMonitorApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

