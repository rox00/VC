//********************************************************************
//	PNGCONTROLSCROLL.CPP 文件注释
//	文件名 : PNGCONTROLSCROLL.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	作者 : 张永
//	创建时间: 2014/12/3 11:12
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PngControlScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///USAGE:  /////////////////////////////////////////////////////////
//StdAfx.h
//#define WINVER  0x0500
//
//CPngControlScroll m_HScroll;
//CPngControlScroll m_VScroll;
//
// void CXxxWnd::PreSubclassWindow() 
// {
// 	CWnd::PreSubclassWindow();
// 	m_HScroll.InitScrollBar(this, SB_HORZ);
// 	m_VScroll.InitScrollBar(this, SB_VERT);
// }
// 
// void CXxxWnd::OnPaint() 
// {
// 	CWnd::OnPaint();
// 	m_HScroll.UpdateScrollBar();
// 	m_VScroll.UpdateScrollBar();
// }
//
/////////////////////////////////////////////////////////////////////////////

#define SCROLLBAR_NORMAL_MINIMUM 45  //经验值,当Scrollbar宽度/高度小于这一数值时需要特殊处理,保证与系统的处理方式一致
/////////////////////////////////////////////////////////////////////////////
// CPngControlScroll

IMPLEMENT_DYNAMIC(CPngControlScroll, CStatic)
CPngControlScroll::CPngControlScroll()
{
	m_bFillOrTitleParentPng	= TRUE;// 父窗口的贴图模式 默认填充

	m_pImgParentBk			= NULL;

	m_pImgScrollBk			= NULL;
	m_pImgVScrollTop		= NULL;
	m_pImgVScrollBot		= NULL;
	m_pImgVScrollThumb		= NULL;
	m_pImgHScrollLeft		= NULL;
	m_pImgHScrollRight		= NULL;
	m_pImgHScrollThumb		= NULL;

}

CPngControlScroll::~CPngControlScroll()
{
}

BEGIN_MESSAGE_MAP(CPngControlScroll, CStatic)
	//{{AFX_MSG_MAP(CPngControlScroll)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPngControlScroll message handlers

void CPngControlScroll::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	if (m_nScrollStyle == SB_VERT) 
	{
		DrawVScroll(&dc);
	}
	else if(m_nScrollStyle == SB_HORZ)
	{
		DrawHScroll(&dc);
	}

	// Do not call CStatic::OnPaint() for painting messages
}

HBRUSH CPngControlScroll::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL CPngControlScroll::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

BOOL CPngControlScroll::InitScrollBar(CWnd *pCtrl, UINT nStyle)
{
	if (pCtrl == NULL) 
	{
		return FALSE;
	}
	m_pCtrl = pCtrl;

	int nx, ny;
	if (nStyle == SB_VERT) 
	{
		nx = ::GetSystemMetrics(SM_CXVSCROLL);
		ny = ::GetSystemMetrics(SM_CYVSCROLL);
	}
	else if(nStyle == SB_HORZ)
	{
		nx = GetSystemMetrics(SM_CXHSCROLL);
		ny = GetSystemMetrics(SM_CYHSCROLL);
	}
	else
	{
		return FALSE;
	}
	m_nScrollStyle = nStyle;


	CRect rect;
	pCtrl->GetWindowRect(rect);
	pCtrl->ModifyStyle(0, WS_CLIPSIBLINGS);
	//	pCtrl->SetWindowPos(&wndBottom, 0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

	CWnd* pParent = pCtrl->GetParent();
	pParent->ScreenToClient(rect);

	if (nStyle == SB_VERT) 
	{
		rect.left = rect.right- nx;		
	}
	else if(nStyle == SB_HORZ)
	{
		rect.top = rect.bottom - ny;
	}

	BOOL bResult = Create(NULL, WS_CHILD|SS_LEFT|WS_VISIBLE|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, rect, pParent);
	if (!bResult)
	{
		return FALSE;
	}

	UpdateScrollBar();
	return TRUE;
}

void CPngControlScroll::UpdateScrollBar()
{
	//OutputDebugString(_T("UpdateScrollBar"));
	if(!IsWindow(m_hWnd) || m_pCtrl == NULL)
	{
		return;
	}

	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	m_pCtrl->GetScrollInfo(m_nScrollStyle, &scrollInfo, SIF_ALL);
	if (scrollInfo.nPage < 0 || !m_pCtrl->IsWindowVisible()) 
	{
		ShowWindow(SW_HIDE);
		return;
	}

	//m_pCtrl->SetWindowPos(&wndBottom, 0,0,0,0,SWP_NOSIZE | SWP_NOMOVE |SWP_NOREDRAW);

	SCROLLBARINFO hscr;
	memset(&hscr, 0, sizeof(SCROLLBARINFO));
	hscr.cbSize = sizeof(SCROLLBARINFO);
	CRect rcScrollBar;

	if(m_nScrollStyle == SB_VERT)
	{
		::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_VSCROLL, &hscr);
		rcScrollBar = CRect(hscr.rcScrollBar);
		if((hscr.rgstate[0] & STATE_SYSTEM_OFFSCREEN) || (hscr.rgstate[0] & STATE_SYSTEM_INVISIBLE))
		{
			if(IsWindowVisible())
				ShowWindow(SW_HIDE);
			return;
		}
		//else if(!IsWindowVisible())
		//{
		//	ShowWindow(SW_SHOW);
		//}
	}
	else if(m_nScrollStyle == SB_HORZ)
	{
		::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_HSCROLL, &hscr);
		rcScrollBar = CRect(hscr.rcScrollBar);

		if((hscr.rgstate[0] & STATE_SYSTEM_OFFSCREEN) || (hscr.rgstate[0] & STATE_SYSTEM_INVISIBLE))
		{
			if(IsWindowVisible())
				ShowWindow(SW_HIDE);
			return;
		}
		//else if(!IsWindowVisible())
		//{
		//	ShowWindow(SW_SHOW);
		//}
	}


	GetParent()->ScreenToClient(&rcScrollBar);
		//TRACE("rcScrollBar.left = %d, rcScrollBar.top = %d\n", rcScrollBar.left, rcScrollBar.top);

	SetWindowPos(&wndTop, rcScrollBar.left, rcScrollBar.top, rcScrollBar.Width(), rcScrollBar.Height(), /*SWP_NOREDRAW|*/SWP_SHOWWINDOW);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
}

//draw vertical scrollbar
void CPngControlScroll::DrawVScroll(CDC* pDC)
{
	if(m_pCtrl == NULL)
	{
		return;
	}

	int cxButton     = ::GetSystemMetrics(SM_CXVSCROLL);
	int cyButton     = ::GetSystemMetrics(SM_CYVSCROLL);

	if(IsWindow(m_hWnd))
	{		
		SCROLLBARINFO vscr;
		memset(&vscr, 0, sizeof(SCROLLBARINFO));
		vscr.cbSize = sizeof(SCROLLBARINFO);
		if ((m_pCtrl->GetStyle() & WS_VSCROLL) && m_pCtrl->GetScrollBarInfo( OBJID_VSCROLL, &vscr))
		{ 
			CRect rcVScrollBar(vscr.rcScrollBar);	

			if((vscr.rgstate[0] & STATE_SYSTEM_OFFSCREEN) || (vscr.rgstate[0] & STATE_SYSTEM_INVISIBLE))
				return;

			int iWidth = vscr.dxyLineButton;
			int iThumbTop = vscr.xyThumbTop;
			int iThumbBottom = vscr.xyThumbBottom;

			//TRACE("iThumbTop = %d, iThumbBottom = %d pos = %d\n", iThumbTop, iThumbBottom, pos);

			//	TRACE("vscr.dxyLineButton = %d\n", iWidth);
			//	TRACE("vscr.xyThumbTop = %d\n", iThumbTop);
			//	TRACE("vscr.xyThumbBottom = %d\n", iThumbBottom);
			//	TRACE("rcVScrollBar: %d, %d, %d, %d \n", rcVScrollBar.left, rcVScrollBar.top, rcVScrollBar.right, rcVScrollBar.bottom);

			if(rcVScrollBar.Height() < ::GetSystemMetrics(SM_CYVSCROLL)*2+1)
			{
				cyButton = (rcVScrollBar.Height() - 1)/2; 
			}

			CRect rcClient;
			GetClientRect(&rcClient);

			CDC			memDC;
			CBitmap		memBmp;
			memDC.CreateCompatibleDC(pDC);
			memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
			CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
			memDC.SetBkMode(TRANSPARENT);


			//画父父窗口背景
			CRect vParentRect = rcClient;
			MemPaintParentPng(this, memDC, vParentRect, m_pImgParentBk, m_bFillOrTitleParentPng);

			CRect vBkRect = rcClient;
			MemPaintPng(memDC, vBkRect, m_pImgScrollBk);

			CRect vTopRect = CRect(0, 0, cxButton, cyButton);
			MemPaintPng(memDC, vTopRect, m_pImgVScrollTop);

			CRect vBotRect = CRect(0, rcVScrollBar.Height() - cyButton, cxButton, rcVScrollBar.Height());
			MemPaintPng(memDC, vBotRect, m_pImgVScrollBot);

			if(rcVScrollBar.Height() > SCROLLBAR_NORMAL_MINIMUM)
			{
				CRect vThumbRect = CRect(0, iThumbTop, iWidth, iThumbBottom);
				if (m_pImgVScrollTop == NULL || SafeGetImageWidth(m_pImgVScrollTop) == 0)
				{
					vThumbRect.top -= cyButton;
				}

				if (m_pImgVScrollBot == NULL || SafeGetImageWidth(m_pImgVScrollBot) == 0)
				{
					vThumbRect.bottom	+= cyButton;
				}

				MemPaintPng(memDC, vThumbRect, m_pImgVScrollThumb);
			}

			pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);


			memDC.SelectObject(pOldBmp);
			memDC.DeleteDC();
			memBmp.DeleteObject();
		}
	}
}

//draw horizontal scrollbar
void CPngControlScroll::DrawHScroll(CDC* pDC)
{
	if(m_pCtrl == NULL)
	{
		return;
	}

	int cxButton     = ::GetSystemMetrics(SM_CXHSCROLL);
	int cyButton     = ::GetSystemMetrics(SM_CYHSCROLL);

	if(IsWindow(m_hWnd))
	{		
		SCROLLBARINFO hscr;
		memset(&hscr, 0, sizeof(SCROLLBARINFO));
		hscr.cbSize = sizeof(SCROLLBARINFO);
		if((m_pCtrl->GetStyle() & WS_HSCROLL)  && ::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_HSCROLL, &hscr))
		{ 
			CRect rcHScrollBar(hscr.rcScrollBar);		

			if((hscr.rgstate[0] & STATE_SYSTEM_OFFSCREEN) || (hscr.rgstate[0] & STATE_SYSTEM_INVISIBLE))
				return;

			int iHeigth = hscr.dxyLineButton;
			int iThumbLeft = hscr.xyThumbTop;
			int iThumbRight = hscr.xyThumbBottom;

			//			TRACE("hscr.dxyLineButton = %d\n", iHeigth);
			//			TRACE("hscr.xyThumbTop = %d\n", iThumbLeft);
			//			TRACE("hscr.xyThumbBottom = %d\n", iThumbRight);
			//			TRACE("rcHScrollBar: %d, %d, %d, %d \n", rcHScrollBar.left, rcHScrollBar.top, rcHScrollBar.right, rcHScrollBar.bottom);

			if(rcHScrollBar.Width() < ::GetSystemMetrics(SM_CXHSCROLL)*2+1)
			{
				cxButton = (rcHScrollBar.Width() - 1)/2; 
			}

			CRect rcClient;
			GetClientRect(&rcClient);

			CDC			memDC;
			CBitmap		memBmp;
			memDC.CreateCompatibleDC(pDC);
			memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
			CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
			memDC.SetBkMode(TRANSPARENT);

			//画父父窗口背景
			CRect vParentRect = rcClient;
			MemPaintParentPng(this, memDC, vParentRect, m_pImgParentBk, m_bFillOrTitleParentPng);

			CRect vBkRect = rcClient;
			MemPaintPng(memDC, vBkRect, m_pImgScrollBk);


			CRect vLeftRect = CRect(0, 0, cxButton, cyButton);
			MemPaintPng(memDC, vLeftRect, m_pImgHScrollLeft);

			CRect vRightRect = CRect(rcHScrollBar.Width() - cxButton, 0, rcHScrollBar.Width(), cyButton);
			MemPaintPng(memDC, vRightRect, m_pImgHScrollRight);

			if(rcHScrollBar.Width() > SCROLLBAR_NORMAL_MINIMUM)
			{

				CRect vThumbRect = CRect(iThumbLeft, 0, iThumbRight, iHeigth);
				if (m_pImgHScrollLeft == NULL || SafeGetImageWidth(m_pImgHScrollLeft) == 0)
				{
					vThumbRect.left -= cxButton;
				}

				if (m_pImgHScrollRight == NULL || SafeGetImageWidth(m_pImgHScrollRight) == 0)
				{
					vThumbRect.right	+= cxButton;
				}

				MemPaintPng(memDC, vThumbRect, m_pImgHScrollThumb);
			}

			pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(pOldBmp);
			memDC.DeleteDC();
			memBmp.DeleteObject();
		}
	}
}

int CPngControlScroll::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	return 0;
}

void CPngControlScroll::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CStatic::OnNcPaint() for painting messages

 	CDC *pDC = GetDC();
 	if (m_nScrollStyle == SB_VERT) 
 	{
 		DrawVScroll(pDC);
 	}
 	else if(m_nScrollStyle == SB_HORZ)
 	{
 		DrawHScroll(pDC);
 	}
 
 	ReleaseDC(pDC);
}
