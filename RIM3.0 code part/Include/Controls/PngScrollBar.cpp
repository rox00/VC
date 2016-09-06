//********************************************************************
//	PNGSCROLLBAR.CPP 文件注释
//	文件名 : PNGSCROLLBAR.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PngScrollBar.h"


// CPngScrollBar

IMPLEMENT_DYNAMIC(CPngScrollBar, CScrollBar)

CPngScrollBar::CPngScrollBar()
{

	m_bFillOrTitleParentPng		= TRUE;
	m_vTopRect					= CRect(0, 0, 0, 0);
	m_vBotRect					= CRect(0, 0, 0, 0);
	m_vThumbRect				= CRect(0, 0, 0, 0);
	m_hLeftRect					= CRect(0, 0, 0, 0);
	m_hRightRect				= CRect(0, 0, 0, 0);
	m_hThumbRect				= CRect(0, 0, 0, 0);

	m_pngParentBkPath			= _T("");		//
	m_disablePngPath			= _T("");
	
	m_pngScrollBk				= _T("");
	m_pngVScrollTop				= _T("");
	m_pngVScrollBot				= _T("");
	m_pngVScrollThumb			= _T("");
	m_pngHScrollLeft			= _T("");
	m_pngHScrollRight			= _T("");
	m_pngHScrollThumb			= _T("");

	m_pImgParentBk				= NULL;
	m_pImgScrollBk				= NULL;
	m_pImgDisable				= NULL;

	m_pImgVScrollTop			= NULL;
	m_pImgVScrollBot			= NULL;
	m_pImgVScrollThumb			= NULL;
	m_pImgHScrollLeft			= NULL;
	m_pImgHScrollRight			= NULL;
	m_pImgHScrollThumb			= NULL;
}

CPngScrollBar::~CPngScrollBar()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngScrollBar, CScrollBar)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CPngScrollBar 消息处理程序

void CPngScrollBar::InitImage()
{
	if (m_pImgParentBk == NULL)		m_pImgParentBk		= SafeImageFrom(m_pngParentBkPath);
	if (m_pImgScrollBk == NULL)		m_pImgScrollBk		= SafeImageFrom(m_pngScrollBk);
	if (m_pImgDisable == NULL)		m_pImgDisable		= SafeImageFrom(m_disablePngPath);

	if (m_pImgVScrollTop == NULL)	m_pImgVScrollTop	= SafeImageFrom(m_pngVScrollTop);
	if (m_pImgVScrollBot == NULL)	m_pImgVScrollBot	= SafeImageFrom(m_pngVScrollBot);
	if (m_pImgVScrollThumb == NULL)	m_pImgVScrollThumb	= SafeImageFrom(m_pngVScrollThumb);
	if (m_pImgHScrollLeft == NULL)	m_pImgHScrollLeft	= SafeImageFrom(m_pngHScrollLeft);
	if (m_pImgHScrollRight == NULL)	m_pImgHScrollRight	= SafeImageFrom(m_pngHScrollRight);
	if (m_pImgHScrollThumb == NULL)	m_pImgHScrollThumb	= SafeImageFrom(m_pngHScrollThumb);
}

void CPngScrollBar::DestoryImage()
{
	if(m_pImgParentBk		== NULL)	{	delete m_pImgParentBk		;		m_pImgParentBk		= NULL;		}					
	if(m_pImgScrollBk		== NULL)	{	delete m_pImgScrollBk		;		m_pImgScrollBk		= NULL;		}	
	if(m_pImgDisable		== NULL)	{	delete m_pImgDisable		;		m_pImgDisable		= NULL;		}	

	if(m_pImgVScrollTop		== NULL)	{	delete m_pImgVScrollTop		;		m_pImgVScrollTop	= NULL;		}		
	if(m_pImgVScrollBot		== NULL)	{	delete m_pImgVScrollBot		;		m_pImgVScrollBot	= NULL;		}	
	if(m_pImgVScrollThumb	== NULL)	{	delete m_pImgVScrollThumb	;		m_pImgVScrollThumb	= NULL;		}		
	if(m_pImgHScrollLeft	== NULL)	{	delete m_pImgHScrollLeft	;		m_pImgHScrollLeft	= NULL;		}	
	if(m_pImgHScrollRight	== NULL)	{	delete m_pImgHScrollRight	;		m_pImgHScrollRight	= NULL;		}		
	if(m_pImgHScrollThumb	== NULL)	{	delete m_pImgHScrollThumb	;		m_pImgHScrollThumb	= NULL;		}	
}

//
//RECT CPngScrollBar::GetRect(UINT uSBCode)
//{
//	SCROLLINFO si = m_si;
//	if (si.nTrackPos == -1) si.nTrackPos = si.nPos;
//	int nInterHei = m_nHei - 2 * m_nWid;
//	if (nInterHei < 0) nInterHei = 0;
//	int	nSlideHei = si.nPage*nInterHei / (si.nMax - si.nMin + 1);
//	if (nSlideHei < THUMB_MINSIZE) nSlideHei = THUMB_MINSIZE;
//	if (nInterHei < THUMB_MINSIZE) nSlideHei = 0;
//	if ((si.nMax - si.nMin - si.nPage + 1) == 0) nSlideHei = 0;
//	int nEmptyHei = nInterHei - nSlideHei;
//	int nArrowHei = m_nWid;
//	if (nInterHei == 0) nArrowHei = m_nHei / 2;
//	RECT rcRet = { 0, 0, m_nWid, nArrowHei };
//	if (uSBCode == SB_LINEUP) goto end;
//	rcRet.top = rcRet.bottom;
//	if ((si.nMax - si.nMin - si.nPage + 1) == 0)
//		rcRet.bottom += nEmptyHei / 2;
//	else
//		rcRet.bottom += nEmptyHei*si.nTrackPos / (si.nMax - si.nMin - si.nPage + 1);
//	if (uSBCode == SB_PAGEUP) goto end;
//	rcRet.top = rcRet.bottom;
//	rcRet.bottom += nSlideHei;
//	if (uSBCode == SB_THUMBTRACK) goto end;
//	rcRet.top = rcRet.bottom;
//	rcRet.bottom = m_nHei - nArrowHei;
//	if (uSBCode == SB_PAGEDOWN) goto end;
//	rcRet.top = rcRet.bottom;
//	rcRet.bottom = m_nHei;
//	if (uSBCode == SB_LINEDOWN) goto end;
//end:
//	if (!IsVertical())
//	{
//		int t = rcRet.left;
//		rcRet.left = rcRet.top;
//		rcRet.top = t;
//		t = rcRet.right;
//		rcRet.right = rcRet.bottom;
//		rcRet.bottom = t;
//	}
//	return rcRet;
//}

//draw vertical scrollbar
void CPngScrollBar::DrawVScroll(CDC* pDC)
{
	int cxButton	= ::GetSystemMetrics(SM_CXVSCROLL);
	int cyButton	= ::GetSystemMetrics(SM_CYVSCROLL);
	int cyThumb		= ::GetSystemMetrics(SM_CYVSCROLL);


	CRect rcClient;
	GetClientRect(&rcClient);

	if (rcClient.Height() < ::GetSystemMetrics(SM_CYVSCROLL) * 2 + 1)
	{
		cyButton = (rcClient.Height() - 1) / 2;
	}

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

	m_vTopRect = CRect(0, 0, cxButton, cyButton);
	MemPaintPng(memDC, m_vTopRect, m_pImgVScrollTop);

	m_vBotRect = CRect(0, rcClient.Height() - cyButton, cxButton, rcClient.Height());
	MemPaintPng(memDC, m_vBotRect, m_pImgVScrollBot);

	if (rcClient.Height() > 45)
	{
		if (m_vThumbRect.Height() == 0)
		{	//计算Thumb大小
			SCROLLINFO vScrollInfo;
			memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
			vScrollInfo.cbSize = sizeof(SCROLLINFO);
			GetScrollInfo(&vScrollInfo, SIF_ALL);

			if (vScrollInfo.nPage != 0)
			{
				cyThumb = int((rcClient.Height() - cyButton * 2)*(vScrollInfo.nPage*1.0) / (vScrollInfo.nMax - vScrollInfo.nMin + 1));
			}

			float posPersent = 0;
			if (vScrollInfo.nMax - vScrollInfo.nMin)
				posPersent = float(vScrollInfo.nPos * 1.0 / (vScrollInfo.nMax - vScrollInfo.nMin + 1));

			int pixelRange = (rcClient.Height() - cyButton * 2);

			int iThumbTop = int(cyButton + pixelRange * posPersent);
			int iThumbBottom = iThumbTop + cyThumb;
			m_vThumbRect = CRect(0, iThumbTop, rcClient.Width(), iThumbBottom);
		}

		CRect thumbRectTemp = m_vThumbRect;
 		if (m_pImgVScrollTop == NULL || SafeGetImageWidth(m_pImgVScrollTop) == 0)
 		{
			thumbRectTemp.top -= cyButton;
 		}
 
 		if (m_pImgVScrollBot == NULL || SafeGetImageWidth(m_pImgVScrollBot) == 0)
 		{
			thumbRectTemp.bottom += cyButton;
 		}
 
		MemPaintPng(memDC, thumbRectTemp, m_pImgVScrollThumb);
	}

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

//draw horizontal scrollbar
void CPngScrollBar::DrawHScroll(CDC* pDC)
{

	int cxButton	= ::GetSystemMetrics(SM_CXHSCROLL);
	int cyButton	= ::GetSystemMetrics(SM_CYHSCROLL);
	int cxThumb		= ::GetSystemMetrics(SM_CXHTHUMB);


	CRect rcClient;
	GetClientRect(&rcClient);

	if (rcClient.Width() < ::GetSystemMetrics(SM_CXHSCROLL) * 2 + 1)
	{
		cxButton = (rcClient.Width() - 1) / 2;
	}

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


	m_hLeftRect= CRect(0, 0, cxButton, cyButton);
	MemPaintPng(memDC, m_hLeftRect, m_pImgHScrollLeft);

	m_hRightRect = CRect(rcClient.Width() - cxButton, 0, rcClient.Width(), cyButton);
	MemPaintPng(memDC, m_hRightRect, m_pImgHScrollRight);

	if (rcClient.Width() > 45)
	{
		if (m_hThumbRect.Width() == 0)
		{
			SCROLLINFO hScrollInfo;
			memset(&hScrollInfo, 0, sizeof(SCROLLINFO));
			hScrollInfo.cbSize = sizeof(SCROLLINFO);
			GetScrollInfo(&hScrollInfo, SIF_ALL);

			if (hScrollInfo.nPage != 0)
			{
				cxThumb = int((rcClient.Width() - cxButton * 2)*(hScrollInfo.nPage*1.0) / (hScrollInfo.nMax - hScrollInfo.nMin));
			}

			double posPersent = 0;
			if (hScrollInfo.nMax - hScrollInfo.nMin)
				posPersent = hScrollInfo.nPos * 1.0 / (hScrollInfo.nMax - hScrollInfo.nMin);

			int pixelRange = (rcClient.Width() - cxButton * 2);

			int iThumbLeft = int(cxButton + pixelRange * posPersent);
			int iThumbRight = iThumbLeft + cxThumb;

			m_hThumbRect = CRect(iThumbLeft, 0, iThumbRight, rcClient.Height());
		}


		CRect thumbRectTemp = m_hThumbRect;
		if (m_pImgHScrollLeft == NULL || SafeGetImageWidth(m_pImgHScrollLeft) == 0)
		{
			thumbRectTemp.left -= cxButton;
		}

		if (m_pImgHScrollRight == NULL || SafeGetImageWidth(m_pImgHScrollRight) == 0)
		{
			thumbRectTemp.right += cxButton;
		}

		MemPaintPng(memDC, thumbRectTemp, m_pImgHScrollThumb);
	}

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

UINT CPngScrollBar::OnHitTest(CPoint point)
{
	if (GetStyle() & SBS_VERT)
	{
		if (m_vTopRect.PtInRect(point))
			return SB_LINEUP;
		if (m_vBotRect.PtInRect(point))
			return SB_LINEDOWN;
		if (m_vThumbRect.PtInRect(point))
			return SB_THUMBTRACK;

		CRect pageUP = CRect(m_vTopRect.left, m_vTopRect.bottom, m_vThumbRect.right, m_vThumbRect.top);
		CRect pageDown = CRect(m_vThumbRect.left, m_vThumbRect.bottom, m_vBotRect.right, m_vBotRect.top);

		if (pageUP.PtInRect(point))
			return SB_PAGEUP;
		if (pageDown.PtInRect(point))
			return SB_PAGEDOWN;

	}
	else
	{
		if (m_hLeftRect.PtInRect(point))
			return SB_LINELEFT;
		if (m_hRightRect.PtInRect(point))
			return SB_LINERIGHT;
		if (m_hThumbRect.PtInRect(point))
			return SB_THUMBTRACK;

		CRect pageLeft = CRect(m_hLeftRect.right, m_hLeftRect.top, m_hThumbRect.left, m_hThumbRect.bottom);
		CRect pageRight = CRect(m_hThumbRect.right, m_hThumbRect.top, m_hRightRect.left, m_hRightRect.bottom);
		if (pageLeft.PtInRect(point))
			return SB_PAGELEFT;
		if (pageRight.PtInRect(point))
			return SB_PAGERIGHT;
	}
	
	return -1;
}

HBRUSH CPngScrollBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CScrollBar::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}


BOOL CPngScrollBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return TRUE;
	return CScrollBar::OnEraseBkgnd(pDC);
}


void CPngScrollBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CScrollBar::OnPaint()
	InitImage();

	CRect rcClient;
	GetClientRect(rcClient);
	DWORD style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if (style & SBS_VERT)
	{
		DrawVScroll(&dc);
		//{
		//	CString str;
		//	str.Format("\n(zyo)DrawVScroll:  %d, %d \n", rcClient.Width(), rcClient.Height());
		//	OutputDebugString(str);
		//}
	}
	else
	{
		DrawHScroll(&dc);
	}
}

void CPngScrollBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(&rcClient);
	int minValue, maxValue;
	GetScrollRange(&minValue, &maxValue);

	UINT hitType = OnHitTest(point);
	BOOL bTrack = FALSE;

	SetCapture();
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	msg.hwnd = m_hWnd;

	int vStartThumbOffY = m_vThumbRect.bottom - point.y;	//与底端的距离
	int hStartThumbOffX = m_hThumbRect.right - point.x;		//与右端的距离

	do{
		if (msg.hwnd == GetSafeHwnd())
		{
			if (GetStyle() & SBS_VERT)
			{
				BOOL bTrackCircle = TRUE;
				while (1)
				{
					if (hitType == SB_LINEUP)
					{
						SetRedraw(FALSE);
						if (GetScrollPos() <= minValue)
							GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(SB_TOP, 0), (LPARAM)m_hWnd);
						else
							GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else if (hitType == SB_LINEDOWN)
					{
						SCROLLINFO vScrollInfo;
						memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
						vScrollInfo.cbSize = sizeof(SCROLLINFO);
						GetScrollInfo(&vScrollInfo, SIF_ALL);

						SetRedraw(FALSE);
						if ((GetScrollPos() + int(vScrollInfo.nPage)) >= maxValue)
							GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), (LPARAM)m_hWnd);
						else
							GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else if (hitType == SB_PAGEUP)
					{
						SetRedraw(FALSE);
						GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else if (hitType == SB_PAGEDOWN)
					{
						SetRedraw(FALSE);
						GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else
					{
						if (bTrackCircle == TRUE)	//如果第一次没有则应该跳出
							break;
					}
					RedrawWindow();

					//没有跳出说明有选中的项
					Sleep(10);

					if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	//鼠标左键不按下时，及时跳出循环
						break;

					CPoint curpt;
					GetCursorPos(&curpt);
					ScreenToClient(&curpt);
					hitType = OnHitTest(curpt);	//重新判断鼠标所有选钮类型
				}

				//////////////////选中滚动条选钮
				if (bTrackCircle && hitType == SB_THUMBTRACK)
				{
					CPoint ptCur = CPoint(0, 0);

					ptCur.x = LOWORD(msg.lParam);
					ptCur.y = HIWORD(msg.lParam);


					int offset = ptCur.y - point.y;
					point = ptCur;

					SCROLLINFO vScrollInfo;
					memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
					vScrollInfo.cbSize = sizeof(SCROLLINFO);
					GetScrollInfo(&vScrollInfo, SIF_ALL);

					if (offset < 0 && vScrollInfo.nPos <= vScrollInfo.nMin ||
						offset > 0 && vScrollInfo.nPos >= vScrollInfo.nMax
						)
						continue;

					if (msg.message == WM_MOUSEMOVE)
					{
						int thumbHeightTemp = m_vThumbRect.Height();
						m_vThumbRect.bottom = ptCur.y + vStartThumbOffY;
						m_vThumbRect.top = m_vThumbRect.bottom - thumbHeightTemp;

						SetRedraw(FALSE);
						if (m_vThumbRect.top < m_vTopRect.bottom)
						{	//不能移出上边界
							m_vThumbRect.OffsetRect(0, m_vTopRect.bottom - m_vThumbRect.top);
							GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nMin), (LPARAM)m_hWnd);
						}
						else if (m_vThumbRect.bottom > m_vBotRect.top)
						{	//不能移出下边界
							m_vThumbRect.OffsetRect(0, m_vBotRect.top - m_vThumbRect.bottom);
							GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nMax - vScrollInfo.nPage + 1), (LPARAM)m_hWnd);
						}
						else
						{
							double pixelRange = 1.0*(rcClient.Height() - m_vTopRect.Height() - m_vBotRect.Height() - m_vThumbRect.Height());
							vScrollInfo.nPos = int((vScrollInfo.nMax - vScrollInfo.nMin - vScrollInfo.nPage + 1) / pixelRange * (m_vThumbRect.top - m_vTopRect.bottom));//nMax的像素位置并不是在最底端
							GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nPos), (LPARAM)m_hWnd);
						}
						SetRedraw(TRUE);
						RedrawWindow();
					}

					if (msg.message == WM_LBUTTONUP)
					{
						SetRedraw(FALSE);
						GetOwner()->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBPOSITION, vScrollInfo.nPos), (LPARAM)m_hWnd);
						SetRedraw(TRUE);
						break;
					}
				}
				else
					break;
				
			}
			else
			{///////////////////////////////////处理水平滚动条
				BOOL bTrackCircle = TRUE;
				while (1)
				{
					if (hitType == SB_LINELEFT)
					{
						SetRedraw(FALSE);
						if (GetScrollPos() <= minValue)
							GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(SB_LEFT, 0), (LPARAM)m_hWnd);
						else
							GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else if (hitType == SB_LINERIGHT)
					{
						SCROLLINFO vScrollInfo;
						memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
						vScrollInfo.cbSize = sizeof(SCROLLINFO);
						GetScrollInfo(&vScrollInfo, SIF_ALL);

						SetRedraw(FALSE);
						if ((GetScrollPos() + int(vScrollInfo.nPage)) >= maxValue)
							GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(SB_RIGHT, 0), (LPARAM)m_hWnd);
						else
							GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else if (hitType == SB_PAGELEFT)
					{
						SetRedraw(FALSE);
						GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else if (hitType == SB_PAGERIGHT)
					{
						SetRedraw(FALSE);
						GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(hitType, 0), (LPARAM)m_hWnd);
						SetRedraw(TRUE);

						bTrackCircle = FALSE;
					}
					else
					{
						if (bTrackCircle == TRUE)	//如果第一次没有则应该跳出
							break;
					}
					RedrawWindow();

					//没有跳出说明有选中的项
					Sleep(10);

					if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	//鼠标左键不按下时，及时跳出循环
						break;

					CPoint curpt;
					GetCursorPos(&curpt);
					ScreenToClient(&curpt);
					hitType = OnHitTest(curpt);	//重新判断鼠标所有选钮类型
				}

				//////////////////选中滚动条选钮
				if (bTrackCircle && hitType == SB_THUMBTRACK)
				{
					CPoint ptCur = CPoint(0, 0);

					ptCur.x = LOWORD(msg.lParam);
					ptCur.y = HIWORD(msg.lParam);


					int offset = ptCur.x - point.x;
					point = ptCur;

					SCROLLINFO hScrollInfo;
					memset(&hScrollInfo, 0, sizeof(SCROLLINFO));
					hScrollInfo.cbSize = sizeof(SCROLLINFO);
					GetScrollInfo(&hScrollInfo, SIF_ALL);

					if (offset < 0 && hScrollInfo.nPos <= hScrollInfo.nMin ||
						offset > 0 && hScrollInfo.nPos >= hScrollInfo.nMax
						)
						continue;

					if (msg.message == WM_MOUSEMOVE)
					{
						int thumbWidthTemp = m_hThumbRect.Width();
						m_hThumbRect.right = ptCur.x + hStartThumbOffX;
						m_hThumbRect.left = m_hThumbRect.right - thumbWidthTemp;

						SetRedraw(FALSE);
						if (m_hThumbRect.left < m_hLeftRect.right)
						{	//不能移出左边界
							m_hThumbRect.OffsetRect(m_hLeftRect.right - m_hThumbRect.left, 0);
							GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(hitType, hScrollInfo.nMin), (LPARAM)m_hWnd);
						}
						else if (m_hThumbRect.right > m_hRightRect.left)
						{	//不能移出右边界
							m_hThumbRect.OffsetRect(m_hRightRect.left - m_hThumbRect.right, 0);
							GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(hitType, hScrollInfo.nMax), (LPARAM)m_hWnd);
						}
						else
						{
							double pixelRange = 1.0*(rcClient.Width() - m_hLeftRect.Width() - m_hRightRect.Width() - m_hThumbRect.Width());
							hScrollInfo.nPos = int((hScrollInfo.nMax - hScrollInfo.nMin - hScrollInfo.nPage + 1) / pixelRange * (m_hThumbRect.left - m_hLeftRect.right));//nMax的像素位置并不是在最底端

							GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(hitType, hScrollInfo.nPos), (LPARAM)m_hWnd);
						}
						SetRedraw(TRUE);
						RedrawWindow();
					}

					if (msg.message == WM_LBUTTONUP)
					{
						SetRedraw(FALSE);
						GetOwner()->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, hScrollInfo.nPos), (LPARAM)m_hWnd);
						SetRedraw(TRUE);
						break;
					}
				}
				else
					break;
			}
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		
	}while (::GetMessage(&msg, 0, 0, 0));

	ReleaseCapture();
	Invalidate();


	/*if (msg.hwnd == m_hWnd && (msg.message == WM_LBUTTONUP || msg.message == WM_LBUTTONDOWN))
	break;

	if (msg.hwnd == m_hWnd && msg.message == WM_MOUSEMOVE)
	{
	CPoint ptCur = CPoint(0, 0);

	ptCur.x = LOWORD(msg.lParam);
	ptCur.y = HIWORD(msg.lParam);

	CDC *pDC = GetDC();
	if (GetStyle() & SBS_VERT)
	{
	int offset = ptCur.y - point.y;

	int cxButton = ::GetSystemMetrics(SM_CXVSCROLL);
	int cyButton = ::GetSystemMetrics(SM_CYVSCROLL);
	int cyThumb = ::GetSystemMetrics(SM_CYVSCROLL);

	SCROLLINFO vScrollInfo;
	memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
	vScrollInfo.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(&vScrollInfo, SIF_ALL);


	if ((m_vThumbRect.top + offset) < m_vTopRect.bottom || (m_vThumbRect.top + offset) > m_vBotRect.top)
	offset = 0;

	CRect rcClient;
	GetClientRect(&rcClient);

	float pixelRange = 1.0*(rcClient.Height() - m_vTopRect.Height() - m_vBotRect.Height());

	int offPos = (vScrollInfo.nMax - vScrollInfo.nMin) / pixelRange * offset;

	vScrollInfo.nPos += offPos;

	SetRedraw(FALSE);
	SetScrollPos(vScrollInfo.nPos);
	SetRedraw(TRUE);

	DrawVScroll(pDC);
	}

	if (GetStyle() & SBS_HORZ)
	{
	int offset = ptCur.x - point.x;

	DrawHScroll(pDC);
	}

	ReleaseDC(pDC);
	point = ptCur;

	continue;
	}

	::TranslateMessage(&msg);
	::DispatchMessage(&msg);
	}
*/

	//CScrollBar::OnLButtonDown(nFlags, point);
}

void CPngScrollBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SetRedraw(FALSE);
	CScrollBar::OnLButtonDblClk(nFlags, point);
	SetRedraw(TRUE);

	Invalidate();
}

BOOL CPngScrollBar::SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	BOOL bRet = CScrollBar::SetScrollInfo(lpScrollInfo, FALSE);
	if (bRedraw == TRUE)
	{
		if(GetStyle() & SBS_VERT)
			m_vThumbRect = CRect(0,0, 0, 0);
		else
			m_hThumbRect = CRect(0,0, 0, 0);
		RedrawWindow();
	}

	return bRet;
}

int CPngScrollBar::SetScrollPos(int nPos, BOOL bRedraw /*= TRUE*/)
{
	int Ret = CScrollBar::SetScrollPos(nPos, FALSE);
	if (bRedraw == TRUE)
	{
		if(GetStyle() & SBS_VERT)
			m_vThumbRect = CRect(0,0, 0, 0);
		else
			m_hThumbRect = CRect(0,0, 0, 0);
		RedrawWindow();
	}

	return Ret;
}

void CPngScrollBar::SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw /*= TRUE*/)
{
	CScrollBar::SetScrollRange(nMinPos, nMaxPos, FALSE);
	if (bRedraw == TRUE)
	{
		if(GetStyle() & SBS_VERT)
			m_vThumbRect = CRect(0,0, 0, 0);
		else
			m_hThumbRect = CRect(0,0, 0, 0);
		RedrawWindow();
	}
}

int CPngScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	return 0;
}
