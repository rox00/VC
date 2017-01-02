//********************************************************************
//	MAINLISTDLG.H �ļ�ע��
//	�ļ���		: MAINLISTDLG.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	����		: ����
//	����ʱ��	: 2014/12/5 9:05
//	�ļ�����	: ������ʾվ�㼰�û��б�
//*********************************************************************	
#pragma once
#include "����\ThreadChatDlg.h"


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

	LPBRANCHINFO				pBranchInfo;			////��ǰ��֧��Ϣ
	vector<LPUSERINFO>			userVec;				////��ǰ��֧�µ��û��б���Ŀ
	vector<tagBRANCHINFOTEMP>	subBranchInfoVec;		////�����֧

}BRANCHINFOTEMP, *LPBRANCHINFOTEMP;

class CMainListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainListDlg)

public:
	CMainListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainListDlg();

// Dialog Data
	enum { IDD = IDD_MAINLISTDLG };

	//�б�����ID
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

	clock_t									m_detectOnLineClk;		//����Ƿ�����״̬		
	BOOL									m_bDisplayChange;		//��ʾ���ֱ��ʸı�
	bool									m_bMouseMove;			//�����ж�����Ƿ��Ƴ�������
	CRgn									m_wndRgn;				//��������

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
	// Returns:   BOOL						1 ����Ҫ��˸ 0��������˸
	// Qualifier:							������ؼ��е�һ��Item �����֧�µ��û�����Ϣʱ�����Ƿ���Ҫ��˸
	// Parameter: CTreeCtrl * pTreeCtrl		���ؼ�ָ��
	// Parameter: HTREEITEM hParentItem		��ľ��
	// Parameter: BOOL bVisible				ָ�����Ƿ�ɼ�
	//************************************
	BOOL									FlackVisibleItemByUserInfo(CTreeCtrl* pTreeCtrl, HTREEITEM hParentItem, BOOL bVisible);
	//************************************
	// Method:    UpdateUserListByItem
	// FullName:  CMainListDlg::UpdateUserListByItem
	// Access:    protected 
	// Returns:   void
	// Qualifier:							�����û��б�ָ��Item�����֧�µ������Ƿ����ߵ�״̬
	// Parameter: CTreeCtrl * pTreeCtrl		���ؼ���ָ��
	// Parameter: HTREEITEM hParentItem		����ľ��
	//************************************
	void									UpdateUserListByItem(CTreeCtrl *pTreeCtrl, HTREEITEM hParentItem);
	//************************************
	// Method:    ReInitListByStation
	// FullName:  CMainListDlg::ReInitListByStation
	// Access:    protected 
	// Returns:   void
	// Qualifier:							���³�ʼ����վ�����еĿؼ�
	// Parameter: CTreeCtrl * pTreeCtrl		�б�ؼ�ָ��
	// Parameter: BOOL bExpandStation		�Ƿ�չ��վ�����
	// Parameter: BOOL bExpandDevice		�Ƿ�չ���豸����
	//************************************
	void									ReInitListByStation(CTreeCtrl* pTreeCtrl, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText = _T(""));
	void									ReInitListByRecentStation(CTreeCtrl* pTreeCtrl, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText = _T(""));
	//************************************
	// Method:    ReInitListByRegion
	// FullName:  CMainListDlg::ReInitListByRegion
	// Access:    protected 
	// Returns:   void
	// Qualifier:							���³�ʼ�����������еĿؼ�
	// Parameter: CTreeCtrl * pTreeCtrl		�б�ؼ�ָ��
	// Parameter: BOOL bExpandRegion		�Ƿ�չ���������
	// Parameter: BOOL bExpandStation		�Ƿ�չ��վ�����
	// Parameter: BOOL bExpandDevice		�Ƿ�չ���豸����
	//************************************
	void									ReInitListByRegion(CTreeCtrl* pTreeCtrl, BOOL bExpandRegion, BOOL bExpandStation, BOOL bExpandDevice, tstring serchText = _T(""));
	//************************************
	// Method:    CreateUserBranchInfo
	// FullName:  CMainListDlg::CreateUserBranchInfo
	// Access:    protected 
	// Returns:   void
	// Qualifier:									�����û�����Ϣ����֧����Ϣ������һ��BRANCHINFOTEMP���Է�֧���û���һ��������Ϣ�顣
	// Parameter: BRANCHINFOTEMP & branchinfotemp	��νṹ˵��
	// Parameter: tstring serchText					���ݲ����ַ����ж��û��Ƿ���ʾ
	//************************************
	void									CreateUserBranchInfo(BRANCHINFOTEMP &branchinfotemp, tstring serchText /*= ""*/);
	//************************************
	// Method:    InitUserBranchInfo
	// FullName:  CMainListDlg::InitUserBranchInfo
	// Access:    protected 
	// Returns:   void
	// Qualifier:							����һ����֧������Ϣ���û�����Ϣ����֧����Ϣ����ʼ��һ��BRANCHINFOTEMP�ṹ���Է�֧���û���һ��������Ϣ�顣
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
	// Qualifier:									��һ��BRANCHINFOTEMP��Ϣ�ṹ��ѭ���������뵽һ�����ؼ���
	// Parameter: CTreeCtrl * pTreeCtrl				���ؼ�ָ��
	// Parameter: HTREEITEM hParentBranchItem		����ľ��
	// Parameter: BOOL bExpandBranch				���ڻ�����֧���Ƿ�Ҫչ�� 1 չ���� 2 ��չ����
	// Parameter: BRANCHINFOTEMP & branchinfotemp	BRANCHINFOTEMP��Ϣ�ṹ
	//************************************
	void 									InsertTreeByUserBranchInfo(CTreeCtrl* pTreeCtrl, HTREEITEM hParentBranchItem, BOOL bExpandBranch, BRANCHINFOTEMP &branchinfotemp);
	//************************************
	// Method:    ReInitListByUser
	// FullName:  CMainListDlg::ReInitListByUser
	// Access:    protected 
	// Returns:   void
	// Qualifier:							���³�ʼ���û��б�ؼ�
	// Parameter: CTreeCtrl * pTreeCtrl		�б�ؼ�ָ��
	// Parameter: BOOL bExpandBranch		�Ƿ�ȫ��չ��
	//************************************
	void									ReInitListByUser(CTreeCtrl* pTreeCtrl, BOOL bExpandBranch, tstring serchText = _T(""));

	//************************************
	// Method:    ReinitListByFlag
	// FullName:  CMainListDlg::ReinitListByFlag
	// Access:    protected 
	// Returns:   void
	// Qualifier:							���ݵ�ǰ��־���³�ʼ�� ��ǰ������ʾ���б�
	//************************************
	void									ReinitSearchListByFlag();
	//************************************
	// Method:    RepositionSubDlg
	// FullName:  CMainListDlg::RepositionSubDlg
	// Access:    protected 
	// Returns:   void
	// Qualifier:							������Ҫ�ƶ����Ӵ���λ��
	// Parameter: bool bRedraw				�ڵ������Ƿ��ػ��Ӵ���
	//************************************
	void									RepositionSubDlg();
	//************************************
	// Method:    UpdateWndShow
	// FullName:  CMainListDlg::UpdateWndShow
	// Access:    protected 
	// Returns:   void
	// Qualifier:							���������Ĳ��������б�������ؿؼ���ʾ
	//************************************
	void									UpdateWndShow();
	void									UpdateChildDeviceStatus(CTreeCtrl* pTreeCtrl, HTREEITEM hParentItem);
	void									Init();

	vector<LPSTATIONINFO>					m_stationArrayTemp;		//վ����Ϣ���� �����豸���豸�������Ĳ���������Ϣ
	vector<LPUSERINFO>						m_userArrayTemp;		//�û���Ϣ����
	vector<LPBRANCHINFO>					m_branchArrayTemp;
	map<string, map<string, vector<long>>>	m_recentMapTemp;		//���ʹ����Ϣ���� �ֱ��Ӧ վ���ţ��豸��ţ����ܱ��

protected:
	BOOL									m_bAlreadyLogon;		//�Ƿ��Ѿ���¼�ɹ�

	CPngButton 								m_btnBranch;
	CPngButton								m_btnInfo;

	CPngStatic 								m_staticBranch;			//�û�������Ϣ

	CImageList								m_imgListStation;		//ͼ���б�
	CRIMTreeCtrl							m_stationTree;			//��վ����ʾ�б�
	CRIMTreeCtrl							m_regionTree;			//��������ʾ�б�
	CRIMTreeCtrl							m_userTree;				//��ϵ���б�
	CRIMTreeCtrl							m_recentTree;			//���ʹ���б�
	CRIMTreeCtrl							m_searchTree;			//�����б�

	int										m_selmode;				//�жϵ�ǰѡ�е��Ǽ��վ������ϵ�˽���  0����վ�㣬 1�����û��б�
	int										m_bRegionOrStationFlag;	//�Ƿ������վ�����				0��������   1����վ��
	CPngRadioButton							m_rdioRegion;
	CPngRadioButton							m_rdioStation;

	CPNGGrayTipEdit							m_searchEdit;			
	CFont									m_listFont;				//�б������

	HTREEITEM								m_hoverItem;
	CInfoTipDlg								m_infoTipDlg;

public:

	BYTE									m_wndAlphaValue;		//����XP��ȡ��׼ȷ������

	void									SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	void									DoLogon(BOOL bAreadyLogon, BOOL bStationOrUser = FALSE);
	void									UpdateUserListByMsg();
	void									UpdateStationStatustByMsg();
	void 									ShowAllMsg();
	void									ShowStationOrUserByFlag(int selMode);
};
