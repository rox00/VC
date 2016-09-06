
#include "stdafx.h"
#include "PngComListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitListBox

CPngComListBox::CPngComListBox()
{

	m_textColor					= RGB(255, 255, 255);
	m_textAlignMode				= DT_SINGLELINE | DT_VCENTER | DT_LEFT;

	m_listLeftPngPath			= _T("");		//COMBOX  列表左边
	m_listTopPngPath			= _T("");		//COMBOX  列表顶部
	m_listRightPngPath			= _T("");		//COMBOX  列表右部
	m_listBottomPngPath			= _T("");		//COMBOX  列表底部
	m_listCenterPngPath			= _T("");		//COMBOX  列表底部
	m_listSelPngPath			= _T("");		//COMBOX  列表选中状态


	m_pngScrollBk				= _T("");
	m_pngVScrollTop				= _T("");
	m_pngVScrollBot				= _T("");
	m_pngVScrollThumb			= _T("");
	m_pngHScrollLeft			= _T("");
	m_pngHScrollRight			= _T("");
	m_pngHScrollThumb			= _T("");

	m_nTextOffsetX				= 4;

	m_pImgListLeft				= NULL;	
	m_pImgListTop				= NULL;		
	m_pImgListRight				= NULL;
	m_pImgListBottom			= NULL;
	m_pImgListCenter			= NULL;
	m_pImgListSel				= NULL;

	m_pImgScrollBk				= NULL;
	m_pImgVScrollTop			= NULL;
	m_pImgVScrollBot			= NULL;
	m_pImgVScrollThumb			= NULL;
	m_pImgHScrollLeft			= NULL;
	m_pImgHScrollRight			= NULL;
	m_pImgHScrollThumb			= NULL;
}

CPngComListBox::~CPngComListBox()
{
	m_NcMemDC.DeleteDC();
	m_NcMemBmp.DeleteObject();

	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngComListBox, CListBox)
 	ON_MESSAGE(WM_PRINTCLIENT, &CPngComListBox::OnPrintClient)
 	ON_MESSAGE(WM_PRINT, &CPngComListBox::OnPrint)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitListBox message handlers


void CPngComListBox::DrawClient(CDC* pDC)
{
	if(m_pImgListLeft && m_pImgListTop && m_pImgListRight && m_pImgListBottom)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect 	listLeftRc = CRect(rcClient.left, rcClient.top, SafeGetImageWidth(m_pImgListLeft), rcClient.bottom);
		CRect 	listTopRc = CRect(rcClient.left, rcClient.top, rcClient.right, SafeGetImageHeight(m_pImgListTop));
		CRect 	listRightRc = CRect(rcClient.right - SafeGetImageWidth(m_pImgListRight), rcClient.top, rcClient.right, rcClient.bottom);
		CRect 	listBottomRc = CRect(rcClient.left, rcClient.bottom - SafeGetImageHeight(m_pImgListBottom), rcClient.right, rcClient.bottom);

		MemPaintPng(*pDC, rcClient, m_pImgListCenter);

		//Draw the left side of the list box
		MemPaintPng(*pDC, listLeftRc, m_pImgListLeft);
		//Draw the right side of the list box
		MemPaintPng(*pDC, listTopRc, m_pImgListTop);
		//Draw the top of the list bar
		MemPaintPng(*pDC, listRightRc, m_pImgListRight);
		//Draw the bottom of the list bar
		MemPaintPng(*pDC, listBottomRc, m_pImgListBottom);

		CRect	rcItem;
		CString	szItemString;
		BOOL	bOutSide = FALSE;
		int		itemCount = GetCount();

		for (int pos = rcClient.top; pos < rcClient.bottom;)
		{
			UINT index = ItemFromPoint(CPoint(rcClient.left, pos), bOutSide);
			GetText(index, szItemString);
			GetItemRect(index, &rcItem);

			//左右边框除外
			CRect desDcRc = rcItem;

			CRect desSelRc = CRect(rcItem.left + listLeftRc.Width(), rcItem.top, rcItem.right - listRightRc.Width(), rcItem.bottom);
			if (index == 0)
				desSelRc.top += listTopRc.Height();
			if (index == itemCount - 1)
				desSelRc.bottom -= listBottomRc.Height();

			if (GetSel(index))
			{	//上下边框除外
				MemPaintPng(*pDC, desSelRc, m_pImgListSel);
			}
			else
			{
				MemPaintPng(*pDC, desSelRc, m_pImgListCenter);
			}

			desDcRc.left += m_nTextOffsetX;
			MemDrawText(*pDC, desDcRc, szItemString, m_textAlignMode);

			pos += GetItemHeight(index);
		}
	}
	
}

void CPngComListBox::DrawVScroll(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int cxButton = ::GetSystemMetrics(SM_CXVSCROLL);
	int cyButton = ::GetSystemMetrics(SM_CYVSCROLL);
	//int cyThumb = ::GetSystemMetrics(SM_CYVSCROLL);


	SCROLLINFO vScrollInfo;
	memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
	vScrollInfo.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_VERT, &vScrollInfo, SIF_ALL);

	if (vScrollInfo.nPage < 0)
	{
		return;
	}

	SCROLLBARINFO vScrollBarInfo;
	memset(&vScrollBarInfo, 0, sizeof(SCROLLBARINFO));
	vScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	if (GetScrollBarInfo(OBJID_VSCROLL, &vScrollBarInfo))
	{
		if((vScrollBarInfo.rgstate[0] & STATE_SYSTEM_OFFSCREEN) || (vScrollBarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE))
		{	//说明不存在滚动条
			return;
		}

		CRect rcVScrollBar(vScrollBarInfo.rcScrollBar);
		ScreenToClient(rcVScrollBar);
		rcVScrollBar.OffsetRect(rcClient.Width() + rcVScrollBar.Width(), 0);


		int iThumbWidth = vScrollBarInfo.dxyLineButton;
		int iThumbTop = vScrollBarInfo.xyThumbTop;
		int iThumbBottom = vScrollBarInfo.xyThumbBottom;

		if (rcVScrollBar.Height() < ::GetSystemMetrics(SM_CYVSCROLL) * 2 + 1)
		{
			cyButton = (rcVScrollBar.Height() - 1) / 2;
		}

		CDC			memDC;
		CBitmap		memBmp;
		memDC.CreateCompatibleDC(pDC);
		memBmp.CreateCompatibleBitmap(pDC, rcVScrollBar.Width(), rcVScrollBar.Height());
		CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
		memDC.SetBkMode(TRANSPARENT);


		CRect vBkRect = CRect(0, 0, rcVScrollBar.Width(), rcVScrollBar.Height());
		MemPaintPng(memDC, vBkRect, m_pImgScrollBk);

		m_vTopRect = CRect(rcVScrollBar.left, rcVScrollBar.top, rcVScrollBar.left + cxButton, rcVScrollBar.top + cyButton);
		MemPaintPng(memDC, CRect(0, 0, cxButton, cyButton), m_pImgVScrollTop);

		m_vBotRect = CRect(rcVScrollBar.left, rcVScrollBar.bottom - cyButton, rcVScrollBar.left + cxButton, rcVScrollBar.bottom);
		MemPaintPng(memDC, CRect(0, rcVScrollBar.Height() - cyButton, cxButton, rcVScrollBar.Height()), m_pImgVScrollBot);

		m_vThumbRect = CRect(rcVScrollBar.left, rcVScrollBar.top + iThumbTop, rcVScrollBar.left + iThumbWidth, rcVScrollBar.top + iThumbBottom);


		CRect thumbRectTemp = CRect(0, m_vThumbRect.top - rcVScrollBar.top, m_vThumbRect.Width(), m_vThumbRect.bottom - rcVScrollBar.top);
		if (m_pImgVScrollTop == NULL || SafeGetImageWidth(m_pImgVScrollTop) == 0)
		{
			thumbRectTemp.top -= cyButton;
		}

		if (m_pImgVScrollBot == NULL || SafeGetImageWidth(m_pImgVScrollBot) == 0)
		{
			thumbRectTemp.bottom += cyButton;
		}

		MemPaintPng(memDC, thumbRectTemp, m_pImgVScrollThumb);

		pDC->BitBlt(rcVScrollBar.left, rcVScrollBar.top, rcVScrollBar.Width(), rcVScrollBar.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		memBmp.DeleteObject();
	}
}

UINT CPngComListBox::OnHitTest(CPoint point)
{
	if (GetStyle() & WS_VSCROLL)
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
	else if (GetStyle() & WS_HSCROLL)
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

void CPngComListBox::InitImage()
{
	if(m_pImgListLeft == NULL)		m_pImgListLeft		= SafeImageFrom(m_listLeftPngPath);
	if(m_pImgListTop == NULL)		m_pImgListTop		= SafeImageFrom(m_listTopPngPath);
	if(m_pImgListRight == NULL)		m_pImgListRight		= SafeImageFrom(m_listRightPngPath);
	if(m_pImgListBottom == NULL)	m_pImgListBottom	= SafeImageFrom(m_listBottomPngPath);
	if(m_pImgListCenter == NULL)	m_pImgListCenter	= SafeImageFrom(m_listCenterPngPath);
	if(m_pImgListSel == NULL)		m_pImgListSel		= SafeImageFrom(m_listSelPngPath);

	if (m_pImgScrollBk == NULL)		m_pImgScrollBk		= SafeImageFrom(m_pngScrollBk);
	if (m_pImgVScrollTop == NULL)	m_pImgVScrollTop	= SafeImageFrom(m_pngVScrollTop);
	if (m_pImgVScrollBot == NULL)	m_pImgVScrollBot	= SafeImageFrom(m_pngVScrollBot);
	if (m_pImgVScrollThumb == NULL)	m_pImgVScrollThumb	= SafeImageFrom(m_pngVScrollThumb);
	if (m_pImgHScrollLeft == NULL)	m_pImgHScrollLeft	= SafeImageFrom(m_pngHScrollLeft);
	if (m_pImgHScrollRight == NULL)	m_pImgHScrollRight	= SafeImageFrom(m_pngHScrollRight);
	if (m_pImgHScrollThumb == NULL)	m_pImgHScrollThumb	= SafeImageFrom(m_pngHScrollThumb);

}

void CPngComListBox::DestoryImage()
{
	if(m_pImgListLeft		== NULL)	{	delete m_pImgListLeft		;	m_pImgListLeft				= NULL;		}					
	if(m_pImgListTop		== NULL)	{	delete m_pImgListTop		;	m_pImgListTop				= NULL;		}	
	if(m_pImgListRight		== NULL)	{	delete m_pImgListRight		;	m_pImgListRight				= NULL;		}
	if(m_pImgListBottom 	== NULL)	{	delete m_pImgListBottom		;	m_pImgListBottom			= NULL;		}
	if(m_pImgListCenter 	== NULL)	{	delete m_pImgListCenter		;	m_pImgListCenter			= NULL;		}
	if(m_pImgListSel		== NULL)	{	delete m_pImgListSel		;	m_pImgListSel				= NULL;		}

	if(m_pImgScrollBk		== NULL)	{	delete m_pImgScrollBk		;	m_pImgScrollBk				= NULL;		}
	if(m_pImgVScrollTop		== NULL)	{	delete m_pImgVScrollTop		;	m_pImgVScrollTop			= NULL;		}
	if(m_pImgVScrollBot		== NULL)	{	delete m_pImgVScrollBot		;	m_pImgVScrollBot			= NULL;		}
	if(m_pImgVScrollThumb	== NULL)	{	delete m_pImgVScrollThumb	;	m_pImgVScrollThumb			= NULL;		}
	if(m_pImgHScrollLeft	== NULL)	{	delete m_pImgHScrollLeft	;	m_pImgHScrollLeft			= NULL;		}
	if(m_pImgHScrollRight	== NULL)	{	delete m_pImgHScrollRight	;	m_pImgHScrollRight			= NULL;		}
	if(m_pImgHScrollThumb	== NULL)	{	delete m_pImgHScrollThumb	;	m_pImgHScrollThumb			= NULL;		}
}

LRESULT CPngComListBox::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

LRESULT CPngComListBox::OnPrint(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = HDC(wParam);

	InitImage();

	CDC dc;
	dc.Attach(hdc);
	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	//dc.BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &m_NcMemDC, 0, 0, SRCCOPY);

	CRect rcClient;
	GetClientRect(&rcClient);

	if (IsWindowVisible())
	{
		CDC			memDC;
		CBitmap		memBmp;
		memDC.CreateCompatibleDC(&dc);
		memBmp.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());
		CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
		memDC.SetBkMode(TRANSPARENT);
		CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);

		DrawClient(&memDC);
		DrawVScroll(&memDC);

		dc.BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		memDC.SelectObject(pOldFont);
		memDC.DeleteDC();
		memBmp.DeleteObject();
	}

	dc.Detach();

	return TRUE;
}

BOOL CPngComListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	InitImage();


	CRect rcClient;
	GetWindowRect(&rcClient);
	ScreenToClient(rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);


	DrawClient(&memDC);
	DrawVScroll(&memDC);

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
	return TRUE;
}

void CPngComListBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);

	InitImage();

	if (IsWindowVisible())
	{

		CDC			memDC;
		CBitmap		memBmp;
		memDC.CreateCompatibleDC(&dc);
		memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
		memDC.SetBkMode(TRANSPARENT);
		CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);

		DrawClient(&memDC);


		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		memDC.SelectObject(pOldFont);
		memDC.DeleteDC();
		memBmp.DeleteObject();
	}
}

void CPngComListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Add your code to draw the specified item

	if( lpDrawItemStruct->itemID == -1 )
		return;


	CRect rcListClient;
	GetClientRect(&rcListClient);

	InitImage();

	CRect 	listLeftRc		= CRect(rcListClient.left, rcListClient.top, SafeGetImageWidth(m_pImgListLeft), rcListClient.bottom);
	CRect 	listTopRc		= CRect(rcListClient.left, rcListClient.top, rcListClient.right, SafeGetImageHeight(m_pImgListTop));
	CRect 	listRightRc		= CRect(rcListClient.right - SafeGetImageWidth(m_pImgListRight), rcListClient.top, rcListClient.right, rcListClient.bottom);
	CRect 	listBottomRc	= CRect(rcListClient.left, rcListClient.bottom - SafeGetImageHeight(m_pImgListBottom), rcListClient.right, rcListClient.bottom);


	CDC	dc;	
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rcItem		= lpDrawItemStruct->rcItem;
	CRect rcItemPaint	= CRect(0, 0, rcItem.Width(), rcItem.Height());

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcItemPaint.Width(), rcItemPaint.Height());
	memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);

	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		MemPaintPng(memDC, rcItemPaint, m_pImgListSel);
	}
	else
	{
		MemPaintPng(memDC, rcItemPaint, m_pImgListCenter);
	}
	//

	////////////绘制Listbox上面的文字   
	CString		sItemText;
	// Copy the text of the item to a string
	GetText(lpDrawItemStruct->itemID, sItemText);
	// Draw the text after the images left postion
	rcItemPaint.left += m_nTextOffsetX;
	MemDrawText(memDC, rcItemPaint, sItemText, m_textAlignMode);

	CRect desDcRc		= CRect(rcItem.left + listLeftRc.Width(), rcItem.top, rcItem.right - listRightRc.Width(), rcItem.bottom);
	CRect sourceRc		= CRect(rcItem.left + listLeftRc.Width(), 0, rcItem.right - listRightRc.Width(), rcItemPaint.bottom);
	if( lpDrawItemStruct->itemID == 0 )
	{
		desDcRc.top		+= listTopRc.Height();
		sourceRc.top	+= listTopRc.Height();
	}

	if( lpDrawItemStruct->itemID == GetCount()-1 )
	{
		desDcRc.bottom -= listBottomRc.Height();
		sourceRc.bottom -= listBottomRc.Height();
	}

	dc.StretchBlt(desDcRc.left, desDcRc.top, desDcRc.Width(), desDcRc.Height(), &memDC,
		sourceRc.left, sourceRc.top, sourceRc.Width(), sourceRc.Height(),SRCCOPY);
	dc.Detach();
}

void CPngComListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);

	CRect rect;
	GetWindowRect(rect);
	CWindowDC dc(this);

	CRect boundRect;
	dc.GetClipBox(boundRect);

	//只画滚动条区域
	dc.IntersectClipRect(CRect(m_vTopRect.left, m_vTopRect.top, m_vTopRect.right, m_vBotRect.bottom));

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, boundRect.Width(), boundRect.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);


	DrawClient(&memDC);
	DrawVScroll(&memDC);

	dc.BitBlt(boundRect.left, boundRect.top, boundRect.Width(), boundRect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CPngComListBox::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CListBox::OnNcPaint() for painting messages
	InitImage();

	CWindowDC dc(this);
	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	//dc.BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &m_NcMemDC, 0, 0, SRCCOPY);

	CRect rcClient;
	GetClientRect(&rcClient);

	if (IsWindowVisible())
	{
		CDC			memDC;
		CBitmap		memBmp;
		memDC.CreateCompatibleDC(&dc);
		memBmp.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());
		CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
		memDC.SetBkMode(TRANSPARENT);
		CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);

		DrawClient(&memDC);
		DrawVScroll(&memDC);

		dc.BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		memDC.SelectObject(pOldFont);
		memDC.DeleteDC();
		memBmp.DeleteObject();
	}
}

void CPngComListBox::PrepareNcPaint()
{

	CWindowDC dc(this);
	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	CRect rcClient;
	GetClientRect(&rcClient);

	CRect 	listLeftRc = CRect(rcClient.left, rcClient.top, SafeGetImageWidth(m_pImgListLeft), rcClient.bottom);
	CRect 	listTopRc = CRect(rcClient.left, rcClient.top, rcClient.right, SafeGetImageHeight(m_pImgListTop));
	CRect 	listRightRc = CRect(rcClient.right - SafeGetImageWidth(m_pImgListRight), rcClient.top, rcClient.right, rcClient.bottom);
	CRect 	listBottomRc = CRect(rcClient.left, rcClient.bottom - SafeGetImageHeight(m_pImgListBottom), rcClient.right, rcClient.bottom);

	if(m_NcMemDC.GetSafeHdc() == NULL)
	{
		m_NcMemDC.CreateCompatibleDC(&dc);
		m_NcMemBmp.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());
		m_NcMemDC.SelectObject(&m_NcMemBmp);
		m_NcMemDC.SetBkMode(TRANSPARENT);
		m_NcMemDC.SelectObject(GetFont());
		::SetTextColor(m_NcMemDC.GetSafeHdc(), m_textColor);
	}

	CRect ncClient = rcClient;
	MemPaintPng(m_NcMemDC, ncClient, m_pImgListCenter);

	//Draw the left side of the list box
	MemPaintPng(m_NcMemDC, listLeftRc, m_pImgListLeft);
	//Draw the right side of the list box
	MemPaintPng(m_NcMemDC, listTopRc, m_pImgListTop);
	//Draw the top of the list bar
	MemPaintPng(m_NcMemDC, listRightRc, m_pImgListRight);
	//Draw the bottom of the list bar
	MemPaintPng(m_NcMemDC, listBottomRc, m_pImgListBottom);

	CRect	rcItem;
	CString	szItemString;
	BOOL	bOutSide = FALSE;
	int		itemCount = GetCount();

	for (int pos = rcClient.top; pos < rcClient.bottom;)
	{	//输出文字是为了防止在NCPaint与OnErase之间发生文字的颤抖
		UINT index = ItemFromPoint(CPoint(rcClient.left, pos), bOutSide);
		GetText(index, szItemString);
		GetItemRect(index, &rcItem);

		//左右边框除外
		CRect desDcRc = rcItem;

		CRect desSelRc = CRect(rcItem.left + listLeftRc.Width(), rcItem.top, rcItem.right - listRightRc.Width(), rcItem.bottom);
		if (index == 0)
			desSelRc.top += listTopRc.Height();
		if (index == itemCount - 1)
			desSelRc.bottom -= listBottomRc.Height();

		if (GetSel(index))
		{	//上下边框除外
			MemPaintPng(m_NcMemDC, desSelRc, m_pImgListSel);
		}
		else
		{
			MemPaintPng(m_NcMemDC, desSelRc, m_pImgListCenter);
		}

		desDcRc.left += m_nTextOffsetX;
		MemDrawText(m_NcMemDC, desDcRc, szItemString, m_textAlignMode);

		pos += GetItemHeight(index);
	}

	{

		SCROLLBARINFO hscr;
		memset(&hscr, 0, sizeof(SCROLLBARINFO));
		hscr.cbSize = sizeof(SCROLLBARINFO);
		CRect rcScrollBar;

		::GetScrollBarInfo(m_hWnd, OBJID_VSCROLL, &hscr);
		rcScrollBar = hscr.rcScrollBar;
		ScreenToClient(rcScrollBar);
		rcScrollBar.OffsetRect(rcClient.Width() + rcScrollBar.Width(), 0);


		CDC			memScrollDC;
		CBitmap		memScrollBmp;
		memScrollDC.CreateCompatibleDC(&dc);
		memScrollBmp.CreateCompatibleBitmap(&dc, rcScrollBar.Width(), rcScrollBar.Height());
		memScrollDC.SelectObject(&memScrollBmp);

		//if(m_VScroll.GetSafeHwnd())
		//	m_VScroll.DrawVScroll(&memScrollDC);		//显示时已经初始化过Img及控件的位置

		m_NcMemDC.BitBlt(rcScrollBar.left, rcScrollBar.top, rcScrollBar.Width(), rcScrollBar.Height(), &memScrollDC, 0, 0, SRCCOPY);
	}

}

void CPngComListBox::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CListBox::PreSubclassWindow();

 	ModifyVScrollStyle();
}

void CPngComListBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CListBox::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}

void CPngComListBox::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default

	if (bCalcValidRects && ModifyVScrollStyle())
	{
		SCROLLBARINFO hscr;
		memset(&hscr, 0, sizeof(SCROLLBARINFO));
		hscr.cbSize = sizeof(SCROLLBARINFO);
		CRect rcScrollBar;

		::GetScrollBarInfo(m_hWnd, OBJID_VSCROLL, &hscr);
		rcScrollBar = hscr.rcScrollBar;

		lpncsp->rgrc[0].left -= rcScrollBar.Width();//去掉默认滚动条的区域
		lpncsp->rgrc[0].right -= rcScrollBar.Width();//腾出我们要绘制的滚动条区域
	}

	CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
}

BOOL CPngComListBox::ModifyVScrollStyle()   //检测是否有垂直滚动条
{
	DWORD Style = GetStyle();
	if ((Style & WS_VSCROLL) == WS_VSCROLL)
	{

		long StyleEx = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		if ((StyleEx&WS_EX_RIGHTSCROLLBAR) == WS_EX_RIGHTSCROLLBAR)
		{
			ModifyStyleEx(WS_EX_RIGHTSCROLLBAR, WS_EX_LEFTSCROLLBAR);
		}
		return true;
	}
	return false;
}

BOOL CPngComListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	BOOL ret = CListBox::OnMouseWheel(nFlags, zDelta, pt);

	CRect rect;
	GetWindowRect(rect);
	CWindowDC dc(this);

	CRect boundRect;
	dc.GetClipBox(boundRect);

	//只画滚动条区域
	dc.IntersectClipRect(CRect(m_vTopRect.left, m_vTopRect.top, m_vTopRect.right, m_vBotRect.bottom));

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, boundRect.Width(), boundRect.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);


	DrawClient(&memDC);
	DrawVScroll(&memDC);

	dc.BitBlt(boundRect.left, boundRect.top, boundRect.Width(), boundRect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();

	return ret;
}

BOOL CPngComListBox::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL bRet = CListBox::OnWndMsg(message, wParam, lParam, pResult);

	if(message == WM_WINDOWPOSCHANGING)
	{

	}

	return bRet;
}

void CPngComListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rcClient;
	GetClientRect(rcClient);

	SCROLLBARINFO vScrollBarInfo;
	memset(&vScrollBarInfo, 0, sizeof(SCROLLBARINFO));
	vScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	GetScrollBarInfo(OBJID_VSCROLL, &vScrollBarInfo);

	CRect rcVScrollBar(vScrollBarInfo.rcScrollBar);
	ScreenToClient(rcVScrollBar);
	rcVScrollBar.OffsetRect(rcClient.Width() + rcVScrollBar.Width(), 0);

	if (rcVScrollBar.PtInRect(point))
	{
		int scrollLines = 0;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, (LPVOID)&scrollLines, 0);

		//ScreenToClient(&point);
		int vStartThumbOffY = m_vThumbRect.bottom - point.y; //鼠标按键相对于滑块的垂直位置


		if (rcVScrollBar.PtInRect(point))
		{
			int minValue, maxValue;
			GetScrollRange(SB_VERT, &minValue, &maxValue);

			UINT hitType = OnHitTest(point);
			BOOL bTrack = FALSE;

			MSG msg;
			memset(&msg, 0, sizeof(MSG));
			msg.hwnd = m_hWnd;

			do{
				if (msg.hwnd == GetSafeHwnd())
				{
					BOOL bTrackCircle = TRUE;
					while (1)
					{
						if (hitType == SB_LINEUP)
						{
							SCROLLINFO vScrollInfo;
							memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
							vScrollInfo.cbSize = sizeof(SCROLLINFO);
							GetScrollInfo(SB_VERT, &vScrollInfo, SIF_ALL);

							if (vScrollInfo.nPos > vScrollInfo.nMin)
							{
								vScrollInfo.nPos -= scrollLines;
								if (vScrollInfo.nPos - scrollLines < vScrollInfo.nMin)
									vScrollInfo.nPos = vScrollInfo.nMin;

								SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nPos), NULL);
							}

							bTrackCircle = FALSE;
						}
						else if (hitType == SB_LINEDOWN)
						{
							SCROLLINFO vScrollInfo;
							memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
							vScrollInfo.cbSize = sizeof(SCROLLINFO);
							GetScrollInfo(SB_VERT, &vScrollInfo, SIF_ALL);

							if (vScrollInfo.nPos + int(vScrollInfo.nPage) <= vScrollInfo.nMax)
							{
								vScrollInfo.nPos += scrollLines;
								if (vScrollInfo.nPos + scrollLines + int(vScrollInfo.nPage) > vScrollInfo.nMax) //nPos与nPage和一般比nMax大1
									vScrollInfo.nPos = vScrollInfo.nMax + 1 - int(vScrollInfo.nPage);

								SendMessage(WM_VSCROLL, MAKELONG(hitType, 0), NULL);
							}

							bTrackCircle = FALSE;
						}
						else if (hitType == SB_PAGEUP)
						{
							SCROLLINFO vScrollInfo;
							memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
							vScrollInfo.cbSize = sizeof(SCROLLINFO);
							GetScrollInfo(SB_VERT, &vScrollInfo, SIF_ALL);

							if (vScrollInfo.nPos > vScrollInfo.nMin)
							{
								vScrollInfo.nPos -= vScrollInfo.nPage;
								if (vScrollInfo.nPos - scrollLines < vScrollInfo.nMin)
									vScrollInfo.nPos = vScrollInfo.nMin;

								SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nPos), NULL);
							}

							bTrackCircle = FALSE;
						}
						else if (hitType == SB_PAGEDOWN)
						{
							SCROLLINFO vScrollInfo;
							memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
							vScrollInfo.cbSize = sizeof(SCROLLINFO);
							GetScrollInfo(SB_VERT, &vScrollInfo, SIF_ALL);

							if (vScrollInfo.nPos + int(vScrollInfo.nPage) <= vScrollInfo.nMax)
							{
								vScrollInfo.nPos += vScrollInfo.nPage;
								if (vScrollInfo.nPos + scrollLines + int(vScrollInfo.nPage) > vScrollInfo.nMax) //nPos与nPage和一般比nMax大1
									vScrollInfo.nPos = vScrollInfo.nMax + 1 - vScrollInfo.nPage;

								SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nPos), NULL);
							}

							bTrackCircle = FALSE;
						}
						else
						{
							if (bTrackCircle == TRUE)	//如果第一次没有则应该跳出
								break;
						}

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
						//ScreenToClient(&ptCur);//list点转为滚动条上的点

						int offset = ptCur.y - point.y;
						point = ptCur;

						SCROLLINFO vScrollInfo;
						memset(&vScrollInfo, 0, sizeof(SCROLLINFO));
						vScrollInfo.cbSize = sizeof(SCROLLINFO);
						GetScrollInfo(SB_VERT, &vScrollInfo, SIF_ALL);

						if (offset < 0 && vScrollInfo.nPos <= vScrollInfo.nMin ||
							offset > 0 && vScrollInfo.nPos >= vScrollInfo.nMax
							)
							continue;

						if (msg.message == WM_MOUSEMOVE)
						{
							int thumbHeightTemp = m_vThumbRect.Height();
							m_vThumbRect.bottom = ptCur.y + vStartThumbOffY;
							m_vThumbRect.top = m_vThumbRect.bottom - thumbHeightTemp;

							if (m_vThumbRect.top < m_vTopRect.bottom)
							{	//不能移出上边界
								m_vThumbRect.OffsetRect(0, m_vTopRect.bottom - m_vThumbRect.top);
								SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nMin), NULL);
							}
							else if (m_vThumbRect.bottom > m_vBotRect.top)
							{	//不能移出下边界
								m_vThumbRect.OffsetRect(0, m_vBotRect.top - m_vThumbRect.bottom);
								SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nMax + 1 - vScrollInfo.nPage), NULL);
							}
							else
							{
								double pixelRange = 1.0*(rcVScrollBar.Height() - m_vTopRect.Height() - m_vBotRect.Height() - m_vThumbRect.Height());
								vScrollInfo.nPos = int((vScrollInfo.nMax - vScrollInfo.nMin - vScrollInfo.nPage + 1) / pixelRange * (m_vThumbRect.top - m_vTopRect.bottom));//nMax的像素位置并不是在最底端

								if (vScrollInfo.nPos <= vScrollInfo.nMin)
									vScrollInfo.nPos = vScrollInfo.nMin;
								if (vScrollInfo.nPos + int(vScrollInfo.nPage) > vScrollInfo.nMax) //nPos与nPage和一般比nMax大1
									vScrollInfo.nPos = vScrollInfo.nMax + 1 - int(vScrollInfo.nPage);
								SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nPos), NULL);
							}

						}

						if (msg.message == WM_LBUTTONUP)
						{
							SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBPOSITION, vScrollInfo.nPos), NULL);
							break;
						}
						//if (msg.message == WM_MOUSEMOVE)
						//{
						//	int thumbHeightTemp = m_vThumbRect.Height();
						//	m_vThumbRect.bottom = ptCur.y + vStartThumbOffY;
						//	m_vThumbRect.top = m_vThumbRect.bottom - thumbHeightTemp;


						//	if (m_vThumbRect.top < m_vTopRect.bottom)
						//	{	//不能移出上边界
						//		m_vThumbRect.OffsetRect(0, m_vTopRect.bottom - m_vThumbRect.top);
						//		SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nMin), NULL);
						//	}
						//	else if (m_vThumbRect.bottom > m_vBotRect.top)
						//	{	//不能移出下边界
						//		m_vThumbRect.OffsetRect(0, m_vBotRect.top - m_vThumbRect.bottom);
						//		SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nMax), NULL);
						//	}
						//	else
						//	{
						//		float pixelRange = 1.0*(rcVScrollBar.Height() - m_vTopRect.Height() - m_vBotRect.Height() - m_hThumbRect.Height());
						//		vScrollInfo.nPos = (vScrollInfo.nMax - vScrollInfo.nMin) / pixelRange * m_VScroll.m_vThumbRect.top;
						//		SendMessage(WM_VSCROLL, MAKELONG(hitType, vScrollInfo.nPos), NULL);
						//	}

						//}

						//if (msg.message == WM_LBUTTONUP)
						//{
						//	SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBPOSITION, vScrollInfo.nPos), NULL);
						//	break;
						//}
					}
					else
						break;

				}
				else
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);

					if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	//鼠标左键不按下时，及时跳出循环
						break;
				}

			} while (::GetMessage(&msg, NULL, 0, 0));
		}

		return;
	}

	CListBox::OnLButtonDown(nFlags, point);
}
