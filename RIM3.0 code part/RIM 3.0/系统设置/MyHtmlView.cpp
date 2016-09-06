// MyHtmlView.cpp : implementation file
//

#include "stdafx.h"
#include "MyHtmlView.h"


// CMyHtmlView

IMPLEMENT_DYNCREATE(CMyHtmlView, CHtmlView)

CMyHtmlView::CMyHtmlView()
{
}

CMyHtmlView::~CMyHtmlView()
{
}

void CMyHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyHtmlView, CHtmlView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CMyHtmlView diagnostics

#ifdef _DEBUG
void CMyHtmlView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CMyHtmlView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CMyHtmlView message handlers

int CMyHtmlView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
