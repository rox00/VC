#pragma once

class CChatRichView : public CRichEditView
{
	DECLARE_DYNCREATE(CChatRichView)

protected:
	CChatRichView();           // protected constructor used by dynamic creation
	virtual ~CChatRichView();

	CPngControlScroll							m_HScroll;			//水平滚动条控件
	CPngControlScroll							m_VScroll;			//垂直滚动条控件
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL								PreTranslateMessage(MSG* pMsg);
	afx_msg int									OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void								OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void								OnEnProtected(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg HBRUSH 								OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH 								CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL 								OnEraseBkgnd(CDC* pDC);
	afx_msg void 								OnPaint();
	afx_msg void 								OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void 								OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void 								OnSize(UINT nType, int cx, int cy);
	afx_msg void 								OnMove(int x, int y);
	afx_msg void 								OnDropFiles(HDROP hDropInfo);
	virtual HRESULT								QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD dwReco, BOOL bReally, HGLOBAL hMetaFile);

public:
	afx_msg void OnDestroy();
};


