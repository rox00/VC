
// RIM 3.0.cpp : ����Ӧ�ó��������Ϊ��
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


// CRIM30App ����


LONG WINAPI  Filter(struct _EXCEPTION_POINTERS *ep) 
{
	TCHAR szExpType[100] = {0};
	switch(ep->ExceptionRecord->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_ACCESS_VIOLATION��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_DATATYPE_MISALIGNMENT��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_BREAKPOINT:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_BREAKPOINT��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_SINGLE_STEP:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_SINGLE_STEP��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_ARRAY_BOUNDS_EXCEEDED��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_DENORMAL_OPERAND��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_DIVIDE_BY_ZERO��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_INEXACT_RESULT��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_INVALID_OPERATION��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_FLT_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_OVERFLOW��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_STACK_CHECK��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_FLT_UNDERFLOW��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INT_DIVIDE_BY_ZERO��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_INT_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INT_OVERFLOW��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_PRIV_INSTRUCTION��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_IN_PAGE_ERROR��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_ILLEGAL_INSTRUCTION��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_NONCONTINUABLE_EXCEPTION��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_STACK_OVERFLOW:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_STACK_OVERFLOW��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INVALID_DISPOSITION��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_GUARD_PAGE:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_GUARD_PAGE��Ҫ����dmp�ļ���");
		break;
	case EXCEPTION_INVALID_HANDLE:
		_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_INVALID_HANDLE��Ҫ����dmp�ļ���");
		break;
		//case EXCEPTION_POSSIBLE_DEADLOCK:
		//	_tcscpy_s(szExpType, sizeof(szExpType), "EXCEPTION_POSSIBLE_DEADLOCK��Ҫ����dmp�ļ���");
		//	break;
	default:
		_tcscpy_s(szExpType, sizeof(szExpType), "δ֪�쳣��Ҫ����dmp�ļ���");
	}
	//	if ( ::MessageBox(NULL, szExpType,  _T("RIM3.0 �쳣"), MB_OKCANCEL) != IDOK )
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
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CRIM30App::~CRIM30App()
{
	if (m_hinstRE)
		FreeLibrary(m_hinstRE);
}

// Ψһ��һ�� CRIM30App ����

CRIM30App theApp;


// CRIM30App ��ʼ��
ULONG_PTR g_token;
BOOL CRIM30App::InitInstance()
{
	LPTOP_LEVEL_EXCEPTION_FILTER lle = NULL;
	lle = SetUnhandledExceptionFilter(Filter);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxOleInit();

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("�׽��ֳ�ʼ��ʧ�ܣ�"));
		return FALSE;
	}

	AfxEnableControlContainer();

	AfxInitRichEdit2();

	//_CrtSetBreakAlloc(929);

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//{

	//	//*********************�޸ı����̵�����Ȩ��********************************
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
	//	//************�޸ı����̶��ⲿ���̵�Ȩ�ޣ�����������Ȩ�������£�*******************************
	//	HANDLE hWarProcess;
	//	hWarProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, GetProcessId(GetCurrentProcess()));
	//	if (!hWarProcess)
	//	{
	//		return FALSE;
	//	}
	//}

	tstring	guidStr		= _T("{64A55AEE-09C0-46b4-83BE-3982EE65895B}");//��֤��Ӧ��Ӧ�ó���Ψһ	
	if(CreateMutex(NULL, FALSE, guidStr.c_str()))
	{
		if(GetLastError() & ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL, _T("������������"), _T("RIM3.0 ��ʾ"), MB_OK);
			return FALSE;
		}
	}
	else
	{
		TCHAR ch[MAX_PATH] = {0};
		_stprintf_s(ch, MAX_PATH, _T("��������ʧ�� ID: %d "), GetLastError());
		MessageBox(NULL, ch, _T("RIM3.0 ����"), MB_OK);
		return FALSE;
	}

	{	////�޸�DLL����·��
		TCHAR	szPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szPath, MAX_PATH);
		*(_tcsrchr(szPath, _T('\\'))+1) = NULL;
		_tcscat_s(szPath, MAX_PATH, _T("bin"));
		SetDllDirectory(szPath);
	}

	m_hinstRE = LoadLibrary(_T("msftedit.dll"));

	

	GdiplusStartupInput gdiplusstartupinput;
	GdiplusStartup(&g_token, &gdiplusstartupinput, NULL);

	{//���������ļ�
		if(RIMConfig::GetInstance() == NULL || ImageConfig::GetInstance() == NULL)		//�����ļ����ش���
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	CThreadUDPMonitorRecive::GetInstance()->End();
	CThreadUDPDataProcess::GetInstance()->End();
	while(RIMConfig::GetInstance()->m_UDPSendQueue.GetQueueCount());	//�ȴ����Ͷ������
	CThreadUDPMonitorSend::GetInstance()->End();

	CThreadTCPMonitorRecive::GetInstance()->End();
	CThreadTCPDataProcess::GetInstance()->End();
	while(RIMConfig::GetInstance()->m_TCPSendQueue.GetQueueCount());	//�ȴ����Ͷ������
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
// 		// TODO: �ڴ˷��ô����ʱ��
// 		//  ��ȷ�������رնԻ���Ĵ���
// 	}
// 	else if (nResponse == IDCANCEL)
// 	{
// 		// TODO: �ڴ˷��ô����ʱ��
// 		//  ��ȡ�������رնԻ���Ĵ���
// 	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CRIM30App::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	GdiplusShutdown(g_token);
	return CWinAppEx::ExitInstance();
}
// �����ļ��Ϸ�����¼���Ϣ������win7��Ч���Խ��win7���ļ��Ϸ�ʧЧ������


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

	// ��ȡChangeWindowMessageFilter�����ӿ�ָ��
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