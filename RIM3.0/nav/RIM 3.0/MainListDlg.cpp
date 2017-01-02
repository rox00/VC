//********************************************************************
//	MAINLISTDLG.CPP �ļ�ע��
//	�ļ��� : MAINLISTDLG.CPP
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	���� : ����
//	����ʱ��: 2014/12/5 9:37
//	�ļ�����: 
//*********************************************************************	
// MainListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "MainListDlg.h"


// CMainListDlg dialog
#define		ITEMIMAGEINDEX_NOIMAGE	1000	/////���ؼ���û��ͼ�������  Ϊ�˼���WIN7��WIN8���Ժ��ϵͳ��������ȡֵ�Ĳ�ͬ
IMPLEMENT_DYNAMIC(CMainListDlg, CDialog)

CMainListDlg::CMainListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainListDlg::IDD, pParent)
	, m_bRegionOrStationFlag(TRUE)
{ 
	m_detectOnLineClk				= 0;

	m_selmode						= FALSE;
	m_bDisplayChange				= FALSE;
	m_wndAlphaValue					= 0;
	m_bAlreadyLogon					= FALSE;
	m_bMouseMove					= FALSE;
}

CMainListDlg::~CMainListDlg()
{
	m_stationArrayTemp.clear();
	m_userArrayTemp.clear();

	m_listFont.DeleteObject();
}

void CMainListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_B_BRANCH, m_btnBranch);
	DDX_Control(pDX, IDC_S_BRANCH, m_staticBranch);
	DDX_Radio(pDX, IDC_R_REGION, m_bRegionOrStationFlag);
	DDX_Control(pDX, IDC_B_INFO, m_btnInfo);
	DDX_Control(pDX, IDC_E_SEARCH, m_searchEdit);
}


BEGIN_MESSAGE_MAP(CMainListDlg, CDialog)
	ON_NOTIFY_RANGE(NM_DBLCLK, CMainListDlg_ID_TreeBegin, CMainListDlg_ID_TreeEnd, &CMainListDlg::OnNMDblclkTree)
	ON_NOTIFY_RANGE(TVN_ITEMHOVERCHANGE, CMainListDlg_ID_TreeBegin, CMainListDlg_ID_TreeEnd, &CMainListDlg::OnTvnItemHoverChangeTree)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipText)

	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_BN_CLICKED(IDOK, &CMainListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainListDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_R_REGION, &CMainListDlg::OnBnClickedRRegion)
	ON_BN_CLICKED(IDC_R_STATION, &CMainListDlg::OnBnClickedRStation)
	ON_EN_CHANGE(IDC_E_SEARCH, &CMainListDlg::OnEnChangeESearch)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CMainListDlg message handlers

void CMainListDlg::Init()
{
	EnableToolTips();


	{	//����ͼ���б�
		DWORD fuLoad		= LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_LOADTRANSPARENT | LR_SHARED;

		m_imgListStation.Create(16, 16, ILC_MASK|ILC_COLORDDB, 10, 3);
		HBITMAP hState0		= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\devicestate0.bmp")).c_str(),	IMAGE_BITMAP, 0, 0, fuLoad);
		HBITMAP hState1		= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\devicestate1.bmp")).c_str(),	IMAGE_BITMAP, 0, 0, fuLoad);
		HBITMAP hState2		= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\devicestate2.bmp")).c_str(),	IMAGE_BITMAP, 0, 0, fuLoad);
		HBITMAP hState3		= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\devicestate3.bmp")).c_str(),	IMAGE_BITMAP, 0, 0, fuLoad);
		HBITMAP hState4		= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\devicestate4.bmp")).c_str(),	IMAGE_BITMAP, 0, 0, fuLoad);
		HBITMAP hState5		= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\station.bmp")).c_str(),		IMAGE_BITMAP, 0, 0, fuLoad);
		HBITMAP hUserOff	= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\useroff.bmp")).c_str(),		IMAGE_BITMAP, 0, 0, fuLoad);
		HBITMAP hUserOn		= (HBITMAP)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\mainlist\\useron.bmp")).c_str(),			IMAGE_BITMAP, 0, 0, fuLoad);

		m_imgListStation.Add(CBitmap::FromHandle(hState0),	RGB(28,34,41));
		m_imgListStation.Add(CBitmap::FromHandle(hState1),	RGB(28,34,41));
		m_imgListStation.Add(CBitmap::FromHandle(hState2),	RGB(28,34,41));
		m_imgListStation.Add(CBitmap::FromHandle(hState3),	RGB(28,34,41));
		m_imgListStation.Add(CBitmap::FromHandle(hState4),	RGB(28,34,41));
		m_imgListStation.Add(CBitmap::FromHandle(hState5),	RGB(28,34,41));
		m_imgListStation.Add(CBitmap::FromHandle(hUserOff),	RGB(28,34,41));
		m_imgListStation.Add(CBitmap::FromHandle(hUserOn),	RGB(28,34,41));
	}

	CString parentPngPath						= RelativeToFullPath(_T("res\\PNG\\mainlist.png")).c_str();

	m_btnBranch.m_pngParentBkPath				= parentPngPath;
	m_btnBranch.m_normalPngPath					= RelativeToFullPath(_T("res\\PNG\\mainlist\\btnbranch.png")).c_str();
	m_btnBranch.m_overPngPath					= RelativeToFullPath(_T("res\\PNG\\mainlist\\btnbranch.png")).c_str();
	m_btnBranch.m_pushPngPath					= RelativeToFullPath(_T("res\\PNG\\mainlist\\btnbranch.png")).c_str();
	m_btnBranch.m_bFillOrTitleParentPng			= FALSE;
 
	m_btnInfo.m_pngParentBkPath					= parentPngPath;
	m_btnInfo.m_normalPngPath					= RelativeToFullPath(_T("res\\PNG\\mainlist\\info1.png")).c_str();
	m_btnInfo.m_overPngPath						= RelativeToFullPath(_T("res\\PNG\\mainlist\\info1.png")).c_str();
	m_btnInfo.m_pushPngPath						= RelativeToFullPath(_T("res\\PNG\\mainlist\\info1.png")).c_str();
	m_btnInfo.m_bFillOrTitleParentPng			= FALSE;


	m_staticBranch.m_pngParentBkPath			= parentPngPath;
	m_staticBranch.m_textColor					= RGB(0, 0, 0);
	m_staticBranch.m_bFillOrTitleParentPng		= FALSE;


	m_rdioRegion.m_pngParentBkPath				= parentPngPath;
	m_rdioRegion.m_pngRadioSel					= RelativeToFullPath(_T("res\\PNG\\mainlist\\regionsel.png")).c_str();
	m_rdioRegion.m_pngRadioUnSel				= RelativeToFullPath(_T("res\\PNG\\mainlist\\regionunsel.png")).c_str();
	m_rdioRegion.SubclassDlgItem(IDC_R_REGION, this);

	m_rdioStation.m_pngParentBkPath				= parentPngPath;
	m_rdioStation.m_pngRadioSel					= RelativeToFullPath(_T("res\\PNG\\mainlist\\stationsel.png")).c_str();
	m_rdioStation.m_pngRadioUnSel				= RelativeToFullPath(_T("res\\PNG\\mainlist\\stationunsel.png")).c_str();
	m_rdioStation.SubclassDlgItem(IDC_R_STATION, this);

	CRect editTextRc							= CRect(0, 0, 0, 0);

	m_searchEdit.m_pngParentBkPath				= parentPngPath;
	m_searchEdit.m_pngBkPath					= RelativeToFullPath(_T("res\\PNG\\mainlist\\searcheditbk.png")).c_str();
	m_searchEdit.m_GrayTipText					= "����:վ�㡢�豸";
	m_searchEdit.m_textColor					= RGB(0, 0, 0);
	m_searchEdit.GetRect(editTextRc); editTextRc.top += 6; m_searchEdit.SetRect(editTextRc);
	m_searchEdit.SetMargins(4, 0);


	{	//���б��ʼ��

		m_listFont.CreatePointFont(100,_T("΢���ź�"));


		CRect listRect;
		GetDlgItem(IDC_P_LIST)->GetWindowRect(listRect);
		ScreenToClient(listRect);

		m_regionTree.m_pngParentBkPath			= RelativeToFullPath(_T("res\\PNG\\sizemiddle.png")).c_str();
		m_regionTree.m_pngTooltipBk				= RelativeToFullPath(_T("res\\PNG\\mainlist\\unsel.png")).c_str();
		m_regionTree.m_pngItemSel				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemsel.png")).c_str();
		m_regionTree.m_pngItemOver				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemover.png")).c_str();
		m_regionTree.m_pngItemBtnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnsel.png")).c_str();
		m_regionTree.m_pngItemBtnUnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnunsel.png")).c_str();
		m_regionTree.m_pngVScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\vscrollThumb.png")).c_str();
		m_regionTree.m_pngHScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\hscrollThumb.png")).c_str();
		m_regionTree.m_textColor				= RGB(0, 0, 0);
		m_regionTree.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|TVS_FULLROWSELECT|TVS_TRACKSELECT|TVS_HASBUTTONS|TVS_LINESATROOT/*|TVS_CHECKBOXES*/|TVS_HASLINES, listRect, this, CMainListDlg_ID_regionTree);
		m_regionTree.SetImageList(&m_imgListStation, TVSIL_NORMAL);
		m_regionTree.SetFont(&m_listFont);



		m_stationTree.m_pngParentBkPath			= RelativeToFullPath(_T("res\\PNG\\sizemiddle.png")).c_str();
		m_stationTree.m_pngTooltipBk			= RelativeToFullPath(_T("res\\PNG\\mainlist\\unsel.png")).c_str();
		m_stationTree.m_pngItemSel				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemsel.png")).c_str();
		m_stationTree.m_pngItemOver				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemover.png")).c_str();
		m_stationTree.m_pngItemBtnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnsel.png")).c_str();
		m_stationTree.m_pngItemBtnUnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnunsel.png")).c_str();
		m_stationTree.m_pngVScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\vscrollThumb.png")).c_str();
		m_stationTree.m_pngHScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\hscrollThumb.png")).c_str();
		m_stationTree.m_textColor				= RGB(0, 0, 0);
		m_stationTree.Create(WS_CHILD|WS_TABSTOP|TVS_FULLROWSELECT|TVS_TRACKSELECT|TVS_HASBUTTONS|TVS_LINESATROOT/*|TVS_CHECKBOXES|TVS_HASLINES*/, listRect, this, CMainListDlg_ID_stationTree);
		m_stationTree.SetImageList(&m_imgListStation, TVSIL_NORMAL);
		m_stationTree.SetFont(&m_listFont);



		m_userTree.m_pngParentBkPath			= RelativeToFullPath(_T("res\\PNG\\sizemiddle.png")).c_str();
		m_userTree.m_pngTooltipBk				= RelativeToFullPath(_T("res\\PNG\\mainlist\\unsel.png")).c_str();
		m_userTree.m_pngItemSel					= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemsel.png")).c_str();
		m_userTree.m_pngItemOver				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemover.png")).c_str();
		m_userTree.m_pngItemBtnSel				= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnsel.png")).c_str();
		m_userTree.m_pngItemBtnUnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnunsel.png")).c_str();
		m_userTree.m_pngVScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\vscrollThumb.png")).c_str();
		m_userTree.m_pngHScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\hscrollThumb.png")).c_str();
		m_userTree.m_textColor					= RGB(0, 0, 0);
		m_userTree.Create(WS_CHILD|WS_TABSTOP|TVS_FULLROWSELECT|TVS_TRACKSELECT|TVS_HASBUTTONS|TVS_LINESATROOT/*|TVS_CHECKBOXES|TVS_HASLINES*/, listRect, this, CMainListDlg_ID_userTree);
		m_userTree.SetImageList(&m_imgListStation, TVSIL_NORMAL);
		m_userTree.SetFont(&m_listFont);

		m_recentTree.m_pngParentBkPath			= RelativeToFullPath(_T("res\\PNG\\sizemiddle.png")).c_str();
		m_recentTree.m_pngTooltipBk				= RelativeToFullPath(_T("res\\PNG\\mainlist\\unsel.png")).c_str();
		m_recentTree.m_pngItemSel				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemsel.png")).c_str();
		m_recentTree.m_pngItemOver				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemover.png")).c_str();
		m_recentTree.m_pngItemBtnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnsel.png")).c_str();
		m_recentTree.m_pngItemBtnUnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnunsel.png")).c_str();
		m_recentTree.m_pngVScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\vscrollThumb.png")).c_str();
		m_recentTree.m_pngHScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\hscrollThumb.png")).c_str();
		m_recentTree.m_textColor				= RGB(0, 0, 0);
		m_recentTree.Create(WS_CHILD|WS_TABSTOP|TVS_FULLROWSELECT|TVS_TRACKSELECT|TVS_HASBUTTONS|TVS_LINESATROOT/*|TVS_CHECKBOXES|TVS_HASLINES*/, listRect, this, CMainListDlg_ID_recentTree);
		m_recentTree.SetImageList(&m_imgListStation, TVSIL_NORMAL);
		m_recentTree.SetFont(&m_listFont);

		m_searchTree.m_pngParentBkPath			= RelativeToFullPath(_T("res\\PNG\\sizemiddle.png")).c_str();
		m_searchTree.m_pngTooltipBk				= RelativeToFullPath(_T("res\\PNG\\mainlist\\unsel.png")).c_str();
		m_searchTree.m_pngItemSel				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemsel.png")).c_str();
		m_searchTree.m_pngItemOver				= RelativeToFullPath(_T("res\\PNG\\mainlist\\treeitemover.png")).c_str();
		m_searchTree.m_pngItemBtnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnsel.png")).c_str();
		m_searchTree.m_pngItemBtnUnSel			= RelativeToFullPath(_T("res\\PNG\\mainlist\\listbtnunsel.png")).c_str();
		m_searchTree.m_pngVScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\vscrollThumb.png")).c_str();
		m_searchTree.m_pngHScrollThumb			= RelativeToFullPath(_T("res\\PNG\\scroll\\hscrollThumb.png")).c_str();
		m_searchTree.m_textColor				= RGB(0, 0, 0);
		m_searchTree.Create(WS_CHILD|WS_TABSTOP|TVS_FULLROWSELECT|TVS_TRACKSELECT|TVS_HASBUTTONS|TVS_LINESATROOT/*|TVS_CHECKBOXES|TVS_HASLINES*/, listRect, this, CMainListDlg_ID_searchTree);
		m_searchTree.SetImageList(&m_imgListStation, TVSIL_NORMAL);
		m_searchTree.SetFont(&m_listFont);

		UpdateWndShow();
	}

	m_infoTipDlg.Create(CInfoTipDlg::IDD, this);
	RepositionSubDlg();
}

void CMainListDlg::RepositionSubDlg()
{
	if (GetDlgItem(IDC_P_LIST))
	{
		HDWP hdwp = BeginDeferWindowPos(6);	//һ����̬�� �����Ի�İ�ť���ĸ��б���ֻ����һ��ͬʱ��ʾ�� �̹�4 ��
		CRect rcClient;
		GetClientRect(rcClient);

		CRect listRect;
		GetDlgItem(IDC_P_LIST)->GetWindowRect(listRect);
		ScreenToClient(listRect);
		long temBtm		= listRect.bottom;
		listRect.bottom = rcClient.bottom - 28;
		listRect.left	= rcClient.left + 2;
		listRect.right	= rcClient.right - 2;
		DeferWindowPos(hdwp, GetDlgItem(IDC_P_LIST)->GetSafeHwnd(), wndTop, listRect.left, listRect.top, listRect.Width(), listRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER);

		//if(m_stationTree.IsWindowVisible())
		DeferWindowPos(hdwp, m_stationTree.GetSafeHwnd(), wndTop, listRect.left, listRect.top, listRect.Width(), listRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER);

		//if(m_regionTree.IsWindowVisible())
		DeferWindowPos(hdwp, m_regionTree.GetSafeHwnd(), wndTop, listRect.left, listRect.top, listRect.Width(), listRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER);

		//if(m_userTree.IsWindowVisible())
		DeferWindowPos(hdwp, m_userTree.GetSafeHwnd(), wndTop, listRect.left, listRect.top, listRect.Width(), listRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER);

		//if(m_searchTree.IsWindowVisible())
		DeferWindowPos(hdwp, m_searchTree.GetSafeHwnd(), wndTop, listRect.left, listRect.top, listRect.Width(), listRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER);

		//if(m_recentTree.IsWindowVisible())
		DeferWindowPos(hdwp, m_recentTree.GetSafeHwnd(), wndTop, listRect.left, listRect.top, listRect.Width(), listRect.Height(), SWP_NOZORDER|SWP_NOOWNERZORDER);

		EndDeferWindowPos(hdwp);
		{
			CDC *pDC = GetDC();

			CRect memRect = CRect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
			pDC->FillSolidRect(memRect, g_transAlphaClr);
			ReleaseDC(pDC);
		}
	}
}

void CMainListDlg::CreateMapByStationGroup(map<tstring, vector<LPSTATIONINFO>> &byStationArray, tstring serchText /*= ""*/)
{
	byStationArray.clear();
	for (UINT i = 0; i < m_stationArrayTemp.size(); ++i)
	{
		LPSTATIONINFO lpstationinfo = m_stationArrayTemp.at(i);
		tstring name = CA2T(lpstationinfo->pRimStationTemp->sName.c_str());
		byStationArray[name].push_back(lpstationinfo);
	}

	//��־���÷�ʽ�� �ҵ��ַ�������ʾ��δ�ҵ��Ͳ���ʾ
	///Ŀǰֻ��վ����豸
	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byStationArray.begin(); it != byStationArray.end(); it++)
	{
		for (UINT j = 0; j < it->second.size(); ++j)
		{	//�ж�վ��Ŀ���ʾ��
			LPSTATIONINFO lpstationinfo = it->second.at(j);

			tstring name = CA2T(lpstationinfo->pRimStationTemp->sName.c_str());
			if(serchText.size() == 0 || tstring::npos != FindStrNoCase(name, 0, serchText))
				lpstationinfo->bShowInList = TRUE;
			else
				lpstationinfo->bShowInList = FALSE;

			for (UINT k = 0; k < lpstationinfo->devicesInfos.size(); ++k)
			{
				LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(k);

				tstring name = CA2T(lpdeviceinfo->pRimDeviceTemp->sName.c_str());
				if(serchText.size() == 0 || tstring::npos != FindStrNoCase(name, 0, serchText))
					lpdeviceinfo->bShowInList = TRUE;
				else
					lpdeviceinfo->bShowInList = FALSE;

				for(UINT l = 0; l < lpdeviceinfo->measureInfos.size(); ++l)
				{
					LPMEASUREINFO lpmeasureinfo = lpdeviceinfo->measureInfos.at(l);
					lpmeasureinfo->bShowInList = TRUE;	////�����ַ�������������
				}
			}
		}
	}
}

void CMainListDlg::CreateRecentMapByStationGroup(map<tstring, vector<LPSTATIONINFO>> &byStationArray, tstring serchText /*= _T("")*/)
{
	byStationArray.clear();
	for (UINT i = 0; i < m_stationArrayTemp.size(); ++i)
	{
		LPSTATIONINFO lpstationinfo = m_stationArrayTemp.at(i);
		tstring name = CA2T(lpstationinfo->pRimStationTemp->sName.c_str());
		byStationArray[name].push_back(lpstationinfo);
	}

	//��־���÷�ʽ�� �ҵ��ַ�������ʾ��δ�ҵ��Ͳ���ʾ
	///Ŀǰֻ��վ����豸
	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byStationArray.begin(); it != byStationArray.end(); it++)
	{
		for (UINT j = 0; j < it->second.size(); ++j)
		{	//�ж�վ��Ŀ���ʾ��
			LPSTATIONINFO lpstationinfo = it->second.at(j);

			////��zy 2015-06-18 ����
			bool bShowStation = false;
			map<string, map<string, vector<long>>>::iterator iterStation = m_recentMapTemp.begin();
			for ( ; iterStation != m_recentMapTemp.end(); ++iterStation)
			{	////��ѯ�Ƿ����ʹ�ù����վ��
				if(iterStation->first == lpstationinfo->pRimStationTemp->sStationNO)
				{	//�ҵ���ͬ��վ��
					bShowStation = true;
					break;
				}
			}
			////��zy 2015-06-18 ����

			tstring name = CA2T(lpstationinfo->pRimStationTemp->sName.c_str());
			if(serchText.size() == 0 || tstring::npos != FindStrNoCase(name, 0, serchText))
				lpstationinfo->bShowInList = bShowStation;
			else
				lpstationinfo->bShowInList = FALSE;

			for (UINT k = 0; k < lpstationinfo->devicesInfos.size(); ++k)
			{
				LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(k);

				////��zy 2015-06-18 ����
				bool bShowDevice = false;
				map<string, vector<long>>::iterator iterDevice;
				if(bShowStation)
				{	////��ѯ�Ƿ����ʹ�ù�����豸
					iterDevice = iterStation->second.begin();
					for ( ; iterDevice != iterStation->second.end(); ++iterDevice)
					{
						if(iterDevice->first == lpdeviceinfo->pRimDeviceTemp->sDeviceNO)
						{	//�ҵ���ͬ��վ��
							bShowDevice = true;
							break;
						}
					}
				}
				////��zy 2015-06-18 ����

				tstring name = CA2T(lpdeviceinfo->pRimDeviceTemp->sName.c_str());
				if(serchText.size() == 0 || tstring::npos != FindStrNoCase(name, 0, serchText))
					lpdeviceinfo->bShowInList = bShowDevice;
				else
					lpdeviceinfo->bShowInList = FALSE;

				for(UINT l = 0; l < lpdeviceinfo->measureInfos.size(); ++l)
				{
					LPMEASUREINFO lpmeasureinfo = lpdeviceinfo->measureInfos.at(l);

					////��zy 2015-06-18 ����
					bool bShowMeasure = false;
					if(bShowDevice)
					{	////��ѯ�Ƿ����ʹ���������
						vector<long>::iterator iterMeasure = iterDevice->second.begin();
						for ( ; iterMeasure != iterDevice->second.end(); ++iterMeasure)
						{
							if(*iterMeasure == lpmeasureinfo->pRtmDeviceFunTemp->nFunNO)
							{	//�ҵ���ͬ��վ��
								bShowMeasure = true;
								break;
							}
						}
					}
					////��zy 2015-06-18 ����

					lpmeasureinfo->bShowInList = bShowMeasure;	////�����ַ�������������
				}
			}
		}
	}
}

void CMainListDlg::CreateMapByRegionGroup(map<tstring, vector<LPSTATIONINFO>> &byRegionArray, tstring serchText /*= ""*/)
{

	byRegionArray.clear();
	for (UINT i = 0; i < m_stationArrayTemp.size(); ++i)
	{
		LPSTATIONINFO lpstationinfo = m_stationArrayTemp.at(i);
		tstring name = CA2T(lpstationinfo->pRegionNoTemp->sRegionName.c_str());
		byRegionArray[name].push_back(lpstationinfo);
	}

	//��־���÷�ʽ�� �ҵ��ַ�������ʾ��δ�ҵ��Ͳ���ʾ
	///Ŀǰֻ��վ����豸
	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byRegionArray.begin(); it != byRegionArray.end(); it++)
	{
		for (UINT j = 0; j < it->second.size(); ++j)
		{	//�ж�վ��Ŀ���ʾ��
			LPSTATIONINFO lpstationinfo = it->second.at(j);

			tstring name = CA2T(lpstationinfo->pRimStationTemp->sName.c_str());
			if(serchText.size() == 0 || tstring::npos != FindStrNoCase(name, 0, serchText))
				lpstationinfo->bShowInList = TRUE;
			else
				lpstationinfo->bShowInList = FALSE;

			for (UINT k = 0; k < lpstationinfo->devicesInfos.size(); ++k)
			{
				LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(k);

				tstring name = CA2T(lpdeviceinfo->pRimDeviceTemp->sName.c_str());
				if(serchText.size() == 0 || tstring::npos != FindStrNoCase(name, 0, serchText))
					lpdeviceinfo->bShowInList = TRUE;
				else
					lpdeviceinfo->bShowInList = FALSE;

				for(UINT l = 0; l < lpdeviceinfo->measureInfos.size(); ++l)
				{
					LPMEASUREINFO lpmeasureinfo = lpdeviceinfo->measureInfos.at(l);
					lpmeasureinfo->bShowInList = TRUE;	////�����ַ�������������
				}
			}
		}
	}

}

void CMainListDlg::CreateMapByUserGroup(map<tstring, vector<LPUSERINFO>> &byBranchArray, tstring serchText /*= ""*/)
{
	byBranchArray.clear();
	for (UINT i = 0; i < m_userArrayTemp.size(); ++i)
	{
		LPUSERINFO lpuserinfo = m_userArrayTemp.at(i);
		byBranchArray[lpuserinfo->branch].push_back(lpuserinfo);
	}

	//��־���÷�ʽ�� �ҵ��ַ�������ʾ��δ�ҵ��Ͳ���ʾ
	// �ж��û��Ƿ���ʾ
	for (map<tstring, vector<LPUSERINFO>>::iterator it = byBranchArray.begin(); it != byBranchArray.end(); it++)
	{
		for (UINT j = 0; j < it->second.size(); ++j)
		{
			LPUSERINFO lpuserinfo = it->second.at(j);
			
			CString str0 = lpuserinfo->UserName.c_str();	str0.MakeLower();
			CString str1 = serchText.c_str();				str1.MakeLower();

			if(serchText.size() == 0 || str0.Find(str1) != -1)
			{////��zy 2015-01-25 �����б��в���ʾ������Ϣ
				//if (lpuserinfo == RIMConfig::GetInstance()->m_pLogonUser)
				//	lpuserinfo->bShowInList = false;
				//else
				lpuserinfo->bShowInList = true;
			}
			else
			{
				lpuserinfo->bShowInList = false;
			}
		}
	}
}

void CMainListDlg::ReInitListByStation(CTreeCtrl* pTreeCtrl, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText /*= _T("")*/)
{
	if (pTreeCtrl == NULL || pTreeCtrl->GetSafeHwnd() == NULL)
		return;

	map<tstring, vector<LPSTATIONINFO>>		byStationArray;		//��վ�����е��ڴ�����
	CreateMapByStationGroup(byStationArray, serchText);

	pTreeCtrl->SetRedraw(FALSE);
	pTreeCtrl->DeleteAllItems();

	HTREEITEM hTreeStationItem = NULL;
	HTREEITEM hTreeDeviceItem = NULL;
	HTREEITEM hTreeMeasureItem = NULL;
	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byStationArray.begin(); it != byStationArray.end(); it++)
	{
		hTreeStationItem = NULL;		//��վ���ÿ�
		hTreeStationItem = pTreeCtrl->InsertItem(it->first.c_str());
		pTreeCtrl->SetItemData(hTreeStationItem, (DWORD_PTR)(it->second).at(0));	//Item������Item��Ϣ
		pTreeCtrl->SetItemImage(hTreeStationItem, 5, 0);

		for (UINT j = 0; j < it->second.size(); ++j)
		{
			hTreeDeviceItem = NULL;			//���豸�ÿ�
			LPSTATIONINFO lpstationinfo = it->second.at(j);
			for (UINT k = 0; k < lpstationinfo->devicesInfos.size(); ++k)
			{
				LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(k);
				tstring name = CA2T(lpdeviceinfo->pRimDeviceTemp->sName.c_str());
				{//�豸վ��״̬

					hTreeDeviceItem = pTreeCtrl->InsertItem(name.c_str(), hTreeStationItem, TVI_LAST);
					pTreeCtrl->SetItemImage(hTreeDeviceItem, lpdeviceinfo->deviceState, 0);	//RED

					pTreeCtrl->SetItemData(hTreeDeviceItem, (DWORD_PTR)lpdeviceinfo);	//Item������Item��Ϣ
				}

				hTreeMeasureItem = NULL;	//�²����ÿ�
				for (UINT l = 0; l < lpdeviceinfo->measureInfos.size(); ++l)
				{
					LPMEASUREINFO lpmeasureinfo = lpdeviceinfo->measureInfos.at(l);
					if(lpmeasureinfo->bShowInList)
					{
						if (hTreeMeasureItem)
							hTreeMeasureItem = pTreeCtrl->InsertItem(lpmeasureinfo->measueName.c_str(), hTreeDeviceItem, hTreeMeasureItem);
						else
							hTreeMeasureItem = pTreeCtrl->InsertItem(lpmeasureinfo->measueName.c_str(), hTreeDeviceItem);
						pTreeCtrl->SetItemData(hTreeMeasureItem, (DWORD_PTR)lpmeasureinfo);	//Item������Item��Ϣ
						pTreeCtrl->SetItemImage(hTreeMeasureItem, ITEMIMAGEINDEX_NOIMAGE, ITEMIMAGEINDEX_NOIMAGE);
					}
				}

				//���ϼ�������ʾ�������ֲ���ʾɾ����
				if (lpstationinfo->bShowInList == FALSE && lpdeviceinfo->bShowInList == FALSE)
				{	//վ�㲻��ʾ������� �豸����ʾ�ͽ��豸ɾ����
					pTreeCtrl->DeleteItem(hTreeDeviceItem);
					continue;
				}

				if (bExpandDevice)
					pTreeCtrl->Expand(hTreeDeviceItem, TVE_EXPAND);
			}
		}

		//������������ʾ����������ɾ����
		if (!pTreeCtrl->ItemHasChildren(hTreeStationItem))
		{	//վ�㲻��ʾ������� û��һ���豸
			pTreeCtrl->DeleteItem(hTreeStationItem);
			continue;
		}

		if (bExpandStation)
			pTreeCtrl->Expand(hTreeStationItem, TVE_EXPAND);
	}

	pTreeCtrl->SetRedraw(TRUE);

	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byStationArray.begin(); it != byStationArray.end(); it++)
		it->second.clear();
	byStationArray.clear();
}

void CMainListDlg::ReInitListByRecentStation(CTreeCtrl* pTreeCtrl, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText /*= _T("")*/)
{
	if (pTreeCtrl == NULL || pTreeCtrl->GetSafeHwnd() == NULL)
		return;

	map<tstring, vector<LPSTATIONINFO>>		byStationArray;		//��վ�����е��ڴ�����
	CreateRecentMapByStationGroup(byStationArray, serchText);

	pTreeCtrl->SetRedraw(FALSE);
	pTreeCtrl->DeleteAllItems();

	HTREEITEM hTreeStationItem = NULL;
	HTREEITEM hTreeDeviceItem = NULL;
	HTREEITEM hTreeMeasureItem = NULL;
	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byStationArray.begin(); it != byStationArray.end(); it++)
	{
		hTreeStationItem = NULL;		//��վ���ÿ�
		hTreeStationItem = pTreeCtrl->InsertItem(it->first.c_str());
		pTreeCtrl->SetItemData(hTreeStationItem, (DWORD_PTR)(it->second).at(0));	//Item������Item��Ϣ
		pTreeCtrl->SetItemImage(hTreeStationItem, 5, 0);

		for (UINT j = 0; j < it->second.size(); ++j)
		{
			hTreeDeviceItem = NULL;			//���豸�ÿ�
			LPSTATIONINFO lpstationinfo = it->second.at(j);
			for (UINT k = 0; k < lpstationinfo->devicesInfos.size(); ++k)
			{
				LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(k);
				tstring name = CA2T(lpdeviceinfo->pRimDeviceTemp->sName.c_str());
				{//�豸վ��״̬

					hTreeDeviceItem = pTreeCtrl->InsertItem(name.c_str(), hTreeStationItem, hTreeDeviceItem);
					pTreeCtrl->SetItemImage(hTreeDeviceItem, lpdeviceinfo->deviceState, 0);	//Black

					pTreeCtrl->SetItemData(hTreeDeviceItem, (DWORD_PTR)lpdeviceinfo);	//Item������Item��Ϣ
				}

				hTreeMeasureItem = NULL;	//�²����ÿ�
				for (UINT l = 0; l < lpdeviceinfo->measureInfos.size(); ++l)
				{
					LPMEASUREINFO lpmeasureinfo = lpdeviceinfo->measureInfos.at(l);
					if(lpmeasureinfo->bShowInList)
					{
						if (hTreeMeasureItem)
							hTreeMeasureItem = pTreeCtrl->InsertItem(lpmeasureinfo->measueName.c_str(), hTreeDeviceItem, hTreeMeasureItem);
						else
							hTreeMeasureItem = pTreeCtrl->InsertItem(lpmeasureinfo->measueName.c_str(), hTreeDeviceItem);
						pTreeCtrl->SetItemData(hTreeMeasureItem, (DWORD_PTR)lpmeasureinfo);	//Item������Item��Ϣ
						pTreeCtrl->SetItemImage(hTreeMeasureItem, ITEMIMAGEINDEX_NOIMAGE, ITEMIMAGEINDEX_NOIMAGE);
					}
				}

				//���ϼ�������ʾ�������ֲ���ʾɾ����
				if (lpstationinfo->bShowInList == FALSE && lpdeviceinfo->bShowInList == FALSE)
				{	//վ�㲻��ʾ������� �豸����ʾ�ͽ��豸ɾ����
					pTreeCtrl->DeleteItem(hTreeDeviceItem);
					continue;
				}

				if (bExpandDevice)
					pTreeCtrl->Expand(hTreeDeviceItem, TVE_EXPAND);
			}
		}

		//������������ʾ����������ɾ����
		if (!pTreeCtrl->ItemHasChildren(hTreeStationItem))
		{	//վ�㲻��ʾ������� û��һ���豸
			pTreeCtrl->DeleteItem(hTreeStationItem);
			continue;
		}

		if (bExpandStation)
			pTreeCtrl->Expand(hTreeStationItem, TVE_EXPAND);
	}

	pTreeCtrl->SetRedraw(TRUE);

	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byStationArray.begin(); it != byStationArray.end(); it++)
		it->second.clear();
	byStationArray.clear();
}

void CMainListDlg::ReInitListByRegion(CTreeCtrl* pTreeCtrl, BOOL bExpandRegion, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText /*= _T("")*/)
{
	if (pTreeCtrl == NULL || pTreeCtrl->GetSafeHwnd() == NULL)
		return;

	map<tstring, vector<LPSTATIONINFO>>		byRegionArray;		//���������е��ڴ�����
	CreateMapByRegionGroup(byRegionArray, serchText);


	pTreeCtrl->SetRedraw(FALSE);
	pTreeCtrl->DeleteAllItems();

	HTREEITEM hTreeRegionItem = NULL;	//�������ÿ�
	HTREEITEM hTreeStationItem = NULL;
	HTREEITEM hTreeDeviceItem = NULL;
	HTREEITEM hTreeMeasureItem = NULL;
	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byRegionArray.begin(); it != byRegionArray.end(); it++)
	{
		hTreeRegionItem = pTreeCtrl->InsertItem(it->first.c_str());
		pTreeCtrl->SetItemImage(hTreeRegionItem, ITEMIMAGEINDEX_NOIMAGE, ITEMIMAGEINDEX_NOIMAGE);

		hTreeStationItem = NULL;		//��վ���ÿ�
		for (UINT j = 0; j < it->second.size(); ++j)
		{
			LPSTATIONINFO lpstationinfo = it->second.at(j);

			tstring name = CA2T(lpstationinfo->pRimStationTemp->sName.c_str());
			if (hTreeStationItem)
				hTreeStationItem = pTreeCtrl->InsertItem(name.c_str(), hTreeRegionItem, hTreeStationItem);
			else
				hTreeStationItem = pTreeCtrl->InsertItem(name.c_str(), hTreeRegionItem);
			pTreeCtrl->SetItemData(hTreeStationItem, (DWORD_PTR)lpstationinfo);	//Item������Item��Ϣ
			pTreeCtrl->SetItemImage(hTreeStationItem, 5, 0);

			hTreeDeviceItem = NULL;			//���豸�ÿ�
			for (UINT k = 0; k < lpstationinfo->devicesInfos.size(); ++k)
			{
				LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(k);
				tstring name = CA2T(lpdeviceinfo->pRimDeviceTemp->sName.c_str());

				{//�豸վ��״̬

					hTreeDeviceItem = pTreeCtrl->InsertItem(name.c_str(), hTreeStationItem, hTreeDeviceItem);
					pTreeCtrl->SetItemImage(hTreeDeviceItem, lpdeviceinfo->deviceState, 0);	//Black

					pTreeCtrl->SetItemData(hTreeDeviceItem, (DWORD_PTR)lpdeviceinfo);	//Item������Item��Ϣ
				}

				hTreeMeasureItem = NULL;	//�²����ÿ�
				for (UINT l = 0; l < lpdeviceinfo->measureInfos.size(); ++l)
				{
					LPMEASUREINFO lpmeasureinfo = lpdeviceinfo->measureInfos.at(l);

					if(lpmeasureinfo->bShowInList)
					{
						if (hTreeMeasureItem)
							hTreeMeasureItem = pTreeCtrl->InsertItem(lpmeasureinfo->measueName.c_str(), hTreeDeviceItem, hTreeMeasureItem);
						else
							hTreeMeasureItem = pTreeCtrl->InsertItem(lpmeasureinfo->measueName.c_str(), hTreeDeviceItem);
						pTreeCtrl->SetItemData(hTreeMeasureItem, (DWORD_PTR)lpmeasureinfo);	//Item������Item��Ϣ
						pTreeCtrl->SetItemImage(hTreeMeasureItem, ITEMIMAGEINDEX_NOIMAGE, ITEMIMAGEINDEX_NOIMAGE);
					}
				}

				//���ϼ�������ʾ�������ֲ���ʾɾ����
				if (lpstationinfo->bShowInList == FALSE && lpdeviceinfo->bShowInList == FALSE)//
				{	//վ�㲻��ʾ������� �豸����ʾ�ͽ��豸ɾ����
					pTreeCtrl->DeleteItem(hTreeDeviceItem);
					continue;
				}

				if (bExpandDevice)
					pTreeCtrl->Expand(hTreeDeviceItem, TVE_EXPAND);
			}

			//������������ʾ����������ɾ����
			if (lpstationinfo->bShowInList == FALSE && !pTreeCtrl->ItemHasChildren(hTreeStationItem))//
			{	//վ�㲻��ʾ������� û��һ���豸
				pTreeCtrl->DeleteItem(hTreeStationItem);
				continue;
			}

			if (bExpandStation)
				pTreeCtrl->Expand(hTreeStationItem, TVE_EXPAND);
		}

		//������������ʾ����������ɾ����
		if (!pTreeCtrl->ItemHasChildren(hTreeRegionItem))//
		{	//û��һ��վ��
			pTreeCtrl->DeleteItem(hTreeRegionItem);
			continue;
		}

		if (bExpandRegion)
			pTreeCtrl->Expand(hTreeRegionItem, TVE_EXPAND);
	}

	pTreeCtrl->SetRedraw(TRUE);

	for (map<tstring, vector<LPSTATIONINFO>>::iterator it = byRegionArray.begin(); it != byRegionArray.end(); it++)
		it->second.clear();
	byRegionArray.clear();
}

void CMainListDlg::InitUserBranchInfo(BRANCHINFOTEMP &branchinfotemp, LPBRANCHINFO pBranch, vector<LPUSERINFO> &userArrayTemp, vector<LPBRANCHINFO> &branchArrayTemp)
{	////��ʼ��BRANCHINFOTEMP��Ϣ�ṹ
	if(pBranch == NULL)
		return;

	branchinfotemp.pBranchInfo = pBranch;

	for (vector<LPUSERINFO>::iterator it = userArrayTemp.begin(); it != userArrayTemp.end(); it++)
	{
		LPUSERINFO lpuserinfo = *it;

		if(lpuserinfo->pBranchInfo == pBranch)
			branchinfotemp.userVec.push_back(lpuserinfo);
	}

	for (vector<LPBRANCHINFO>::iterator it = branchArrayTemp.begin(); it != branchArrayTemp.end(); it++)
	{
		LPBRANCHINFO lpbranchinfo = *it;

		if(lpbranchinfo->pParentBranch == pBranch)
		{
			BRANCHINFOTEMP branchInfoTemp0;
			InitUserBranchInfo(branchInfoTemp0, lpbranchinfo, userArrayTemp, branchArrayTemp);
			branchinfotemp.subBranchInfoVec.push_back(branchInfoTemp0);
		}
	}
}

void CMainListDlg::CreateUserBranchInfo(BRANCHINFOTEMP	&branchinfotemp, tstring serchText /*= ""*/)
{
	vector<LPUSERINFO>	userArrayTemp = m_userArrayTemp;		//�û���Ϣ����
	//��־���÷�ʽ�� �ҵ��ַ�������ʾ��δ�ҵ��Ͳ���ʾ
	// �ж��û��Ƿ���ʾ
	for (vector<LPUSERINFO>::iterator it = userArrayTemp.begin(); it != userArrayTemp.end(); it++)
	{
		LPUSERINFO lpuserinfo = *it;

		if(serchText.size() == 0 || tstring::npos != FindStrNoCase(lpuserinfo->UserName, 0, serchText))
		{////��zy 2015-01-25 �����б��в���ʾ������Ϣ
			//if (lpuserinfo == RIMConfig::GetInstance()->m_pLogonUser)
			//	lpuserinfo->bShowInList = false;
			//else
			lpuserinfo->bShowInList = true;
		}
		else
		{
			lpuserinfo->bShowInList = false;
		}

	}

	vector<LPBRANCHINFO>	branchArrayTemp = m_branchArrayTemp;

	for(UINT i = 0; i < branchArrayTemp.size(); ++i)
	{
		LPBRANCHINFO pBranch = branchArrayTemp.at(i);
		if(pBranch->pParentBranch == NULL)
		{	////ֻ��ʼ������� �����
			BRANCHINFOTEMP branchInfoTemp0;
			InitUserBranchInfo(branchInfoTemp0, pBranch, userArrayTemp, branchArrayTemp);
			branchinfotemp.subBranchInfoVec.push_back(branchInfoTemp0);
		}
	}

	for(UINT i = 0; i < m_userArrayTemp.size(); ++i)
	{
		LPUSERINFO pUserInfo = m_userArrayTemp.at(i);
		if(pUserInfo->pBranchInfo == NULL)
		{	////ֻ��ʼ��û�з�֧���û�
			branchinfotemp.userVec.push_back(pUserInfo);
		}
	}
}

void CMainListDlg::InsertTreeByUserBranchInfo(CTreeCtrl* pTreeCtrl, HTREEITEM hParentBranchItem, BOOL bExpandBranch, BRANCHINFOTEMP &branchinfotemp)
{
	if (pTreeCtrl == NULL || pTreeCtrl->GetSafeHwnd() == NULL)
		return;

	TVSORTCB	tvSort; 
	tvSort.lParam		= (LPARAM)pTreeCtrl; 
	tvSort.lpfnCompare  = TreeCompareProc; 
	HTREEITEM hTreeBranchItem	= NULL;
	HTREEITEM hTreeUserItem		= NULL;

	{
		if(branchinfotemp.pBranchInfo)
		{
			hTreeBranchItem = pTreeCtrl->InsertItem(branchinfotemp.pBranchInfo->Deptname.c_str(), hParentBranchItem, TVI_LAST);
			pTreeCtrl->SetItemImage(hTreeBranchItem, ITEMIMAGEINDEX_NOIMAGE, ITEMIMAGEINDEX_NOIMAGE);
			pTreeCtrl->SetItemData(hTreeBranchItem, (DWORD_PTR)branchinfotemp.pBranchInfo);
		}

		for (vector<LPUSERINFO>::iterator it = branchinfotemp.userVec.begin(); it != branchinfotemp.userVec.end(); it++)
		{	////// �Ȳ��뵱ǰ��֧�µ��û��б�
			LPUSERINFO lpuserinfo = *it;

			//if(lpuserinfo == RIMConfig::GetInstance()->m_pLogonUser)
			//	continue;

			{//�豸վ��״̬

				if (lpuserinfo->state == USERINFO::USER_STATE_OFFLINE)//������
				{
					hTreeUserItem = pTreeCtrl->InsertItem(lpuserinfo->UserName.c_str(), hTreeBranchItem, TVI_LAST);
					pTreeCtrl->SetItemData(hTreeUserItem, (DWORD_PTR)lpuserinfo);	//�û�Item�����û���Ϣ
					pTreeCtrl->SetItemImage(hTreeUserItem, 6, 0);	//Black
				}
				else if (lpuserinfo->state == USERINFO::USER_STATE_ONLINE)//����
				{
					hTreeUserItem = pTreeCtrl->InsertItem(lpuserinfo->UserName.c_str(), hTreeBranchItem, TVI_FIRST);
					pTreeCtrl->SetItemData(hTreeUserItem, (DWORD_PTR)lpuserinfo);	//�û�Item�����û���Ϣ
					pTreeCtrl->SetItemImage(hTreeUserItem, 7, 0);	//Green
				}
			}

			////������������ʾ����������ɾ����
			if (lpuserinfo->bShowInList == FALSE && !pTreeCtrl->ItemHasChildren(hTreeUserItem))//
			{	//û��һ��վ��
				pTreeCtrl->DeleteItem(hTreeUserItem);
				continue;
			}
		}

		for (vector<BRANCHINFOTEMP>::iterator it = branchinfotemp.subBranchInfoVec.begin(); it != branchinfotemp.subBranchInfoVec.end(); it++)
		{	////���뱾��֧�µ��ӷ�֧�б�
			BRANCHINFOTEMP &subBranchInfo = *it;
			InsertTreeByUserBranchInfo(pTreeCtrl, hTreeBranchItem, bExpandBranch, subBranchInfo);
		}

		//������������ʾ����������ɾ����
		if (!pTreeCtrl->ItemHasChildren(hTreeBranchItem))
		{	//û��һ��վ��
			pTreeCtrl->DeleteItem(hTreeBranchItem);
			return;
		}
		else
		{
			tvSort.hParent		= hTreeBranchItem; 
			pTreeCtrl->SortChildrenCB(&tvSort);
		}

		if (bExpandBranch)
			pTreeCtrl->Expand(hTreeBranchItem, TVE_EXPAND);
	}

}

void CMainListDlg::ReInitListByUser(CTreeCtrl* pTreeCtrl, BOOL bExpandBranch, tstring serchText /*= _T("")*/)
{
	if (pTreeCtrl == NULL || pTreeCtrl->GetSafeHwnd() == NULL)
		return;
		
		TVSORTCB	tvSort; 
		tvSort.lParam		= (LPARAM)pTreeCtrl; 
		tvSort.lpfnCompare  = TreeCompareProc; 

		pTreeCtrl->SetRedraw(FALSE);
		pTreeCtrl->DeleteAllItems();
		HTREEITEM hTreeBranchItem = NULL;
		HTREEITEM hTreeUserItem = NULL;


		BRANCHINFOTEMP					branchinfotemp;
		CreateUserBranchInfo(branchinfotemp, serchText);
		InsertTreeByUserBranchInfo(pTreeCtrl, NULL, bExpandBranch, branchinfotemp);
		pTreeCtrl->SetRedraw(TRUE);
}

int CALLBACK CMainListDlg::TreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	CTreeCtrl*   pmyTreeCtrl	=   (CTreeCtrl*)   lParamSort; 
	LPINFOTYPE	lpinfotype1		=   (LPINFOTYPE)lParam1; 
	LPINFOTYPE	lpinfotype2		=   (LPINFOTYPE)lParam2; 

	if(lpinfotype1 == NULL || lpinfotype2 == NULL)
		return 0;
	else if(lpinfotype1)
	{
		switch(lpinfotype1->type)
		{
		case INFOTYPE::INFOTYPE_MEASURE:
			break;
		case INFOTYPE::INFOTYPE_DEVICE:
			break;
		case INFOTYPE::INFOTYPE_STATION:
			break;
		case INFOTYPE::INFOTYPE_USER:
			{
				if(lpinfotype2->type == INFOTYPE::INFOTYPE_BRANCH)
					return -1;
				else if(lpinfotype2->type == INFOTYPE::INFOTYPE_USER)
				{
					LPUSERINFO pUserInfo1 = (LPUSERINFO)lParam1;
					LPUSERINFO pUserInfo2 = (LPUSERINFO)lParam2;

					int state0 = 0, state1 = 0;
					if(pUserInfo1)
						state0 = pUserInfo1->state;
					if(pUserInfo2)
						state1 = pUserInfo2->state;

					return state1 - state0; ////��һ����ǰ���ظ�ֵ���ں󷵻���ֵ�����䷵��0;
				}
			}
			break;
		case INFOTYPE::INFOTYPE_BRANCH:
			{
				if(lpinfotype2->type == INFOTYPE::INFOTYPE_USER)
					return 1;
				else if(lpinfotype2->type == INFOTYPE::INFOTYPE_BRANCH)
					return 0;
			}
			break;
		}
	}
	
	return -1;
} 

void CMainListDlg::ReinitSearchListByFlag()
{
	CString strSearchText = _T("");
	m_searchEdit.GetWindowText(strSearchText);

	if(strSearchText.GetLength())
	{	//���ﶼ���޸�m_searchTree
		UpdateWndShow();

		if (m_selmode == 0)
		{
			if(m_bRegionOrStationFlag)
			{
				ReInitListByStation(&m_searchTree, TRUE, TRUE, (LPCTSTR)strSearchText);
			}
			else
			{
				ReInitListByRegion(&m_searchTree, TRUE, TRUE, TRUE, (LPCTSTR)strSearchText);
			}
		}
		else if(m_selmode == 1)
		{
			ReInitListByUser(&m_searchTree, TRUE, (LPCTSTR)strSearchText);
		}
		else if(m_selmode == 2)
		{
			ReInitListByRecentStation(&m_searchTree, TRUE, TRUE, (LPCTSTR)strSearchText);
		}
	}
	else
	{
		UpdateWndShow();
	}
}

void CMainListDlg::ShowStationOrUserByFlag(int selMode)
{
	m_searchEdit.SetWindowText(_T(""));
	m_selmode	= selMode;
	UpdateWndShow();
}

void CMainListDlg::UpdateWndShow()
{
	CRect listRect;
	GetDlgItem(IDC_P_LIST)->GetWindowRect(listRect);
	ScreenToClient(listRect);

	CString searchText = _T("");
	m_searchEdit.GetWindowText(searchText);

	if(searchText.GetLength())
	{
		//���ظ��б�
		m_userTree.ShowWindow(SW_HIDE);
		m_stationTree.ShowWindow(SW_HIDE);
		m_regionTree.ShowWindow(SW_HIDE);
		m_recentTree.ShowWindow(SW_HIDE);

		if(!m_searchTree.IsWindowVisible())
		{
			m_searchTree.MoveWindow(listRect);
			m_searchTree.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_searchTree.ShowWindow(SW_HIDE);

		if(m_selmode == 0)
		{
			{	//�ƶ�������

				CRect rcWnd;	//���ɹ�����ʱ�ͻ������С��
				GetWindowRect(rcWnd);

				CRect regionBtnRect;
				m_rdioRegion.GetWindowRect(regionBtnRect);
				ScreenToClient(regionBtnRect);

				CRect stationBtnRect;
				m_rdioStation.GetWindowRect(stationBtnRect);
				ScreenToClient(stationBtnRect);

				CRect editRect;
				m_searchEdit.GetWindowRect(editRect);
				ScreenToClient(editRect);

				if(editRect.left != stationBtnRect.right  + 2)
				{
					editRect.left = stationBtnRect.right  + 2 ;
					m_searchEdit.m_GrayTipText = "����:վ�㡢�豸";
					m_searchEdit.SetWindowPos(&wndTop, editRect.left, editRect.top, editRect.Width(), editRect.Height(), SWP_SHOWWINDOW);
					CRect editTextRc = CRect(0, 0, 0, 0);
					m_searchEdit.GetRect(editTextRc); editTextRc.top += 6; m_searchEdit.SetRect(editTextRc);
				}
			}

			//����ʾ������ť
			if(!m_rdioRegion.IsWindowVisible())
				m_rdioRegion.ShowWindow(SW_SHOW);
			if(!m_rdioStation.IsWindowVisible())
				m_rdioStation.ShowWindow(SW_SHOW);

			if(m_bRegionOrStationFlag)
			{
				if(!m_stationTree.IsWindowVisible())
				{
					m_stationTree.ShowWindow(SW_SHOW);
				}
				m_regionTree.ShowWindow(SW_HIDE);
				m_userTree.ShowWindow(SW_HIDE);
				m_recentTree.ShowWindow(SW_HIDE);
			}
			else
			{
				if(!m_regionTree.IsWindowVisible())
				{
					m_regionTree.ShowWindow(SW_SHOW);
				}
				m_stationTree.ShowWindow(SW_HIDE);
				m_userTree.ShowWindow(SW_HIDE);
				m_recentTree.ShowWindow(SW_HIDE);
			}
		}
		else if(m_selmode == 1)
		{
			{	//�ƶ�������
				CRect rcWnd;	//���ɹ�����ʱ�ͻ������С��
				GetWindowRect(rcWnd);

				CRect regionBtnRect;
				m_rdioRegion.GetWindowRect(regionBtnRect);
				ScreenToClient(regionBtnRect);

				CRect stationBtnRect;
				m_rdioStation.GetWindowRect(stationBtnRect);
				ScreenToClient(stationBtnRect);

				CRect editRect;
				m_searchEdit.GetWindowRect(editRect);
				ScreenToClient(editRect);

				if(editRect.left != regionBtnRect.left)
				{
					editRect.left = regionBtnRect.left;
					m_searchEdit.m_GrayTipText = "����:�û���";
					m_searchEdit.SetWindowPos(&wndTop, editRect.left,  editRect.top, editRect.Width(), editRect.Height(), SWP_SHOWWINDOW);
					CRect editTextRc = CRect(0, 0, 0, 0);
					m_searchEdit.GetRect(editTextRc); editTextRc.top += 6; m_searchEdit.SetRect(editTextRc);
				}
			}

			m_rdioRegion.ShowWindow(SW_HIDE);
			m_rdioStation.ShowWindow(SW_HIDE);


			if(!m_userTree.IsWindowVisible())
			{
				m_userTree.ShowWindow(SW_SHOW);
			}

			m_stationTree.ShowWindow(SW_HIDE);
			m_regionTree.ShowWindow(SW_HIDE);
			m_recentTree.ShowWindow(SW_HIDE);
		}
		else if(m_selmode == 2)
		{
			{	//�ƶ�������
				CRect rcWnd;	//���ɹ�����ʱ�ͻ������С��
				GetWindowRect(rcWnd);

				CRect regionBtnRect;
				m_rdioRegion.GetWindowRect(regionBtnRect);
				ScreenToClient(regionBtnRect);

				CRect stationBtnRect;
				m_rdioStation.GetWindowRect(stationBtnRect);
				ScreenToClient(stationBtnRect);

				CRect editRect;
				m_searchEdit.GetWindowRect(editRect);
				ScreenToClient(editRect);

				if(editRect.left != regionBtnRect.left)
				{
					editRect.left = regionBtnRect.left;
					m_searchEdit.m_GrayTipText = "����:���ʹ��վ�㣬�豸";
					m_searchEdit.SetWindowPos(&wndTop, editRect.left,  editRect.top, editRect.Width(), editRect.Height(), SWP_SHOWWINDOW);
					CRect editTextRc = CRect(0, 0, 0, 0);
					m_searchEdit.GetRect(editTextRc); editTextRc.top += 6; m_searchEdit.SetRect(editTextRc);
				}
			}

			m_rdioRegion.ShowWindow(SW_HIDE);
			m_rdioStation.ShowWindow(SW_HIDE);


			if(!m_recentTree.IsWindowVisible())
			{
				ReInitListByRecentStation(&m_recentTree, TRUE, TRUE);
				m_recentTree.ShowWindow(SW_SHOW);
			}

			m_stationTree.ShowWindow(SW_HIDE);
			m_regionTree.ShowWindow(SW_HIDE);
			m_userTree.ShowWindow(SW_HIDE);
		}
	}
}

void CMainListDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CMainListDlg::OnBnClickedCancel()
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

HBRUSH CMainListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if(pWnd == this)
		return g_hTransAlphaBrush;
	else
		return hbr;
}

BOOL CMainListDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

void CMainListDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CMainListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyle(WS_BORDER | WS_SIZEBOX | WS_DLGFRAME | DS_MODALFRAME, WS_POPUP);
	ModifyStyleEx(WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME, WS_EX_LAYERED);

	SetLayeredAttributes(g_transAlphaClr, 0, LWA_COLORKEY | LWA_ALPHA);

	Init();

	return TRUE;
}

void CMainListDlg::OnBnClickedRRegion()
{
	UpdateData(TRUE);
	UpdateWndShow();

	CString strSearchText = _T("");
	m_searchEdit.GetWindowText(strSearchText);

	if(strSearchText.GetLength())
		ReinitSearchListByFlag();
}

void CMainListDlg::OnBnClickedRStation()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateWndShow();

	CString strSearchText = _T("");
	m_searchEdit.GetWindowText(strSearchText);

	if(strSearchText.GetLength())
		ReinitSearchListByFlag();
}

//void CMainListDlg::OnBnClickedBRefreash()
//{
//	// TODO: Add your control notification handler code here
//
//	RIMConfig::GetInstance()->ReInitStationInfo();
//	m_stationArrayTemp.clear();
//	m_stationArrayTemp	= RIMConfig::GetInstance()->m_stationArray;
//	ReInitListByRegion(&m_regionTree, TRUE, TRUE, FALSE);
//	ReInitListByStation(&m_stationTree, TRUE, FALSE);
//}

void CMainListDlg::OnEnChangeESearch()
{
	ReinitSearchListByFlag();
}

LRESULT CMainListDlg::OnNcHitTest(CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return HTTRANSPARENT;
}




void CMainListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������

	RepositionSubDlg();
}

void CMainListDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}

void CMainListDlg::OnNMDblclkTree(UINT ID, NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	CTreeCtrl *pTreeCtrl = NULL;

	switch(ID)
	{
	case CMainListDlg_ID_regionTree:
		pTreeCtrl = &m_regionTree;
		break;
	case CMainListDlg_ID_stationTree:
		pTreeCtrl = &m_stationTree;
		break;
	case CMainListDlg_ID_userTree:
		pTreeCtrl = &m_userTree;
		break;
	case CMainListDlg_ID_searchTree:
		pTreeCtrl = &m_searchTree;
		break;
	case CMainListDlg_ID_recentTree:
		pTreeCtrl = &m_recentTree;
		break;
	}

	if(pTreeCtrl)
	{
		HTREEITEM hItem = pTreeCtrl->GetSelectedItem();
		LPINFOTYPE pInfoType = (LPINFOTYPE)pTreeCtrl->GetItemData(hItem);

		if(pInfoType)
		{
			switch(pInfoType->type)
			{
			case INFOTYPE::INFOTYPE_MEASURE:
				{
					LPMEASUREINFO lpmeasureinfo = (LPMEASUREINFO)pInfoType;

					{	////���ʹ���б�
						string	sStationNO	= lpmeasureinfo->pRtmDeviceFunTemp->sStationNO;
						string	sDeviceNO	= lpmeasureinfo->pRtmDeviceFunTemp->sDeviceNO;
						long	nFunNO		= lpmeasureinfo->pRtmDeviceFunTemp->nFunNO;

						if(m_recentMapTemp[sStationNO][sDeviceNO].end() == find(m_recentMapTemp[sStationNO][sDeviceNO].begin(), m_recentMapTemp[sStationNO][sDeviceNO].end(), nFunNO))
							m_recentMapTemp[sStationNO][sDeviceNO].push_back(nFunNO);
					}
 
					if(	lpmeasureinfo->measueName == _T("��Ƶ����")	||
						lpmeasureinfo->measueName == _T("��Ƶ����")	||
						lpmeasureinfo->measueName == _T("��Ƶ����")	||
						lpmeasureinfo->measueName == _T("��Ƶ����")	||
						lpmeasureinfo->measueName == _T("��ɢɨ��")	||
						lpmeasureinfo->measueName == _T("Ƶ��ɨ��")	||
						lpmeasureinfo->measueName == _T("����ɨ��")	||
						lpmeasureinfo->measueName == _T("ITU����")	
						)
					{	////��ѯ������ģ���·��
						tstring strfullPath = _T("");
						for (UINT i = 0; i < RIMConfig::GetInstance()->m_moudleInfoArray.size(); ++i)
						{
							if(RIMConfig::GetInstance()->m_moudleInfoArray[i]->bShowInList)
							{
								LPSUBMOUDLEINFO lpsubmoudleinfo = RIMConfig::GetInstance()->m_moudleInfoArray[i];

								if(lpsubmoudleinfo->moudleName == _T("������"))
								{	//////�����ȡ��ǰ
									strfullPath = RelativeToFullPath(lpsubmoudleinfo->moudleEXEPath);
									break;
								}
							}
						}

						if(strfullPath.size() && IsFileExist(strfullPath.c_str()))
						{
							DWORD data[3] = {0};
							data[0] = atoi(lpmeasureinfo->pRtmDeviceFunTemp->sStationNO.c_str());
							data[1] = atoi(lpmeasureinfo->pRtmDeviceFunTemp->sDeviceNO.c_str());
							data[2] = lpmeasureinfo->pRtmDeviceFunTemp->nFunNO;
							LPRIM30MSG pMsgTemp = RIM30MSG::MakeMsg(RIM30GUID_INITPROCESS, RIM30GUID_INITPROCESS, RIM30MSG_MEASUREDF_START_ASK, 
								RIM30MSG::DATATYPE_BINARY, (LPBYTE)data, sizeof(data));

							//GetRIM30Server()->PushSendMsg(pMsgTemp);
							GetRIM30Server()->OpenSubSystem(string(CT2A(strfullPath.c_str())), pMsgTemp);
						}
					}
				}
				break;
			case INFOTYPE::INFOTYPE_DEVICE:
				break;
			case INFOTYPE::INFOTYPE_STATION:
				break;
			case INFOTYPE::INFOTYPE_USER:
				{
					LPUSERINFO pUserInfo = (LPUSERINFO)pInfoType;

					if (pUserInfo && pUserInfo->pChatThread == NULL)
					{
						pUserInfo->pChatThread	= (CWinThread*)AfxBeginThread(RUNTIME_CLASS(CThreadChatDlg), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
						pUserInfo->pChatThread->m_bAutoDelete = TRUE;
						((CThreadChatDlg*)pUserInfo->pChatThread)->SetUser(pUserInfo);
						pUserInfo->pChatThread->ResumeThread();
					}
					else if (pUserInfo && pUserInfo->pChatThread)
					{
						((CThreadChatDlg*)pUserInfo->pChatThread)->ActiveWnd();
					}
				}
				break;
			default:break;
			}
		}
		
	}

	*pResult = 0;
}

void CMainListDlg::OnTvnItemHoverChangeTree(UINT ID, NMHDR *pNMHDR, LRESULT *pResult)
{
	LPITEMHOVER pITEMHOVER = reinterpret_cast<LPITEMHOVER>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//return;
	CTreeCtrl *pTreeCtrl = NULL;

	switch(ID)
	{
	case CMainListDlg_ID_regionTree:
		pTreeCtrl = &m_regionTree;
		break;
	case CMainListDlg_ID_stationTree:
		pTreeCtrl = &m_stationTree;
		break;
	case CMainListDlg_ID_userTree:
		pTreeCtrl = &m_userTree;
		break;
	case CMainListDlg_ID_searchTree:
		pTreeCtrl = &m_searchTree;
		break;
	case CMainListDlg_ID_recentTree:
		pTreeCtrl = &m_recentTree;
		break;
	}

	if(pTreeCtrl)
	{
		LPINFOTYPE pInfoType = NULL;
		if (pITEMHOVER->m_hHoverNewItem && (pInfoType = (LPINFOTYPE)pTreeCtrl->GetItemData(pITEMHOVER->m_hHoverNewItem)))
		{
			switch(pInfoType->type)
			{
			case INFOTYPE::INFOTYPE_MEASURE:
				{
					m_infoTipDlg.UpdateWindowShow(INFOTYPE::INFOTYPE_MEASURE, (LPVOID)pInfoType, FALSE);
				}
				break;
			case INFOTYPE::INFOTYPE_DEVICE:
				{
					m_infoTipDlg.UpdateWindowShow(INFOTYPE::INFOTYPE_DEVICE, (LPVOID)pInfoType, TRUE);
				}
				break;
			case INFOTYPE::INFOTYPE_STATION:
				{
					m_infoTipDlg.UpdateWindowShow(INFOTYPE::INFOTYPE_STATION, (LPVOID)pInfoType, TRUE);
				}
				break;
			case INFOTYPE::INFOTYPE_USER:
				{
					m_infoTipDlg.UpdateWindowShow(INFOTYPE::INFOTYPE_USER, (LPVOID)pInfoType, TRUE);
				}
				break;
			default:
				m_infoTipDlg.UpdateWindowShow(INFOTYPE::INFOTYPE_MEASURE, (LPVOID)pInfoType, FALSE);
				break;
			}

			{//�ƶ���Ϣ��ʾ���ڵ�λ��
				CRect screenRc;
				SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&screenRc, 0);

				CRect dlgRect, dlgRectTemp;
				m_infoTipDlg.GetWindowRect(dlgRect);

				CRect itemLineRect;
				pTreeCtrl->GetItemRect(pITEMHOVER->m_hHoverNewItem, itemLineRect, FALSE);
				pTreeCtrl->ClientToScreen(itemLineRect);

				if (itemLineRect.left - dlgRect.Width() > screenRc.left)
					dlgRectTemp.left = itemLineRect.left - dlgRect.Width();
				else
					dlgRectTemp.left = itemLineRect.right;
				if (itemLineRect.bottom - dlgRect.Height() < screenRc.bottom)
					dlgRectTemp.top = itemLineRect.bottom - dlgRect.Height();
				else
					dlgRectTemp.top = itemLineRect.top;

				m_infoTipDlg.SetWindowPos(&wndTop, dlgRectTemp.left, dlgRectTemp.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
			}
		}
		else
			m_infoTipDlg.UpdateWindowShow(INFOTYPE::INFOTYPE_USER, NULL, FALSE);
	}

	*pResult = 0;
}

void CMainListDlg::DoLogon(BOOL bAreadyLogon, BOOL bStationOrUser /*= FALSE*/)
{
	m_bAlreadyLogon = bAreadyLogon;
	m_selmode = bStationOrUser;

	if(m_bAlreadyLogon)
	{
		tstring str = RIMConfig::GetInstance()->m_pLogonUser->UserName + _T("  ") + RIMConfig::GetInstance()->m_pLogonUser->branch;
		m_recentMapTemp = RIMConfig::GetInstance()->m_recentMap;

		m_staticBranch.SetWindowText(str.c_str());

		m_stationArrayTemp.clear();
		m_userArrayTemp.clear();
		m_branchArrayTemp.clear();
		m_stationArrayTemp	= RIMConfig::GetInstance()->m_stationArray;
		m_userArrayTemp		= RIMConfig::GetInstance()->m_userArray;
		m_branchArrayTemp	= RIMConfig::GetInstance()->m_branchArray;

		{	//Ĭ��
			ReInitListByRegion(&m_regionTree, TRUE, TRUE, FALSE);
			ReInitListByStation(&m_stationTree, TRUE, FALSE);
			ReInitListByUser(&m_userTree, TRUE);
		}
		UpdateWndShow();

		SetTimer(1000, 500, NULL);
	}
	else
	{
		RIMConfig::GetInstance()->m_recentMap = m_recentMapTemp;
		KillTimer(1000);
		m_stationTree.DeleteAllItems();
		m_regionTree.DeleteAllItems();
		m_userTree.DeleteAllItems();
		m_searchTree.DeleteAllItems();

		m_stationArrayTemp.clear();
		m_userArrayTemp.clear();
		m_branchArrayTemp.clear();

		m_searchEdit.SetWindowText(_T(""));
	}
}

void CMainListDlg::OnDestroy()
{
	// TODO: Add your message handler code here

	CDialog::OnDestroy();
}

BOOL CMainListDlg::FlackVisibleItemByUserInfo(CTreeCtrl* pTreeCtrl, HTREEITEM hCurentItem, BOOL bVisible)
{
	if(pTreeCtrl == NULL || pTreeCtrl->GetSafeHwnd() == NULL)
		return FALSE;

	BOOL		bMakeFlack	= FALSE;	//�豸����ͼ����˸
	BOOL		bGoOut		= FALSE;
	if(pTreeCtrl->ItemHasChildren(hCurentItem))
	{
		BOOL			bExpand			= TVIS_EXPANDED & pTreeCtrl->GetItemState(hCurentItem, TVIS_EXPANDED);
		LPBRANCHINFO	lpbranchinfo	= (LPBRANCHINFO)pTreeCtrl->GetItemData(hCurentItem);

		if(bExpand && bVisible)	/////չ���ҿɼ� �Ͳ���Ҫ��ѯ��Ŀ¼��
		{
			if (pTreeCtrl->GetItemText(hCurentItem).GetLength() == 0)
				pTreeCtrl->SetItemText(hCurentItem, lpbranchinfo->Deptname.c_str());

			bMakeFlack = FALSE;
		}
		else
		{	////δչ���򲻿ɼ�  ����Ҫ��ѯ��Ŀ¼
			HTREEITEM	hChildItem		= pTreeCtrl->GetNextItem(hCurentItem, TVGN_CHILD);
			LPINFOTYPE	lpinfotype		= NULL;
			while (bGoOut == FALSE && hChildItem != NULL && (lpinfotype = (LPINFOTYPE)pTreeCtrl->GetItemData(hChildItem)))
			{
				////////���ж��Ƿ�����Ϣ
				switch (lpinfotype->type)
				{
				case INFOTYPE::INFOTYPE_USER:
					{
						LPUSERINFO lpuserinfo = (LPUSERINFO)lpinfotype;

						BOOL bFileFlack = FALSE;
						if (lpuserinfo->receiveFileQueue.GetQueueCount())	///�ж��ļ�����
							bFileFlack = TRUE;

						int		msgCnt	= lpuserinfo->receiveMsgQueue.GetQueueCount();

						if (msgCnt || bFileFlack)
						{	//����Ϣ Ҫ��˸

							bGoOut		= TRUE;	//δչ��������£����������˸������ͱ�������ѭ��
							bMakeFlack	= TRUE;	//����Ϣ
						}
					}
					break;
				case INFOTYPE::INFOTYPE_BRANCH:
					{
						bMakeFlack = FlackVisibleItemByUserInfo(pTreeCtrl, hChildItem, FALSE);

						////δչ��������£����������˸������ͱ�������ѭ��
						bGoOut = bMakeFlack;
					}
					break;
				}
				hChildItem = pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
			}

			if (bMakeFlack && bVisible)
			{	//Ҫ��˸

				if (pTreeCtrl->GetItemText(hCurentItem).GetLength())
					pTreeCtrl->SetItemText(hCurentItem, _T(""));
				else
					pTreeCtrl->SetItemText(hCurentItem, lpbranchinfo->Deptname.c_str());
			}
			else
			{	////��������û����Ϣ���ı���ָ�
				if (pTreeCtrl->GetItemText(hCurentItem).GetLength() == 0)
					pTreeCtrl->SetItemText(hCurentItem, lpbranchinfo->Deptname.c_str());
			}
		}
	}
	else
	{	/////û������  ˵�������ڸ�Ŀ¼��

		LPINFOTYPE	lpinfotype	= (LPINFOTYPE)pTreeCtrl->GetItemData(hCurentItem);
		if(lpinfotype)
		{////////���ж��Ƿ�����Ϣ
			switch (lpinfotype->type)
			{
			case INFOTYPE::INFOTYPE_USER:
				{
					LPUSERINFO lpuserinfo = (LPUSERINFO)lpinfotype;

					BOOL bFileFlack = FALSE;
					if (lpuserinfo->receiveFileQueue.GetQueueCount())	///�ж��ļ�����
						bFileFlack = TRUE;

					int		msgCnt	= lpuserinfo->receiveMsgQueue.GetQueueCount();

					if (msgCnt || bFileFlack)
					{	//��ʱ�޸�������ı�
						bMakeFlack = TRUE;	//����Ϣ
					}

					if(bMakeFlack)
					{
						if (pTreeCtrl->GetItemText(hCurentItem).GetLength())
							pTreeCtrl->SetItemText(hCurentItem, _T(""));
						else
							pTreeCtrl->SetItemText(hCurentItem, lpuserinfo->UserName.c_str());
					}
					else
					{////�������û����Ϣ���ı���ָ�
						if (pTreeCtrl->GetItemText(hCurentItem).GetLength() == 0)
							pTreeCtrl->SetItemText(hCurentItem, lpuserinfo->UserName.c_str());
					}
				}
				break;
			}
		}
	}

	return bMakeFlack;
}

void CMainListDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_userTree.GetSafeHwnd())
	{
		clock_t		curClr		= clock();
		BOOL		bMakeFlack	= FALSE;	//�豸����ͼ����˸
		HTREEITEM	hItem		= m_userTree.GetFirstVisibleItem();
		int			vCnt		= m_userTree.GetVisibleCount();
		while (hItem != NULL && vCnt >= 0)
		{
			bMakeFlack |= FlackVisibleItemByUserInfo(&m_userTree, hItem, TRUE);
			
			hItem = m_userTree.GetNextVisibleItem(hItem);
			vCnt--;
		}

		AfxGetMainWnd()->PostMessage(WM_FLICKERNOTIFYICON, bMakeFlack);

		////��zy 2015-05-06 ����  ÿ��һ������ȫ�������������ݰ�
		if(clock() - m_detectOnLineClk > 1000*60)
		{
			m_detectOnLineClk = clock();

			////��zy 2015-02-07 ����	///��ȫ���η��͹㲥��
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
			////��zy 2015-02-07 ����
		}
		////��zy 2015-05-06 ����
		
		//////ʵʱ�����û���״̬
		RIMConfig::GetInstance()->m_pLogonUser->stateUpdateClk = curClr;		////���ȸ��µ�¼�û���״̬ʱ��
		for (vector<LPUSERINFO>::iterator it = m_userArrayTemp.begin(); it != m_userArrayTemp.end(); it++)
		{	//�����Ժ���Ҫͨ���ӿ��������жϵ�
			LPUSERINFO lpuserinfo = *it;

			clock_t spanTime = curClr - lpuserinfo->stateUpdateClk;
			
			if(curClr - lpuserinfo->stateUpdateClk > 1000*60*5 && lpuserinfo->state == USERINFO::USER_STATE_ONLINE)
			{	//����и��û���״̬����5����û�и���ǿ�����߲������û��б� 
				lpuserinfo->state = USERINFO::USER_STATE_OFFLINE;
				UpdateUserListByMsg();
			}
			//else if(spanTime > 1000*60 && spanTime < 1000*60*2 && lpuserinfo->state == USERINFO::USER_STATE_ONLINE)
			//{//����и��û���״̬����һ��������������֮��û�и��������߾�������û����͸���״̬��Ϣ
			//	CNetData *pUDPData = new CNetData();
			//	string transBuf = CT2A(RIMConfig::GetInstance()->m_pLogonUser->name.c_str());
			//	pUDPData->MakeNETBuffer(
			//		CNetData::UDP_ONLINE_NOTIFY,
			//		RIMConfig::GetInstance()->m_pLogonUser,
			//		lpuserinfo,
			//		(LPBYTE)transBuf.c_str(),
			//		transBuf.size()
			//		);

			//	RIMConfig::GetInstance()->m_UDPSendQueue.Push(pUDPData);
			//}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CMainListDlg::UpdateUserListByItem(CTreeCtrl *pTreeCtrl, HTREEITEM hParentItem)
{
	if(pTreeCtrl == NULL || pTreeCtrl->GetSafeHwnd() == NULL)
		return;

	TVSORTCB	tvSort; 
	tvSort.lParam		= (LPARAM)pTreeCtrl; 
	tvSort.lpfnCompare  = TreeCompareProc; 

	if(hParentItem == NULL)
	{	/////�������
		if (m_userArrayTemp.size() != RIMConfig::GetInstance()->m_userArray.size())
		{
			m_userArrayTemp.clear();
			m_userArrayTemp		= RIMConfig::GetInstance()->m_userArray;

			BOOL		bFind			= FALSE;
			HTREEITEM	hItemUnknown	= NULL;
			//HTREEITEM	hItem			= pTreeCtrl->GetRootItem();
			//while (hItem != NULL)
			//{
			//	CString strText = pTreeCtrl->GetItemText(hItem);
			//	if (_tcscmp(USERINFO::UNKNOWNBRANCHSTR.c_str(), strText) == 0)
			//	{
			//		hItemUnknown = hItem;
			//		break;
			//	}
			//	hItem	= pTreeCtrl->GetNextItem(hItem, TVGN_NEXT);
			//}

			////�Ƿ��ҵ������û��ķ���
			//if(hItemUnknown == NULL)
			//{
			//	hItemUnknown = pTreeCtrl->InsertItem(USERINFO::UNKNOWNBRANCHSTR.c_str(),  -1, -1, NULL, TVI_FIRST);
			//	pTreeCtrl->SetItemImage(hItemUnknown, ITEMINDEX_NOIMAGE, ITEMINDEX_NOIMAGE);
			//}

			//���ҵ�ǰ�����е��û��Ƿ�����������û�������
			for(UINT i = 0; i < m_userArrayTemp.size(); ++i)
			{
				LPUSERINFO	lpuserinfo	= m_userArrayTemp.at(i);
				if(_tcscmp(USERINFO::UNKNOWNBRANCHSTR.c_str(), lpuserinfo->branch.c_str()) != 0)
					continue;

				BOOL		bFind		= FALSE;
				HTREEITEM	hChildItem	= pTreeCtrl->GetNextItem(hItemUnknown, TVGN_CHILD);
				while (hChildItem != NULL)
				{	
					CString name = pTreeCtrl->GetItemText(hChildItem);	///������ǰ���е������û���
					if(_tcscmp(name, lpuserinfo->UserName.c_str()) == 0)			
					{
						bFind = TRUE;
						break;											//˵���û��Ѿ����������б��м���������һ���û�
					}
					hChildItem = pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
				}

				if(bFind == FALSE)
				{	//���û�ҵ���Ҫ�����µ������û�
					if(lpuserinfo->state == USERINFO::USER_STATE_OFFLINE)//������ Ҳ����
					{
						HTREEITEM hTreeUserItem = pTreeCtrl->InsertItem(lpuserinfo->UserName.c_str(), hItemUnknown, TVI_LAST);
						pTreeCtrl->SetItemData(hTreeUserItem, (DWORD_PTR)lpuserinfo);	//�û�Item�����û���Ϣ
						pTreeCtrl->SetItemImage(hTreeUserItem, 6, 0);	//Black
					}
					else if(lpuserinfo->state == USERINFO::USER_STATE_ONLINE)//���� �������
					{
						HTREEITEM hTreeUserItem = pTreeCtrl->InsertItem(lpuserinfo->UserName.c_str(), hItemUnknown, TVI_FIRST);
						pTreeCtrl->SetItemData(hTreeUserItem, (DWORD_PTR)lpuserinfo);	//�û�Item�����û���Ϣ
						pTreeCtrl->SetItemImage(hTreeUserItem, 7, 0);	//Green
					}
				}
			}
		}
		else
		{
			HTREEITEM	hItem			= pTreeCtrl->GetRootItem();
			while (hItem != NULL)
			{
				UpdateUserListByItem(pTreeCtrl, hItem);
				hItem	= pTreeCtrl->GetNextItem(hItem, TVGN_NEXT);
			}

			tvSort.hParent = hParentItem;
			pTreeCtrl->SortChildrenCB(&tvSort);
		}
	}
	else
	{	////��鱾hParentItem���µ�����
		if(pTreeCtrl->ItemHasChildren(hParentItem))
		{	/////��Item������
			HTREEITEM	hChildItem	= pTreeCtrl->GetNextItem(hParentItem, TVGN_CHILD);
			while (hChildItem)
			{
				if(pTreeCtrl->ItemHasChildren(hChildItem))
				{
					UpdateUserListByItem(pTreeCtrl, hChildItem);

					tvSort.hParent = hChildItem;
					pTreeCtrl->SortChildrenCB(&tvSort);
				}
				else
				{
					LPINFOTYPE pInfoType = (LPINFOTYPE)pTreeCtrl->GetItemData(hChildItem);

					if(pInfoType)
					{
						switch(pInfoType->type)
						{
						case INFOTYPE::INFOTYPE_USER:
							{
								LPUSERINFO pUserInfo = (LPUSERINFO)pInfoType;

								if (pUserInfo)
								{
									if(pUserInfo->state == USERINFO::USER_STATE_OFFLINE)						//������
										pTreeCtrl->SetItemImage(hChildItem, 6, 0);	//Black
									else if(pUserInfo->state == USERINFO::USER_STATE_ONLINE)					//����
										pTreeCtrl->SetItemImage(hChildItem, 7, 0);	//Green
								}
							}
							break;
						}
					}
				}

				hChildItem = pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
			}

			tvSort.hParent = hParentItem;
			pTreeCtrl->SortChildrenCB(&tvSort);
		}
		else
		{	/////��Item������
			LPINFOTYPE pInfoType = (LPINFOTYPE)pTreeCtrl->GetItemData(hParentItem);

			if(pInfoType)
			{
				switch(pInfoType->type)
				{
				case INFOTYPE::INFOTYPE_USER:
					{
						LPUSERINFO pUserInfo = (LPUSERINFO)pInfoType;

						if (pUserInfo)
						{
							if(pUserInfo->state == USERINFO::USER_STATE_OFFLINE)						//������
								pTreeCtrl->SetItemImage(hParentItem, 6, 0);	//Black
							else if(pUserInfo->state == USERINFO::USER_STATE_ONLINE)					//����
								pTreeCtrl->SetItemImage(hParentItem, 7, 0);	//Green
						}
					}
					break;
				}
			}
		}
	}

}

void CMainListDlg::UpdateUserListByMsg()
{	//����Ӧ����Ӧ�����б��������ԭ״̬��Ч��

	UpdateUserListByItem(&m_userTree, NULL);
}

void CMainListDlg::UpdateChildDeviceStatus(CTreeCtrl* pTreeCtrl, HTREEITEM hParentItem)
{
	HTREEITEM	hChildItem	= pTreeCtrl->GetNextItem(hParentItem, TVGN_CHILD);
	while (hChildItem)
	{
		LPINFOTYPE pInfoType = (LPINFOTYPE)pTreeCtrl->GetItemData(hChildItem);
		switch(pInfoType->type)
		{
		case INFOTYPE::INFOTYPE_DEVICE:
			{
				LPDEVICEINFO pDeviceInfo = (LPDEVICEINFO)pInfoType;

				int normalImgSel = 0;
				int selectImgSel = 0;
				pTreeCtrl->GetItemImage(hChildItem, normalImgSel, selectImgSel);
				if(normalImgSel != pDeviceInfo->deviceState)
					pTreeCtrl->SetItemImage(hChildItem, pDeviceInfo->deviceState, 0);
			}
			break;
		default:
			UpdateChildDeviceStatus(pTreeCtrl, hChildItem);
			break;
		}

		hChildItem = pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
	}
}

void CMainListDlg::UpdateStationStatustByMsg()
{	//����Ӧ����Ӧ�����б��������ԭ״̬��Ч��

	{	////����������ʾ�б�
		HTREEITEM	hItem		= m_regionTree.GetRootItem();
		int			vCnt		= m_regionTree.GetCount();
		while (hItem != NULL && vCnt >= 0)
		{
			UpdateChildDeviceStatus(&m_regionTree, hItem);

			hItem = m_regionTree.GetNextItem(hItem, TVGN_NEXT);
			vCnt--;
		}
	}

	{	////����վ����ʾ�б�
		HTREEITEM	hItem		= m_stationTree.GetRootItem();
		int			vCnt		= m_stationTree.GetCount();
		while (hItem != NULL && vCnt >= 0)
		{
			UpdateChildDeviceStatus(&m_stationTree, hItem);

			hItem = m_stationTree.GetNextItem(hItem, TVGN_NEXT);
			vCnt--;
		}
	}
}

void CMainListDlg::ShowAllMsg()
{
	if (m_userTree.GetSafeHwnd())
	{
		for (UINT i = 0; i < m_userArrayTemp.size(); ++i)
		{
			LPUSERINFO pUserInfo = (LPUSERINFO)m_userArrayTemp.at(i);
			if (pUserInfo)
			{
				int msgCnt = pUserInfo->receiveMsgQueue.GetQueueCount();
				msgCnt += pUserInfo->receiveFileQueue.GetQueueCount();
				msgCnt += pUserInfo->sendFileQueue.GetQueueCount();
				if (msgCnt && pUserInfo->pChatThread == NULL)	//��Ϣ������Ϊ�գ��һ�û�����촰���߳�
				{
					pUserInfo->pChatThread	= (CWinThread*)AfxBeginThread(RUNTIME_CLASS(CThreadChatDlg), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
					pUserInfo->pChatThread->m_bAutoDelete = TRUE;
					((CThreadChatDlg*)pUserInfo->pChatThread)->SetUser(pUserInfo);
					pUserInfo->pChatThread->ResumeThread();
				}
				else if (pUserInfo && pUserInfo->pChatThread)
				{
					((CThreadChatDlg*)pUserInfo->pChatThread)->ActiveWnd();
				}
			}
		}
	}

}

void CMainListDlg::SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	m_wndAlphaValue = bAlpha;
	SetLayeredWindowAttributes(crKey, bAlpha, dwFlags);
}

void CMainListDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	if (m_bDisplayChange && AfxGetMainWnd())
	{
		AfxGetMainWnd()->PostMessage(WM_REPOSITIONSUBDLG);
		m_bDisplayChange = FALSE;
	}
}

LRESULT CMainListDlg::OnDisplayChange(WPARAM BitsPerPixel, LPARAM size)
{
	AfxGetMainWnd()->PostMessage(WM_REPOSITIONSUBDLG);
	//m_bDisplayChange = TRUE;
	return TRUE;
}

BOOL CMainListDlg::OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult ) 
{ 
	// ��Ҫ����ANSI��UNICODE���ָ�ʽ 
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR; 

	UINT nID = pNMHDR->idFrom; 

	// ���ش������Լ�������ToolTip��Ϣ 
	if(pNMHDR->code == TTN_NEEDTEXT && pTTT->uFlags & TTF_IDISHWND) 
	{
		if(nID == (UINT)m_rdioRegion.GetSafeHwnd())
		{
			_tcscpy_s(pTTT->szText, 80, _T("����������"));
		}
		else if(nID == (UINT)m_rdioStation.GetSafeHwnd())
		{
			_tcscpy_s(pTTT->szText, 80, _T("��վ������"));
		}
	}

	return TRUE; // ��Ϣ������� 
} 
