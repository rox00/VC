#pragma once
#include "..\resource.h"
#include "afxcmn.h"
#include "afxwin.h"


// CCustomSetDlg dialog

class CCustomSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomSetDlg)

public:
	CCustomSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomSetDlg();

// Dialog Data
	enum { IDD = IDD_SETDLG_CUSTOMSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	void										Init();
	void										UpdateListItems();

	afx_msg void 								OnBnClickedOk();
	afx_msg void 								OnBnClickedCancel();
	afx_msg BOOL 								OnEraseBkgnd(CDC* pDC);
	virtual BOOL								OnInitDialog();
	afx_msg void								OnBnClickedChkShow();
	afx_msg void								OnBnClickedChkOpen();
	afx_msg void								OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void 								OnBnClickedBUp();
	afx_msg void 								OnBnClickedBDown();

	//CListCtrl									m_listCtrl;
	CPngListCtrl								m_listCtrl;
	CButton 									m_chkOpen;
	CButton										m_chkShow;

	CPngButton 									m_btnUp;
	CPngButton 									m_btnDown;

	vector<LPSUBMOUDLEINFO>						m_moudleInfoArray;		//子模块信息数组

public:
	void										ApplyConfig();


};
