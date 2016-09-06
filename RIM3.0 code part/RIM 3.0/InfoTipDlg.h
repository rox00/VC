#pragma once
#include "resource.h"

// CInfoTipDlg �Ի���

class CInfoTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoTipDlg)

public:
	CInfoTipDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoTipDlg();

// �Ի�������
	enum { IDD = IDD_INFOTIP };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL						OnEraseBkgnd(CDC* pDC);
	afx_msg void						OnPaint();
	virtual BOOL						OnInitDialog();
	afx_msg void						OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void						OnLButtonDown(UINT nFlags, CPoint point);
	virtual void 						OnCancel();
	virtual void 						OnOK();

	CFont								m_font;				//�б������

	DWORD								m_infoType;
	LPVOID								m_pInfo;
public:
	void								UpdateWindowShow(DWORD infoType, LPVOID pInfo, BOOL bShow);
	void								UpdateView(byte Alpha /*= 255*/);

};
