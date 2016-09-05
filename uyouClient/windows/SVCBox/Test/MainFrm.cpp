
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SVCBoxTest.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TRACE_TAG "[SVCBoxTest] "

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_TEST_BACK, &CMainFrame::OnTestBack)
	ON_COMMAND(ID_TEST_HOME, &CMainFrame::OnTestHome)
	ON_COMMAND(ID_TEST_TASK_MGR, &CMainFrame::OnTestTaskMgr)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainFrame, CFrameWnd)
	ON_EVENT(CMainFrame, m_nOcxControlId, eventidConnectStateChanged, OnConnectStateChanged, VTS_UI4)
	ON_EVENT(CMainFrame, m_nOcxControlId, eventidNotifyRunningApp, OnNotifyRunningApp, VTS_BSTR VTS_BSTR)
	ON_EVENT(CMainFrame, m_nOcxControlId, eventidNotifyDeviceScreenSize, OnNotifyDeviceScreenSize, VTS_UI4 VTS_UI4)
	ON_EVENT(CMainFrame, m_nOcxControlId, eventidHintInputMethodStatus, OnHintInputMethodStatus, VTS_UI2)
	ON_EVENT(CMainFrame, m_nOcxControlId, eventidNotifySdCardSpace, OnNotifySdCardSpace, VTS_UI4 VTS_UI4)
END_EVENTSINK_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
	m_ocx.ClearTouchedKey();
	m_ocx.Exit();                    ////控件优先退出
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rtClient;
	GetClientRect(rtClient);
	m_ocx.Create(_T("ocx"), WS_CHILD | WS_VISIBLE, rtClient, this, m_nOcxControlId);
	m_ocx.Start(TRUE);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//需要调整此值，使得client大小恰好为1280*720
	cs.cx = 1296;
	cs.cy = 778;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if(IsWindow(m_ocx.GetSafeHwnd()))
		m_ocx.MoveWindow(0,0, cx, cy);
}

void CMainFrame::OnTestBack()
{
	m_ocx.SendOperateCmd(0, 0, 0, 0, CComBSTR(""));
}

void CMainFrame::OnTestHome()
{
	m_ocx.SendOperateCmd(1, 0, 0, 0, CComBSTR(""));
}


void CMainFrame::OnTestTaskMgr()
{
	m_ocx.SendOperateCmd(2, 0, 0, 0, CComBSTR(""));
}

BOOL CMainFrame::OnConnectStateChanged(ULONG ulState)
{
	TRACE(TRACE_TAG"OnConnectStateChanged to %u\n", ulState);
	return TRUE;
}

BOOL CMainFrame::OnNotifyRunningApp(LPCSTR bstrPackageName, LPCSTR bstrAppName)
{
	//注意，即使是unicode版本，这里的两个字符串仍然是ansi的，必要的话需要自己转换
	TRACE(TRACE_TAG"OnNotifyRunningApp %s(%s)\n", bstrAppName, bstrPackageName);
	return TRUE;
}

BOOL CMainFrame::OnNotifyDeviceScreenSize(ULONG ulWidth, ULONG ulHeight)
{
	TRACE(TRACE_TAG"OnNotifyDeviceScreenSize %u %u\n", ulWidth, ulHeight);
	return TRUE;
}

BOOL CMainFrame::OnHintInputMethodStatus(USHORT bInputMethodShouldOpen)
{
	TRACE(TRACE_TAG"OnHintInputMethodStatus to %u\n", bInputMethodShouldOpen);
	return TRUE;
}

BOOL CMainFrame::OnNotifySdCardSpace(ULONG ulAvailable, ULONG ulTotal)
{
	TRACE(TRACE_TAG"OnNotifySdCardSpace available=%u total=%u\n", ulAvailable, ulTotal);
	return TRUE;

}