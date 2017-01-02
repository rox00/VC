//********************************************************************
//	RIM 3.0DLG.H �ļ�ע��
//	�ļ���		: RIM 3.0DLG.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	����		: ����
//	����ʱ��	: 2014/12/5 9:04
//	�ļ�����	: ������ʾ�����Ĵ���
//*********************************************************************	
// RIM30Dlg.h : ͷ�ļ�
//

#pragma once

#include "LogonWndDlg.h"
#include "CircleDlg.h"
#include "DockDlg.h"
#include "MainListDlg.h"

// CRIM30Dlg �Ի���
class CRIM30Dlg : public CDialog
{
// ����
public:
	CRIM30Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CRIM30Dlg();	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RIM30_DIALOG };

	protected:
	virtual void							DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	static CRIM30Dlg*						m_pSizeWnd;
	static LRESULT CALLBACK					ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK					WndProcHook(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK					MouseHook(int code, WPARAM wParam, LPARAM lParam);



	////��zy 2014-11-20 ������ʾ����
	/************************************************************************************************************************************************
	��ʾͨ������������Ҫ��Ϊ����ģʽ��
	SHOWMODE_DOCKΪ������ͻ���ͣ���� ����������ʾ�Ĵ��ڵ����غ���ʾ��
	SHOWMODE_MENUΪͨ���Ҽ��˵���Ӧ�������ʾģʽ���������д�����ʾ�����ء�
	SHOWMODE_MAIN��ҪΪ˫����Բ�����ж��Ƿ���ʾ�����档
	SHOWMODE_MAIN�������ԣ�m_bShowMainImg������¼�������Ƿ���ʾ�����ڱ���SHOWMODE_DOCK��SHOWMODE_MENU����ģʽ����ʾ������֮�������

	m_clientBound m_clientcircleBound��Ҫ������ͣ��ʱͨ��m_bShowMainImg���ж���������ƶ�����������������Ļ��Χ
	************************************************************************************************************************************************/
	
	enum WNDDOCK{							//����ͣ������
		DOCK_NORMAL		,					//
		DOCK_LEFT		,
		DOCK_TOP		,
		DOCK_RIGHT		,
		DOCK_BOTTOM 	,
	};
	WNDDOCK									m_dockType;

 	enum SHOWOPERA{							//��ͬ������ ��ͬ����ʾģʽ
 		SHOWOPERA_DOCK	,					//ʵʱ���ͣ�����ܸ�����ʾ����
 		SHOWOPERA_MENU	,					//�˵�������ʾ������
		SHOWOPERA_MAIN	,					//��Ҫ����Ϊ���������ʾ���� ��Բ������
		SHOWOPERA_LOGON	,					//��¼������ʾ
 	};

	BOOL									m_bShowMainImg;								//�ж��������Ƿ�����ʾ
	CRect									m_clientRect;								//����Բ������������¼����ľ��εĺϼ�������ڿͻ�����λ�ã���Ҫ���������ƶ�ʱ�Ľ�����
	CRect									m_clientcircleRect;							//Բ����������ڿͻ�����λ�ã���Ҫ���ڼ���������������ʱ���ƶ�ʱ�ı߽�����

	////��zy 2014-11-20 ����


	BOOL									m_bBalloonShow;
	HICON									m_hIcon;
	NOTIFYICONDATA							m_icondata;									//����ͼ��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL 							OnInitDialog();
	afx_msg void 							OnPaint();
	afx_msg HCURSOR							OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL							OnEraseBkgnd(CDC* pDC);
	afx_msg void							OnSize(UINT nType, int cx, int cy);
	afx_msg void							OnMove(int x, int y);
	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void 							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 							OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void							OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void							OnMouseLeave();
	afx_msg void							OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void							OnDestroy();
	afx_msg LRESULT							OnNcHitTest(CPoint point);
	afx_msg void							OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void							OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void							OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void							OnClose();
	afx_msg void							OnTimer(UINT_PTR nIDEvent);

	//��Ӧ��󻯣���С�����رյȣ����������д�����ʾ������л�
	virtual BOOL							OnCommand(WPARAM wParam, LPARAM lParam);

	//************************************
	// Method:    RepositionSubDlg
	// FullName:  CRIM30Dlg::RepositionSubDlg
	// Access:    protected 
	// Returns:   void
	// Qualifier:							�����Ӵ��ڵ�λ��
	// Parameter: bool bRedraw				�������Ƿ��ػ��Ӵ���
	//************************************
	void									RepositionSubDlg(bool bRedraw = false);		//�����Ӵ��ڵ�λ��
	//************************************
	// Method:    PtInWindow
	// FullName:  CRIM30Dlg::PtInWindow
	// Access:    protected 
	// Returns:   BOOL
	// Qualifier:							�жϵ�ǰ���λ�� �Ƿ��ڵ�ǰ����ɼ��������ڵ�������
	// Parameter: CPoint scrPoint			�����Ļλ�á�
	//************************************
	BOOL									PtInWindow(CPoint scrPoint);
	//************************************
	// Method:    PtInDock
	// FullName:  CRIM30Dlg::PtInDock
	// Access:    protected 
	// Returns:   BOOL						TRUE�������ڸ�������   FALSE:�����ڸ�������
	// Qualifier:							�жϵ�ǰ���λ�� �Ƿ���ͣ���Ի������ڵ�������
	// Parameter: CPoint scrPoint			�����Ļλ�á�
	//************************************
	BOOL									PtInDock(CPoint scrPoint);

	//************************************
	// Method:    OnDockBound
	// FullName:  CRIM30Dlg::OnDockBound
	// Access:    protected 
	// Returns:   LRESULT					Ĭ�Ϸ���TRUE
	// Qualifier:							�Ի���ͣ����ʾ������л�
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	//************************************
	afx_msg LRESULT							OnShowDockBound(WPARAM wParam, LPARAM lParam);

	//************************************
	// Method:    OnShowMianWnd
	// FullName:  CRIM30Dlg::OnShowMianWnd
	// Access:    protected 
	// Returns:   LRESULT					Ĭ�Ϸ���TRUE
	// Qualifier:							��ӦWM_SHOWMAINWND��Ϣ ����������ʾ������֮������л�
	// Parameter: WPARAM wParam				����
	// Parameter: LPARAM lParam				����
	//************************************
	afx_msg LRESULT							OnShowMianWnd(WPARAM wParam, LPARAM lParam);
	//************************************
	// Method:    OnAlredyLogon
	// FullName:  CRIM30Dlg::OnAlredyLogon
	// Access:    protected 
	// Returns:   LRESULT					Ĭ�Ϸ���TRUE
	// Qualifier:							��ӦWM_ALREDYLOGON��Ϣ �ж��Ƿ��¼
	// Parameter: WPARAM wParam				0  ��¼ʧ��  1  ��¼�ɹ�  
	// Parameter: LPARAM lParam				����
	//************************************
	afx_msg LRESULT							OnAlredyLogon(WPARAM wParam, LPARAM lParam);


	//************************************
	// Method:    UpdateView
	// FullName:  CRIM30Dlg::UpdateView
	// Access:    protected 
	// Returns:   void
	// Qualifier:							����Layerd��������ʾ, ������������Ҫ�������죬�������������Ϊ�����ֻ���
	// Parameter: Image * pImageMain		��¼���������������ϲ�����Ҫ���Ƶ�ͼƬ
	// Parameter: Image * pImageSizeMid		��¼���������������м䲿����Ҫ���Ƶ�ͼƬ
	// Parameter: Image * pImageSizeBttom	��¼���������������²�����Ҫ���Ƶ�ͼƬ
	// Parameter: Image * pImageCircle		Բ������������Ҫ���Ƶ�ͼƬ
	// Parameter: Image * pImageLogo		Բ���������Ĳ�����Ҫ���Ƶ�LogoͼƬ
	// Parameter: byte Alpha				�������ͼƬ��͸����
	//************************************
	void									UpdateView(byte Alpha = 255);

	//************************************
	// Method:    ShowDock
	// FullName:  CRIM30Dlg::ShowDock
	// Access:    protected 
	// Returns:   void
	// Qualifier:							�����ڲ�ͬ��ʾģʽ֮��ִ���л���ʾ
	// Parameter: SHOWOPERA shwoOpera		ִ�иò���ʱ�������еĲ�������
	// Parameter: int nCmdShow				��ʾ��ʽ�� SW_SHOW��SW_HIDE
	//************************************
	void									ShowMode(SHOWOPERA shwoOpera, BOOL bShow);
	
	//************************************
	// Method:    OnIconMsg
	// FullName:  CRIM30Dlg::OnIconMsg
	// Access:    protected 
	// Returns:   LRESULT
	// Qualifier:							������Ӧ�������ĵ�����¼�
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	//************************************
	afx_msg LRESULT							OnIconMsg(WPARAM wParam, LPARAM lParam);


	afx_msg LRESULT							OnUpdateConfig(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnUpdateUserList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnUpdateStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnFlickerNotifyIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnRepositionSubdlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);

	void									Init();
	bool									Exit();
protected:

	Image*									m_pImgLogonTemp;		//��¼����ͼƬ��������������ε��ϲ�
	Image*									m_pImgSizeMidTemp;		//��������ʱ�����м䲿�ֵ�PNGͼƬ
	Image*									m_pImgSizebottomTemp;	//��������ʱ���Ƶײ���PNGͼƬ
	Image*									m_pImgMainListTemp;		//���ڻ��Ƶ�¼������������ϲ���PNGͼƬ
	Image*									m_pImgLogoTemp;			//���ڻ���Բ�����������LogoͼƬ
	Image*									m_pImgCircleTemp;		//���ڻ���Բ�������ͼƬ
	Image*									m_pImgChkSelTemp;		//���ڻ����²���ťѡ��״̬
	Image*									m_pImgChkUnSelTemp;		//���ڻ����²���ťδѡ��״̬


	BOOL									m_bAlreadyLogon;		//�Ƿ��Ѿ���¼�ɹ�
	BYTE									m_wndAlphaValue;		//�����ڵ�͸����ֵ ��Ϊ������ʹ��UpdateView�Ĵ��ڣ�����Ҫ��¼����͸��ֵ

	CDockDlg								m_dockDlg;				//ͣ���Ի���
	CLogonWndDlg							m_logonDlg;				//��¼�Ի���
	CCircleDlg								m_circleDlg;			//Բ���Ի���
	CMainListDlg							m_mainListDlg;			//�û���վ���б�Ի���


	HRGN									m_hRgnCircle;			//��Բ����ڿͻ���������
	HRGN									m_hRgnMain;				//��¼���û��б����ڵ�����ڿͻ���������
	HRGN									m_hRgnLogo;				//logo����ڿͻ���������
	CRgn									m_MainPixelRgn;			//��ǰ��ʾ���������� ������϶��

	int										m_selMode;				//�жϵ�ǰѡ�е��Ǽ��վ������ϵ�˽���  0����վ������ 1�����û��б� 2�������ʹ���б�
	Rect									m_rcStation;			//վ�������б�		��ͼ����
	Rect									m_rcUser;				//�û��б�			ѡ��Ļ�ͼ����
	Rect									m_rcRecent;				//���ʹ��			ѡ��Ļ�ͼ����
public:
};

