#pragma once
#include "..\resource.h"
#include "afxwin.h"

// CSystemSetDlg dialog

class CSystemSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSystemSetDlg)

public:
	CSystemSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSystemSetDlg();

// Dialog Data
	enum { IDD = IDD_SETDLG_SYSTEMSET };

protected:
	virtual void							DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CButton									m_chkAutoStart;
	TCHAR									m_startDic[MAX_PATH];
	tstring									m_linkFileName;


	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnBnClickedChkAutostart();
	virtual BOOL							OnInitDialog();

public:
	void									ApplyConfig();
};
