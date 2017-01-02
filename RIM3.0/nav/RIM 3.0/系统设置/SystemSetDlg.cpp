// SystemSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SystemSetDlg.h"


// CSystemSetDlg dialog

IMPLEMENT_DYNAMIC(CSystemSetDlg, CDialog)

CSystemSetDlg::CSystemSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemSetDlg::IDD, pParent)
{
	ZeroMemory(m_startDic, MAX_PATH * sizeof(TCHAR));
	LPITEMIDLIST	ppidl				= NULL;
	if (SHGetSpecialFolderLocation(NULL, CSIDL_STARTUP, &ppidl) == S_OK)
		SHGetPathFromIDList(ppidl, m_startDic);

	m_linkFileName		= _T("RIM3.0.exe.lnk");
}

CSystemSetDlg::~CSystemSetDlg()
{
}

void CSystemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_AUTOSTART, m_chkAutoStart);
}


BEGIN_MESSAGE_MAP(CSystemSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSystemSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSystemSetDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHK_AUTOSTART, &CSystemSetDlg::OnBnClickedChkAutostart)
END_MESSAGE_MAP()


// CSystemSetDlg message handlers

void CSystemSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CSystemSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	if (GetParent())
	{
		GetParent()->PostMessage(WM_COMMAND, IDCANCEL, 0);
		return;
	}
}

BOOL CSystemSetDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	pDC->FillSolidRect(rcClient, RGB(0,255,0));
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CSystemSetDlg::OnBnClickedChkAutostart()
{
	// TODO: Add your control notification handler code here
	GetParent()->PostMessage(WM_UPDATECONFIGFLAG, 1);
}

BOOL CSystemSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	tstring linkFilePath = tstring(m_startDic) + _T("\\") + m_linkFileName;
	m_chkAutoStart.SetCheck(IsFileExist(linkFilePath.c_str())?BST_CHECKED:BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSystemSetDlg::ApplyConfig()
{
	tstring linkFilePath = tstring(m_startDic) + _T("\\") + m_linkFileName;

	if(m_chkAutoStart.GetCheck())
		CreateFileShortcut(NULL, m_startDic, m_linkFileName.c_str(), MAKEWORD(VK_F12, HOTKEYF_CONTROL), _T("That is a test"));
	else
		DeleteFile(linkFilePath.c_str());
}
