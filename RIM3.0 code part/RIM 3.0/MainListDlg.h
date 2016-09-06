//********************************************************************
//	MAINLISTDLG.H 文件注释
//	文件名		: MAINLISTDLG.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/5 9:05
//	文件描述	: 用于显示站点及用户列表
//*********************************************************************	
#pragma once
#include "聊天\ThreadChatDlg.h"


// CMainListDlg dialog

typedef class tagBRANCHINFOTEMP
{
public:
	tagBRANCHINFOTEMP()
	{
		pBranchInfo				= NULL;
	}
	~tagBRANCHINFOTEMP()
	{
	}

	LPBRANCHINFO				pBranchInfo;			////当前分支信息
	vector<LPUSERINFO>			userVec;				////当前分支下的用户列表项目
	vector<tagBRANCHINFOTEMP>	subBranchInfoVec;		////子项分支

}BRANCHINFOTEMP, *LPBRANCHINFOTEMP;

class CMainListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainListDlg)

public:
	CMainListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainListDlg();

// Dialog Data
	enum { IDD = IDD_MAINLISTDLG };

	//列表树的ID
	enum { 
		CMainListDlg_ID_TreeBegin = 1500	,
		CMainListDlg_ID_stationTree			,
		CMainListDlg_ID_regionTree			,
		CMainListDlg_ID_userTree			,
		CMainListDlg_ID_searchTree			,
		CMainListDlg_ID_recentTree			,
		CMainListDlg_ID_TreeEnd
	};

protected:
	HANDLE									m_hStationStatusThread;

	clock_t									m_detectOnLineClk;		//检查是否在线状态		
	BOOL									m_bDisplayChange;		//显示器分辨率改变
	bool									m_bMouseMove;			//用来判断鼠标是否移出本窗口
	CRgn									m_wndRgn;				//窗口区域

	static int CALLBACK						TreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	virtual void							DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();
	BOOL									OnInitDialog();
	afx_msg void							OnBnClickedRRegion();
	afx_msg void							OnBnClickedRStation();
	afx_msg void							OnSize(UINT nType, int cx, int cy);
	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void							OnEnChangeESearch();
	afx_msg LRESULT							OnNcHitTest(CPoint point);
	afx_msg void							OnDestroy();
	afx_msg void							OnTimer(UINT_PTR nIDEvent);
	afx_msg void							OnMove(int x, int y);
	afx_msg LRESULT							OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);
	afx_msg void							OnNMDblclkTree(UINT ID, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL							OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	void									OnTvnItemHoverChangeTree(UINT ID, NMHDR *pNMHDR, LRESULT *pResult);

	void 									CreateMapByStationGroup(__out map<tstring, vector<LPSTATIONINFO>> &byStationArray, tstring serchText = _T(""));
	void									CreateRecentMapByStationGroup(map<tstring, vector<LPSTATIONINFO>> &byStationArray, tstring serchText = _T(""));
	void 									CreateMapByRegionGroup(__out map<tstring, vector<LPSTATIONINFO>> &byRegionArray, tstring serchText = _T(""));
	void 									CreateMapByUserGroup(__out map<tstring, vector<LPUSERINFO>> &byBranchArray, tstring serchText = _T(""));

	//************************************
	// Method:    FlackVisibleItemByUserInfo
	// FullName:  CMainListDlg::FlackVisibleItemByUserInfo
	// Access:    protected 
	// Returns:   BOOL						1 代表要闪烁 0代表不需闪烁
	// Qualifier:							检查树控件中的一个Item 当其分支下的用户有消息时，其是否需要闪烁
	// Parameter: CTreeCtrl * pTreeCtrl		树控件指针
	// Parameter: HTREEITEM hParentItem		项的句柄
	// Parameter: BOOL bVisible				指定项是否可见
	//************************************
	BOOL									FlackVisibleItemByUserInfo(CTreeCtrl* pTreeCtrl, HTREEITEM hParentItem, BOOL bVisible);
	//************************************
	// Method:    UpdateUserListByItem
	// FullName:  CMainListDlg::UpdateUserListByItem
	// Access:    protected 
	// Returns:   void
	// Qualifier:							更新用户列表指定Item及其分支下的子项是否在线的状态
	// Parameter: CTreeCtrl * pTreeCtrl		树控件的指针
	// Parameter: HTREEITEM hParentItem		树项的句柄
	//************************************
	void									UpdateUserListByItem(CTreeCtrl *pTreeCtrl, HTREEITEM hParentItem);
	//************************************
	// Method:    ReInitListByStation
	// FullName:  CMainListDlg::ReInitListByStation
	// Access:    protected 
	// Returns:   void
	// Qualifier:							重新初始化按站点排列的控件
	// Parameter: CTreeCtrl * pTreeCtrl		列表控件指针
	// Parameter: BOOL bExpandStation		是否展开站点分组
	// Parameter: BOOL bExpandDevice		是否展开设备分组
	//************************************
	void									ReInitListByStation(CTreeCtrl* pTreeCtrl, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText = _T(""));
	void									ReInitListByRecentStation(CTreeCtrl* pTreeCtrl, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText = _T(""));
	//************************************
	// Method:    ReInitListByRegion
	// FullName:  CMainListDlg::ReInitListByRegion
	// Access:    protected 
	// Returns:   void
	// Qualifier:							重新初始化按区域排列的控件
	// Parameter: CTreeCtrl * pTreeCtrl		列表控件指针
	// Parameter: BOOL bExpandRegion		是否展开区域分组
	// Parameter: BOOL bExpandStation		是否展开站点分组
	// Parameter: BOOL bExpandDevice		是否展开设备分组
	//************************************
	void									ReInitListByRegion(CTreeCtrl* pTreeCtrl, BOOL bExpandRegion, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText = _T(""));
	//************************************
	// Method:    CreateUserBranchInfo
	// FullName:  CMainListDlg::CreateUserBranchInfo
	// Access:    protected 
	// Returns:   void
	// Qualifier:									根据用户组信息及分支组信息，创建一个BRANCHINFOTEMP的自分支到用户的一个集合信息组。
	// Parameter: BRANCHINFOTEMP & branchinfotemp	详参结构说明
	// Parameter: tstring serchText					根据查找字符串判定用户是否显示
	//************************************
	void									CreateUserBranchInfo(BRANCHINFOTEMP &branchinfotemp, tstring serchText /*= ""*/);
	//************************************
	// Method:    InitUserBranchInfo
	// FullName:  CMainListDlg::InitUserBranchInfo
	// Access:    protected 
	// Returns:   void
	// Qualifier:							根据一个分支机构信息和用户组信息及分支组信息，初始化一个BRANCHINFOTEMP结构的自分支到用户的一个集合信息组。
	// Parameter: BRANCHINFOTEMP & branchinfotemp
	// Parameter: LPBRANCHINFO pBranch
	// Parameter: vector<LPUSERINFO> & userArrayTemp
	// Parameter: vector<LPBRANCHINFO> & branchArrayTemp
	//************************************
	void 									InitUserBranchInfo(BRANCHINFOTEMP &branchinfotemp, LPBRANCHINFO pBranch, vector<LPUSERINFO> &userArrayTemp, vector<LPBRANCHINFO> &branchArrayTemp);
	//************************************
	// Method:    InsertTreeByUserBranchInfo
	// FullName:  CMainListDlg::InsertTreeByUserBranchInfo
	// Access:    protected 
	// Returns:   void
	// Qualifier:									将一个BRANCHINFOTEMP信息结构，循环遍历插入到一个树控件中
	// Parameter: CTreeCtrl * pTreeCtrl				树控件指针
	// Parameter: HTREEITEM hParentBranchItem		父项的句柄
	// Parameter: BOOL bExpandBranch				对于机构分支，是否要展开 1 展开， 2 不展开。
	// Parameter: BRANCHINFOTEMP & branchinfotemp	BRANCHINFOTEMP信息结构
	//************************************
	void 									InsertTreeByUserBranchInfo(CTreeCtrl* pTreeCtrl, HTREEITEM hParentBranchItem, BOOL bExpandBranch, BRANCHINFOTEMP &branchinfotemp);
	//************************************
	// Method:    ReInitListByUser
	// FullName:  CMainListDlg::ReInitListByUser
	// Access:    protected 
	// Returns:   void
	// Qualifier:							重新初始化用户列表控件
	// Parameter: CTreeCtrl * pTreeCtrl		列表控件指针
	// Parameter: BOOL bExpandBranch		是否全部展开
	//************************************
	void									ReInitListByUser(CTreeCtrl* pTreeCtrl, BOOL bExpandBranch, tstring serchText = _T(""));

	//************************************
	// Method:    ReinitListByFlag
	// FullName:  CMainListDlg::ReinitListByFlag
	// Access:    protected 
	// Returns:   void
	// Qualifier:							根据当前标志重新初始化 当前正在显示的列表
	//************************************
	void									ReinitSearchListByFlag();
	//************************************
	// Method:    RepositionSubDlg
	// FullName:  CMainListDlg::RepositionSubDlg
	// Access:    protected 
	// Returns:   void
	// Qualifier:							调整需要移动的子窗口位置
	// Parameter: bool bRedraw				在调整后是否重画子窗口
	//************************************
	void									RepositionSubDlg();
	//************************************
	// Method:    UpdateWndShow
	// FullName:  CMainListDlg::UpdateWndShow
	// Access:    protected 
	// Returns:   void
	// Qualifier:							根据所做的操作更新列表界面的相关控件显示
	//************************************
	void									UpdateWndShow();
	void									UpdateChildDeviceStatus(CTreeCtrl* pTreeCtrl, HTREEITEM hParentItem);
	void									Init();

	vector<LPSTATIONINFO>					m_stationArrayTemp;		//站点信息数组 包括设备及设备所包含的测量功能信息
	vector<LPUSERINFO>						m_userArrayTemp;		//用户信息数组
	vector<LPBRANCHINFO>					m_branchArrayTemp;
	map<string, map<string, vector<long>>>	m_recentMapTemp;		//最近使用信息数组 分别对应 站点编号，设备编号，功能编号

protected:
	BOOL									m_bAlreadyLogon;		//是否已经登录成功

	CPngButton 								m_btnBranch;
	CPngButton								m_btnInfo;

	CPngStatic 								m_staticBranch;			//用户部门信息

	CImageList								m_imgListStation;		//图像列表
	CRIMTreeCtrl							m_stationTree;			//按站点显示列表
	CRIMTreeCtrl							m_regionTree;			//按区域显示列表
	CRIMTreeCtrl							m_userTree;				//联系人列表
	CRIMTreeCtrl							m_recentTree;			//最近使用列表
	CRIMTreeCtrl							m_searchTree;			//搜索列表

	int										m_selmode;				//判断当前选中的是监测站还是联系人界面  0代表站点， 1代表用户列表
	int										m_bRegionOrStationFlag;	//是否按区域或按站点分组				0代表区域   1代表站点
	CPngRadioButton							m_rdioRegion;
	CPngRadioButton							m_rdioStation;

	CPNGGrayTipEdit							m_searchEdit;			
	CFont									m_listFont;				//列表的字体

	HTREEITEM								m_hoverItem;
	CInfoTipDlg								m_infoTipDlg;

public:

	BYTE									m_wndAlphaValue;		//兼容XP获取不准确的问题

	void									SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	void									DoLogon(BOOL bAreadyLogon, BOOL bStationOrUser = FALSE);
	void									UpdateUserListByMsg();
	void									UpdateStationStatustByMsg();
	void 									ShowAllMsg();
	void									ShowStationOrUserByFlag(int selMode);
};
