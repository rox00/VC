// SVCBoxMonitor.cpp : Implementation of CSVCBoxMonitorApp and DLL registration.

#include "stdafx.h"
#include "SVCBoxMonitor.h"
#include  <dbghelp.h> 
#pragma comment(lib,  "dbghelp.lib")

#include "../Common/Common.h"

LONG WINAPI  Filter(struct _EXCEPTION_POINTERS *ep)
{
	TCHAR szExpType[100] ={ 0 };
	switch (ep->ExceptionRecord->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_ACCESS_VIOLATION，要导出dmp文件吗？"));
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_DATATYPE_MISALIGNMENT，要导出dmp文件吗？"));
		break;
	case EXCEPTION_BREAKPOINT:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_BREAKPOINT，要导出dmp文件吗？"));
		break;
	case EXCEPTION_SINGLE_STEP:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_SINGLE_STEP，要导出dmp文件吗？"));
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_ARRAY_BOUNDS_EXCEEDED，要导出dmp文件吗？"));
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_FLT_DENORMAL_OPERAND，要导出dmp文件吗？"));
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_FLT_DIVIDE_BY_ZERO，要导出dmp文件吗？"));
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_FLT_INEXACT_RESULT，要导出dmp文件吗？"));
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_FLT_INVALID_OPERATION，要导出dmp文件吗？"));
		break;
	case EXCEPTION_FLT_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_FLT_OVERFLOW，要导出dmp文件吗？"));
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_FLT_STACK_CHECK，要导出dmp文件吗？"));
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_FLT_UNDERFLOW，要导出dmp文件吗？"));
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_INT_DIVIDE_BY_ZERO，要导出dmp文件吗？"));
		break;
	case EXCEPTION_INT_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_INT_OVERFLOW，要导出dmp文件吗？"));
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_PRIV_INSTRUCTION，要导出dmp文件吗？"));
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_IN_PAGE_ERROR，要导出dmp文件吗？"));
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_ILLEGAL_INSTRUCTION，要导出dmp文件吗？"));
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_NONCONTINUABLE_EXCEPTION，要导出dmp文件吗？"));
		break;
	case EXCEPTION_STACK_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_STACK_OVERFLOW，要导出dmp文件吗？"));
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_INVALID_DISPOSITION，要导出dmp文件吗？"));
		break;
	case EXCEPTION_GUARD_PAGE:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_GUARD_PAGE，要导出dmp文件吗？"));
		break;
	case EXCEPTION_INVALID_HANDLE:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("EXCEPTION_INVALID_HANDLE，要导出dmp文件吗？"));
		break;
		//case EXCEPTION_POSSIBLE_DEADLOCK:
		//	_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_POSSIBLE_DEADLOCK，要导出dmp文件吗？");
		//	break;
	default:
		_tcscpy_s(szExpType, sizeof(szExpType), _T("未知异常，要导出dmp文件吗？"));
	}
	//	if ( ::MessageBox(NULL, szExpType,  _T("异常"), MB_OKCANCEL) != IDOK )
	//		return EXCEPTION_EXECUTE_HANDLER;

	TCHAR szTime[MAX_PATH] ={ 0 };
	TCHAR szName[MAX_PATH] ={ 0 };
	if (!::GetModuleFileName(NULL, szName, sizeof(szName)))
		return EXCEPTION_EXECUTE_HANDLER;
	TCHAR * pName = _tcsrchr(szName, _T('\\')) + 1;
	*pName = _T('\0');

	SYSTEMTIME sysTime;   // Win32 time information
	GetLocalTime(&sysTime);
	COleDateTime dateTime(sysTime);
	_stprintf_s(szTime, MAX_PATH, _T("%04d_%02d_%02d_%02d_%02d_%02d.dmp"), dateTime.GetYear(),
		dateTime.GetMonth(), dateTime.GetDay(), dateTime.GetHour(),
		dateTime.GetMinute(), dateTime.GetSecond());
	_tcscat_s(szName, MAX_PATH, szTime);

	HANDLE hFile = CreateFile(szName,
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	DWORD dwError = ::GetLastError();
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	MINIDUMP_EXCEPTION_INFORMATION    exceptioninfo;
	exceptioninfo.ExceptionPointers = ep;
	exceptioninfo.ThreadId          = GetCurrentThreadId();
	exceptioninfo.ClientPointers    = FALSE;

	if (!MiniDumpWriteDump(GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		MiniDumpNormal,
		&exceptioninfo,
		NULL,
		NULL))
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	CloseHandle(hFile);
	return EXCEPTION_EXECUTE_HANDLER;
}

CSVCBoxMonitorApp theApp;

const GUID CDECL _tlid = { 0xf7cf1a20, 0x6693, 0x408c, { 0x99, 0x67, 0xf, 0x1c, 0x84, 0xd8, 0x46, 0x76 } }; 
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

CString g_szEXEPath;
// CSVCBoxMonitorApp::InitInstance - DLL initialization

BOOL CSVCBoxMonitorApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		//////捕获异常
		LPTOP_LEVEL_EXCEPTION_FILTER lle = NULL;
		lle = SetUnhandledExceptionFilter(Filter);

		TCHAR lpszModulePath[MAX_PATH];
		GetModuleFileName(NULL, lpszModulePath, MAX_PATH);
		g_szEXEPath = lpszModulePath;
		g_szEXEPath.Delete(g_szEXEPath.ReverseFind(_T('\\')), MAX_PATH);				
	}

	return bInit;
}



// CSVCBoxMonitorApp::ExitInstance - DLL termination

int CSVCBoxMonitorApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);
	
	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

