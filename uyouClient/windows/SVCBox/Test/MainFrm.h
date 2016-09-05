
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "CDSVCBox.h"

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

private:
	//copy from SVCBoxCtrl.h
	enum {
		eventidNotifySdCardSpace = 5L,
		eventidHintInputMethodStatus = 4L,
		eventidNotifyDeviceScreenSize = 3L,
		eventidNotifyRunningApp = 2L,
		eventidConnectStateChanged = 1L,
		dispidGetIMEStatus = 10L,
		dispidSetIMEStatus = 9L,
		dispidSVCDowndloadApp = 7L,
		dispidSendOperateCmd = 6L,
		dispidLoadKSConfigNStart = 5L,
	};

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);	
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnTestBack();
	afx_msg void OnTestHome();
	afx_msg void OnTestTaskMgr();
	
protected:
	BOOL OnConnectStateChanged(ULONG ulState);
	BOOL OnNotifyRunningApp(LPCSTR bstrPackageName, LPCSTR bstrAppName);
	BOOL OnNotifyDeviceScreenSize(ULONG ulWidth, ULONG ulHeight);
	BOOL OnHintInputMethodStatus(USHORT bInputMethodShouldOpen);
	BOOL OnNotifySdCardSpace(ULONG ulAvailable, ULONG ulTotal);

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

private:
	CDSVCBox m_ocx;
	static const UINT m_nOcxControlId = 100;
};


