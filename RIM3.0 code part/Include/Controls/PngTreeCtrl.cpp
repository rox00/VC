// PngTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PngTreeCtrl.h"

//********************************************************************
//	PNGTREECTRL.CPP 文件注释
//	文件名 : PNGTREECTRL.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: 
//*********************************************************************	
// CPngTreeCtrl

IMPLEMENT_DYNAMIC(CPngTreeCtrl, CTreeCtrl)

CPngTreeCtrl::CPngTreeCtrl()
{
	m_bFillOrTitleParentPng			= TRUE;
	m_bScrollInit					= FALSE;

	m_pngParentBkPath				= _T("");		//父背景图片
	m_pngBkPath						= _T("");		//背景图片

	m_pngItemBtnSel					= _T("");		//HasButton风格
	m_pngItemBtnUnSel				= _T("");		//HasButton风格
	m_pngItemChkSel					= _T("");		//chekbox风格
	m_pngItemChkUnSel				= _T("");		//chekbox风格
	m_pngItemOver					= _T("");		//鼠标经过时和图片
	m_pngItemSel					= _T("");		//选中时的图片
	m_disablePngPath				= _T("");		//禁用时显示图片		有效性：非强制有效

	m_pngScrollBk					= _T("");		//滚动条的背景 水平与垂直
	m_pngVScrollTop					= _T("");		//垂直滚动条的上箭头
	m_pngVScrollBot					= _T("");		//垂直滚动条的下箭头
	m_pngVScrollThumb				= _T("");		//垂直滚动条的滑块
	m_pngHScrollLeft				= _T("");		//水平滚动条的上箭头
	m_pngHScrollRight				= _T("");		//水平滚动条的下箭头
	m_pngHScrollThumb				= _T("");		//水平滚动条的滑块

	m_pngTooltipBk					= _T("");		//工具提示条的背景

	m_textOffsetX					= 2;
	m_textAlignMode					= DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_textColor 					= RGB(255, 255, 255);

	m_pImgParentBk					= NULL;
 	m_pImgBk						= NULL;
	m_pImgButtonSel					= NULL;
	m_pImgButtonUnSel				= NULL;
	m_pImgChkSel					= NULL;
	m_pImgChkUnSel					= NULL;
 	m_pImgDisable					= NULL;
 	m_pImgItemSel					= NULL;
 	m_pImgItemOver					= NULL;

	m_pImgScrollBk					= NULL;
	m_pImgVScrollTop				= NULL;
	m_pImgVScrollBot				= NULL;
	m_pImgVScrollThumb				= NULL;
	m_pImgHScrollLeft				= NULL;
	m_pImgHScrollRight				= NULL;
	m_pImgHScrollThumb				= NULL;

	m_pImgToolTip					= NULL;

}

CPngTreeCtrl::~CPngTreeCtrl()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngTreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPngTreeCtrl message handlers

void CPngTreeCtrl::InitImage()
{
	if(m_pImgParentBk == NULL)		m_pImgParentBk		= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)			m_pImgBk			= SafeImageFrom(m_pngBkPath);
	if(m_pImgButtonSel == NULL)		m_pImgButtonSel		= SafeImageFrom(m_pngItemBtnSel);
	if(m_pImgButtonUnSel == NULL)	m_pImgButtonUnSel	= SafeImageFrom(m_pngItemBtnUnSel);
	if(m_pImgChkSel == NULL)		m_pImgChkSel		= SafeImageFrom(m_pngItemChkSel);
	if(m_pImgChkUnSel == NULL)		m_pImgChkUnSel		= SafeImageFrom(m_pngItemChkUnSel);
	if(m_pImgDisable == NULL)		m_pImgDisable		= SafeImageFrom(m_disablePngPath);
	if(m_pImgItemSel == NULL)		m_pImgItemSel		= SafeImageFrom(m_pngItemSel);
	if(m_pImgItemOver == NULL)		m_pImgItemOver		= SafeImageFrom(m_pngItemOver);

	if(m_pImgScrollBk == NULL)		m_pImgScrollBk		= SafeImageFrom(m_pngScrollBk);
	if(m_pImgVScrollTop == NULL)	m_pImgVScrollTop	= SafeImageFrom(m_pngVScrollTop);
	if(m_pImgVScrollBot == NULL)	m_pImgVScrollBot	= SafeImageFrom(m_pngVScrollBot);
	if(m_pImgVScrollThumb == NULL)	m_pImgVScrollThumb	= SafeImageFrom(m_pngVScrollThumb);
	if(m_pImgHScrollLeft == NULL)	m_pImgHScrollLeft	= SafeImageFrom(m_pngHScrollLeft);
	if(m_pImgHScrollRight == NULL)	m_pImgHScrollRight	= SafeImageFrom(m_pngHScrollRight);
	if(m_pImgHScrollThumb == NULL)	m_pImgHScrollThumb	= SafeImageFrom(m_pngHScrollThumb);

	if(m_pImgToolTip == NULL)		m_pImgToolTip		= SafeImageFrom(m_pngTooltipBk);

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

	//工具提示
	m_pngToolTipCtrl.m_pImgBk			= m_pImgToolTip;
}

void CPngTreeCtrl::DestoryImage()
{
	if(m_pImgParentBk		== NULL)	{	delete m_pImgParentBk		;		m_pImgParentBk		= NULL;		}					
	if(m_pImgBk				== NULL)	{	delete m_pImgBk				;		m_pImgBk			= NULL;		}	
	if(m_pImgButtonSel		== NULL)	{	delete m_pImgButtonSel		;		m_pImgButtonSel		= NULL;		}	
	if(m_pImgButtonUnSel	== NULL)	{	delete m_pImgButtonUnSel	;		m_pImgButtonUnSel	= NULL;		}					
	if(m_pImgChkSel			== NULL)	{	delete m_pImgChkSel			;		m_pImgChkSel		= NULL;		}	
	if(m_pImgChkUnSel		== NULL)	{	delete m_pImgChkUnSel		;		m_pImgChkUnSel		= NULL;		}	
	if(m_pImgDisable		== NULL)	{	delete m_pImgDisable		;		m_pImgDisable		= NULL;		}					
	if(m_pImgItemSel		== NULL)	{	delete m_pImgItemSel		;		m_pImgItemSel		= NULL;		}	
	if(m_pImgItemOver		== NULL)	{	delete m_pImgItemOver		;		m_pImgItemOver		= NULL;		}	

	if(m_pImgScrollBk		== NULL)	{	delete m_pImgScrollBk		;		m_pImgScrollBk		= NULL;		}		
	if(m_pImgVScrollTop		== NULL)	{	delete m_pImgVScrollTop		;		m_pImgVScrollTop	= NULL;		}	
	if(m_pImgVScrollBot		== NULL)	{	delete m_pImgVScrollBot		;		m_pImgVScrollBot	= NULL;		}		
	if(m_pImgVScrollThumb	== NULL)	{	delete m_pImgVScrollThumb	;		m_pImgVScrollThumb	= NULL;		}	
	if(m_pImgHScrollLeft	== NULL)	{	delete m_pImgHScrollLeft	;		m_pImgHScrollLeft	= NULL;		}		
	if(m_pImgHScrollRight	== NULL)	{	delete m_pImgHScrollRight	;		m_pImgHScrollRight	= NULL;		}
	if(m_pImgHScrollThumb	== NULL)	{	delete m_pImgHScrollThumb	;		m_pImgHScrollThumb	= NULL;		}
}

HBRUSH CPngTreeCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
	return NULL;
}

void CPngTreeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CTreeCtrl::OnPaint() for painting messages
	InitImage();
	dc.SetBkMode(TRANSPARENT);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);

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
	

	//CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	HTREEITEM	hItem		= GetFirstVisibleItem();
	int			vCnt		= GetVisibleCount();
	DWORD		listStyle	= GetStyle();
	BOOL		bHoverItem	= (listStyle & TVS_TRACKSELECT);

	IMAGEINFO	imageInfo;
	CRect 		itemLineRect= CRect(0, 0, 0, 0);
	CRect 		itemTextRect	= CRect(0, 0, 0, 0);
	CString		itemText	= _T("");
	CRect		textRc		= CRect(0, 0, 0, 0);
	while(hItem != NULL && vCnt>=0)
	{

		GetItemRect(hItem, &itemTextRect, TRUE );
		GetItemRect(hItem, &itemLineRect, FALSE );
		textRc = itemTextRect;

		UINT selFlag = /*TVIS_DROPHILITED |*/TVIS_SELECTED;

		CImageList *pImageListNormal	= GetImageList(TVSIL_NORMAL);
		CImageList *pImageListState 	= GetImageList(TVSIL_STATE);
		int 		imagNormalIndex 	= -1;
		int 		imagSelIndex		= -1;

		BOOL bHasBtn = FALSE;	//是否有BTn
		if(ItemHasChildren(hItem) && ((GetStyle() & (TVS_LINESATROOT|TVS_HASBUTTONS))))
			bHasBtn = TRUE;

		BOOL bHasChk = FALSE;	//是否有CHECKBOX
		if(GetStyle() & TVS_CHECKBOXES)
			bHasChk = TRUE;

		BOOL bFullRowSelect = FALSE;	//是否选中全行
		if(GetStyle() & TVS_FULLROWSELECT)
			bFullRowSelect = TRUE;

		CPoint	imagePt		= CPoint(itemTextRect.left, itemTextRect.top);
		int		imgWidth	= 16;
		int		imagHeight	= 16;

		if(pImageListNormal)
		{
			pImageListNormal->GetImageInfo(0, &imageInfo);
			imgWidth	= imageInfo.rcImage.right - imageInfo.rcImage.left;
			imagHeight	= imageInfo.rcImage.bottom - imageInfo.rcImage.top;
		}

		if ( !(GetItemState( hItem, selFlag ) & selFlag ))	//	未选中
		{
			CPoint mousePt;
			GetCursorPos(&mousePt);
			ScreenToClient(&mousePt);
			if(bHoverItem && rcClient.PtInRect(mousePt))	//当鼠标在客户区处于移动状态时才画经过的底图
			{
				if(bFullRowSelect && itemLineRect.PtInRect(mousePt))		//鼠标经过，是画全行还是单个Item
					MemPaintPng(memDC, itemLineRect, m_pImgItemOver);
				else if(itemTextRect.PtInRect(mousePt))
					MemPaintPng(memDC, itemTextRect, m_pImgItemOver);
			}

			GetItemImage(hItem, imagNormalIndex, imagSelIndex);
			if(pImageListNormal && imagNormalIndex >= 0)		//画常态图标
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);
				pImageListNormal->Draw(&memDC, imagNormalIndex, imagePt, ILD_TRANSPARENT);
				//pImageListNormal->DrawEx(&memDC, imagNormalIndex, imagePt, imageSize, CLR_NONE, CLR_NONE, ILD_TRANSPARENT);

			}
			else if(pImageListNormal)
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);
			}

			itemText = GetItemText(hItem);
			textRc.left += m_textOffsetX;
			MemDrawText(memDC, textRc, itemText, m_textAlignMode);

		}
		else
		{
			if(bFullRowSelect)
				MemPaintPng(memDC, itemLineRect, m_pImgItemSel);
			else
				MemPaintPng(memDC, itemTextRect, m_pImgItemSel);

			GetItemImage(hItem, imagNormalIndex, imagSelIndex);
			if(pImageListState && imagSelIndex >= 0)			//画状态图标
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);

				pImageListState->Draw(&memDC, imagSelIndex, imagePt, ILD_TRANSPARENT);
			}
			else if(pImageListNormal && imagNormalIndex >= 0)	//画常态图标
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);

				pImageListNormal->Draw(&memDC, imagNormalIndex, imagePt, ILD_TRANSPARENT);
			}
			else if(pImageListNormal)
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);
			}

			itemText = GetItemText(hItem);
			textRc.left += m_textOffsetX;
			MemDrawText(memDC, textRc, itemText, m_textAlignMode);
		}

		if(bHasChk)
		{
			if(GetCheck(hItem))
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - SafeGetImageHeight(m_pImgChkSel))/2);
				if(m_pImgChkSel) MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + SafeGetImageWidth(m_pImgChkSel), imagePt.y + SafeGetImageHeight(m_pImgChkSel)), m_pImgChkSel);
			}
			else
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - SafeGetImageHeight(m_pImgChkUnSel))/2);
				if(m_pImgChkUnSel) MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + SafeGetImageWidth(m_pImgChkUnSel), imagePt.y + SafeGetImageHeight(m_pImgChkUnSel)), m_pImgChkUnSel);
			}
		}

		if(bHasBtn)
		{
			if((GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED))
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - SafeGetImageHeight(m_pImgButtonSel))/2);
				if(m_pImgButtonSel) MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + SafeGetImageWidth(m_pImgButtonSel), imagePt.y + SafeGetImageHeight(m_pImgButtonSel)), m_pImgButtonSel);
			}
			else
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - SafeGetImageHeight(m_pImgButtonSel))/2);
				if(m_pImgButtonUnSel) MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + SafeGetImageWidth(m_pImgButtonUnSel), imagePt.y + SafeGetImageHeight(m_pImgButtonUnSel)), m_pImgButtonUnSel);
			}
		}


		hItem = GetNextVisibleItem( hItem );
		vCnt--;
	}


	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

  	m_HScroll.UpdateScrollBar();
  	m_VScroll.UpdateScrollBar();


	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

BOOL CPngTreeCtrl::OnEraseBkgnd(CDC* pDC){
	// TODO: Add your message handler code here and/or call default

	InitImage();
	return TRUE;
	return CTreeCtrl::OnEraseBkgnd(pDC);
}

void CPngTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(m_pngToolTipCtrl.GetSafeHwnd() == NULL && GetToolTips())
	{
		m_pngToolTipCtrl.SubclassWindow(GetToolTips()->GetSafeHwnd());
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CPngTreeCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CTreeCtrl::PreSubclassWindow();

	if(AfxGetThreadState()->m_pWndInit == NULL && m_bScrollInit == FALSE)
	{
 		m_HScroll.InitScrollBar(this, SB_HORZ);
 		m_VScroll.InitScrollBar(this, SB_VERT);
 		m_bScrollInit = TRUE;
	}
}

int CPngTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
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

BOOL CPngTreeCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	BOOL bRet = CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
	Invalidate();
	return bRet;
}

void CPngTreeCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CTreeCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	SetRedraw(TRUE);
}

void CPngTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	SetRedraw(TRUE);
}

void CPngTreeCtrl::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CTreeCtrl::OnNcPaint() for painting messages
	InitImage();

	if(m_VScroll.GetSafeHwnd() && m_HScroll.GetSafeHwnd())
	{
		CWindowDC dc(this);

		CRect rcWnd;
		GetWindowRect(&rcWnd);
		ScreenToClient(rcWnd);

		CRect rcClient;
		GetClientRect(&rcClient);

		dc.ExcludeClipRect(rcClient);
		MemPaintParentPng(this, dc, rcWnd, m_pImgParentBk, m_bFillOrTitleParentPng);
	}
}

void CPngTreeCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{

	CTreeCtrl::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	InitImage();
	if(IsWindow(m_HScroll.GetSafeHwnd()))
		m_HScroll.ShowWindow(bShow);
	if(IsWindow(m_VScroll.GetSafeHwnd()))
		m_VScroll.ShowWindow(bShow);
}

BOOL CPngTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message == 0x2100) //gun
		return TRUE;

	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CPngTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CTreeCtrl::OnLButtonDown(nFlags, point);
	SetRedraw(TRUE);
}

void CPngTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
	SetRedraw(TRUE);
}

void CPngTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CTreeCtrl::OnLButtonUp(nFlags, point);
	SetRedraw(TRUE);
}


void CPngTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTreeCtrl::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码

	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}
