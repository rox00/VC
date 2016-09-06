// DialogSet.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSet.h"

// CDialogSet dialog

IMPLEMENT_DYNAMIC(CDialogSet, CDialog)

CDialogSet::CDialogSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSet::IDD, pParent)
{
	m_bUpdateConfig				= FALSE;

	////【zy 2015-01-05 】根据图片路径进行初始化
	m_pImageBkTemp									= ImageConfig::GetInstance()->m_pImageDlgSetBk;
	m_bOffLine										= (RIMConfig::GetInstance()->m_pLogonUser->userTypeID == USERINFO::USER_ID_UNKNOWN);
	m_pHtmlView										= new CMyHtmlView;
}

CDialogSet::~CDialogSet()
{
}

void CDialogSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDAPPLAY, m_btnApply);
	DDX_Control(pDX, IDC_LISTSELECTE, m_listSelect);
	DDX_Control(pDX, IDC_B_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDialogSet, CDialog)
	ON_MESSAGE(WM_UPDATECONFIGFLAG, OnUpdateConfigFlag)

	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDialogSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSet::OnBnClickedCancel)
	ON_BN_CLICKED(IDAPPLAY, &CDialogSet::OnBnClickedApply)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LISTSELECTE, &CDialogSet::OnLbnSelchangeListselecte)
	ON_BN_CLICKED(IDC_B_CLOSE, &CDialogSet::OnBnClickedBClose)
END_MESSAGE_MAP()


// CDialogSet message handlers


void CDialogSet::Init()
{
	//设置圆角窗口
	SetRoundRgnWnd(m_hWnd, 12, 12);

	m_btnClose.m_pngParentBkPath				= RelativeToFullPath(_T("res\\PNG\\systemset\\systemset.bmp")).c_str();
	m_btnClose.m_normalPngPath					= RelativeToFullPath(_T("res\\PNG\\systemset\\closenormal.png")).c_str();
	m_btnClose.m_overPngPath					= RelativeToFullPath(_T("res\\PNG\\systemset\\closeover.png")).c_str();
	m_btnClose.m_pushPngPath					= RelativeToFullPath(_T("res\\PNG\\systemset\\closepush.png")).c_str();


	m_listSelect.m_pngBkPath	 				= RelativeToFullPath(_T("res\\PNG\\systemset\\listboxbk.png")).c_str();
	m_listSelect.m_pngSelPath					= RelativeToFullPath(_T("res\\PNG\\systemset\\radiosel.png")).c_str();;
	m_listSelect.m_textColor					= RGB(0,0,0);
	m_listSelect.m_nTextOffsetX					= 5;
	m_listSelect.InsertString(0, _T("系统设置"));
	m_listSelect.SetItemHeight(0, 25);
	m_listSelect.InsertString(1, _T("定制系统"));
	m_listSelect.SetItemHeight(1, 25);
	if(m_bOffLine == FALSE)
	{	////非便携登录下才会显示修改密码选项
		m_listSelect.InsertString(2, _T("修改密码"));
		m_listSelect.SetItemHeight(2, 25);
	}
	m_listSelect.SetCurSel(0);

	//创建子对话框
	m_sysDlg.Create(CSystemSetDlg::IDD, this);
	m_cusDlg.Create(CCustomSetDlg::IDD, this);
	m_pHtmlView->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1000);

	if(m_bOffLine == FALSE)
	{
		if (m_pHtmlView && ::IsWindow(m_pHtmlView->m_hWnd))
		{
			CString bsHttp = _T("");
			bool bGoOn = true;
			for(map<DWORD, vector<LPWEBADDRINFO>>::iterator it = RIMConfig::GetInstance()->m_webAddrInfoMap.begin(); bGoOn && it != RIMConfig::GetInstance()->m_webAddrInfoMap.end(); ++it)
			{
				if(	it->first == 99)
				{
					for (UINT i = 0; bGoOn && i < it->second.size(); ++i)
					{
						if(it->second.at(i)->nSubModeID == 1)
						{
							bsHttp = CA2T(it->second.at(i)->sLocation.c_str());
						}
					}

					bGoOn = false;
				}
			}

			CTime time = CTime::GetCurrentTime();
			CString strTime = time.Format("%A, %B %d, %Y %H:%M:%S");

			bsHttp += CString(_T("/sys/User/changepassword.aspx?"));
			bsHttp += CString(_T("guid="))	+ RIMConfig::GetInstance()->m_pLogonUser->UserID.c_str();
			bsHttp += CString(_T("&time="))	+ strTime;
			bsHttp += CString(_T("&token=")) + RIMConfig::GetInstance()->m_pLogonUser->userToken.c_str();
			bsHttp += CString(_T("&theme=")) + RIMConfig::GetInstance()->m_modifyPassTheme.c_str();
			m_pHtmlView->Navigate2(bsHttp);
		}
	}

	//处理子对话框的位置 及 显示
	RepositionSubDlg();
	UpdateWndShow();

	//m_listSelect.set(1, 25);

}

void CDialogSet::RepositionSubDlg(bool bRedraw /*= false*/)
{
	if(GetDlgItem(IDC_P_DLG))
	{
		CRect subDlgRc;
		GetDlgItem(IDC_P_DLG)->GetWindowRect(subDlgRc);
		ScreenToClient(subDlgRc);

		m_sysDlg.MoveWindow(subDlgRc, bRedraw);
		m_cusDlg.MoveWindow(subDlgRc, bRedraw);
		m_pHtmlView->MoveWindow(subDlgRc, bRedraw);
	}
}

void CDialogSet::UpdateWndShow()
{
	switch(m_listSelect.GetCurSel())
	{
	case 0:
		m_sysDlg.ShowWindow(SW_SHOW);
		m_cusDlg.ShowWindow(SW_HIDE);
		m_pHtmlView->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_sysDlg.ShowWindow(SW_HIDE);
		m_cusDlg.ShowWindow(SW_SHOW);
		m_pHtmlView->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_sysDlg.ShowWindow(SW_HIDE);
		m_cusDlg.ShowWindow(SW_HIDE);
		m_pHtmlView->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}

void CDialogSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	UpdateConfig();
}

void CDialogSet::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDialogSet::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here

	UpdateConfig();
}

LRESULT CDialogSet::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return HTCAPTION;
	return CDialog::OnNcHitTest(point);
}

BOOL CDialogSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSet::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	RepositionSubDlg();
}

LRESULT CDialogSet::OnUpdateConfigFlag(WPARAM wParam, LPARAM lParam)
{
	m_bUpdateConfig = (BOOL)wParam;
	m_btnApply.EnableWindow(m_bUpdateConfig);

	return TRUE;
}

void CDialogSet::UpdateConfig()
{
	if(m_bUpdateConfig)
	{
		m_sysDlg.ApplyConfig();	
		m_cusDlg.ApplyConfig();	
		AfxGetMainWnd()->PostMessage(WM_UPDATECONFIG);

		m_btnApply.EnableWindow(m_bUpdateConfig = FALSE);
	}
}

BOOL CDialogSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	CRect  rcClient;
	GetClientRect(&rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());

	MemPaintPng(memDC, CRect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom), m_pImageBkTemp);
	//memDC.FillSolidRect(rcClient, RGB(255,0,0));
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDialogSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

}

void CDialogSet::OnLbnSelchangeListselecte()
{
	// TODO: Add your control notification handler code here
	UpdateWndShow();
}

void CDialogSet::OnBnClickedBClose()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
