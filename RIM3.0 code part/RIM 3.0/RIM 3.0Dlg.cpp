//********************************************************************
//	RIM 3.0DLG.CPP 文件注释
//	文件名		: RIM 3.0DLG.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/5 9:38
//	文件描述	: 用于显示背景的窗口
//*********************************************************************	
// RIM30Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "RIM 3.0Dlg.h"
#include "系统设置\DialogSet.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRIM30Dlg 对话框

CRIM30Dlg::CRIM30Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRIM30Dlg::IDD, pParent)
{
	m_bBalloonShow				= FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pImgLogonTemp				= ImageConfig::GetInstance()->m_pImgLogon;			
	m_pImgSizeMidTemp			= ImageConfig::GetInstance()->m_pImgSizeMid;			
	m_pImgSizebottomTemp		= ImageConfig::GetInstance()->m_pImgSizebottom;		
	m_pImgMainListTemp			= ImageConfig::GetInstance()->m_pImgMainList;			
	m_pImgLogoTemp				= ImageConfig::GetInstance()->m_pImgLogo;				
	m_pImgCircleTemp			= ImageConfig::GetInstance()->m_pImgCircle;			
	m_pImgChkSelTemp			= ImageConfig::GetInstance()->m_pImgChkSel;	
	m_pImgChkUnSelTemp			= ImageConfig::GetInstance()->m_pImgChkUnSel;	

	m_wndAlphaValue				= 255;

	m_dockType					= DOCK_NORMAL;

	m_bAlreadyLogon				= FALSE;
	m_bShowMainImg				= TRUE;

	m_selMode					= 0;
	m_rcStation					= Rect(0,0,0,0);
	m_rcUser					= Rect(0,0,0,0);	
}

CRIM30Dlg::~CRIM30Dlg()
{
	DeleteObject(m_hRgnMain);
	DeleteObject(m_hRgnCircle);
	DeleteObject(m_hRgnLogo);
	m_MainPixelRgn.DeleteObject();

	Shell_NotifyIcon(NIM_DELETE, &m_icondata);
}

void CRIM30Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRIM30Dlg, CDialog)
	ON_MESSAGE(WM_SHOWMAINWND, OnShowMianWnd)
	ON_MESSAGE(WM_SHOWDOCKBOUND, OnShowDockBound)
	//ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)

	ON_MESSAGE(WM_LOGON, OnAlredyLogon)
	ON_MESSAGE(WM_ICON, &CRIM30Dlg::OnIconMsg)

	ON_MESSAGE(WM_UPDATECONFIG, OnUpdateConfig)
	ON_MESSAGE(WM_UPDATEUSERLIST, OnUpdateUserList)
	ON_MESSAGE(WM_UPDATESTATIONSTATUS, OnUpdateStationStatus)
	ON_MESSAGE(WM_FLICKERNOTIFYICON, OnFlickerNotifyIcon)
	ON_MESSAGE(WM_REPOSITIONSUBDLG, OnRepositionSubdlg)

	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CRIM30Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRIM30Dlg::OnBnClickedCancel)
 	ON_WM_CTLCOLOR()
 	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSELEAVE()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_SIZING()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CRIM30Dlg*			CRIM30Dlg::m_pSizeWnd = NULL;
// CRIM30Dlg 消息处理程序

void CRIM30Dlg::Init()
{
	SetTimer(1, 500, NULL);
	{	//为了使圆环与LOGO的像素相对位置严格保持一致
		DWORD circleWidth	= m_pImgCircleTemp->GetWidth();
		DWORD circleHeight	= m_pImgCircleTemp->GetHeight();
		DWORD logoWidth		= m_pImgLogoTemp->GetWidth();
		DWORD logoHeight	= m_pImgLogoTemp->GetHeight();

		CRect circleRect, logoRect;
		GetDlgItem(IDC_P_CIRCLE)->GetWindowRect(circleRect);
		GetDlgItem(IDC_P_LOGO)->GetWindowRect(logoRect);

		ScreenToClient(circleRect);
		ScreenToClient(logoRect);

		circleRect.right	= circleRect.left + circleWidth;
		circleRect.bottom	= circleRect.top + circleHeight;

		logoRect.left		= circleRect.left + (circleWidth - logoWidth)/2;
		logoRect.top		= circleRect.top + (circleHeight - logoHeight)/2;
		logoRect.right		= logoRect.left + logoWidth;
		logoRect.bottom		= logoRect.top + logoHeight;

		GetDlgItem(IDC_P_CIRCLE)->MoveWindow(circleRect);
		GetDlgItem(IDC_P_LOGO)->MoveWindow(logoRect);
	}


	{	//计算移动的边界区域
		CRect mainRect, circleRect, logoRect;
		GetDlgItem(IDC_P_MAINWND)->GetWindowRect(mainRect);
		GetDlgItem(IDC_P_CIRCLE)->GetWindowRect(circleRect);
		GetDlgItem(IDC_P_LOGO)->GetWindowRect(logoRect);

		ScreenToClient(mainRect);
		ScreenToClient(circleRect);
		ScreenToClient(logoRect);

		m_hRgnCircle = AlphaImageToRgn(m_pImgCircleTemp);
		m_hRgnMain = AlphaImageToRgn(m_pImgLogonTemp);
		m_hRgnLogo = AlphaImageToRgn(m_pImgLogoTemp);
		OffsetRgn(m_hRgnCircle, circleRect.left, circleRect.top);
		OffsetRgn(m_hRgnMain, mainRect.left, mainRect.top);
		OffsetRgn(m_hRgnLogo, logoRect.left, logoRect.top);


		m_clientcircleRect = circleRect;
		//圆环最底部的像素与圆环矩形最底部的值（这里取一个固定值 ） 调节隐藏主界面后可以向下拖动的位置 
		m_clientcircleRect.bottom -= 25;	

		m_clientRect.UnionRect(&mainRect, &circleRect);
	}


	{	//添加托盘图标

		
		LPCTSTR lpszDllName = _T("C:\\Windows\\System32\\Shell32.dll");
		DWORD dwVer = GetVersion(lpszDllName);
		DWORD dwTarget = PACKVERSION(6, 0);

		if (dwVer > dwTarget)
		{
			// This version of Shell32.dll is version 6.0 or later.
			m_icondata.cbSize			= sizeof(NOTIFYICONDATA);
			//m_icondata.uVersion			= NOTIFYICON_VERSION_4;
		}
		else
		{
			// Proceed knowing that version 6.0 or later additions are not available.
			// Use an alternate approach for older the DLL version.
			m_icondata.cbSize			= NOTIFYICONDATA_V3_SIZE;
		}

		m_icondata.hWnd				= m_hWnd;
		m_icondata.uID				= IDR_MAINFRAME;
		m_icondata.uFlags			= NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO;
		m_icondata.uCallbackMessage	= WM_ICON;
		m_icondata.hIcon			= m_hIcon;
		lstrcpy(m_icondata.szTip, _T("RIM 3.0"));

		m_icondata.uTimeout			= 3000;
		m_icondata.dwInfoFlags		= NIF_TIP|NIIF_NONE;
		lstrcpy(m_icondata.szInfoTitle, _T(""));
		lstrcpy(m_icondata.szInfo, _T(""));

		Shell_NotifyIcon(NIM_ADD, &m_icondata);//添加托盘图标
	}

	UpdateView();

	{	//初始启动靠右边显示
		CRect rcClient;
		GetClientRect(rcClient);
		CRect screenRc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&screenRc, 0);

		CRect wndRect;
		wndRect.left	= screenRc.right - rcClient.Width();
		wndRect.top		= screenRc.top + (screenRc.Height() - rcClient.Height())/2;
		wndRect.right	= wndRect.left + rcClient.Width();
		wndRect.bottom	= wndRect.top + rcClient.Height();
		MoveWindow(wndRect);
	}

	{
		m_dockDlg.Create(CDockDlg::IDD, this);
		m_dockDlg.MoveWindow(0, 0, m_pImgLogoTemp->GetWidth(), m_pImgLogoTemp->GetHeight());
		m_logonDlg.Create(CLogonWndDlg::IDD, this);
		m_mainListDlg.Create(CMainListDlg::IDD, this);
		m_circleDlg.Create(CCircleDlg::IDD, this);


		m_logonDlg.MoveWindow(CRect(0,0,0,0));
		m_mainListDlg.MoveWindow(CRect(0,0,0,0));

		m_logonDlg.ShowWindow(SW_SHOW);
		m_dockDlg.ShowWindow(SW_SHOW);
		m_mainListDlg.ShowWindow(SW_SHOW);
		m_circleDlg.ShowWindow(SW_SHOW);

		RepositionSubDlg(TRUE);
		///所有窗口的初始属性都是设置为隐藏状态的，为了显示效果
	}


	//设置鼠标勾子确保对话框正常隐藏
	m_pSizeWnd = this;
	//m_hWndProcHook = ::SetWindowsHookEx(WH_CALLWNDPROC, CRIM30Dlg::WndProcHook, GetModuleHandle(NULL), GetCurrentThreadId());
	//m_hWndProcHook = ::SetWindowsHookEx(WH_MOUSE_LL, CRIM30Dlg::MouseHook, GetModuleHandle(NULL), 0);

	if(RIMConfig::GetInstance()->m_pLogonUser == NULL && RIMConfig::GetInstance()->m_systemSet.bAutoLogon)
		m_logonDlg.Logon(TRUE);
}

void CRIM30Dlg::RepositionSubDlg( bool bRedraw /*= false*/ )
{//这里可心在调整位置之后，调用子窗口的RepositionSubDlg函数。

	if (m_logonDlg.GetSafeHwnd())
	{
		DWORD redrawFlag = bRedraw ? 0 : SWP_NOREDRAW;
		CRect mainWndRect, rectTemp;
		GetDlgItem(IDC_P_MAINWND)->GetWindowRect(mainWndRect);

		CRect wndRect;
		GetWindowRect(wndRect);
		BOOL bUp = mainWndRect.bottom > (wndRect.bottom - 10);
		mainWndRect.bottom = wndRect.bottom - 10;
		rectTemp = mainWndRect;
		ScreenToClient(rectTemp);
		GetDlgItem(IDC_P_MAINWND)->MoveWindow(rectTemp);

		HDWP hdwp = BeginDeferWindowPos(3);

		CRect circleRect;
		GetDlgItem(IDC_P_CIRCLE)->GetWindowRect(circleRect);
		DeferWindowPos(hdwp, m_circleDlg.GetSafeHwnd(), wndTop, circleRect.left, circleRect.top, circleRect.Width(), circleRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | redrawFlag);

		DeferWindowPos(hdwp, m_logonDlg.GetSafeHwnd(), wndTop, mainWndRect.left, mainWndRect.top, mainWndRect.Width(), mainWndRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | redrawFlag);
		DeferWindowPos(hdwp, m_mainListDlg.GetSafeHwnd(), wndTop, mainWndRect.left, mainWndRect.top, mainWndRect.Width(), mainWndRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | redrawFlag);

		UpdateView(m_wndAlphaValue);
		EndDeferWindowPos(hdwp);
	}
}

bool CRIM30Dlg::Exit()
{
	{ //退出前相关提示
		BOOL bHasChat = FALSE;
		for (UINT i = 0; i < RIMConfig::GetInstance()->m_userArray.size(); ++i)
		{
			LPUSERINFO pUserInfo = RIMConfig::GetInstance()->m_userArray.at(i);
			if (pUserInfo && pUserInfo->pChatThread)
			{
				bHasChat = TRUE;	break;
			}
		}

		BOOL bHasSubSys = (GetRIM30Server()->GetSubSystemCnt() != 0);

		if(bHasChat && bHasSubSys)
		{
			if(IDNO == MessageBox(_T("有聊天窗口及相关子系统尚未关闭，是否要关闭程序?"), _T("RIM3.0 提示"), MB_YESNO))
				return false;
		}
		else if (bHasChat)
		{
			if(IDNO == MessageBox(_T("有聊天窗口尚未关闭，是否要关闭程序?"), _T("RIM3.0 提示"), MB_YESNO))
				return false;
		}
		else if(bHasSubSys)
		{
			if(IDNO == MessageBox(_T("有子系统尚未关闭，是否要关闭程序?"), _T("RIM3.0 提示"), MB_YESNO))
				return false;
		}
	}

	return m_logonDlg.Logon(FALSE);
}

BOOL CRIM30Dlg::PtInWindow(CPoint scrPoint)
{
	ScreenToClient(&scrPoint);
	return m_MainPixelRgn.PtInRegion(scrPoint);
}

BOOL CRIM30Dlg::PtInDock(CPoint scrPoint)
{
	BOOL bRet = FALSE;

	CRect dockRect;
	m_dockDlg.GetWindowRect(dockRect);

	if(dockRect.PtInRect(scrPoint))
		bRet = TRUE;

	return bRet;
}

void CRIM30Dlg::ShowMode(SHOWOPERA shwoOpera, BOOL bShow)
{
	switch (shwoOpera){

	case SHOWOPERA_DOCK:
		{
			//目前只能做主界面的动画效果

			int alphaValue	= -1;
			int alphaStep	= -1;
			if (bShow == FALSE)
			{
				alphaValue = 0;
				alphaStep = 5;
			}
			else if (bShow == TRUE)
			{
				alphaValue = 255;
				alphaStep = -5;
			}

			while (alphaValue >= 0 && alphaValue <= 255)
			{
				if (m_bAlreadyLogon)
					UpdateView(alphaValue);
				else
					UpdateView(alphaValue);

				if (m_bShowMainImg)	//根据图片判断，如果图片不显示，则在该图片区域的所有窗口均不能显示
				{
					if (m_bAlreadyLogon)
						m_mainListDlg.SetLayeredAttributes(g_transAlphaClr, alphaValue, LWA_COLORKEY | LWA_ALPHA);
					else
						m_logonDlg.SetLayeredAttributes(g_transAlphaClr, alphaValue, LWA_COLORKEY | LWA_ALPHA);
				}

				m_circleDlg.SetLayeredAttributes(g_transAlphaClr, alphaValue, LWA_COLORKEY | LWA_ALPHA);

				m_dockDlg.UpdateView(m_pImgLogoTemp, 255 - alphaValue);	//停靠窗口始终要隐藏，
				alphaValue += alphaStep;

				MSG msg;
				while (::PeekMessage(&msg, m_hWnd, 0, 0, PM_NOREMOVE))
				{	//增强用户鼠标移动体验
					if (msg.message != WM_SHOWDOCKBOUND)
					{
						::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE);
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
					else
					{
						break;
					}
				}
			}
		}
		break;
	case SHOWOPERA_MENU:
		{
			if (m_bShowMainImg)//根据图片判断，如果图片不显示，则在该图片区域的所有窗口均不能显示
			{
				if (m_bAlreadyLogon)
					m_mainListDlg.SetLayeredAttributes(g_transAlphaClr, bShow ? 255 : 0, LWA_COLORKEY | LWA_ALPHA);
				else
					m_logonDlg.SetLayeredAttributes(g_transAlphaClr, bShow ? 255 : 0, LWA_COLORKEY | LWA_ALPHA);
			}
			m_circleDlg.SetLayeredAttributes(g_transAlphaClr, bShow ? 255 : 0, LWA_COLORKEY | LWA_ALPHA);

			if (m_bAlreadyLogon)
				UpdateView(bShow ? 255 : 0);
			else
				UpdateView(bShow ? 255 : 0);

			m_dockDlg.UpdateView(m_pImgLogoTemp, 0);	//停靠窗口始终要隐藏，
		}
		break;
	case SHOWOPERA_MAIN:
		{
			if (m_bAlreadyLogon)
			{
				m_mainListDlg.SetLayeredAttributes(g_transAlphaClr, bShow ? 255 : 0, LWA_COLORKEY | LWA_ALPHA);
				UpdateView(255);
			}
			else
			{
				m_logonDlg.SetLayeredAttributes(g_transAlphaClr, bShow ? 255 : 0, LWA_COLORKEY | LWA_ALPHA);
				UpdateView(255);
			}
		}
		break;
	case SHOWOPERA_LOGON:
		{
			if(m_bAlreadyLogon)
			{
				m_mainListDlg.RedrawWindow();	//重画DC
				if(m_wndAlphaValue == 255)		//整个界面显示时才做更新操作
				{
					m_logonDlg.SetLayeredAttributes(g_transAlphaClr, 0, LWA_COLORKEY | LWA_ALPHA);
					m_mainListDlg.SetLayeredAttributes(g_transAlphaClr, 255, LWA_COLORKEY | LWA_ALPHA);
					UpdateView(255);
				}
			}
			else
			{
				m_bShowMainImg = TRUE;			//退出时要显示主界面
				m_logonDlg.RedrawWindow();		//重画DC
				if(m_wndAlphaValue == 255)		//整个界面显示时才做更新操作
				{
					m_logonDlg.SetLayeredAttributes(g_transAlphaClr, 255, LWA_COLORKEY | LWA_ALPHA);
					m_mainListDlg.SetLayeredAttributes(g_transAlphaClr, 0, LWA_COLORKEY | LWA_ALPHA);
					UpdateView(255);
				}
			}
		}
		break;
	}

}

BOOL CRIM30Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
 
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	ModifyStyle(WS_CAPTION | WS_CHILD, WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX);
	ModifyStyleEx(0, WS_EX_LAYERED | WS_OVERLAPPED);

	Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRIM30Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC dc(this); // 用于绘制的设备上下文

		CRect rcClient;
		GetClientRect(&rcClient);

		//Graphics graphics(this);

		{
			// 		GraphicsPath graphicspath;
			// 		{
			// 			graphicspath.AddArc(right-offsetX, top, offsetX, offsetY, 270, 90);
			// 			graphicspath.AddArc(right-offsetX, bottom-offsetY, offsetX, offsetY, 0, 90);
			// 			graphicspath.AddArc(left, bottom - offsetY, offsetX, offsetY, 90, 90);
			// 			graphicspath.AddArc(left, top, offsetX, offsetY, 180, 90);
			// 			graphicspath.AddLine(left + offsetX, top, right - offsetX/2, top);
			// 		}
			
		}




	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRIM30Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRIM30Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	if(RIMConfig::GetInstance()->m_pLogonUser == NULL)
		m_logonDlg.Logon(TRUE);
}

void CRIM30Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (Exit())
		OnCancel();
}

HBRUSH CRIM30Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired

	return hbr;
}

BOOL CRIM30Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

void CRIM30Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	RepositionSubDlg(FALSE);
}

void CRIM30Dlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	//if (GetDlgItem(IDC_P_MAINWND))
	//{	//效率处理
	//	CRect wndRect;
	//	GetWindowRect(wndRect);
	//	HDWP hdwp = BeginDeferWindowPos(3);

	//	CRect mainWndRect;
	//	GetDlgItem(IDC_P_MAINWND)->GetWindowRect(mainWndRect);
	//	//mainWndRect.DeflateRect(2, 2);	///计算登录对话框和列表窗口的位置且留出边框

	//	ScreenToClient(mainWndRect);
	//	mainWndRect.left		= wndRect.left + mainWndRect.left;
	//	mainWndRect.top		= wndRect.top + mainWndRect.top;
	//	mainWndRect.right 	= wndRect.left + mainWndRect.right;
	//	mainWndRect.bottom	= wndRect.top + mainWndRect.bottom;
	//	DeferWindowPos(hdwp, m_logonDlg.GetSafeHwnd(), wndTop, mainWndRect.left, mainWndRect.top, mainWndRect.Width(), mainWndRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOREDRAW);
	//	DeferWindowPos(hdwp, m_mainListDlg.GetSafeHwnd(), wndTop, mainWndRect.left, mainWndRect.top, mainWndRect.Width(), mainWndRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOREDRAW);

	//	CRect circleRect;
	//	circleRect.left		= wndRect.left + m_clientcircleRect.left;
	//	circleRect.top		= wndRect.top + m_clientcircleRect.top;
	//	circleRect.right 	= wndRect.left + m_clientcircleRect.right;
	//	circleRect.bottom	= wndRect.top + m_clientcircleRect.bottom;

	//	DeferWindowPos(hdwp, m_circleDlg.GetSafeHwnd(), wndTop, circleRect.left, circleRect.top, circleRect.Width(), circleRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOREDRAW);
	//	EndDeferWindowPos(hdwp);
	//}

	// TODO: Add your message handler code here
}

void CRIM30Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(m_mainListDlg.m_wndAlphaValue == 255)
	{
		if(m_rcStation.Contains(point.x, point.y))
		{
			m_mainListDlg.ShowStationOrUserByFlag(m_selMode = 0);
			UpdateView();
			return;
		}
		else if(m_rcUser.Contains(point.x, point.y))
		{
			m_mainListDlg.ShowStationOrUserByFlag(m_selMode = 1);
			UpdateView();
			return;
		}
		else if(m_rcRecent.Contains(point.x, point.y))
		{
			m_mainListDlg.ShowStationOrUserByFlag(m_selMode = 2);
			UpdateView();
			return;
		}
	}
	
	ClientToScreen(&point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

void CRIM30Dlg::UpdateView(byte Alpha /*= 255*/)
{
	////【zy 2015-02-12 ↓】	//自动判断当前应当显示状态
	Image* pImageMain = NULL, *pImageSizeMid = NULL, *pImageSizeBttom = NULL, *pImageCircle = NULL, *pImageLogo = NULL;
	Image* pImgChkSel = NULL, *pImgChkUnSel = NULL;

	{	
		if(m_bShowMainImg)				//是否显示主界面
		{	
			if (m_bAlreadyLogon)		//是否已经登录
				pImageMain = m_pImgMainListTemp;
			else
				pImageMain = m_pImgLogonTemp;
		}
		else
			pImageMain = NULL;


		pImageSizeMid		= m_pImgSizeMidTemp;
		pImageSizeBttom 	= m_pImgSizebottomTemp;
		pImageCircle		= m_pImgCircleTemp;
		pImageLogo			= m_pImgLogoTemp;
		pImgChkSel			= m_pImgChkSelTemp;
		pImgChkUnSel		= m_pImgChkUnSelTemp;
	}
	////【zy 2015-02-12 ↑】

	m_wndAlphaValue = Alpha;

	CRect rcClient;
	GetClientRect(&rcClient);

	CRect mainRect, circleRect, logoRect;
	GetDlgItem(IDC_P_MAINWND)->GetWindowRect(mainRect);
	GetDlgItem(IDC_P_CIRCLE)->GetWindowRect(circleRect);
	GetDlgItem(IDC_P_LOGO)->GetWindowRect(logoRect);

	ScreenToClient(mainRect);
	ScreenToClient(circleRect);
	ScreenToClient(logoRect);
	m_clientRect.UnionRect(&mainRect, &circleRect);


	CDC			*pDC = GetDC();

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SelectObject(GetFont());

	Graphics graphics(memDC.GetSafeHdc());
	ImageAttributes imageattributes;
	imageattributes.SetWrapMode(WrapModeTile);


	Rect imgMainRect	= Rect(0, 0, 0, 0);
	if (pImageMain)
	{
		imgMainRect = Rect(mainRect.left, mainRect.top, pImageMain->GetWidth(), pImageMain->GetHeight());
		graphics.DrawImage(pImageMain, imgMainRect);	//画主界面的上部分

		if(mainRect.Height() > int(pImageMain->GetHeight() + 10))
		{
			//画主界面的中间拉伸部分
 			Rect imgSizeMidRect = Rect(imgMainRect.GetLeft(), imgMainRect.GetBottom(), pImageSizeBttom->GetWidth(), mainRect.Height() - pImageMain->GetHeight() - pImageSizeBttom->GetHeight());
 			graphics.DrawImage(pImageSizeMid, imgSizeMidRect, 0, 0, pImageSizeMid->GetWidth(), pImageSizeMid->GetHeight(), UnitPixel, &imageattributes);

			//画主界面的下底边框部分
			Rect imgSizeBttomRect = Rect(imgMainRect.GetLeft(), mainRect.bottom - pImageSizeBttom->GetHeight(), pImageSizeBttom->GetWidth(), pImageSizeBttom->GetHeight());
			graphics.DrawImage(pImageSizeBttom, imgSizeBttomRect, 0, 0, pImageSizeBttom->GetWidth(), pImageSizeBttom->GetHeight(), UnitPixel, &imageattributes);
			
			if (m_mainListDlg.m_wndAlphaValue > 0)
			{	//用户列表显示
				int height = 28;
				m_rcStation = Rect(mainRect.left,							mainRect.bottom - height, mainRect.Width()/3 - 1, height); m_rcStation.Offset(1, 0);
				m_rcUser	= Rect(mainRect.left + mainRect.Width()/3,		mainRect.bottom - height, mainRect.Width()/3, height);
				m_rcRecent	= Rect(mainRect.left + mainRect.Width() * 2/3,	mainRect.bottom - height, mainRect.Width()/3 - 1, height);
				
				if(m_selMode == 0)
				{
					graphics.DrawImage(pImgChkSel, m_rcStation, 0, 0, pImgChkSel->GetWidth(), pImgChkSel->GetHeight(), UnitPixel, &imageattributes);
					graphics.DrawImage(pImgChkUnSel, m_rcUser, 0, 0, pImgChkUnSel->GetWidth(), pImgChkUnSel->GetHeight(), UnitPixel, &imageattributes);
					graphics.DrawImage(pImgChkUnSel, m_rcRecent, 0, 0, pImgChkUnSel->GetWidth(), pImgChkUnSel->GetHeight(), UnitPixel, &imageattributes);
				}
				else if(m_selMode == 1)
				{
					graphics.DrawImage(pImgChkUnSel, m_rcStation, 0, 0, pImgChkUnSel->GetWidth(), pImgChkUnSel->GetHeight(), UnitPixel, &imageattributes);
					graphics.DrawImage(pImgChkSel, m_rcUser, 0, 0, pImgChkSel->GetWidth(), pImgChkSel->GetHeight(), UnitPixel, &imageattributes);
					graphics.DrawImage(pImgChkUnSel, m_rcRecent, 0, 0, pImgChkUnSel->GetWidth(), pImgChkUnSel->GetHeight(), UnitPixel, &imageattributes);
				}
				else if(m_selMode == 2)
				{
					graphics.DrawImage(pImgChkUnSel, m_rcStation, 0, 0, pImgChkUnSel->GetWidth(), pImgChkUnSel->GetHeight(), UnitPixel, &imageattributes);
					graphics.DrawImage(pImgChkUnSel, m_rcUser, 0, 0, pImgChkUnSel->GetWidth(), pImgChkUnSel->GetHeight(), UnitPixel, &imageattributes);
					graphics.DrawImage(pImgChkSel, m_rcRecent, 0, 0, pImgChkSel->GetWidth(), pImgChkSel->GetHeight(), UnitPixel, &imageattributes);
				}

				StringFormat stringformat;
				stringformat.SetAlignment(StringAlignmentCenter);
				stringformat.SetLineAlignment(StringAlignmentCenter);
				graphics.DrawString(CStringW("监测站"), CStringW("监测站").GetLength(), 
									&Gdiplus::Font(memDC.GetSafeHdc()), 
									RectF(float(m_rcStation.X), float(m_rcStation.Y), float(m_rcStation.Width), float(m_rcStation.Height)), 
									&stringformat, &SolidBrush(Color(254, 255, 255 , 255)));
				graphics.DrawString(CStringW("联系人"), CStringW("联系人").GetLength(), 
									&Gdiplus::Font(memDC.GetSafeHdc()), 
									RectF(float(m_rcUser.X), float(m_rcUser.Y), float(m_rcUser.Width), float(m_rcUser.Height)), 
									&stringformat, &SolidBrush(Color(254, 255, 255 , 255)));
				graphics.DrawString(CStringW("最近使用"), CStringW("最近使用").GetLength(), 
					&Gdiplus::Font(memDC.GetSafeHdc()), 
					RectF(float(m_rcRecent.X), float(m_rcRecent.Y), float(m_rcRecent.Width), float(m_rcRecent.Height)), 
					&stringformat, &SolidBrush(Color(254, 255, 255 , 255)));
			}
		}
	}
	Rect imgCircleRect	= Rect(circleRect.left, circleRect.top, pImageCircle->GetWidth(),	pImageCircle->GetHeight());
	Rect imgLogoRect	= Rect(logoRect.left,	logoRect.top,	pImageLogo->GetWidth(),		pImageLogo->GetHeight());
	graphics.DrawImage(pImageCircle, imgCircleRect);	//画圆环
	graphics.DrawImage(pImageLogo, imgLogoRect);		//画圆环中的小圆图片


	{	//重新计算当前主界面显示的像素区域
		m_MainPixelRgn.DeleteObject();
		m_MainPixelRgn.CreateRectRgn(0, 0, 0, 0);

		if (pImageMain)
		{
			CRgn cirRgn, mainRgn;
			cirRgn.CreateEllipticRgn(imgCircleRect.X, imgCircleRect.Y, imgCircleRect.GetRight(), imgCircleRect.GetBottom());
			mainRgn.CreateRectRgn(mainRect.left, mainRect.top, mainRect.right, mainRect.bottom);
			m_MainPixelRgn.CombineRgn(&cirRgn, &mainRgn, RGN_OR);

			cirRgn.DeleteObject();
			mainRgn.DeleteObject();
		}
		else
		{
			CRgn *pRgnCircle = CRgn::FromHandle(m_hRgnCircle);
			CRgn *pRgnLogo = CRgn::FromHandle(m_hRgnLogo);
			m_MainPixelRgn.CombineRgn(pRgnCircle, pRgnLogo, RGN_OR);
		}
	}

	//graphics.FillRegion(&SolidBrush(Color::Green), Region::FromHRGN(m_MainPixelRgn));

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	BLENDFUNCTION stBlend = { AC_SRC_OVER, 0, Alpha, AC_SRC_ALPHA };
	::UpdateLayeredWindow(m_hWnd, pDC->GetSafeHdc(), &CPoint(rcWindow.left, rcWindow.top), &CSize(rcWindow.Width(), rcWindow.Height()), 
						  memDC.GetSafeHdc(), &CPoint(0, 0), 0, &stBlend, ULW_ALPHA);
	ReleaseDC(pDC);

	graphics.ReleaseHDC(memDC.GetSafeHdc());
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CRIM30Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseMove(nFlags, point);
}

void CRIM30Dlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(nHitTest == HTCAPTION)
	{
		ScreenToClient(&point);
		if (!m_MainPixelRgn.PtInRegion(point))
			return;

		SetCapture();

		CPoint cursorPos;
		GetCursorPos(&cursorPos);
		MSG msg;
		while(::GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.hwnd == m_hWnd)
			{
				if(msg.message == WM_MOUSEMOVE)
				{
					CPoint ptCur = CPoint(0, 0);
					GetCursorPos(&ptCur);

					int offsetX = ptCur.x - cursorPos.x;
					int offsetY = ptCur.y - cursorPos.y;
					CRect wndRect;
					GetWindowRect(wndRect);
					wndRect.OffsetRect(offsetX, offsetY);

					CRect mainRect = CRect(wndRect.left + m_clientRect.left, wndRect.top + m_clientRect.top, wndRect.left + m_clientRect.right, wndRect.top + m_clientRect.bottom);
					if(m_bShowMainImg == NULL)	//主窗口不显示
						mainRect = CRect(wndRect.left + m_clientcircleRect.left, wndRect.top + m_clientcircleRect.top, wndRect.left + m_clientcircleRect.right, wndRect.top + m_clientcircleRect.bottom);


					CRect screenRc;
					SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&screenRc, 0);
					if(mainRect.top < screenRc.top)
					{
						wndRect.OffsetRect(0, screenRc.top - mainRect.top);
					}
					if(mainRect.bottom > screenRc.bottom)
					{
						if(offsetY > 10)	//避免迅速移动时造成用户体验不佳
							wndRect.OffsetRect(0, screenRc.bottom - mainRect.bottom);
						else if(offsetY > 0)//这里特殊处理，当处于折叠（只可能下底超出屏幕），时禁止向下移动
							wndRect.OffsetRect(0, 0 - offsetY);
					}
					if(mainRect.left < screenRc.left)
					{
						if(offsetX < 10)	//避免迅速移动时造成用户体验不佳
							wndRect.OffsetRect(screenRc.left - mainRect.left, 0);
						else if(offsetX < 0)	//这里特殊处理，当处于折叠（只可能下底超出屏幕），时禁止向下移动
							wndRect.OffsetRect(0 - offsetX, 0);
					}
					if(mainRect.right > screenRc.right)
					{
						wndRect.OffsetRect(screenRc.right - mainRect.right, 0);
					}

					{	//效率处理
						HDWP hdwp = BeginDeferWindowPos(4);

						DeferWindowPos(hdwp, GetSafeHwnd(), NULL, wndRect.left, wndRect.top, wndRect.Width(), wndRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOREDRAW);

						CRect mainWndRect;
						GetDlgItem(IDC_P_MAINWND)->GetWindowRect(mainWndRect);
						//mainWndRect.bottom -= 10;	///计算登录对话框和列表窗口的位置且留出边框

						ScreenToClient(mainWndRect);
						mainWndRect.left	= wndRect.left + mainWndRect.left;
						mainWndRect.top		= wndRect.top + mainWndRect.top;
						mainWndRect.right 	= wndRect.left + mainWndRect.right;
						mainWndRect.bottom	= wndRect.top + mainWndRect.bottom;
						DeferWindowPos(hdwp, m_logonDlg.GetSafeHwnd(), wndTop, mainWndRect.left, mainWndRect.top, mainWndRect.Width(), mainWndRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOREDRAW);
						DeferWindowPos(hdwp, m_mainListDlg.GetSafeHwnd(), wndTop, mainWndRect.left, mainWndRect.top, mainWndRect.Width(), mainWndRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOREDRAW);

						CRect circleRect;
						circleRect.left		= wndRect.left + m_clientcircleRect.left;
						circleRect.top		= wndRect.top + m_clientcircleRect.top;
						circleRect.right 	= wndRect.left + m_clientcircleRect.right;
						circleRect.bottom	= wndRect.top + m_clientcircleRect.bottom;

						DeferWindowPos(hdwp, m_circleDlg.GetSafeHwnd(), wndTop, circleRect.left, circleRect.top, circleRect.Width(), circleRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOREDRAW);
						EndDeferWindowPos(hdwp);			
					}
					cursorPos = ptCur;
				}
				else if(msg.message == WM_LBUTTONUP)
				{
					break;
				}
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	//鼠标左键不按下时，及时跳出循环
				break;
		}
	}
	else if (nHitTest == HTBOTTOM)
	{
		BOOL bDefaultDrag = FALSE;
		SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, (LPVOID)&bDefaultDrag, 0);

		if (bDefaultDrag == FALSE)
			SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
		if (bDefaultDrag == FALSE)
			SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, 0, 0);
	}

	ReleaseCapture();

	//CDialog::OnNcLButtonDown(nHitTest, point);
}

afx_msg LRESULT CRIM30Dlg::OnShowMianWnd(WPARAM wParam, LPARAM lParam)
{
	CPoint ptCur = CPoint(0, 0);
	GetCursorPos(&ptCur);
	ScreenToClient(&ptCur);

	if (PtInRegion(m_hRgnLogo, ptCur.x, ptCur.y))
	{
		if (m_logonDlg.m_wndAlphaValue == 255 || m_mainListDlg.m_wndAlphaValue == 255)
		{
			m_bShowMainImg = FALSE;
			ShowMode(SHOWOPERA_MAIN, FALSE);
		}
		else
		{
			m_bShowMainImg = TRUE;
			ShowMode(SHOWOPERA_MAIN, TRUE);
		}
	}

	return TRUE;
}

afx_msg LRESULT CRIM30Dlg::OnShowDockBound(WPARAM wParam, LPARAM lParam)
{
	{	//判断可停靠的类型
		long cxScreen = GetSystemMetrics(SM_CXSCREEN);
		long cyScreen = GetSystemMetrics(SM_CYSCREEN);

		CPoint ptCur = CPoint(0, 0);
		GetCursorPos(&ptCur);

		CRect wndRect;
		GetWindowRect(wndRect);

		CRect mainRect = CRect(wndRect.left + m_clientRect.left, wndRect.top + m_clientRect.top, wndRect.left + m_clientRect.right, wndRect.top + m_clientRect.bottom);
		if(m_bShowMainImg == NULL)	//主窗口不显示
			mainRect = CRect(wndRect.left + m_clientcircleRect.left, wndRect.top + m_clientcircleRect.top, wndRect.left + m_clientcircleRect.right, wndRect.top + m_clientcircleRect.bottom);

		CRect screenRc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&screenRc, 0);
		m_dockType = DOCK_NORMAL;	//保证除任务栏以外的其它地方都可停靠
		if(mainRect.top <= screenRc.top && screenRc.top <= 0)//在顶端
		{
			m_dockType = DOCK_TOP;
		}
		if(mainRect.bottom >= screenRc.bottom && screenRc.bottom >= cyScreen)
		{
			m_dockType = DOCK_BOTTOM;
		}
		if(mainRect.left <= screenRc.left && screenRc.left <= 0)//在左端
		{
			m_dockType = DOCK_LEFT;
		}
		if(mainRect.right >= screenRc.right && screenRc.right >= cxScreen)
		{
			m_dockType = DOCK_RIGHT;
		}
	}


	CPoint ptCur = CPoint(0, 0);
	GetCursorPos(&ptCur);

	if (PtInDock(ptCur) || PtInWindow(ptCur) && m_wndAlphaValue == 255)
		m_dockType = DOCK_NORMAL;

		//if (m_dockType != DOCK_NORMAL && m_wndAlphaValue == 255)	//m_dockType != DOCK_NORMAL 说明在可停靠的边上
	//{
	//	if (PtInWindow(ptCur) || PtInDock(ptCur))
	//		m_dockType = DOCK_NORMAL;
	//}
	//else if (m_dockDlg.m_wndAlphaValue == 255)	//如果处于已停靠的状态
	//{
	//	if (PtInDock(ptCur))
	//		m_dockType = DOCK_NORMAL;
	//}

	if (m_dockType != DOCK_NORMAL)//鼠标不在区域内，同时又依靠在边上
	{
		if (m_dockDlg.GetSafeHwnd())
		{
			CRect logoRect;
			GetDlgItem(IDC_P_LOGO)->GetWindowRect(logoRect);
			CRect dockRect;
			m_dockDlg.GetWindowRect(dockRect);

			switch (m_dockType)
			{
			case DOCK_LEFT:
				m_dockDlg.SetWindowPos(&wndTopMost, 0 - dockRect.Width() * 4/ 5, logoRect.top, 0, 0, SWP_NOSIZE);
				break;
			case DOCK_TOP:
				m_dockDlg.SetWindowPos(&wndTopMost, logoRect.left, 0 - dockRect.Width() * 4 / 5, 0, 0, SWP_NOSIZE);
				break;
			case DOCK_RIGHT:
				{
					DWORD cxScreen = GetSystemMetrics(SM_CXSCREEN);
					m_dockDlg.SetWindowPos(&wndTopMost, cxScreen - dockRect.Width() * 1 / 5, logoRect.top, 0, 0, SWP_NOSIZE);
				}
				break;
			case DOCK_BOTTOM:
				{
					DWORD cyScreen = GetSystemMetrics(SM_CYSCREEN);
					m_dockDlg.SetWindowPos(&wndTopMost, logoRect.left, cyScreen - dockRect.Width() * 1 / 5, 0, 0, SWP_NOSIZE);
				}
				break;
			}
		}

		if (m_dockDlg.m_wndAlphaValue == 0)
			ShowMode(SHOWOPERA_DOCK, TRUE);
	}
	else
	{
		if (m_dockDlg.m_wndAlphaValue == 255)
			ShowMode(SHOWOPERA_DOCK, FALSE);
	}

	return TRUE;
}

LRESULT CRIM30Dlg::OnAlredyLogon(WPARAM wParam, LPARAM lParam)
{
	m_bAlreadyLogon = BOOL(wParam);

	m_mainListDlg.DoLogon(m_bAlreadyLogon, m_selMode = 0);//初次登录，列表显示界面重置
	m_circleDlg.DoLogon(m_bAlreadyLogon);
	ShowMode(SHOWOPERA_LOGON, m_bAlreadyLogon);
	return TRUE;
}

LRESULT CRIM30Dlg::OnDisplayChange(WPARAM BitsPerPixel, LPARAM size)
{
	//CWnd::OnDisplayChange(BitsPerPixel, size);
	return TRUE;
	RepositionSubDlg();
	return TRUE;
}

LRESULT CRIM30Dlg::OnIconMsg(WPARAM wParam, LPARAM lParam)
{	//响应托盘图标上的消息
	if(wParam = IDR_MAINFRAME)
	{
		CRect rect;
		POINT point;
		HMENU hmenu;
		switch(lParam)
		{
		case WM_RBUTTONUP:
			GetCursorPos(&point);
			SetForegroundWindow();//更新托盘图标菜单显示
			hmenu = CreatePopupMenu();
			//AppendMenu(hmenu, MF_SEPARATOR, 0, NULL);
			AppendMenu(hmenu, MF_STRING, WM_ICONOPEN,		ICONOPEN_STR);
			AppendMenu(hmenu, MF_STRING, WM_ICONSET,		ICONSET_STR);
			AppendMenu(hmenu, MF_STRING, WM_ICONSHOWMINWND, ICONSHOWMINWND_STR);
			AppendMenu(hmenu, MF_STRING, WM_ICONEXITUSER,	ICONEXITUSER_STR);
			AppendMenu(hmenu, MF_STRING, WM_ICONEXIT,		ICONEXIT_STR);
			GetWindowRect(&rect);
			TrackPopupMenu(hmenu, TPM_LEFTALIGN, point.x, point.y, 0, m_hWnd, NULL);

			DeleteObject(hmenu);
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_LBUTTONDOWN:
			ShowMode(SHOWOPERA_MENU, TRUE);
			SetForegroundWindow();
			break;
		case WM_LBUTTONDBLCLK:
			ShowMode(SHOWOPERA_MENU, TRUE);
			SetForegroundWindow();

			m_mainListDlg.ShowAllMsg();
			break;
		case NIN_BALLOONSHOW:
			m_bBalloonShow = TRUE;
			break;
		case NIN_BALLOONTIMEOUT:
		case NIN_BALLOONHIDE:
			m_bBalloonShow = FALSE;
			break;
		case NIN_BALLOONUSERCLICK:
			m_mainListDlg.ShowAllMsg();
			break;
		default:
			break;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRIM30Dlg::OnCommand(WPARAM wParam, LPARAM lParam)
{	//托盘图标菜单消息
	// TODO: Add your specialized code here and/or call the base class

	switch(LOWORD(wParam))
	{
	case WM_ICONEXIT://弹出菜单产生的消息是Notify消息
		OnBnClickedCancel();
		break;
	case WM_ICONEXITUSER://弹出菜单产生的消息是Notify消息
		{ //退出前相关提示
			if (Exit())
				m_logonDlg.Logon(FALSE);
		}
		break;
	case WM_ICONOPEN:
		{
			ShowMode(SHOWOPERA_MENU, TRUE);
		}
		SetForegroundWindow();
		break;
	case WM_ICONSET:
		{
			if(RIMConfig::GetInstance()->m_pLogonUser)
			{
				m_circleDlg.EnableWindow(FALSE);
				m_logonDlg.EnableWindow(FALSE);
				m_mainListDlg.EnableWindow(FALSE);

				CDialogSet dlgSet;
				dlgSet.DoModal();

				m_circleDlg.EnableWindow(TRUE);
				m_logonDlg.EnableWindow(TRUE);
				m_mainListDlg.EnableWindow(TRUE);
			}
		}
		break;
	case WM_ICONSHOWMINWND:
		{
			ShowMode(SHOWOPERA_MENU, FALSE);
		}
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CRIM30Dlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (PtInRegion(m_hRgnLogo, point.x, point.y))
	{
		PostMessage(WM_SHOWMAINWND, 0, 0);
	}

	//CDialog::OnLButtonDblClk(nFlags, point);
}

void CRIM30Dlg::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	//当鼠标离开可显示区域时，发送消息到父窗口判断是否需要停靠到窗口边界上
	PostMessage(WM_SHOWDOCKBOUND, 0, 0);
}

void CRIM30Dlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here

	CPoint clientPt = point;
	ScreenToClient(&clientPt);
	if (PtInRegion(m_hRgnLogo, clientPt.x, clientPt.y))
	{
		CMenu poupoMenu;
		poupoMenu.CreatePopupMenu();

		poupoMenu.AppendMenu(MF_STRING, WM_ICONSET,			ICONSET_STR);
		poupoMenu.AppendMenu(MF_STRING, WM_ICONSHOWMINWND,	ICONSHOWMINWND_STR);
		poupoMenu.AppendMenu(MF_STRING, WM_ICONEXITUSER,	ICONEXITUSER_STR);
		poupoMenu.AppendMenu(MF_STRING, WM_ICONEXIT,		ICONEXIT_STR);

		poupoMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		poupoMenu.DestroyMenu();
	}
}


void CRIM30Dlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
}



LRESULT CRIM30Dlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CRect rect;
	GetDlgItem(IDC_P_MAINWND)->GetWindowRect(&rect);
	CRect rect1 = rect;
	rect1.DeflateRect(10, 10, -10, -10);
	rect1.NormalizeRect();

	if (m_mainListDlg.m_wndAlphaValue == 255 && point.y >= rect.bottom - 3)
		return HTBOTTOM;

	return CDialog::OnNcHitTest(point);
}

void CRIM30Dlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialog::OnNcLButtonDblClk(nHitTest, point);
}

void CRIM30Dlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
}

void CRIM30Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

 	if (lpMMI->ptMinTrackSize.y <= 600)  
 	{  
 		lpMMI->ptMinTrackSize.y = 600;  
 	}  

	CDialog::OnGetMinMaxInfo(lpMMI);
}

LRESULT CRIM30Dlg::OnUpdateConfig(WPARAM wParam, LPARAM lParam)
{
	m_circleDlg.ReUpdateFromConfig();

	return TRUE;
}

LRESULT CRIM30Dlg::OnUpdateUserList(WPARAM wParam, LPARAM lParam)
{
	if(m_mainListDlg.GetSafeHwnd())
		m_mainListDlg.UpdateUserListByMsg();

	return TRUE;
}

LRESULT CRIM30Dlg::OnUpdateStationStatus(WPARAM wParam, LPARAM lParam)
{
	if(m_mainListDlg.GetSafeHwnd())
		m_mainListDlg.UpdateStationStatustByMsg();

	return TRUE;
}

LRESULT CRIM30Dlg::OnFlickerNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	int makeFlack = (int)wParam;		//是否闪烁

//#define		WM_FLICKERNOTIFYICON					WM_USER + 9							//闪烁托盘图标		wParam 0 不闪烁图标   1 执行闪烁图标 2 消息通知，3文件消息通知 lParam  用户信息指针
	switch(makeFlack)
	{
	case 0:	//不闪烁
		if (m_icondata.hIcon == NULL)	//不再闪烁了
		{
			lstrcpy(m_icondata.szInfoTitle, _T(""));
			lstrcpy(m_icondata.szInfo, _T(""));
			m_icondata.hIcon				= m_hIcon;
			Shell_NotifyIcon(NIM_MODIFY, &m_icondata);//
		}
		break;
	case 1:	//要求闪烁
		if (FALSE == m_bBalloonShow && makeFlack == 1)						
		{	
			lstrcpy(m_icondata.szInfoTitle, _T(""));
			lstrcpy(m_icondata.szInfo, _T(""));

			if (m_icondata.hIcon)
				m_icondata.hIcon			= NULL;
			else
				m_icondata.hIcon			= m_hIcon;

			Shell_NotifyIcon(NIM_MODIFY, &m_icondata);//
		}
		break;
	case 2:	//聊天消息
		if(lParam)
		{
			LPUSERINFO lpUserInfo = (LPUSERINFO)lParam;

			m_icondata.hIcon			= m_hIcon;
			lstrcpy(m_icondata.szInfoTitle, _T("信息"));

			CString str;
			str.Format(_T("来自%s的信息."), lpUserInfo->UserID.c_str());
			lstrcpy(m_icondata.szInfo, str.GetBuffer());

			Shell_NotifyIcon(NIM_MODIFY, &m_icondata);//
		}
		break;
	case 3:	//文件消息
		if(lParam)
		{
			LPUSERINFO lpUserInfo = (LPUSERINFO)lParam;

			m_icondata.hIcon			= m_hIcon;
			lstrcpy(m_icondata.szInfoTitle, _T("信息"));

			CString str;
			str.Format(_T("来自%s的文件传输."), lpUserInfo->UserID.c_str());
			lstrcpy(m_icondata.szInfo, str.GetBuffer());

			Shell_NotifyIcon(NIM_MODIFY, &m_icondata);//
		}
		break;
	}

	return TRUE;
}

LRESULT CRIM30Dlg::OnRepositionSubdlg(WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, WM_REPOSITIONSUBDLG, WM_REPOSITIONSUBDLG, PM_REMOVE));

	RepositionSubDlg(FALSE);

	return TRUE;
}

void CRIM30Dlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (Exit())
		CDialog::OnClose();
}
 
void CRIM30Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000) && m_dockDlg.GetSafeHwnd())	//鼠标左键不按下时，
	{
		CPoint ptCur;	GetCursorPos(&ptCur);
		if (m_dockType != DOCK_NORMAL)		//认为当前状态为主界面隐藏状态
		{
			if (m_dockDlg.m_wndAlphaValue == 255 && PtInDock(ptCur))
				PostMessage(WM_SHOWDOCKBOUND);
			else if (m_dockDlg.m_wndAlphaValue == 0 && !(PtInWindow(ptCur) || PtInDock(ptCur)))
				PostMessage(WM_SHOWDOCKBOUND);
		}
		else if (m_dockType == DOCK_NORMAL) //认为当前状态主界面处于正常显示状态
		{
			if (!(PtInWindow(ptCur) || PtInDock(ptCur)))
				PostMessage(WM_SHOWDOCKBOUND);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
