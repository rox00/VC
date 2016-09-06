#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 

// CMyHtmlView html view

class CMyHtmlView : public CHtmlView
{
	DECLARE_DYNCREATE(CMyHtmlView)

public:
	CMyHtmlView();
	virtual ~CMyHtmlView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int						OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


