#pragma once
#include "resource.h"

// CInfoTipDlg 对话框

class CInfoTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoTipDlg)

public:
	CInfoTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoTipDlg();

// 对话框数据
	enum { IDD = IDD_INFOTIP };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL						OnEraseBkgnd(CDC* pDC);
	afx_msg void						OnPaint();
	virtual BOOL						OnInitDialog();
	afx_msg void						OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void						OnLButtonDown(UINT nFlags, CPoint point);
	virtual void 						OnCancel();
	virtual void 						OnOK();

	CFont								m_font;				//列表的字体

	DWORD								m_infoType;
	LPVOID								m_pInfo;
public:
	void								UpdateWindowShow(DWORD infoType, LPVOID pInfo, BOOL bShow);
	void								UpdateView(byte Alpha /*= 255*/);

};
