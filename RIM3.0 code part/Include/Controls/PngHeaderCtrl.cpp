//********************************************************************
//	PNGHEADERCTRL.CPP 文件注释
//	文件名		: PNGHEADERCTRL.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者		: 张永
//	创建时间	: 2014/12/29 15:30
//	文件描述	: 目前仅支持文字，如需要图标到时再进行扩展，请联系原作者
//*********************************************************************	
// E:\My Project\visual studio 2008\myskinDlg\myskinDlg\Controls\PngHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PngHeaderCtrl.h"


// CPngHeaderCtrl

IMPLEMENT_DYNAMIC(CPngHeaderCtrl, CHeaderCtrl)

CPngHeaderCtrl::CPngHeaderCtrl()
{
	m_divideSelIndex				= -1;

	m_separatorWidth				= 2;
	m_separatorColor				= RGB(255, 255, 255);


	m_textOffsetX					= 2;
	m_textAlignMode					= DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	m_textColor 					= RGB(255, 255, 255);

	m_pImgBk						= NULL;
	m_pImgHoverBk					= NULL;
	m_pImgDisable					= NULL;
}

CPngHeaderCtrl::~CPngHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CPngHeaderCtrl, CHeaderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CPngHeaderCtrl message handlers



HBRUSH CPngHeaderCtrl::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

HBRUSH CPngHeaderCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CHeaderCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CPngHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default


//  	CRect rcClient;
//  	GetClientRect(rcClient);
//  	pDC->FillSolidRect(rcClient, RGB(0,255,255));
//  	pDC->SetBkMode(TRANSPARENT);
	return TRUE;
	//return CHeaderCtrl::OnEraseBkgnd(pDC);
}

void CPngHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CHeaderCtrl::OnPaint() for painting messages
  	//DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0); 
  	//return;

	CRect rcClient;
	GetClientRect(rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);

	if(IsWindowEnabled())
	{
		//对于背景，设置了就画，未设置则不绘制背景
		MemPaintPng(memDC, CRect(0, 0, rcClient.Width(), rcClient.Height()), m_pImgBk);
	}
	else
	{
		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
		MemPaintPng(memDC, CRect(0, 0, rcClient.Width(), rcClient.Height()), m_pImgDisable);
	}

	HDITEM hdItem;
	TCHAR  lpBuffer[MAX_PATH];

	hdItem.mask = HDI_TEXT | HDI_HEIGHT | HDI_STATE;
	hdItem.pszText = lpBuffer;
	hdItem.cchTextMax = MAX_PATH;

	CPen sparetorPen(PS_SOLID, m_separatorWidth, m_separatorColor);
	CPen *pOldPen = (CPen*)memDC.SelectObject(sparetorPen);

	{
		CPoint curPt;
		GetCursorPos(&curPt);
		ScreenToClient(&curPt);
		HDHITTESTINFO hdhittestinfo;
		hdhittestinfo.pt = curPt;
		HitTest(&hdhittestinfo);

		if(!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))	//鼠标未按下时记录经过的Item
			m_divideSelIndex = hdhittestinfo.iItem;
	}

	int colCount; 
	colCount = GetItemCount();						//得到有几个列单元 
	for(int i = 0; i<colCount;i ++) 
	{//绘制项目文本
		CRect tRect;
		GetItemRect(i,&tRect);						//得到Item的尺寸

		memset(lpBuffer, 0, MAX_PATH);
		GetItem(i, &hdItem);

		int R = 0,G = 255,B = 0;
		CRect nRect(tRect);							//拷贝尺寸到新的容器中 

		CPoint pt[2] = {
			CPoint(nRect.left-1, 2),
			CPoint(nRect.left-1, rcClient.Height()-2)
		};
		memDC.Polyline(pt, 2);

		if(m_divideSelIndex == i)
		{	//鼠标经过时的图片
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)	//鼠标按下
				MemPaintPng(memDC, nRect, m_pImgPushBk);
			else
				MemPaintPng(memDC, nRect, m_pImgHoverBk);
		}

		tRect.left += m_textOffsetX;
		memDC.DrawText(lpBuffer, &tRect, m_textAlignMode);//绘制文本
	} 

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

LRESULT CPngHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 

// 	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
// 	RECT *prc = hdl.prc;			//父窗口的矩形区域，包含头控件所占区域
// 	WINDOWPOS *pwpos = hdl.pwpos;	//头控件的位置
// 
// 	//表头高度为原来1.5倍，如果要动态修改表头高度的话，将1.5设成一个全局变量 
// 	int nHeight = (int)(pwpos->cy * m_multiHeight);
// 	pwpos->cy = nHeight; 
// 	prc->top = nHeight; 
// 
// 	m_updateRc.left		= pwpos->x;
// 	m_updateRc.top		= pwpos->y;
// 	m_updateRc.right	= pwpos->cx + pwpos->x;
// 	m_updateRc.bottom	= pwpos->y + pwpos->cy;
// 	//m_updateRc.OffsetRect(-pwpos->x, pwpos->y);
	return lResult;
}

void CPngHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	__super::OnMouseMove(nFlags, point);
	////【zy 2015-01-05 ↓】  拖动头控件时要实时刷新

	if(MK_LBUTTON & nFlags)	//鼠标按下
	{
		if(m_divideSelIndex >= 0)
		{
			RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
		}
	}
	////【zy 2015-01-05 ↑】
}
