//********************************************************************
//	LOGONWNDDLG.H �ļ�ע��
//	�ļ���		: LOGONWNDDLG.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	����		: ����
//	����ʱ��	: 2014/12/5 9:05
//	�ļ�����	: ������ʾ��¼����
//*********************************************************************	
#pragma once
#include "afxwin.h"


// CLogonWndDlg dialog

class CLogonWndDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonWndDlg)

public:
	CLogonWndDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonWndDlg();

// Dialog Data
	enum { IDD = IDD_LOGONWNDDLG };

protected:
	static UINT __stdcall					ThreadDoLogon(void *pDt);
	BOOL									m_bDisplayChange;
	bool									m_bMouseMove;			//�����ж�����Ƿ��Ƴ�������
	////���������
	CRect									m_processRect;			//����������
	DWORD									m_processPercent0_100;	//����������
	DWORD									m_logonInfo[2];			//�����������������Ϣ�Ľṹ
	HANDLE									m_hThrdLogon;			//��¼�߳̾��
	Color									m_processBkColor;		//����������ɫ
	tstring									m_errorInfo;			//��¼���ؽ����Ϣ

	virtual void							DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();
	afx_msg void							OnMove(int x, int y);
	afx_msg LRESULT							OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);

	virtual BOOL 							OnInitDialog();
	afx_msg void							OnBnClickedButton1();
	afx_msg LRESULT							OnNcHitTest(CPoint point);
	afx_msg BOOL							OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void							OnSize(UINT nType, int cx, int cy);
	afx_msg void 							OnBnClickedChkPassword();
	afx_msg void 							OnBnClickedChkAutologon();
	afx_msg void							OnBnClickedChkOffline();
	afx_msg void 							OnEnChangeEUser();
	afx_msg void 							OnEnChangeEPass();
	afx_msg void 							OnEnChangeESevrip();
	afx_msg LRESULT							OnProgress(WPARAM wParam, LPARAM lParam);


	void									Init();
	void									UpdateSubWndShow();
	void									SetOrSaveSystemSet(BOOL bShowOrSave = TRUE);
	CPNGGrayTipEdit 						m_editUser;				//�û���
	CPNGGrayTipEdit 						m_editPass;				//����
	CPNGGrayTipEdit 						m_editDmsIP;			//DMS����IP
	CPNGGrayTipEdit 						m_editRmsIP;			//RMS����IP
	CPNGGrayTipEdit 						m_editSasIP;			//SAS����IP

	CPngButton								m_btnLogon;				//��¼
	
	CRgn									m_wndMoveRgn;			//����϶�����
	CRgn									m_wndRgn;				//��������
	CPngCheckBox 							m_chkPassword;			//��ס����
	CPngCheckBox 							m_chkAutoLogon;			//�Զ���¼
	CPngCheckBox							m_chkOffLine;			//�Ƿ��� ��
	CPngStatic 								m_staticVersion;		//�汾

public:
	BYTE									m_wndAlphaValue;		//����XP��ȡ��׼ȷ������
	void									SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	bool									Logon(BOOL bLogon);
};
