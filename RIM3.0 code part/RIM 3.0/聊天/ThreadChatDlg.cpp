// ChatMonitorThread.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadChatDlg.h"

IMPLEMENT_DYNCREATE(CThreadChatDlg, CWinThread)

CThreadChatDlg::CThreadChatDlg()
{
	m_pFriendUser = NULL;
}

CThreadChatDlg::~CThreadChatDlg()
{
}

BOOL CThreadChatDlg::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	TRACE(_T("%s %d \n"), _T("CChatDlgThread"), GetCurrentThreadId());

	ASSERT(m_pFriendUser);

	CChatBkDlg dlg;
	m_pMainWnd=&dlg;
	dlg.InitUserInfo(m_pFriendUser);
	//dlg.Create(CChatBkDlg::IDD, NULL);
	//dlg.RunModalLoop();
	dlg.DoModal();  

	//{
	//	// InitCommonControlsEx() is required on Windows XP if an application
	//	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	//	// visual styles.  Otherwise, any window creation will fail.
	//	INITCOMMONCONTROLSEX InitCtrls;
	//	InitCtrls.dwSize = sizeof(InitCtrls);
	//	// Set this to include all the common control classes you want to use
	//	// in your application.
	//	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	//	InitCommonControlsEx(&InitCtrls);

	//	// Initialize OLE libraries
	//	if (!AfxOleInit())
	//	{
	//		return FALSE;
	//	}
	//	AfxEnableControlContainer();
	//	// Standard initialization
	//	// If you are not using these features and wish to reduce the size
	//	// of your final executable, you should remove from the following
	//	// the specific initialization routines you do not need
	//	// Change the registry key under which our settings are stored
	//	// TODO: You should modify this string to be something appropriate
	//	// such as the name of your company or organization
	//	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//	// To create the main window, this code creates a new frame window
	//	// object and then sets it as the application's main window object
	//	CChatFrameWnd* m_pChatFrame = (CChatFrameWnd*)RUNTIME_CLASS(CChatFrameWnd)->CreateObject();
	//	if (!m_pChatFrame)
	//		return FALSE;
	//	m_pMainWnd = m_pChatFrame;
	//	m_pChatFrame->InitUserInfo(m_pFriendUser);
	//	// create and load the frame with its resources
	//	m_pChatFrame->LoadFrame(IDR_MAINFRAME1, WS_OVERLAPPEDWINDOW, NULL, NULL);


	//	// The one and only window has been initialized, so show and update it
	//	m_pChatFrame->ShowWindow(SW_SHOW);
	//	m_pChatFrame->UpdateWindow();
	//	// call DragAcceptFiles only if there's a suffix
	//	//  In an SDI app, this should occur after ProcessShellCommand
	//	return TRUE;
	//}
	return FALSE;
}

int CThreadChatDlg::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if(m_pFriendUser)
		m_pFriendUser->pChatThread = NULL;

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadChatDlg, CWinThread)
END_MESSAGE_MAP()

void CThreadChatDlg::SetUser(LPUSERINFO pUser)
{
	m_pFriendUser = pUser;
}

void CThreadChatDlg::ActiveWnd()
{
	if (m_pMainWnd)
	{
		m_pMainWnd->ShowWindow(SW_NORMAL);
		m_pMainWnd->BringWindowToTop();
	}
}


// CChatDlgThread message handlers
