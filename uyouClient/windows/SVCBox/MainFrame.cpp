// MainFrame.cpp : implementation file
//

#include "stdafx.h"
#include "SVCBox.h"
#include "MainFrame.h"
#include "Input/InputManager.h"
#include "afxdialogex.h"

// CMainFrame dialog

IMPLEMENT_DYNAMIC(CMainFrame, CDialogEx)

CMainFrame::CMainFrame(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINFRAME, pParent)
{
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainFrame, CDialogEx)
	ON_MESSAGE(WM_FGNOTIFY, OnFgNotify)

	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()	
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMainFrame message handlers

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//RECT rectClient;
	//GetClientRect(&rectClient);

	//dc.SelectObject(&m_BrushBackground);
	//dc.FillRect(&rectClient, &m_BrushBackground);
	//TODO 增加等待图标的显示
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_BrushBackground.CreateSolidBrush(RGB(0,0,0));

	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

	DragAcceptFiles();
	return 0;
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	m_WindowToTouchScreenMapping.SetWindowSize(cx, cy, GetDeviceScreenSize().cx, GetDeviceScreenSize().cy);	
}


BOOL CMainFrame::OnInitDialog()
{
	CDialogEx::OnInitDialog();		
	SetWindowText(_T("UYouPlayer"));
	
	InputManager::GetInstance().GetTouchScreen().SetTouchMappingFunction(
		boost::bind(&WindowToTouchScreenMapping::DoMapping, &m_WindowToTouchScreenMapping, _1, _2));
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMainFrame::OnDestroy()
{
	Stop();

	GdiplusShutdown(m_gdiplusToken);
	CDialogEx::OnDestroy();	
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//MessageBox(_T("123"));
	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL CMainFrame::InitVideo(LPCTSTR lpszCableName, DWORD nCableId)
{
	if (m_Video.Play(this, this, lpszCableName, nCableId))
	{
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::Stop()
{
	m_Video.Release();
	CDC *pDC = GetDC();
	pDC->TextOut(0, 0, _T("连接断开"));
	ReleaseDC(pDC);
}

LRESULT CMainFrame::OnFgNotify(WPARAM wParam, LPARAM lParam)
{
	if (!m_Video.ProcessFgNotify(wParam, lParam))
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);
	}
	return TRUE;
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}
