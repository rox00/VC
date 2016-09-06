#pragma once
#include "..\resource.h"
#include "SystemSetDlg.h"
#include "CustomSetDlg.h"
#include "MyHtmlView.h"
#include "afxwin.h"


// CDialogSet dialog

class CDialogSet : public CDialog
{
	DECLARE_DYNAMIC(CDialogSet)

public:
	CDialogSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogSet();

// Dialog Data
	enum { IDD = IDD_SETDLG };

protected:
	virtual void				DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void 				OnBnClickedOk();
	afx_msg void 				OnBnClickedCancel();
	afx_msg LRESULT				OnNcHitTest(CPoint point);
	afx_msg void				OnLbnSelchangeListselecte();

	BOOL						m_bOffLine;


	CSystemSetDlg				m_sysDlg;		//系统设置
	CCustomSetDlg				m_cusDlg;		//定制系统
	CMyHtmlView					*m_pHtmlView;

	Image*						m_pImageBkTemp;


	virtual BOOL				OnInitDialog();
	void						Init();
	void						RepositionSubDlg(bool bRedraw = false);
	afx_msg void				OnSize(UINT nType, int cx, int cy);
	afx_msg void 				OnBnClickedRSystemset();
	afx_msg void 				OnBnClickedRCustomset();
	void						UpdateWndShow();

	BOOL						m_bUpdateConfig;		//立即更新配置
	afx_msg LRESULT				OnUpdateConfigFlag(WPARAM wParam, LPARAM lParam);
	void						UpdateConfig();
	afx_msg void 				OnBnClickedApply();
	afx_msg BOOL 				OnEraseBkgnd(CDC* pDC);
	afx_msg void 				OnPaint();

	CPngButton					m_btnClose;
	CButton 					m_btnOk;
	CButton 					m_btnCancel;
	CButton 					m_btnApply;


	CPngListBox					m_listSelect;
	afx_msg void OnBnClickedBClose();
};
