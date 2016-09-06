// E:\My Project\visual studio 2008\RIM 3.0\系统设置\CustomSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CustomSetDlg.h"


// CCustomSetDlg dialog

IMPLEMENT_DYNAMIC(CCustomSetDlg, CDialog)

CCustomSetDlg::CCustomSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomSetDlg::IDD, pParent)
{

}

CCustomSetDlg::~CCustomSetDlg()
{
	//清除子系统信息
	for (UINT i = 0; i < m_moudleInfoArray.size(); i++)
	{
		LPSUBMOUDLEINFO pSubMoudleInfo = m_moudleInfoArray.at(i);
		delete pSubMoudleInfo;
	}
	m_moudleInfoArray.clear();
}

void CCustomSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_listCtrl);
	DDX_Control(pDX, IDC_CHK_SHOW, m_chkShow);
	DDX_Control(pDX, IDC_CHK_OPEN, m_chkOpen);
	DDX_Control(pDX, IDC_B_UP, m_btnUp);
	DDX_Control(pDX, IDC_B_DOWN, m_btnDown);
}


BEGIN_MESSAGE_MAP(CCustomSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCustomSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCustomSetDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHK_SHOW, &CCustomSetDlg::OnBnClickedChkShow)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CCustomSetDlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_B_UP, &CCustomSetDlg::OnBnClickedBUp)
	ON_BN_CLICKED(IDC_B_DOWN, &CCustomSetDlg::OnBnClickedBDown)
	ON_BN_CLICKED(IDC_CHK_OPEN, &CCustomSetDlg::OnBnClickedChkOpen)
END_MESSAGE_MAP()


// CCustomSetDlg message handlers

void CCustomSetDlg::Init()
{
	////【zy 2015-01-05 】根据图片路径进行初始化
	m_btnUp.m_pngBkPath 				= RelativeToFullPath(_T("res\\PNG\\systemset\\btnbk.png")).c_str();
	m_btnUp.m_normalPngPath 			= RelativeToFullPath(_T("res\\PNG\\systemset\\upnormal.png")).c_str();
	m_btnUp.m_overPngPath				= RelativeToFullPath(_T("res\\PNG\\systemset\\uphover.png")).c_str();
	m_btnUp.m_pushPngPath				= RelativeToFullPath(_T("res\\PNG\\systemset\\uppush.png")).c_str();

	m_btnDown.m_pngBkPath 				= RelativeToFullPath(_T("res\\PNG\\systemset\\btnbk.png")).c_str();
	m_btnDown.m_normalPngPath 			= RelativeToFullPath(_T("res\\PNG\\systemset\\downnormal.png")).c_str();
	m_btnDown.m_overPngPath				= RelativeToFullPath(_T("res\\PNG\\systemset\\downhover.png")).c_str();
	m_btnDown.m_pushPngPath				= RelativeToFullPath(_T("res\\PNG\\systemset\\downpush.png")).c_str();


	m_listCtrl.m_pngParentBkPath		= RelativeToFullPath(_T("res\\PNG\\systemset\\listctrlbk.png")).c_str();
	m_listCtrl.m_pngHeaderBk			= RelativeToFullPath(_T("res\\PNG\\systemset\\headerbk.png")).c_str();
	m_listCtrl.m_pngVScrollThumb		= RelativeToFullPath(_T("res\\PNG\\scroll\\vscrollThumb.png")).c_str();
	m_listCtrl.m_pngHScrollThumb		= RelativeToFullPath(_T("res\\PNG\\scroll\\hscrollThumb.png")).c_str();
	m_listCtrl.m_headerTextColor		= RGB(0,0,0);

	////复制子系统数组
	for (UINT i = 0; i < RIMConfig::GetInstance()->m_moudleInfoArray.size(); ++i)
	{
		LPSUBMOUDLEINFO lpSubmoudleInfo = RIMConfig::GetInstance()->m_moudleInfoArray[i];

		LPSUBMOUDLEINFO lpSubmoudleInfoTemp = new SUBMOUDLEINFO;
		*lpSubmoudleInfoTemp	= *lpSubmoudleInfo;
		m_moudleInfoArray.push_back(lpSubmoudleInfoTemp);
	}


	{	//对列表进行初始化
		CRect listRect;
		m_listCtrl.GetWindowRect(listRect);
		ScreenToClient(listRect);

		DWORD   dwStyle   =   m_listCtrl.GetExtendedStyle();   
		dwStyle |=  LVS_EX_FULLROWSELECT| LVS_SHOWSELALWAYS; 
		dwStyle	&=	(~LVS_EX_TRACKSELECT); 
		m_listCtrl.SetExtendedStyle(dwStyle);

		CStringArray strColumnArray;

		{	//向列表控件中插入列

			strColumnArray.Add(_T("子系统"));	
			strColumnArray.Add(_T("显示"));	
			strColumnArray.Add(_T("启动默认打开"));	

			LVCOLUMN lvColumn;
			lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_ORDER /*| LVCF_MINWIDTH | LVCF_DEFAULTWIDTH*/;
			lvColumn.fmt = LVCFMT_CENTER;
			lvColumn.cchTextMax = MAX_PATH;
			for (int i = 0; i< strColumnArray.GetCount(); ++i)
			{
				lvColumn.iOrder = i;
				lvColumn.pszText = strColumnArray[i].GetBuffer();
				lvColumn.cx = (listRect.Width() - 5)/strColumnArray.GetCount();
				//m_listCtrl.InsertColumn(i, &lvColumn);
				m_listCtrl.InsertColumn(i, lvColumn.pszText, lvColumn.fmt, lvColumn.cx);
			}
		}

		UpdateListItems();
	}
	
}

void CCustomSetDlg::UpdateListItems()
{
	//m_listCtrl.SetRedraw(FALSE);
	m_listCtrl.DeleteAllItems();

	//向列表控件中插入行
	LVITEM lvItem;
	memset(&lvItem,0,sizeof(LVITEM));
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.cchTextMax = MAX_PATH;

	for (UINT i = 0; i< m_moudleInfoArray.size(); ++i)
	{
		LPSUBMOUDLEINFO lpSubmoudleInfo = m_moudleInfoArray[i];
		m_listCtrl.InsertItem(i, _T(""));
		lvItem.iItem = i;
		for (int j = 0; j< 3; ++j)
		{
			lvItem.iImage = 0;
			lvItem.iSubItem = j;
			if(j == 0)
				lvItem.pszText = (TCHAR*)lpSubmoudleInfo->moudleName.c_str();
			else if(j == 1)
				lvItem.pszText = lpSubmoudleInfo->bShowInList?_T("显示"):_T("隐藏");
			else if(j == 2)
				lvItem.pszText = lpSubmoudleInfo->bDefaultOpen?_T("是"):_T("否");

			m_listCtrl.SetItem(&lvItem);
		}
	}

	m_listCtrl.SetRedraw(TRUE);
}


void CCustomSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CCustomSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	if (GetParent())
	{
		GetParent()->PostMessage(WM_COMMAND, IDCANCEL, 0);
		return;
	}
}

BOOL CCustomSetDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//CRect rcClient;
	//GetClientRect(rcClient);
	//pDC->FillSolidRect(rcClient, RGB(0,0,255));
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CCustomSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomSetDlg::OnBnClickedChkShow()
{
	// TODO: Add your control notification handler code here
	int selCnt = m_listCtrl.GetSelectedCount();
	if(selCnt)
	{
		vector<int> selIndexVec;
		POSITION position =  m_listCtrl.GetFirstSelectedItemPosition();
		while (position)
		{
			int itemIndex = m_listCtrl.GetNextSelectedItem(position);
			selIndexVec.push_back(itemIndex);
		}

		for(UINT i = 0; i < selIndexVec.size(); ++i)
		{
			m_moudleInfoArray[selIndexVec[i]]->bShowInList = (m_chkShow.GetCheck() == BST_CHECKED);
		}

		UpdateListItems();
		for(UINT i = 0; i < selIndexVec.size(); ++i)
		{	//重新设置选中项
			m_listCtrl.SetItemState(selIndexVec[i], LVIS_SELECTED, LVIS_SELECTED);
		}
		GetParent()->PostMessage(WM_UPDATECONFIGFLAG, 1);	//设置更新标志
	}
}

void CCustomSetDlg::OnBnClickedChkOpen()
{
	// TODO: Add your control notification handler code here
	int selCnt = m_listCtrl.GetSelectedCount();
	if(selCnt)
	{
		vector<int> selIndexVec;
		POSITION position =  m_listCtrl.GetFirstSelectedItemPosition();
		while (position)
		{
			int itemIndex = m_listCtrl.GetNextSelectedItem(position);
			selIndexVec.push_back(itemIndex);
		}

		//for(UINT i = 0; i < selIndexVec.size(); ++i)
		//{ 选中项均设置打开或关闭
		//	m_moudleInfoArray[selIndexVec[i]]->bDefaultOpen = (m_chkOpen.GetCheck() == BST_CHECKED);
		//}

		////【zy 2015-01-06 ↓】		//设置只有一个子系统可以默认打开
		if(m_chkOpen.GetCheck() == BST_CHECKED)
		{
			for(UINT i = 0; i < m_moudleInfoArray.size(); ++i)
			{	//设置有且仅有一个可以选中
				if(i == selIndexVec[0])
					m_moudleInfoArray[i]->bDefaultOpen = (m_chkOpen.GetCheck() == BST_CHECKED);
				else
					m_moudleInfoArray[i]->bDefaultOpen = 0;
			}
		}
		else
		{
			for(UINT i = 0; i < selIndexVec.size(); ++i)
			{	//选中项均设置打开或关闭
				m_moudleInfoArray[selIndexVec[i]]->bDefaultOpen = (m_chkOpen.GetCheck() == BST_CHECKED);
			}
		}
		////【zy 2015-01-06 ↑】


		UpdateListItems();
		for(UINT i = 0; i < selIndexVec.size(); ++i)
		{	//重新设置选中项
			m_listCtrl.SetItemState(selIndexVec[i], LVIS_SELECTED, LVIS_SELECTED);
		}
		GetParent()->PostMessage(WM_UPDATECONFIGFLAG, 1);	//设置更新标志
	}
}

void CCustomSetDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	if(pNMLV->uChanged == LVIF_STATE)
	{
		if(pNMLV->uNewState & LVIS_SELECTED)
		{
			if(pNMLV->iItem >= 0)
			{
				m_chkOpen.SetCheck(m_moudleInfoArray[pNMLV->iItem]->bDefaultOpen);
				m_chkShow.SetCheck(m_moudleInfoArray[pNMLV->iItem]->bShowInList);
			}
		}
	}

	*pResult = 0;
}

void CCustomSetDlg::OnBnClickedBUp()
{
	// TODO: Add your control notification handler code here

	int selCnt = m_listCtrl.GetSelectedCount();

	if(selCnt > 0)
	{
		vector<int> selIndexVec;
		POSITION position =  m_listCtrl.GetFirstSelectedItemPosition();
		while (position)
		{
			int itemIndex = m_listCtrl.GetNextSelectedItem(position);
			selIndexVec.push_back(itemIndex);
		}

		if(selIndexVec[0] > 0)
		{//顺序处理

			for(UINT i = 0; i < selIndexVec.size(); ++i)
			{
				LPSUBMOUDLEINFO lpSubmoudleInfoTemp = m_moudleInfoArray[selIndexVec[i] - 1];
				m_moudleInfoArray[selIndexVec[i] - 1] = m_moudleInfoArray[selIndexVec[i]];
				m_moudleInfoArray[selIndexVec[i]] = lpSubmoudleInfoTemp;
			}

			UpdateListItems();
			for(UINT i = 0; i < selIndexVec.size(); ++i)
			{	//重新设置选中项
				m_listCtrl.SetItemState(selIndexVec[i] - 1, LVIS_SELECTED, LVIS_SELECTED);
			}
			GetParent()->PostMessage(WM_UPDATECONFIGFLAG, 1);	//设置更新标志
		}
	}
}

void CCustomSetDlg::OnBnClickedBDown()
{
	// TODO: Add your control notification handler code here
	int selCnt = m_listCtrl.GetSelectedCount();

	if(selCnt > 0)
	{
		vector<int> selIndexVec;
		POSITION position =  m_listCtrl.GetFirstSelectedItemPosition();
		while (position)
		{
			int itemIndex = m_listCtrl.GetNextSelectedItem(position);
			selIndexVec.push_back(itemIndex);
		}

		if(selIndexVec[selCnt - 1] < m_listCtrl.GetItemCount() - 1)
		{

			for(int i = selCnt - 1; i >= 0 && i < selCnt; --i)
			{//逆序处理
				LPSUBMOUDLEINFO lpSubmoudleInfoTemp = m_moudleInfoArray[selIndexVec[i] + 1];
				m_moudleInfoArray[selIndexVec[i] + 1] = m_moudleInfoArray[selIndexVec[i]];
				m_moudleInfoArray[selIndexVec[i]] = lpSubmoudleInfoTemp;
			}

			UpdateListItems();
			for(UINT i = 0; i < selIndexVec.size(); ++i)
			{	//重新设置选中项
				m_listCtrl.SetItemState(selIndexVec[i] + 1, LVIS_SELECTED, LVIS_SELECTED);
			}
			GetParent()->PostMessage(WM_UPDATECONFIGFLAG, 1);	//设置更新标志
		}
	}
}

void CCustomSetDlg::ApplyConfig()
{
	////复制子系统数组
	for (UINT i = 0; i < m_moudleInfoArray.size(); ++i)
	{
		LPSUBMOUDLEINFO lpSubmoudleInfo = m_moudleInfoArray[i];
		*(RIMConfig::GetInstance()->m_moudleInfoArray[i])	= *lpSubmoudleInfo;
	}
}
