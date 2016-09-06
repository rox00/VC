//********************************************************************
//	CIRCLEDLG.H �ļ�ע��
//	�ļ���		: CIRCLEDLG.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	����		: ����
//	����ʱ��	: 2014/12/5 9:05
//	�ļ�����	: ������ʾԲ������
//*********************************************************************	
#pragma once


// CCircleDlg dialog

class CCircleDlg : public CDialog
{
	DECLARE_DYNAMIC(CCircleDlg)

public:
	CCircleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCircleDlg();

// Dialog Data
	enum { IDD = IDD_CIRCLEDLG };

protected:
	virtual void							DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg LRESULT							OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();

	virtual BOOL 							OnInitDialog();
	afx_msg void 							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 							OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL 							OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void 							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT							OnNcHitTest(CPoint point);
	afx_msg void							OnSize(UINT nType, int cx, int cy);
	afx_msg void							OnMouseLeave();
	afx_msg void 							OnDestroy();
	virtual BOOL 							PreTranslateMessage(MSG* pMsg);
	afx_msg void							OnMove(int x, int y);

	//************************************
	// Method:    RepositionSubMoudle
	// FullName:  CCircleDlg::RepositionSubMoudle
	// Access:    protected 
	// Returns:   void
	// Qualifier:							������ϵͳ��ť��λ��
	// Parameter: float offAngle			������ϵͳ����ƫ�ƵĽǶ�  ��������ϵͳ֮���Թ̶��Ƕȼ����
	//************************************
	void									RepositionSubMoudle(float offAngle = 0);
	//************************************
	// Method:    OpenSubSys
	// FullName:  CCircleDlg::OpenSubSys
	// Access:    protected 
	// Returns:   void
	// Qualifier:							����ϵͳģ��
	//************************************
	void									OpenSubSys(LPSUBMOUDLEINFO pSubMoudleInfo);
	//************************************
	// Method:    Init
	// FullName:  CCircleDlg::Init
	// Access:    protected 
	// Returns:   void
	// Qualifier:							���Ի���ĳ�ʼ�����������ɵ���һ��
	//************************************
	void									Init();
	//************************************
	// Method:    InitMoudleShowArray
	// FullName:  CCircleDlg::InitMoudleShowArray
	// Access:    protected 
	// Returns:   void
	// Qualifier:							��ʼ����ʾ����  m_moudleShowArray
	//************************************
	void									ReInitMoudleShowArray();

	BOOL									m_bDisplayChange;
	Image*									m_pImgLogoTemp;			//���ڻ���Բ�����������LogoͼƬ
	Image*									m_pImgCircleTemp;		//���ڻ���Բ�������ͼƬ
	vector<LPSUBMOUDLEINFO>					m_moudleShowArrayTemp;	//ֻ����Ԥ�������ļ�������ָ�룬�����ֶ�����

	bool									m_bMouseMove;			//�����ж�����Ƿ��Ƴ�������
	bool									m_bShowMainWnd;			//�����жϷ�����ʾ������������

	CRgn									m_wndCircleRgn;			//Բ���ڱ���������ռ������
	CRgn									m_wndLogoRgn;			//Logo�ڱ���������ռ������
	CRgn									m_wndRoleRgn;			//�����ڱ���������ռ������
	CRgn									m_wndRoleRgnAlpha;		//��m_wndRoleRgn����Բ����2������
	CRgn									m_wndUnRoleRgn;			//Բ�������뻷������򣬼����˻�����������߿����������

	SUBMOUDLEINFO							*m_pSubMoudleInfoHover;	//��굱ǰͣ���İ�ť
	double									m_offSetAngle;			//������ϵͳͼ�����ƫ�ƵĽǶ� ˳ʱ��Ϊ������ʱ��Ϊ��
	BOOL									m_bDrag;				//��ǰ�Ƿ����϶�״̬
	CPoint									m_dragPt;				//��ǰ�϶����������


	float									m_startAngle;			//�ڷŰ�ť����ʼ�Ƕ�
	float									m_stepAngle;			//ÿ����ť֮��ļ���Ƕ�
	float									m_angleMax;				//���ڵİڷŰ�ť�ĽǶ�

	HCURSOR									m_handDo;				//ץ��ͼ��
	HCURSOR									m_handUndo;				//δץ��ͼ��
	CToolTipCtrl							m_pngToolTipCtrl;

	CString									m_showText;
public:
	BOOL									m_bAlreadyLogon;		//�Ƿ��Ѿ���¼�ɹ�

	BYTE									m_wndAlphaValue;		//����XP��ȡ��׼ȷ������
	void									SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	void									ReUpdateFromConfig();
	void									DoLogon(BOOL bLogon);
};
