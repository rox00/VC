//********************************************************************
//	RIM 3.0DLG.CPP �ļ�ע��
//	�ļ���		: RIM 3.0DLG.CPP
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	����		: ����
//	����ʱ��	: 2014/12/5 9:38
//	�ļ�����	: ������ʾ�����Ĵ���
//*********************************************************************	
// RIM30Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "RIM 3.0Dlg.h"
#include "ϵͳ����\DialogSet.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRIM30Dlg �Ի���

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
// CRIM30Dlg ��Ϣ�������

void CRIM30Dlg::Init()
{
	SetTimer(1, 500, NULL);
	{	//Ϊ��ʹԲ����LOGO���������λ���ϸ񱣳�һ��
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


	{	//�����ƶ��ı߽�����
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
		//Բ����ײ���������Բ��������ײ���ֵ������ȡһ���̶�ֵ �� �����������������������϶���λ�� 
		m_clientcircleRect.bottom -= 25;	

		m_clientRect.UnionRect(&mainRect, &circleRect);
	}


	{	//�������ͼ��

		
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

		Shell_NotifyIcon(NIM_ADD, &m_icondata);//�������ͼ��
	}

	UpdateView();

	{	//��ʼ�������ұ���ʾ
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
		///���д��ڵĳ�ʼ���Զ�������Ϊ����״̬�ģ�Ϊ����ʾЧ��
	}


	//������깴��ȷ���Ի�����������
	m_pSizeWnd = this;
	//m_hWndProcHook = ::SetWindowsHookEx(WH_CALLWNDPROC, CRIM30Dlg::WndProcHook, GetModuleHandle(NULL), GetCurrentThreadId());
	//m_hWndProcHook = ::SetWindowsHookEx(WH_MOUSE_LL, CRIM30Dlg::MouseHook, GetModuleHandle(NULL), 0);

	if(RIMConfig::GetInstance()->m_pLogonUser == NULL && RIMConfig::GetInstance()->m_systemSet.bAutoLogon)
		m_logonDlg.Logon(TRUE);
}

void CRIM30Dlg::RepositionSubDlg( bool bRedraw /*= false*/ )
{//��������ڵ���λ��֮�󣬵����Ӵ��ڵ�RepositionSubDlg������

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
	{ //�˳�ǰ�����ʾ
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
			if(IDNO == MessageBox(_T("�����촰�ڼ������ϵͳ��δ�رգ��Ƿ�Ҫ�رճ���?"), _T("RIM3.0 ��ʾ"), MB_YESNO))
				return false;
		}
		else if (bHasChat)
		{
			if(IDNO == MessageBox(_T("�����촰����δ�رգ��Ƿ�Ҫ�رճ���?"), _T("RIM3.0 ��ʾ"), MB_YESNO))
				return false;
		}
		else if(bHasSubSys)
		{
			if(IDNO == MessageBox(_T("����ϵͳ��δ�رգ��Ƿ�Ҫ�رճ���?"), _T("RIM3.0 ��ʾ"), MB_YESNO))
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
			//Ŀǰֻ����������Ķ���Ч��

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

				if (m_bShowMainImg)	//����ͼƬ�жϣ����ͼƬ����ʾ�����ڸ�ͼƬ��������д��ھ�������ʾ
				{
					if (m_bAlreadyLogon)
						m_mainListDlg.SetLayeredAttributes(g_transAlphaClr, alphaValue, LWA_COLORKEY | LWA_ALPHA);
					else
						m_logonDlg.SetLayeredAttributes(g_transAlphaClr, alphaValue, LWA_COLORKEY | LWA_ALPHA);
				}

				m_circleDlg.SetLayeredAttributes(g_transAlphaClr, alphaValue, LWA_COLORKEY | LWA_ALPHA);

				m_dockDlg.UpdateView(m_pImgLogoTemp, 255 - alphaValue);	//ͣ������ʼ��Ҫ���أ�
				alphaValue += alphaStep;

				MSG msg;
				while (::PeekMessage(&msg, m_hWnd, 0, 0, PM_NOREMOVE))
				{	//��ǿ�û�����ƶ�����
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
			if (m_bShowMainImg)//����ͼƬ�жϣ����ͼƬ����ʾ�����ڸ�ͼƬ��������д��ھ�������ʾ
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

			m_dockDlg.UpdateView(m_pImgLogoTemp, 0);	//ͣ������ʼ��Ҫ���أ�
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
				m_mainListDlg.RedrawWindow();	//�ػ�DC
				if(m_wndAlphaValue == 255)		//����������ʾʱ�������²���
				{
					m_logonDlg.SetLayeredAttributes(g_transAlphaClr, 0, LWA_COLORKEY | LWA_ALPHA);
					m_mainListDlg.SetLayeredAttributes(g_transAlphaClr, 255, LWA_COLORKEY | LWA_ALPHA);
					UpdateView(255);
				}
			}
			else
			{
				m_bShowMainImg = TRUE;			//�˳�ʱҪ��ʾ������
				m_logonDlg.RedrawWindow();		//�ػ�DC
				if(m_wndAlphaValue == 255)		//����������ʾʱ�������²���
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
 
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	ModifyStyle(WS_CAPTION | WS_CHILD, WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX);
	ModifyStyleEx(0, WS_EX_LAYERED | WS_OVERLAPPED);

	Init();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRIM30Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//{	//Ч�ʴ���
	//	CRect wndRect;
	//	GetWindowRect(wndRect);
	//	HDWP hdwp = BeginDeferWindowPos(3);

	//	CRect mainWndRect;
	//	GetDlgItem(IDC_P_MAINWND)->GetWindowRect(mainWndRect);
	//	//mainWndRect.DeflateRect(2, 2);	///�����¼�Ի�����б��ڵ�λ���������߿�

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
	////��zy 2015-02-12 ����	//�Զ��жϵ�ǰӦ����ʾ״̬
	Image* pImageMain = NULL, *pImageSizeMid = NULL, *pImageSizeBttom = NULL, *pImageCircle = NULL, *pImageLogo = NULL;
	Image* pImgChkSel = NULL, *pImgChkUnSel = NULL;

	{	
		if(m_bShowMainImg)				//�Ƿ���ʾ������
		{	
			if (m_bAlreadyLogon)		//�Ƿ��Ѿ���¼
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
	////��zy 2015-02-12 ����

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
		graphics.DrawImage(pImageMain, imgMainRect);	//����������ϲ���

		if(mainRect.Height() > int(pImageMain->GetHeight() + 10))
		{
			//����������м����첿��
 			Rect imgSizeMidRect = Rect(imgMainRect.GetLeft(), imgMainRect.GetBottom(), pImageSizeBttom->GetWidth(), mainRect.Height() - pImageMain->GetHeight() - pImageSizeBttom->GetHeight());
 			graphics.DrawImage(pImageSizeMid, imgSizeMidRect, 0, 0, pImageSizeMid->GetWidth(), pImageSizeMid->GetHeight(), UnitPixel, &imageattributes);

			//����������µױ߿򲿷�
			Rect imgSizeBttomRect = Rect(imgMainRect.GetLeft(), mainRect.bottom - pImageSizeBttom->GetHeight(), pImageSizeBttom->GetWidth(), pImageSizeBttom->GetHeight());
			graphics.DrawImage(pImageSizeBttom, imgSizeBttomRect, 0, 0, pImageSizeBttom->GetWidth(), pImageSizeBttom->GetHeight(), UnitPixel, &imageattributes);
			
			if (m_mainListDlg.m_wndAlphaValue > 0)
			{	//�û��б���ʾ
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
				graphics.DrawString(CStringW("���վ"), CStringW("���վ").GetLength(), 
									&Gdiplus::Font(memDC.GetSafeHdc()), 
									RectF(float(m_rcStation.X), float(m_rcStation.Y), float(m_rcStation.Width), float(m_rcStation.Height)), 
									&stringformat, &SolidBrush(Color(254, 255, 255 , 255)));
				graphics.DrawString(CStringW("��ϵ��"), CStringW("��ϵ��").GetLength(), 
									&Gdiplus::Font(memDC.GetSafeHdc()), 
									RectF(float(m_rcUser.X), float(m_rcUser.Y), float(m_rcUser.Width), float(m_rcUser.Height)), 
									&stringformat, &SolidBrush(Color(254, 255, 255 , 255)));
				graphics.DrawString(CStringW("���ʹ��"), CStringW("���ʹ��").GetLength(), 
					&Gdiplus::Font(memDC.GetSafeHdc()), 
					RectF(float(m_rcRecent.X), float(m_rcRecent.Y), float(m_rcRecent.Width), float(m_rcRecent.Height)), 
					&stringformat, &SolidBrush(Color(254, 255, 255 , 255)));
			}
		}
	}
	Rect imgCircleRect	= Rect(circleRect.left, circleRect.top, pImageCircle->GetWidth(),	pImageCircle->GetHeight());
	Rect imgLogoRect	= Rect(logoRect.left,	logoRect.top,	pImageLogo->GetWidth(),		pImageLogo->GetHeight());
	graphics.DrawImage(pImageCircle, imgCircleRect);	//��Բ��
	graphics.DrawImage(pImageLogo, imgLogoRect);		//��Բ���е�СԲͼƬ


	{	//���¼��㵱ǰ��������ʾ����������
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
					if(m_bShowMainImg == NULL)	//�����ڲ���ʾ
						mainRect = CRect(wndRect.left + m_clientcircleRect.left, wndRect.top + m_clientcircleRect.top, wndRect.left + m_clientcircleRect.right, wndRect.top + m_clientcircleRect.bottom);


					CRect screenRc;
					SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&screenRc, 0);
					if(mainRect.top < screenRc.top)
					{
						wndRect.OffsetRect(0, screenRc.top - mainRect.top);
					}
					if(mainRect.bottom > screenRc.bottom)
					{
						if(offsetY > 10)	//����Ѹ���ƶ�ʱ����û����鲻��
							wndRect.OffsetRect(0, screenRc.bottom - mainRect.bottom);
						else if(offsetY > 0)//�������⴦���������۵���ֻ�����µ׳�����Ļ����ʱ��ֹ�����ƶ�
							wndRect.OffsetRect(0, 0 - offsetY);
					}
					if(mainRect.left < screenRc.left)
					{
						if(offsetX < 10)	//����Ѹ���ƶ�ʱ����û����鲻��
							wndRect.OffsetRect(screenRc.left - mainRect.left, 0);
						else if(offsetX < 0)	//�������⴦���������۵���ֻ�����µ׳�����Ļ����ʱ��ֹ�����ƶ�
							wndRect.OffsetRect(0 - offsetX, 0);
					}
					if(mainRect.right > screenRc.right)
					{
						wndRect.OffsetRect(screenRc.right - mainRect.right, 0);
					}

					{	//Ч�ʴ���
						HDWP hdwp = BeginDeferWindowPos(4);

						DeferWindowPos(hdwp, GetSafeHwnd(), NULL, wndRect.left, wndRect.top, wndRect.Width(), wndRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOREDRAW);

						CRect mainWndRect;
						GetDlgItem(IDC_P_MAINWND)->GetWindowRect(mainWndRect);
						//mainWndRect.bottom -= 10;	///�����¼�Ի�����б��ڵ�λ���������߿�

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

			if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	//������������ʱ����ʱ����ѭ��
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
	{	//�жϿ�ͣ��������
		long cxScreen = GetSystemMetrics(SM_CXSCREEN);
		long cyScreen = GetSystemMetrics(SM_CYSCREEN);

		CPoint ptCur = CPoint(0, 0);
		GetCursorPos(&ptCur);

		CRect wndRect;
		GetWindowRect(wndRect);

		CRect mainRect = CRect(wndRect.left + m_clientRect.left, wndRect.top + m_clientRect.top, wndRect.left + m_clientRect.right, wndRect.top + m_clientRect.bottom);
		if(m_bShowMainImg == NULL)	//�����ڲ���ʾ
			mainRect = CRect(wndRect.left + m_clientcircleRect.left, wndRect.top + m_clientcircleRect.top, wndRect.left + m_clientcircleRect.right, wndRect.top + m_clientcircleRect.bottom);

		CRect screenRc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&screenRc, 0);
		m_dockType = DOCK_NORMAL;	//��֤������������������ط�����ͣ��
		if(mainRect.top <= screenRc.top && screenRc.top <= 0)//�ڶ���
		{
			m_dockType = DOCK_TOP;
		}
		if(mainRect.bottom >= screenRc.bottom && screenRc.bottom >= cyScreen)
		{
			m_dockType = DOCK_BOTTOM;
		}
		if(mainRect.left <= screenRc.left && screenRc.left <= 0)//�����
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

		//if (m_dockType != DOCK_NORMAL && m_wndAlphaValue == 255)	//m_dockType != DOCK_NORMAL ˵���ڿ�ͣ���ı���
	//{
	//	if (PtInWindow(ptCur) || PtInDock(ptCur))
	//		m_dockType = DOCK_NORMAL;
	//}
	//else if (m_dockDlg.m_wndAlphaValue == 255)	//���������ͣ����״̬
	//{
	//	if (PtInDock(ptCur))
	//		m_dockType = DOCK_NORMAL;
	//}

	if (m_dockType != DOCK_NORMAL)//��겻�������ڣ�ͬʱ�������ڱ���
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

	m_mainListDlg.DoLogon(m_bAlreadyLogon, m_selMode = 0);//���ε�¼���б���ʾ��������
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
{	//��Ӧ����ͼ���ϵ���Ϣ
	if(wParam = IDR_MAINFRAME)
	{
		CRect rect;
		POINT point;
		HMENU hmenu;
		switch(lParam)
		{
		case WM_RBUTTONUP:
			GetCursorPos(&point);
			SetForegroundWindow();//��������ͼ��˵���ʾ
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
{	//����ͼ��˵���Ϣ
	// TODO: Add your specialized code here and/or call the base class

	switch(LOWORD(wParam))
	{
	case WM_ICONEXIT://�����˵���������Ϣ��Notify��Ϣ
		OnBnClickedCancel();
		break;
	case WM_ICONEXITUSER://�����˵���������Ϣ��Notify��Ϣ
		{ //�˳�ǰ�����ʾ
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

	//������뿪����ʾ����ʱ��������Ϣ���������ж��Ƿ���Ҫͣ�������ڱ߽���
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

	// TODO:  �ڴ˴������Ϣ����������
}



LRESULT CRIM30Dlg::OnNcHitTest(CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	int makeFlack = (int)wParam;		//�Ƿ���˸

//#define		WM_FLICKERNOTIFYICON					WM_USER + 9							//��˸����ͼ��		wParam 0 ����˸ͼ��   1 ִ����˸ͼ�� 2 ��Ϣ֪ͨ��3�ļ���Ϣ֪ͨ lParam  �û���Ϣָ��
	switch(makeFlack)
	{
	case 0:	//����˸
		if (m_icondata.hIcon == NULL)	//������˸��
		{
			lstrcpy(m_icondata.szInfoTitle, _T(""));
			lstrcpy(m_icondata.szInfo, _T(""));
			m_icondata.hIcon				= m_hIcon;
			Shell_NotifyIcon(NIM_MODIFY, &m_icondata);//
		}
		break;
	case 1:	//Ҫ����˸
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
	case 2:	//������Ϣ
		if(lParam)
		{
			LPUSERINFO lpUserInfo = (LPUSERINFO)lParam;

			m_icondata.hIcon			= m_hIcon;
			lstrcpy(m_icondata.szInfoTitle, _T("��Ϣ"));

			CString str;
			str.Format(_T("����%s����Ϣ."), lpUserInfo->UserID.c_str());
			lstrcpy(m_icondata.szInfo, str.GetBuffer());

			Shell_NotifyIcon(NIM_MODIFY, &m_icondata);//
		}
		break;
	case 3:	//�ļ���Ϣ
		if(lParam)
		{
			LPUSERINFO lpUserInfo = (LPUSERINFO)lParam;

			m_icondata.hIcon			= m_hIcon;
			lstrcpy(m_icondata.szInfoTitle, _T("��Ϣ"));

			CString str;
			str.Format(_T("����%s���ļ�����."), lpUserInfo->UserID.c_str());
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (Exit())
		CDialog::OnClose();
}
 
void CRIM30Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000) && m_dockDlg.GetSafeHwnd())	//������������ʱ��
	{
		CPoint ptCur;	GetCursorPos(&ptCur);
		if (m_dockType != DOCK_NORMAL)		//��Ϊ��ǰ״̬Ϊ����������״̬
		{
			if (m_dockDlg.m_wndAlphaValue == 255 && PtInDock(ptCur))
				PostMessage(WM_SHOWDOCKBOUND);
			else if (m_dockDlg.m_wndAlphaValue == 0 && !(PtInWindow(ptCur) || PtInDock(ptCur)))
				PostMessage(WM_SHOWDOCKBOUND);
		}
		else if (m_dockType == DOCK_NORMAL) //��Ϊ��ǰ״̬�����洦��������ʾ״̬
		{
			if (!(PtInWindow(ptCur) || PtInDock(ptCur)))
				PostMessage(WM_SHOWDOCKBOUND);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
