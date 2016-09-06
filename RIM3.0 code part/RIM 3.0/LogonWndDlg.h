//********************************************************************
//	LOGONWNDDLG.H 文件注释
//	文件名		: LOGONWNDDLG.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/5 9:05
//	文件描述	: 用于显示登录窗口
//*********************************************************************	
#pragma once
#include "afxwin.h"


// CLogonWndDlg dialog

class CLogonWndDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonWndDlg)

public:
	CLogonWndDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonWndDlg();

// Dialog Data
	enum { IDD = IDD_LOGONWNDDLG };

protected:
	static UINT __stdcall					ThreadDoLogon(void *pDt);
	BOOL									m_bDisplayChange;
	bool									m_bMouseMove;			//用来判断鼠标是否移出本窗口
	////进度条相关
	CRect									m_processRect;			//进度条区域
	DWORD									m_processPercent0_100;	//进度条进度
	DWORD									m_logonInfo[2];			//用来向函数传递相关信息的结构
	HANDLE									m_hThrdLogon;			//登录线程句柄
	Color									m_processBkColor;		//进度条的颜色
	tstring									m_errorInfo;			//登录返回结果信息

	virtual void							DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();
	afx_msg void							OnMove(int x, int y);
	afx_msg LRESULT							OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);

	virtual BOOL 							OnInitDialog();
	afx_msg void							OnBnClickedButton1();
	afx_msg LRESULT							OnNcHitTest(CPoint point);
	afx_msg BOOL							OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void							OnSize(UINT nType, int cx, int cy);
	afx_msg void 							OnBnClickedChkPassword();
	afx_msg void 							OnBnClickedChkAutologon();
	afx_msg void							OnBnClickedChkOffline();
	afx_msg void 							OnEnChangeEUser();
	afx_msg void 							OnEnChangeEPass();
	afx_msg void 							OnEnChangeESevrip();
	afx_msg LRESULT							OnProgress(WPARAM wParam, LPARAM lParam);


	void									Init();
	void									UpdateSubWndShow();
	void									SetOrSaveSystemSet(BOOL bShowOrSave = TRUE);
	CPNGGrayTipEdit 						m_editUser;				//用户名
	CPNGGrayTipEdit 						m_editPass;				//密码
	CPNGGrayTipEdit 						m_editDmsIP;			//DMS服务IP
	CPNGGrayTipEdit 						m_editRmsIP;			//RMS服务IP
	CPNGGrayTipEdit 						m_editSasIP;			//SAS服务IP

	CPngButton								m_btnLogon;				//登录
	
	CRgn									m_wndMoveRgn;			//鼠标拖动区域
	CRgn									m_wndRgn;				//窗口区域
	CPngCheckBox 							m_chkPassword;			//记住密码
	CPngCheckBox 							m_chkAutoLogon;			//自动登录
	CPngCheckBox							m_chkOffLine;			//是否脱 机
	CPngStatic 								m_staticVersion;		//版本

public:
	BYTE									m_wndAlphaValue;		//兼容XP获取不准确的问题
	void									SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	bool									Logon(BOOL bLogon);
};
