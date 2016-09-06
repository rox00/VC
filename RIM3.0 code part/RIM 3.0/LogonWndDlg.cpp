//********************************************************************
//	LOGONWNDDLG.CPP 文件注释
//	文件名		: LOGONWNDDLG.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/16 9:35
//	文件描述	: 
//*********************************************************************	
// MainWndDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "LogonWndDlg.h"



// CLogonWndDlg dialog

IMPLEMENT_DYNAMIC(CLogonWndDlg, CDialog)

CLogonWndDlg::CLogonWndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonWndDlg::IDD, pParent)
{
	m_bDisplayChange				= FALSE;
	m_wndAlphaValue					= 0;
	m_bMouseMove					= FALSE;

	m_processPercent0_100			= 0;
	m_hThrdLogon					= INVALID_HANDLE_VALUE;
	m_processBkColor				= Color::Blue;
	m_errorInfo						= _T("");
	ZeroMemory(m_logonInfo, sizeof(m_logonInfo));

	m_wndMoveRgn.CreateRectRgn(0, 0, ImageConfig::GetInstance()->m_pImgLogon->GetWidth(),  30);
}

CLogonWndDlg::~CLogonWndDlg()
{

}

void CLogonWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_USER, m_editUser);
	DDX_Control(pDX, IDC_E_PASS, m_editPass);
	DDX_Control(pDX, IDC_E_SEVRIP, m_editDmsIP);
	DDX_Control(pDX, IDC_E_RMSIP, m_editRmsIP);
	DDX_Control(pDX, IDC_E_SASIP, m_editSasIP);
	DDX_Control(pDX, IDC_BUTTON1, m_btnLogon);
	DDX_Control(pDX, IDC_CHK_PASSWORD, m_chkPassword);
	DDX_Control(pDX, IDC_CHK_AUTOLOGON, m_chkAutoLogon);
	DDX_Control(pDX, IDC_CHK_OFFLINE, m_chkOffLine);
	DDX_Control(pDX, IDC_S_VERSION, m_staticVersion);
}


BEGIN_MESSAGE_MAP(CLogonWndDlg, CDialog)
	ON_MESSAGE(WM_PROGRESS, OnProgress)

	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_BN_CLICKED(IDOK, &CLogonWndDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLogonWndDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CLogonWndDlg::OnBnClickedButton1)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHK_PASSWORD, &CLogonWndDlg::OnBnClickedChkPassword)
	ON_BN_CLICKED(IDC_CHK_AUTOLOGON, &CLogonWndDlg::OnBnClickedChkAutologon)
	ON_BN_CLICKED(IDC_CHK_OFFLINE, &CLogonWndDlg::OnBnClickedChkOffline)
	ON_EN_CHANGE(IDC_E_USER, &CLogonWndDlg::OnEnChangeEUser)
	ON_EN_CHANGE(IDC_E_PASS, &CLogonWndDlg::OnEnChangeEPass)
	ON_EN_CHANGE(IDC_E_SEVRIP, &CLogonWndDlg::OnEnChangeESevrip)
	ON_WM_MOVE()
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CMainWndDlg message handlers
void CLogonWndDlg::Init()
{
	////【zy 2015-01-05 】根据图片路径进行初始化
	CRect editTextRc;

	m_editUser.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_editUser.m_pngBkPath				= RelativeToFullPath(_T("res\\PNG\\editBk.png")).c_str();
	m_editUser.m_GrayTipText			= "用户名";
	m_editUser.m_textColor				= RGB(0, 0, 0);
	m_editUser.GetRect(editTextRc); editTextRc.top += 8; m_editUser.SetRect(editTextRc);
	m_editUser.SetMargins(10,0);

	m_editPass.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_editPass.m_pngBkPath				= RelativeToFullPath(_T("res\\PNG\\editBk.png")).c_str();
	m_editPass.m_GrayTipText			= "密码";
	m_editPass.m_textColor				= RGB(0, 0, 0);
	m_editPass.GetRect(editTextRc); editTextRc.top += 8; m_editPass.SetRect(editTextRc);
	m_editPass.SetMargins(10,0);
#ifdef _UNICODE
	m_editPass.SetPasswordChar(0x25CF);
#else
	m_editPass.SendMessage(EM_SETPASSWORDCHAR, WPARAM(MAKEWORD(0xa1, 0xf1)), 0);/*	int aa = '●';*/
#endif

	m_editDmsIP.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_editDmsIP.m_pngBkPath				= RelativeToFullPath(_T("res\\PNG\\editBk.png")).c_str();
	m_editDmsIP.m_GrayTipText			= "服务IP(192.168.1.1)";
	m_editDmsIP.m_textColor				= RGB(0, 0, 0);
	m_editDmsIP.GetRect(editTextRc); editTextRc.top += 8; m_editDmsIP.SetRect(editTextRc);
	m_editDmsIP.SetMargins(10,0);

	m_editRmsIP.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_editRmsIP.m_pngBkPath				= RelativeToFullPath(_T("res\\PNG\\editBk.png")).c_str();
	m_editRmsIP.m_GrayTipText			= "监测服务IP(192.168.1.1)";
	m_editRmsIP.m_textColor				= RGB(0, 0, 0);
	m_editRmsIP.GetRect(editTextRc); editTextRc.top += 8; m_editRmsIP.SetRect(editTextRc);
	m_editRmsIP.SetMargins(10,0);

	m_editSasIP.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_editSasIP.m_pngBkPath				= RelativeToFullPath(_T("res\\PNG\\editBk.png")).c_str();
	m_editSasIP.m_GrayTipText			= "统计服务IP(192.168.1.1)";
	m_editSasIP.m_textColor				= RGB(0, 0, 0);
	m_editSasIP.GetRect(editTextRc); editTextRc.top += 8; m_editSasIP.SetRect(editTextRc);
	m_editSasIP.SetMargins(10,0);


	m_btnLogon.m_normalPngPath			= RelativeToFullPath(_T("res\\PNG\\btnNormal.png")).c_str();
	m_btnLogon.m_overPngPath			= RelativeToFullPath(_T("res\\PNG\\btnNormal.png")).c_str();
	m_btnLogon.m_pushPngPath			= RelativeToFullPath(_T("res\\PNG\\btnPush.png")).c_str();
	m_btnLogon.m_textColor				= RGB(0, 0, 0);


	m_chkPassword.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_chkPassword.m_pngChkSel			= RelativeToFullPath(_T("res\\PNG\\chksel.png")).c_str();
	m_chkPassword.m_pngChkUnSel			= RelativeToFullPath(_T("res\\PNG\\chkunsel.png")).c_str();
	m_chkPassword.m_textColor			= RGB(0, 0, 0);
	m_chkPassword.m_textOffsetX			= 5;
	m_chkPassword.m_textAlignMode		= DT_LEFT;

	m_chkAutoLogon.m_pngParentBkPath	= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_chkAutoLogon.m_pngChkSel			= RelativeToFullPath(_T("res\\PNG\\chksel.png")).c_str();
	m_chkAutoLogon.m_pngChkUnSel		= RelativeToFullPath(_T("res\\PNG\\chkunsel.png")).c_str();
	m_chkAutoLogon.m_textColor			= RGB(0, 0, 0);
	m_chkAutoLogon.m_textOffsetX		= 5;
	m_chkAutoLogon.m_textAlignMode		= DT_LEFT;

	m_chkOffLine.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_chkOffLine.m_pngChkSel			= RelativeToFullPath(_T("res\\PNG\\chksel.png")).c_str();
	m_chkOffLine.m_pngChkUnSel			= RelativeToFullPath(_T("res\\PNG\\chkunsel.png")).c_str();
	m_chkOffLine.m_textColor			= RGB(0, 0, 0);
	m_chkOffLine.m_textOffsetX			= 5;
	m_chkOffLine.m_textAlignMode		= DT_LEFT;

	m_staticVersion.m_pngParentBkPath	= RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str();
	m_staticVersion.m_textColor			= RGB(0, 0, 0);
	m_staticVersion.m_textAlignMode		= DT_SINGLELINE | DT_VCENTER | DT_CENTER;

	SetOrSaveSystemSet(FALSE);
	UpdateSubWndShow();
}

void CLogonWndDlg::UpdateSubWndShow()
{
	//m_editUser;	
	//m_editPass;	
	//m_editSevrIP;
	/////以上三个EDIT位置是放好的，不需移动

	CRect userRc, serverRc, sasRc, rmsRc;

	m_editUser.GetWindowRect(userRc);		ScreenToClient(userRc);
	m_editDmsIP.GetWindowRect(serverRc);	ScreenToClient(serverRc);

	m_editRmsIP.GetWindowRect(rmsRc);		ScreenToClient(rmsRc);
	m_editSasIP.GetWindowRect(sasRc);		ScreenToClient(sasRc);

	if(rmsRc.top != (userRc.top + userRc.Height()/3))
	{
		userRc.OffsetRect(0, userRc.Height()/3 + 20);
		m_editRmsIP.MoveWindow(userRc);
	}

	if(sasRc.top != (serverRc.top - serverRc.Height()/3))
	{
		serverRc.OffsetRect(0, 0 - serverRc.Height()/3 + 20);
		m_editSasIP.MoveWindow(serverRc);
	}

	if(m_chkOffLine.GetCheck())
	{
		m_editUser.ShowWindow(SW_HIDE);
		m_editDmsIP.ShowWindow(SW_HIDE);
		m_editPass.ShowWindow(SW_HIDE);
		m_chkPassword.ShowWindow(SW_HIDE);

		m_editRmsIP.ShowWindow(SW_SHOW);
		m_editSasIP.ShowWindow(SW_SHOW);
	}
	else
	{
		m_editUser.ShowWindow(SW_SHOW);
		m_editDmsIP.ShowWindow(SW_SHOW);
		m_editPass.ShowWindow(SW_SHOW);
		m_chkPassword.ShowWindow(SW_SHOW);

		m_editRmsIP.ShowWindow(SW_HIDE);
		m_editSasIP.ShowWindow(SW_HIDE);
	}
}

void CLogonWndDlg::SetOrSaveSystemSet(BOOL bShowOrSave /*= TRUE*/)
{
	////0 设置   1保存
	if(bShowOrSave)
	{
		CString strText = _T("");
		RIMConfig::GetInstance()->m_systemSet.bRememberPass 	= m_chkPassword.GetCheck();
		RIMConfig::GetInstance()->m_systemSet.bAutoLogon		= m_chkAutoLogon.GetCheck();
		RIMConfig::GetInstance()->m_systemSet.bOffLineLogon		= m_chkOffLine.GetCheck();

		if(RIMConfig::GetInstance()->m_systemSet.bOffLineLogon)
		{
			RIMConfig::GetInstance()->m_systemSet.lastRmsIP		= (m_editRmsIP.GetWindowText(strText = _T(""))	, strText.Trim(), strText);
			RIMConfig::GetInstance()->m_systemSet.lastMsaIP		= (m_editSasIP.GetWindowText(strText = _T(""))	, strText.Trim(), strText);
		}
		else
		{
			RIMConfig::GetInstance()->m_systemSet.lastUserID	= (m_editUser.GetWindowText(strText = _T(""))	, strText);
			RIMConfig::GetInstance()->m_systemSet.lastPass		= (m_editPass.GetWindowText(strText = _T(""))	, strText);
			RIMConfig::GetInstance()->m_systemSet.lastDmsIP	= (m_editDmsIP.GetWindowText(strText = _T(""))	, strText.Trim(), strText);
		}
	}
	else
	{
		m_editUser.SetWindowText(RIMConfig::GetInstance()->m_systemSet.lastUserID.c_str());
		m_editDmsIP.SetWindowText(RIMConfig::GetInstance()->m_systemSet.lastDmsIP.c_str());
		m_editRmsIP.SetWindowText(RIMConfig::GetInstance()->m_systemSet.lastRmsIP.c_str());
		m_editSasIP.SetWindowText(RIMConfig::GetInstance()->m_systemSet.lastMsaIP.c_str());

		if(RIMConfig::GetInstance()->m_systemSet.bRememberPass)
		{
			m_chkPassword.SetCheck(TRUE);
			m_editPass.SetWindowText(RIMConfig::GetInstance()->m_systemSet.lastPass.c_str());
		}
		m_chkAutoLogon.SetCheck(RIMConfig::GetInstance()->m_systemSet.bAutoLogon);
		m_chkOffLine.SetCheck(RIMConfig::GetInstance()->m_systemSet.bOffLineLogon);
	}
}

UINT __stdcall CLogonWndDlg::ThreadDoLogon(void *pDt)
{
	CoInitialize(NULL);

	DWORD			*pDword 	= (DWORD*)pDt;
	CLogonWndDlg	*pDlg		= (CLogonWndDlg*)(*pDword);
	BOOL			bLogon		= *(pDword + 1);
	pDlg->m_errorInfo			= _T("");

	if(bLogon)
	{
		if(RIMConfig::GetInstance()->m_pLogonUser == NULL)
		{
			pDlg->SetOrSaveSystemSet(TRUE);	////保存设置
			{	////登录
				RIMConfig::GetInstance()->DoLogon(TRUE, pDlg->m_chkOffLine.GetCheck(), pDlg->m_errorInfo, pDlg->GetSafeHwnd());
			}


			if(RIMConfig::GetInstance()->m_pLogonUser)
			{
				////////////////////////////////////////////////////登录成功  首先开始执行各项界面的登录及保存用户帐户操作
				if (AfxGetApp()->GetMainWnd())
					AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGON, 1, 0);
				//////【zy 2015-01-20 ↓】

				////【zy 2015-02-07 ↓】	///向全网段发送广播包
				vector<tstring>  ipList;
				GetAvalibleBroadcastAddress(ipList);
				ipList.push_back(_T("255.255.255.255"));
				for (UINT i = 0; i < ipList.size(); i++)
				{
					CNetData *pUDPData = new CNetData();
					USERINFO desUserInfo;
					desUserInfo.userUdpPortTemp	= UDP_CHATSERVER_PORT;

					string transBuf = CT2A(RIMConfig::GetInstance()->m_pLogonUser->UserID.c_str());
					pUDPData->MakeNETBuffer(
						CNetData::UDP_ONLINE_NOTIFY,
						RIMConfig::GetInstance()->m_pLogonUser,
						&desUserInfo, ipList.at(i),
						(LPBYTE)transBuf.c_str()
						,
						transBuf.size()
						);

					RIMConfig::GetInstance()->m_UDPSendQueue.Push(pUDPData);
				}
				ipList.clear();
				////【zy 2015-02-07 ↑】


				pDlg->m_errorInfo		= _T("");
				pDlg->m_processBkColor	= Color::Blue;
			}
			else
			{

				int i  = 255;
				//while(i --)
				{
					//pDlg->m_processBkColor	= Color(i, 0xFF,0x45,0x00);
					pDlg->m_processBkColor	= Color(i, 0xFF,0,0x00);
					pDlg->PostMessage(WM_PROGRESS, 0, 0);
					Sleep(1);
				}

				//pDlg->m_errorInfo		= _T("");
				//pDlg->m_processBkColor	= Color::Blue;
				//pDlg->PostMessage(WM_PROGRESS, 0, 0);
			}
		}
	}
	else
	{	/////////////////////////////////////////////////开始执行各项退出操作

		if(RIMConfig::GetInstance()->m_pLogonUser)
		{	//如果已经登录

			////【zy 2015-02-07 ↓】	///向全网段发送广播包
			vector<tstring>  ipList;
			GetAvalibleBroadcastAddress(ipList);
			ipList.push_back(_T("255.255.255.255"));
			for (UINT i = 0; i < ipList.size(); i++)
			{
				CNetData *pUDPData = new CNetData();
				USERINFO desUserInfo;
				desUserInfo.userUdpPortTemp	= UDP_CHATSERVER_PORT;
				//下线
				string transBuf = CT2A(RIMConfig::GetInstance()->m_pLogonUser->UserID.c_str());
				pUDPData->MakeNETBuffer(
					CNetData::UDP_OFFLINE,
					RIMConfig::GetInstance()->m_pLogonUser,
					&desUserInfo, ipList.at(i),
					(LPBYTE)transBuf.c_str()
					,
					transBuf.size()
					);

				RIMConfig::GetInstance()->m_UDPSendQueue.Push(pUDPData);
			}
			ipList.clear();
			////【zy 2015-02-07 ↑】


			GetRIM30Server()->CloseAllSubSystem();

			if (AfxGetApp()->GetMainWnd())///////////////首先进行各项界面的退出
				AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGON, 0, 0);

			RIMConfig::GetInstance()->DoLogon(FALSE, FALSE, pDlg->m_errorInfo, NULL);
			RIMConfig::GetInstance()->m_pLogonUser = NULL;

			pDlg->SetOrSaveSystemSet(FALSE);
			pDlg->UpdateSubWndShow();
		}
	}

	CoUninitialize();

	pDlg->m_processPercent0_100 = 0;
	CloseHandle(pDlg->m_hThrdLogon);
	pDlg->m_hThrdLogon = INVALID_HANDLE_VALUE;

	return 0;
}

bool CLogonWndDlg::Logon(BOOL bLogon)
{
	if(m_hThrdLogon == INVALID_HANDLE_VALUE)
	{
		m_logonInfo[0]	= DWORD(this);
		m_logonInfo[1]	= bLogon;
		m_hThrdLogon	= (HANDLE)_beginthreadex(NULL, 0, ThreadDoLogon, (LPVOID)m_logonInfo, 0, NULL);

		if(bLogon == FALSE)
		{	////如果要是登出，应当等待所有资源析构完成
			MSG msg;
			while(m_hThrdLogon != INVALID_HANDLE_VALUE)
			{
				if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && !::IsDialogMessage(NULL, &msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
		}

		return true;
	}
	else
		return false;
}

void CLogonWndDlg::SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	m_wndAlphaValue = bAlpha;
	SetLayeredWindowAttributes(crKey, bAlpha, dwFlags);
}

void CLogonWndDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CLogonWndDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CWnd *pParent = GetParent();

	if(GetParent())
	{
		GetParent()->PostMessage(WM_COMMAND, IDCANCEL, 0);
		return;
	}
	OnCancel();
}

HBRUSH CLogonWndDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	//pDC->SetBkMode(TRANSPARENT);

	if(pWnd == this)
		return g_hTransAlphaBrush;
 	else
		return hbr;
}

BOOL CLogonWndDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CLogonWndDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	memDC.SelectObject(&memBmp);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(255, 255, 255));
	memDC.FillSolidRect(rcClient, g_transAlphaClr);

	Graphics graphics(memDC.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBilinear);
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);
	stringformat.SetFormatFlags(StringFormatFlagsNoClip);

	FontFamily		faimily(L"微软雅黑");
	Gdiplus::Font	myFont(&faimily, 15, FontStyleRegular, UnitPixel);

	RectF processRcF = RectF(float(m_processRect.left), float(m_processRect.top), float(m_processRect.Width()), float(m_processRect.Height()));
	graphics.FillRectangle(&SolidBrush(Color::Blue), processRcF);
	graphics.FillRectangle(&SolidBrush(m_processBkColor), processRcF);

	RectF percentRectF = RectF(float(m_processRect.left), float(m_processRect.top), float(m_processRect.Width() * 1.0 * m_processPercent0_100/100), float(m_processRect.Height()));
	graphics.FillRectangle(&SolidBrush(Color::Green), percentRectF);

	CStringW strW = CStringW(m_errorInfo.c_str());
	graphics.DrawString(strW, -1,
		&myFont,
		processRcF,
		&stringformat, &SolidBrush(Color(254, 254, 254, 254)));

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	memBmp.DeleteObject();
}

BOOL CLogonWndDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

 	ModifyStyle(WS_BORDER | WS_SIZEBOX | WS_DLGFRAME | DS_MODALFRAME, WS_POPUP);
 	ModifyStyleEx(WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME, WS_EX_LAYERED);
 
	SetLayeredAttributes(g_transAlphaClr, 255, LWA_COLORKEY | LWA_ALPHA);

	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLogonWndDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

// 	m_editUser.EnableWindow(TRUE);
// 	m_editPass.EnableWindow(TRUE);
// 	m_editSevrIP.EnableWindow(TRUE);
// 	m_btnLogon.EnableWindow(TRUE);
	if(RIMConfig::GetInstance()->m_pLogonUser == NULL)
		Logon(TRUE);
}

LRESULT CLogonWndDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return HTTRANSPARENT;	//除了控件就是透明
	return CDialog::OnNcHitTest(point);
}

BOOL CLogonWndDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	if(m_hThrdLogon != INVALID_HANDLE_VALUE)
	{
		HCURSOR   hCursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);
		::SetCursor(hCursor);
		return TRUE;
	}
	else if(pWnd == &m_btnLogon)
	{
		HCURSOR   hCursor   =   AfxGetApp()-> LoadStandardCursor(IDC_HAND);   
		::SetCursor(hCursor);
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CLogonWndDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码

	if(m_btnLogon.GetSafeHwnd())
	{
		CRect btnRect;
		m_btnLogon.GetWindowRect(btnRect);
		ScreenToClient(btnRect);

		m_processRect = CRect(20, btnRect.top + 50, cx - 20, btnRect.top + 65);
	}
}

void CLogonWndDlg::OnBnClickedChkPassword()
{
	// TODO: Add your control notification handler code here
	//不记住密码状态下，不能自动登录
	if(!m_chkPassword.GetCheck())
		m_chkAutoLogon.SetCheck(FALSE);
}

void CLogonWndDlg::OnBnClickedChkAutologon()
{
	// TODO: Add your control notification handler code here
	//自动登录状态下，必须记住密码
	if(m_chkAutoLogon.GetCheck())
		m_chkPassword.SetCheck(TRUE);
}

void CLogonWndDlg::OnBnClickedChkOffline()
{
	// TODO: Add your control notification handler code here
	UpdateSubWndShow();
}

void CLogonWndDlg::OnEnChangeEUser()
{
}

void CLogonWndDlg::OnEnChangeEPass()
{

}

void CLogonWndDlg::OnEnChangeESevrip()
{
}

LRESULT CLogonWndDlg::OnProgress(WPARAM wParam, LPARAM lParam)
{
	m_processPercent0_100 = wParam;
	InvalidateRect(m_processRect);

	return TRUE;
}

void CLogonWndDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	if (m_bDisplayChange && AfxGetMainWnd())
	{
		AfxGetMainWnd()->PostMessage(WM_REPOSITIONSUBDLG);
		m_bDisplayChange = FALSE;
	}
}

LRESULT CLogonWndDlg::OnDisplayChange(WPARAM BitsPerPixel, LPARAM size)
{
	//m_bDisplayChange = TRUE;
	AfxGetMainWnd()->PostMessage(WM_REPOSITIONSUBDLG);
	return TRUE;
}
