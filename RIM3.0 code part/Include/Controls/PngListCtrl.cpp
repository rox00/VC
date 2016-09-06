//********************************************************************
//	PNGLISTCTRL.CPP 文件注释
//	文件名		: PNGLISTCTRL.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者		: 张永
//	创建时间	: 2014/12/29 15:31
//	文件描述	: 目前仅支持文字，如需要图标到时再进行扩展，请联系原作者
//*********************************************************************	
// E:\My Project\visual studio 2008\myskinDlg\myskinDlg\Controls\PngListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PngListCtrl.h"


// CPngListCtrl

IMPLEMENT_DYNAMIC(CPngListCtrl, CListCtrl)

CPngListCtrl::CPngListCtrl()
{
	m_bScrollInit					= FALSE;
	m_bFillOrTitleParentPng			= TRUE;

	m_pngParentBkPath				= _T("");		//父背景图片
	m_pngBkPath						= _T("");		//背景图片
	m_disablePngPath				= _T("");		//禁用时显示图片		有效性：非强制有效

	m_pngHeaderBk					= _T("");		//头控件整体的背景图片
	m_pngHeaderItemOverBk			= _T("");		//头控件鼠标经过时的背景图片
	m_pngHeaderItemPushBk			= _T("");		//头控件鼠标按下时的背景图片
	m_pngHeaderDisable				= _T("");		//头控件被禁用时的背景图片

	m_pngScrollBk					= _T("");		//滚动条的背景 水平与垂直
	m_pngVScrollTop					= _T("");		//垂直滚动条的上箭头
	m_pngVScrollBot					= _T("");		//垂直滚动条的下箭头
	m_pngVScrollThumb				= _T("");		//垂直滚动条的滑块
	m_pngHScrollLeft				= _T("");		//水平滚动条的上箭头
	m_pngHScrollRight				= _T("");		//水平滚动条的下箭头
	m_pngHScrollThumb				= _T("");		//水平滚动条的滑块


	m_pImgParentBk					= NULL;
	m_pImgBk						= NULL;


	//头控件
	m_headerSeparatorWidth			= 2;	
	m_headerSeparatorColor			= RGB(255, 255, 255);	
	m_headerTextOffsetX				= 2;
	m_headerTextAlignMode			= DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	m_headerTextColor				= RGB(255, 255, 255);

	m_pImgHeaderBk					= NULL;
	m_pImgHeaderItemOverBk			= NULL;
	m_pImgHeaderItemPushBk			= NULL;
	m_pImgHeaderDisable				= NULL;

	m_pImgScrollBk					= NULL;
	m_pImgVScrollTop				= NULL;
	m_pImgVScrollBot				= NULL;
	m_pImgVScrollThumb				= NULL;
	m_pImgHScrollLeft				= NULL;
	m_pImgHScrollRight				= NULL;
	m_pImgHScrollThumb				= NULL;
}

CPngListCtrl::~CPngListCtrl()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngListCtrl, CListCtrl)
	ON_MESSAGE(WM_PRINTCLIENT, &CPngListCtrl::OnPrintClient)
	ON_MESSAGE(WM_PRINT, &CPngListCtrl::OnPrint)

	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// CPngListCtrl message handlers


void CPngListCtrl::InitImage()
{
	if(m_pImgParentBk == NULL)				m_pImgParentBk			= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)					m_pImgBk				= SafeImageFrom(m_pngBkPath);
	if(m_pImgDisable == NULL)				m_pImgDisable			= SafeImageFrom(m_disablePngPath);

	if(m_pImgHeaderBk == NULL)				m_pImgHeaderBk			= SafeImageFrom(m_pngHeaderBk);
	if(m_pImgHeaderItemOverBk == NULL)		m_pImgHeaderItemOverBk	= SafeImageFrom(m_pngHeaderItemOverBk);
	if(m_pImgHeaderItemPushBk == NULL)		m_pImgHeaderItemPushBk	= SafeImageFrom(m_pngHeaderItemPushBk);
	if(m_pImgHeaderDisable == NULL)			m_pImgHeaderDisable		= SafeImageFrom(m_pngHeaderDisable);

	if(m_pImgScrollBk == NULL)				m_pImgScrollBk			= SafeImageFrom(m_pngScrollBk);
	if(m_pImgVScrollTop == NULL)			m_pImgVScrollTop		= SafeImageFrom(m_pngVScrollTop);
	if(m_pImgVScrollBot == NULL)			m_pImgVScrollBot		= SafeImageFrom(m_pngVScrollBot);
	if(m_pImgVScrollThumb == NULL)			m_pImgVScrollThumb		= SafeImageFrom(m_pngVScrollThumb);
	if(m_pImgHScrollLeft == NULL)			m_pImgHScrollLeft		= SafeImageFrom(m_pngHScrollLeft);
	if(m_pImgHScrollRight == NULL)			m_pImgHScrollRight		= SafeImageFrom(m_pngHScrollRight);
	if(m_pImgHScrollThumb == NULL)			m_pImgHScrollThumb		= SafeImageFrom(m_pngHScrollThumb);

	//头控件
	m_Header.m_separatorWidth 			= m_headerSeparatorWidth;
	m_Header.m_separatorColor 			= m_headerSeparatorColor;
	m_Header.m_textOffsetX				= m_headerTextOffsetX;
	m_Header.m_textAlignMode			= m_headerTextAlignMode;
	m_Header.m_textColor				= m_headerTextColor;

	m_Header.m_pImgBk					= m_pImgHeaderBk;
	m_Header.m_pImgHoverBk				= m_pImgHeaderItemOverBk;
	m_Header.m_pImgPushBk				= m_pImgHeaderItemPushBk;
	m_Header.m_pImgDisable				= m_pImgHeaderDisable;

 	//滚动条
 	m_HScroll.m_pImgParentBk			= m_pImgParentBk;
 	m_HScroll.m_pImgScrollBk			= m_pImgScrollBk;
 	m_HScroll.m_pImgHScrollLeft			= m_pImgHScrollLeft;
 	m_HScroll.m_pImgHScrollRight		= m_pImgHScrollRight;
 	m_HScroll.m_pImgHScrollThumb		= m_pImgHScrollThumb;
 
 	m_VScroll.m_pImgParentBk			= m_pImgParentBk;
 	m_VScroll.m_pImgScrollBk			= m_pImgScrollBk;
 	m_VScroll.m_pImgVScrollTop			= m_pImgVScrollTop;
 	m_VScroll.m_pImgVScrollBot			= m_pImgVScrollBot;
 	m_VScroll.m_pImgVScrollThumb		= m_pImgVScrollThumb;

}

void CPngListCtrl::DestoryImage()
{
	if(m_pImgHeaderBk			== NULL)	{	delete m_pImgHeaderBk			;		m_pImgHeaderBk			= NULL;		}					
	if(m_pImgHeaderItemOverBk	== NULL)	{	delete m_pImgHeaderItemOverBk	;		m_pImgHeaderItemOverBk	= NULL;		}	
	if(m_pImgHeaderItemPushBk	== NULL)	{	delete m_pImgHeaderItemPushBk	;		m_pImgHeaderItemPushBk	= NULL;		}	
	if(m_pImgHeaderDisable		== NULL)	{	delete m_pImgHeaderDisable		;		m_pImgHeaderDisable		= NULL;		}		

	if(m_pImgScrollBk			== NULL)	{	delete m_pImgScrollBk			;		m_pImgScrollBk			= NULL;		}					
	if(m_pImgVScrollTop			== NULL)	{	delete m_pImgVScrollTop			;		m_pImgVScrollTop		= NULL;		}	
	if(m_pImgVScrollBot			== NULL)	{	delete m_pImgVScrollBot			;		m_pImgVScrollBot		= NULL;		}	
	if(m_pImgVScrollThumb		== NULL)	{	delete m_pImgVScrollThumb		;		m_pImgVScrollThumb		= NULL;		}	
	if(m_pImgHScrollLeft		== NULL)	{	delete m_pImgHScrollLeft		;		m_pImgHScrollLeft		= NULL;		}	
	if(m_pImgHScrollRight		== NULL)	{	delete m_pImgHScrollRight		;		m_pImgHScrollRight		= NULL;		}	
	if(m_pImgHScrollThumb		== NULL)	{	delete m_pImgHScrollThumb		;		m_pImgHScrollThumb		= NULL;		}	
}

HBRUSH CPngListCtrl::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

HBRUSH CPngListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CPngListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	InitImage();
	//CRect rcClient;
	//GetClientRect(rcClient);
	//pDC->FillSolidRect(rcClient, RGB(0,255,255));

	return TRUE;
	return CListCtrl::OnEraseBkgnd(pDC);
}

void CPngListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages

	InitImage();

	CRect headerRect;
 	GetDlgItem(0)->GetWindowRect(&headerRect);//获取头控件区域
 
 	CRect rcClient;
	GetClientRect(rcClient);
 	//dc.GetClipBox(rcClient);
 
 	CDC			memDC;
 	CBitmap		memBmp;
 	memDC.CreateCompatibleDC(&dc);
 	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
 	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
 	memDC.SetBkMode(TRANSPARENT);
 	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());

	////【zy 2014-10-17 ↓】  画背景
	MemPaintParentPng(this, memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);

	if(IsWindowEnabled())
	{
		//对于背景，设置了就画，未设置则不绘制背景
		MemPaintPng(memDC, rcClient, m_pImgBk);
	}
	else
	{
		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
		MemPaintPng(memDC, rcClient, m_pImgDisable);
	}

 	//memDC.FillSolidRect(rcClient, RGB(52,188,52));
// 
// 	//这一句是调用默认的OnPaint(),把图形画在内存DC表上
 	DefWindowProc(WM_PAINT,(WPARAM)memDC.m_hDC,(LPARAM)0);
// 

 	dc.BitBlt(0, headerRect.Height(), rcClient.Width(), rcClient.Height(),&memDC,0, headerRect.Height(),SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();

	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}

int CPngListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	if(AfxGetThreadState()->m_pWndInit == NULL && m_bScrollInit == FALSE)
	{
		CHeaderCtrl *pHeader = GetHeaderCtrl();
		m_Header.SubclassWindow(pHeader->GetSafeHwnd());
		m_bScrollInit = TRUE;

		m_HScroll.InitScrollBar(this, SB_HORZ);
		m_VScroll.InitScrollBar(this, SB_VERT);

 		SetBkColor(CLR_NONE);
 		SetTextBkColor(CLR_NONE);
	}

	return 0;
}

void CPngListCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class


	if(AfxGetThreadState()->m_pWndInit == NULL && m_bScrollInit == FALSE)
	{
		CHeaderCtrl *pHeader = GetHeaderCtrl();
		m_Header.SubclassWindow(pHeader->GetSafeHwnd());
		m_bScrollInit = TRUE;

		m_HScroll.InitScrollBar(this, SB_HORZ);
		m_VScroll.InitScrollBar(this, SB_VERT);

 		//SetBkColor(CLR_NONE);		//WM_PRINT
 		SetTextBkColor(CLR_NONE);
	}

	CListCtrl::PreSubclassWindow();
}

void CPngListCtrl::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnNcPaint() for painting messages
	InitImage();

}

void CPngListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnLButtonDown(nFlags, point);
}

void CPngListCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(IsWindow(m_HScroll.GetSafeHwnd()))
		m_HScroll.ShowWindow(bShow);
	if(IsWindow(m_VScroll.GetSafeHwnd()))
		m_VScroll.ShowWindow(bShow);
}

void CPngListCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}

void CPngListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	__super::OnHScroll(nSBCode, nPos, pScrollBar);
	m_Header.Invalidate(FALSE);
}


LRESULT CPngListCtrl::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	return FALSE; 
}

LRESULT CPngListCtrl::OnPrint(WPARAM wParam, LPARAM lParam)
{
	//return FALSE;
	HDC hdc = HDC(wParam);

	InitImage();

	CDC dc;
	dc.Attach(hdc);

	InitImage();

	CRect headerRect;
	GetDlgItem(0)->GetWindowRect(&headerRect);//获取头控件区域

	CRect rcClient;
	GetClientRect(rcClient);
	//dc.GetClipBox(rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());

	////【zy 2014-10-17 ↓】  画背景
	MemPaintParentPng(this, memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);

	if(IsWindowEnabled())
	{
		//对于背景，设置了就画，未设置则不绘制背景
		MemPaintPng(memDC, rcClient, m_pImgBk);
	}
	else
	{
		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
		MemPaintPng(memDC, rcClient, m_pImgDisable);
	}

	//memDC.FillSolidRect(rcClient, RGB(52,188,52));
	// 
	// 	//这一句是调用默认的OnPaint(),把图形画在内存DC表上
	DefWindowProc(WM_PRINTCLIENT,(WPARAM)memDC.m_hDC,(LPARAM)0);
	// 

	dc.BitBlt(0, headerRect.Height(), rcClient.Width(), rcClient.Height(),&memDC,0, headerRect.Height(),SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();

	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();


	dc.Detach();

	return FALSE;
}