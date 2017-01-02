
// RIM 3.0.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "RIM 3.0Dlg.h"
#include "LogonWndDlg.h"

#include  <dbghelp.h> 
#pragma comment(lib,  "dbghelp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


// CRIM30App

BEGIN_MESSAGE_MAP(CRIM30App, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRIM30App 构造


LONG WINAPI  Filter(struct _EXCEPTION_POINTERS *ep) 
{
	TCHAR szExpType[100] = {0};
	switch(ep->ExceptionRecord->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_ACCESS_VIOLATION，要导出dmp文件吗？");
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_DATATYPE_MISALIGNMENT，要导出dmp文件吗？");
		break;
	case EXCEPTION_BREAKPOINT:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_BREAKPOINT，要导出dmp文件吗？");
		break;
	case EXCEPTION_SINGLE_STEP:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_SINGLE_STEP，要导出dmp文件吗？");
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_ARRAY_BOUNDS_EXCEEDED，要导出dmp文件吗？");
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_DENORMAL_OPERAND，要导出dmp文件吗？");
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_DIVIDE_BY_ZERO，要导出dmp文件吗？");
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_INEXACT_RESULT，要导出dmp文件吗？");
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_INVALID_OPERATION，要导出dmp文件吗？");
		break;
	case EXCEPTION_FLT_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_OVERFLOW，要导出dmp文件吗？");
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_STACK_CHECK，要导出dmp文件吗？");
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_UNDERFLOW，要导出dmp文件吗？");
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INT_DIVIDE_BY_ZERO，要导出dmp文件吗？");
		break;
	case EXCEPTION_INT_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INT_OVERFLOW，要导出dmp文件吗？");
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_PRIV_INSTRUCTION，要导出dmp文件吗？");
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_IN_PAGE_ERROR，要导出dmp文件吗？");
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_ILLEGAL_INSTRUCTION，要导出dmp文件吗？");
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_NONCONTINUABLE_EXCEPTION，要导出dmp文件吗？");
		break;
	case EXCEPTION_STACK_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_STACK_OVERFLOW，要导出dmp文件吗？");
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INVALID_DISPOSITION，要导出dmp文件吗？");
		break;
	case EXCEPTION_GUARD_PAGE:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_GUARD_PAGE，要导出dmp文件吗？");
		break;
	case EXCEPTION_INVALID_HANDLE:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INVALID_HANDLE，要导出dmp文件吗？");
		break;
		//case EXCEPTION_POSSIBLE_DEADLOCK:
		//	_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_POSSIBLE_DEADLOCK，要导出dmp文件吗？");
		//	break;
	default:
		_tcscpy_s(szExpType, sizeof(szExpType), "未知异常，要导出dmp文件吗？");
	}
	//	if ( ::MessageBox(NULL, szExpType,  _T("RIM3.0 异常"), MB_OKCANCEL) != IDOK )
	//		return EXCEPTION_EXECUTE_HANDLER;

	TCHAR szTime[MAX_PATH] = {0};
	TCHAR szName[MAX_PATH] = {0};
	if (!::GetModuleFileName(NULL, szName, sizeof(szName)) ) 
		return EXCEPTION_EXECUTE_HANDLER;
	TCHAR * pName = _tcsrchr(szName, _T('\\')) + 1;
	*pName = _T('\0');

	SYSTEMTIME sysTime;   // Win32 time information
	GetLocalTime(&sysTime);
	COleDateTime dateTime(sysTime);
	_stprintf_s(szTime, MAX_PATH, _T("RIM30_%04d_%02d_%02d_%02d_%02d_%02d.dmp"), dateTime.GetYear(),
		dateTime.GetMonth(), dateTime.GetDay(), dateTime.GetHour(), 
		dateTime.GetMinute(), dateTime.GetSecond());
	_tcscat_s(szName, MAX_PATH, szTime);

	HANDLE hFile = CreateFile(szName, 
		GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,
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


CRIM30App::CRIM30App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

CRIM30App::~CRIM30App()
{
	if (m_hinstRE)
		FreeLibrary(m_hinstRE);
}

// 唯一的一个 CRIM30App 对象

CRIM30App theApp;


// CRIM30App 初始化
ULONG_PTR g_token;
BOOL CRIM30App::InitInstance()
{
	LPTOP_LEVEL_EXCEPTION_FILTER lle = NULL;
	lle = SetUnhandledExceptionFilter(Filter);

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxOleInit();

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("套接字初始化失败！"));
		return FALSE;
	}

	AfxEnableControlContainer();

	AfxInitRichEdit2();

	//_CrtSetBreakAlloc(929);

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//{

	//	//*********************修改本进程的自身权限********************************
	//	HANDLE htoke;
	//	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &htoke))
	//	{
	//		return FALSE;
	//	}
	//	LUID luid;
	//	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	//	{
	//		return FALSE;
	//	}
	//	TOKEN_PRIVILEGES NewState;
	//	NewState.PrivilegeCount = 1;
	//	NewState.Privileges[0].Luid = luid;
	//	NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//	if (!AdjustTokenPrivileges(htoke, FALSE, &NewState, sizeof(NewState), NULL, NULL))
	//	{
	//		return FALSE;
	//	}
	//	//************修改本进程对外部进程的权限（必须在自身权限允许下）*******************************
	//	HANDLE hWarProcess;
	//	hWarProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, GetProcessId(GetCurrentProcess()));
	//	if (!hWarProcess)
	//	{
	//		return FALSE;
	//	}
	//}

	tstring	guidStr		= _T("{64A55AEE-09C0-46b4-83BE-3982EE65895B}");//保证对应的应用程序唯一	
	if(CreateMutex(NULL, FALSE, guidStr.c_str()))
	{
		if(GetLastError() & ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL, _T("程序已启动！"), _T("RIM3.0 提示"), MB_OK);
			return FALSE;
		}
	}
	else
	{
		TCHAR ch[MAX_PATH] = {0};
		_stprintf_s(ch, MAX_PATH, _T("程序启动失败 ID: %d "), GetLastError());
		MessageBox(NULL, ch, _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}

	{	////修改DLL搜索路径
		TCHAR	szPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szPath, MAX_PATH);
		*(_tcsrchr(szPath, _T('\\'))+1) = NULL;
		_tcscat_s(szPath, MAX_PATH, _T("bin"));
		SetDllDirectory(szPath);
	}

	m_hinstRE = LoadLibrary(_T("msftedit.dll"));

	

	GdiplusStartupInput gdiplusstartupinput;
	GdiplusStartup(&g_token, &gdiplusstartupinput, NULL);

	{//解析配置文件
		if(RIMConfig::GetInstance() == NULL || ImageConfig::GetInstance() == NULL)		//配置文件加载错误
			return FALSE;
	}

	m_pServer = GetRIM30Server();
	m_pServer->Begin(TCP_MSGSERVER_PORT);


	CThreadUDPMonitorRecive::GetInstance()->Begin(TRUE);
	CThreadUDPDataProcess::GetInstance()->Begin(TRUE);
	CThreadUDPMonitorSend::GetInstance()->Begin(TRUE);

	CThreadTCPMonitorRecive::GetInstance()->Begin(TRUE);
	CThreadTCPDataProcess::GetInstance()->Begin(TRUE);
	CThreadTCPMonitorSend::GetInstance()->Begin(TRUE);


	ChangeWndMessageFilter(WM_DROPFILES, TRUE);
	ChangeWndMessageFilter(0x0049, TRUE); // 0x0049 - WM_COPYGLOBALDATA


	CRIM30Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	CThreadUDPMonitorRecive::GetInstance()->End();
	CThreadUDPDataProcess::GetInstance()->End();
	while(RIMConfig::GetInstance()->m_UDPSendQueue.GetQueueCount());	//等待发送队列清空
	CThreadUDPMonitorSend::GetInstance()->End();

	CThreadTCPMonitorRecive::GetInstance()->End();
	CThreadTCPDataProcess::GetInstance()->End();
	while(RIMConfig::GetInstance()->m_TCPSendQueue.GetQueueCount());	//等待发送队列清空
	CThreadTCPMonitorSend::GetInstance()->End();

	RIMConfig::ReleaseInstance();
	ImageConfig::ReleaseInstance();

	CThreadUDPMonitorRecive::ReleaseInstance();
	CThreadUDPDataProcess::ReleaseInstance();
	CThreadUDPMonitorSend::ReleaseInstance();
	CThreadTCPMonitorRecive::ReleaseInstance();
	CThreadTCPDataProcess::ReleaseInstance();
	CThreadTCPMonitorSend::ReleaseInstance();

	m_pServer->End();


	// 	CLogonWndDlg dlg;
// 	//m_pMainWnd = &dlg;
// 	INT_PTR nResponse = dlg.DoModal();
// 	if (nResponse == IDOK)
// 	{
// 		// TODO: 在此放置处理何时用
// 		//  “确定”来关闭对话框的代码
// 	}
// 	else if (nResponse == IDCANCEL)
// 	{
// 		// TODO: 在此放置处理何时用
// 		//  “取消”来关闭对话框的代码
// 	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CRIM30App::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	GdiplusShutdown(g_token);
	return CWinAppEx::ExitInstance();
}
// 放行文件拖放相关事件消息，仅对win7有效，以解决win7中文件拖放失效的问题


BOOL CRIM30App::ChangeWndMessageFilter(UINT uMessage, BOOL bAllow)
{
	typedef BOOL(WINAPI* ChangeWindowMessageFilterFn)(UINT, DWORD);
	HMODULE hUserMod = NULL;
	BOOL bResult = FALSE;
	hUserMod = LoadLibrary(_T("user32.dll"));
	if (hUserMod == NULL)
	{
		return FALSE;
	}

	// 获取ChangeWindowMessageFilter函数接口指针
	ChangeWindowMessageFilterFn pfnChangeWindowMessageFilter = (ChangeWindowMessageFilterFn)GetProcAddress(hUserMod, "ChangeWindowMessageFilter");
	if (pfnChangeWindowMessageFilter == NULL)
	{
		FreeLibrary(hUserMod);
		return FALSE;
	}

	bResult = pfnChangeWindowMessageFilter(uMessage, bAllow ? 1 : 2); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
	FreeLibrary(hUserMod);
	return bResult;
}