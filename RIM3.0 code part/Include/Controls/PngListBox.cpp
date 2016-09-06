//********************************************************************
//	PNGLISTBOX.CPP 文件注释
//	文件名		: PNGLISTBOX.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者		: 张永
//	创建时间	: 2014/12/22 15:33
//	文件描述	: 
//*********************************************************************	
// PngListBox.cpp : implementation file
//

#include "stdafx.h"
#include "PngListBox.h"


// CPngListBox

IMPLEMENT_DYNAMIC(CPngListBox, CListBox)

CPngListBox::CPngListBox()
{
	m_bScrollInit					= FALSE;
	m_bMouseMove					= FALSE;
	m_bFillOrTitleParentPng			= TRUE;
	m_textColor						= RGB(255, 255, 255);

	m_textAlignMode					= DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_nTextOffsetX					= 0;


	m_pngParentBkPath				= _T("");		//父背景图片
	m_pngBkPath						= _T("");
	m_pngOverPath					= _T("");		//鼠标经过时的图片
	m_pngSelPath					= _T("");		//鼠标选中时的图片
	m_disablePngPath				= _T("");		//禁用时显示图片		有效性：非强制

	m_pngScrollBk					= _T("");		//滚动条的背景 水平与垂直
	m_pngVScrollTop					= _T("");		//垂直滚动条的上箭头
	m_pngVScrollBot					= _T("");		//垂直滚动条的下箭头
	m_pngVScrollThumb				= _T("");		//垂直滚动条的滑块
	m_pngHScrollLeft				= _T("");		//水平滚动条的上箭头
	m_pngHScrollRight				= _T("");		//水平滚动条的下箭头
	m_pngHScrollThumb				= _T("");		//水平滚动条的滑块


	m_pImgParentBk					= NULL;
	m_pImgBk						= NULL;
	m_pImgListOver					= NULL;
	m_pImgListSel					= NULL;
	m_pImgDisable					= NULL;

	m_pImgScrollBk					= NULL;
	m_pImgVScrollTop				= NULL;
	m_pImgVScrollBot				= NULL;
	m_pImgVScrollThumb				= NULL;
	m_pImgHScrollLeft				= NULL;
	m_pImgHScrollRight				= NULL;
	m_pImgHScrollThumb				= NULL;
}

CPngListBox::~CPngListBox()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngListBox, CListBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// CPngListBox message handlers


void CPngListBox::InitImage()
{
	if(m_pImgParentBk == NULL)			m_pImgParentBk			= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)				m_pImgBk				= SafeImageFrom(m_pngBkPath);
	if(m_pImgListOver == NULL)			m_pImgListOver			= SafeImageFrom(m_pngOverPath);
	if(m_pImgListSel == NULL)			m_pImgListSel			= SafeImageFrom(m_pngSelPath);
	if(m_pImgDisable == NULL)			m_pImgDisable			= SafeImageFrom(m_disablePngPath);


	if (m_pImgScrollBk == NULL)			m_pImgScrollBk			= SafeImageFrom(m_pngScrollBk);
	if (m_pImgVScrollTop == NULL)		m_pImgVScrollTop		= SafeImageFrom(m_pngVScrollTop);
	if (m_pImgVScrollBot == NULL)		m_pImgVScrollBot		= SafeImageFrom(m_pngVScrollBot);
	if (m_pImgVScrollThumb == NULL)		m_pImgVScrollThumb		= SafeImageFrom(m_pngVScrollThumb);
	if (m_pImgHScrollLeft == NULL)		m_pImgHScrollLeft		= SafeImageFrom(m_pngHScrollLeft);
	if (m_pImgHScrollRight == NULL)		m_pImgHScrollRight		= SafeImageFrom(m_pngHScrollRight);
	if (m_pImgHScrollThumb == NULL)		m_pImgHScrollThumb		= SafeImageFrom(m_pngHScrollThumb);

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

void CPngListBox::DestoryImage()
{
	if(m_pImgParentBk		== NULL)	{	delete m_pImgParentBk		;		m_pImgParentBk		= NULL;		}					
	if(m_pImgBk				== NULL)	{	delete m_pImgBk				;		m_pImgBk			= NULL;		}	
	if(m_pImgListOver		== NULL)	{	delete m_pImgListOver		;		m_pImgListOver		= NULL;		}	
	if(m_pImgListSel		== NULL)	{	delete m_pImgListSel		;		m_pImgListSel		= NULL;		}	
	if(m_pImgDisable		== NULL)	{	delete m_pImgDisable		;		m_pImgDisable		= NULL;		}	

	if(m_pImgScrollBk		== NULL)	{	delete m_pImgScrollBk		;		m_pImgScrollBk		= NULL;		}					
	if(m_pImgVScrollTop		== NULL)	{	delete m_pImgVScrollTop		;		m_pImgVScrollTop	= NULL;		}	
	if(m_pImgVScrollBot		== NULL)	{	delete m_pImgVScrollBot		;		m_pImgVScrollBot	= NULL;		}	
	if(m_pImgVScrollThumb	== NULL)	{	delete m_pImgVScrollThumb	;		m_pImgVScrollThumb	= NULL;		}	
	if(m_pImgHScrollLeft	== NULL)	{	delete m_pImgHScrollLeft	;		m_pImgHScrollLeft	= NULL;		}	
	if(m_pImgHScrollRight	== NULL)	{	delete m_pImgHScrollRight	;		m_pImgHScrollRight	= NULL;		}	
	if(m_pImgHScrollThumb	== NULL)	{	delete m_pImgHScrollThumb	;		m_pImgHScrollThumb	= NULL;		}	
}

void CPngListBox::DrawClient(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	MemPaintParentPng(this, *pDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);

	if(IsWindowEnabled())
	{
		::SetTextColor(pDC->GetSafeHdc(), m_textColor);
		MemPaintPng(*pDC, rcClient, m_pImgBk);
	}
	else
	{
		::SetTextColor(pDC->GetSafeHdc(), RGB(128, 128, 128));
		MemPaintPng(*pDC, rcClient, m_pImgDisable);
	}

	CRect	rcItem;
	CString	szItemString;
	BOOL	bOutSide = FALSE;
	int		itemCount = GetCount();

	CPoint	ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	for (int pos = rcClient.top; pos < rcClient.bottom;)
	{
		UINT index = ItemFromPoint(CPoint(rcClient.left, pos), bOutSide);

		if(bOutSide)
			break;

		GetText(index, szItemString);
		GetItemRect(index, &rcItem);

		//左右边框除外
		CRect desRc = rcItem;

		if (GetSel(index) || rcItem.PtInRect(ptCursor))
		{	//上下边框除外
			MemPaintPng(*pDC, desRc, m_pImgListSel);
		}
		else
		{
			//MemPaintPng(memDC, desRc, m_pImgListCenter);
		}

		desRc.left += m_nTextOffsetX;
		MemDrawText(*pDC, desRc, szItemString, m_textAlignMode);

		pos += GetItemHeight(index);
	}
}

HBRUSH CPngListBox::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	pDC->SetBkMode(TRANSPARENT); 
	pDC->SetTextColor(m_textColor);

	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

void CPngListBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CListBox::OnPaint() for painting messages

	InitImage();

	CRect  rcClient;
	GetClientRect(&rcClient);

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

	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}

void CPngListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetRedraw(FALSE);
	CListBox::OnLButtonDown(nFlags, point);
	SetRedraw(TRUE);
	Invalidate(FALSE);
}

void CPngListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bMouseMove == false)
	{
		///////////////绘制鼠标移动状态

		//要求接收鼠标离开消息
		TRACKMOUSEEVENT		csTME;
		csTME.cbSize	= sizeof(csTME);
		csTME.dwFlags	= TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&csTME);

		m_bMouseMove = true;
	}


	//CListBox::OnMouseMove(nFlags, point);
 	Invalidate(FALSE);		//如果出现鼠标划动时出现系统蓝色选中将此处禁用，只允许点选 
}

void CPngListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//SetRedraw(FALSE);
	CListBox::OnLButtonUp(nFlags, point);
	//SetRedraw(TRUE);
}

void CPngListBox::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	m_bMouseMove = FALSE;
	__super::OnMouseLeave();
	Invalidate(FALSE);
}

void CPngListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	__super::OnLButtonDblClk(nFlags, point);
}

void CPngListBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	//{
	//	static int i= 0;
	//	char ch[MAX_PATH] = {0};
	//	sprintf_s(ch, MAX_PATH, "\n(zyo): name1:%d name2:%d name3:%d Index(i):%d \n", 0, 0, 0, i++);
	//	OutputDebugString(ch);//***zy***
	//}
	SetRedraw(FALSE);
	__super::OnTimer(nIDEvent);
	SetRedraw(TRUE);
}

void CPngListBox::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	__super::PreSubclassWindow();

	if(AfxGetThreadState()->m_pWndInit == NULL && m_bScrollInit == FALSE)
	{
		m_HScroll.InitScrollBar(this, SB_HORZ);
		m_VScroll.InitScrollBar(this, SB_VERT);
		m_bScrollInit = TRUE;
	}
}

int CPngListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if(AfxGetThreadState()->m_pWndInit == NULL && m_bScrollInit == FALSE)
	{
		m_HScroll.InitScrollBar(this, SB_HORZ);
		m_VScroll.InitScrollBar(this, SB_VERT);
		m_bScrollInit = TRUE;
	}

	return 0;
}

void CPngListBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	InitImage();
	if(IsWindow(m_HScroll.GetSafeHwnd()))
		m_HScroll.ShowWindow(bShow);
	if(IsWindow(m_VScroll.GetSafeHwnd()))
		m_VScroll.ShowWindow(bShow);
}

void CPngListBox::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}

void CPngListBox::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call __super::OnNcPaint() for painting messages
}
