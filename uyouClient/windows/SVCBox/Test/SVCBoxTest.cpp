
// SVCBoxTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SVCBoxTest.h"
#include "MainFrm.h"
#include "CDSVCBox.h"
#include "CDSVCBox.h"
#include "CDSVCBox.h"
#include "CDSVCBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSVCBoxTestApp

BEGIN_MESSAGE_MAP(CSVCBoxTestApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSVCBoxTestApp::OnAppAbout)
END_MESSAGE_MAP()


CString g_szEXEPath;

DWORD RegisterServer(CString szServerDllPathName)
{
	HMODULE hInprocDll = LoadLibrary(szServerDllPathName);
	if (hInprocDll == 0)
	{
		return 10000 + GetLastError();
	}

	void(*pDllRegisterServer)(void) = NULL;
	pDllRegisterServer = (void(*)(void))GetProcAddress(hInprocDll, "DllRegisterServer");

	if (!pDllRegisterServer)
		return 20000 + GetLastError();

	pDllRegisterServer();

	FreeLibrary(hInprocDll);

	return 0;
}


// CSVCBoxTestApp construction

CSVCBoxTestApp::CSVCBoxTestApp()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SVCBoxTest.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSVCBoxTestApp object

CSVCBoxTestApp theApp;


// CSVCBoxTestApp initialization

BOOL CSVCBoxTestApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	//get exe path
	TCHAR lpszDrive[255], lpszPath[1024], lpszTemp[255];
	_wsplitpath(m_pszHelpFilePath, lpszDrive, lpszPath, lpszTemp, lpszTemp);

	g_szEXEPath = lpszDrive;
	g_szEXEPath += lpszPath;

	SetCurrentDirectory(g_szEXEPath);

#ifdef DEBUG
	auto iErrorCode = RegisterServer(g_szEXEPath + _T("SVCBoxD.ocx"));
#else
	auto iErrorCode = RegisterServer(g_szEXEPath + _T("SVCBox.ocx"));
#endif

	if (iErrorCode)
	{
		CString sz;
		sz.Format(_T("×¢²áÊ§°Ü£¬´íÎóÂë %d"), iErrorCode);
		AfxMessageBox(sz);
	}

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

int CSVCBoxTestApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CSVCBoxTestApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// App command to run the dialog
void CSVCBoxTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

