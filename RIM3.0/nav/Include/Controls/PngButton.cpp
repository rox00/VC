//********************************************************************
//	PNGBUTTON.CPP 文件注释
//	文件名 : PNGBUTTON.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:14
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PngButton.h"


// CPngButton

IMPLEMENT_DYNAMIC(CPngButton, CButton)

CPngButton::CPngButton()
{
	m_bFillOrTitleParentPng	= TRUE;

	m_bLButtonDown			= false;
	m_bMouseMove			= false;

	m_textColor				= RGB(255, 255, 255);
	m_textAlignMode			= DT_SINGLELINE | DT_VCENTER | DT_CENTER;


	m_pngParentBkPath		= _T("");		//父背景图片
	m_pngBkPath				= _T("");			//背景图片

	m_normalPngPath			= _T("");		//初始化显示图片		有效性：非强制有效
	m_pushPngPath			= _T("");		//按下时显示图片		有效性：非强制有效	
	m_overPngPath			= _T("");		//鼠标经过时显示图片	有效性：非强制有效
	m_disablePngPath		= _T("");		//禁用时显示图片		有效性：非强制有效

	m_pImgParentBk			= NULL;
	m_pImgBk				= NULL;

	m_pImgNormal			= NULL;
	m_pImgPush				= NULL;
	m_pImgOver				= NULL;
	m_pImgDisable			= NULL;

}

CPngButton::~CPngButton()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngButton, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CPngButton::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CPngButton message handlers

void CPngButton::InitImage()
{
	if(m_pImgParentBk == NULL)			m_pImgParentBk			= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)				m_pImgBk				= SafeImageFrom(m_pngBkPath);

	if(m_pImgNormal == NULL)			m_pImgNormal			= SafeImageFrom(m_normalPngPath);
	if(m_pImgPush == NULL)				m_pImgPush				= SafeImageFrom(m_pushPngPath);
	if(m_pImgOver == NULL)				m_pImgOver				= SafeImageFrom(m_overPngPath);
	if(m_pImgDisable == NULL)			m_pImgDisable			= SafeImageFrom(m_disablePngPath);
}

void CPngButton::DestoryImage()
{
	if(m_pImgParentBk		== NULL)	{	delete m_pImgParentBk	;		m_pImgParentBk	= NULL;		}					
	if(m_pImgBk				== NULL)	{	delete m_pImgBk			;		m_pImgBk		= NULL;		}	

	if(m_pImgNormal			== NULL)	{	delete m_pImgNormal		;		m_pImgNormal	= NULL;		}
	if(m_pImgPush			== NULL)	{	delete m_pImgPush		;		m_pImgPush		= NULL;		}
	if(m_pImgOver			== NULL)	{	delete m_pImgOver		;		m_pImgOver		= NULL;		}
	if(m_pImgDisable		== NULL)	{	delete m_pImgDisable	;		m_pImgDisable	= NULL;		}

}

HBRUSH CPngButton::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	pDC->SetBkMode(TRANSPARENT); 
	pDC->SetTextColor(m_textColor);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CPngButton::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CPngButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(m_hWnd, 0, BS_OWNERDRAW, 0);

	CButton::PreSubclassWindow();
}

void CPngButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Add your code to draw the specified item
	InitImage();

	// This code only works with buttons.
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	CRect  rcClient;
	GetClientRect(&rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(CDC::FromHandle(lpDrawItemStruct->hDC));
	memBmp.CreateCompatibleBitmap(CDC::FromHandle(lpDrawItemStruct->hDC), rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);

	

	{	//对于背景，设置了就画，未设置则不绘制背景

			MemPaintParentPng(this, memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);
			MemPaintPng(memDC, rcClient, m_pImgBk);
	}


	{	//向DC上绘制图像
		if((lpDrawItemStruct->itemState & ODS_DISABLED) && m_disablePngPath.GetLength())
		{
			::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
			MemPaintPng(memDC, rcClient, m_pImgDisable);
		}
		else if((lpDrawItemStruct->itemState & ODS_SELECTED) && m_pushPngPath.GetLength())
		{
			MemPaintPng(memDC, rcClient, m_pImgPush);
		}
		else
		{
			MemPaintPng(memDC, rcClient, m_pImgNormal);
		}
	}

	CString strText;
	GetWindowText(strText);
 	MemDrawText(memDC, rcClient, strText, m_textAlignMode);
 
 	(CDC::FromHandle(lpDrawItemStruct->hDC))->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CPngButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

 	if ((m_bLButtonDown == false) && m_bMouseMove == false)
 	{
		///////////////绘制鼠标移动状态
 		
		DrawButton(m_pImgOver);

		//要求接收鼠标离开消息
		TRACKMOUSEEVENT		csTME;
		csTME.cbSize	= sizeof(csTME);
		csTME.dwFlags	= TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&csTME);

		m_bMouseMove = true;
 	}

	CButton::OnMouseMove(nFlags, point);
}

void CPngButton::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	if ((m_bLButtonDown == false) && m_bMouseMove == true)
	{
		///////////////绘制鼠标常态

		DrawButton(m_pImgNormal);
		m_bMouseMove = true;
	}

	m_bMouseMove = false;
	CButton::OnMouseLeave();
}

void CPngButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButtonDown = true;

	CButton::OnLButtonDown(nFlags, point);


}

void CPngButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = false;
	CButton::OnLButtonUp(nFlags, point);


	///////////////绘制鼠标移动状态
	DrawButton(m_pImgOver);
}

void CPngButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButtonDown = true;

	DrawButton(m_pImgPush);

	CButton::OnLButtonDblClk(nFlags, point);
}

void CPngButton::DrawButton(Image *pImage)
{
	InitImage();

	CDC *pDC = GetDC();

	CRect  rcClient;
	GetClientRect(&rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);
	memDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pDC, 0, 0, SRCCOPY);

	{	//对于背景，设置了就画，未设置则不绘制背景
			MemPaintParentPng(this, memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);
			MemPaintPng(memDC, rcClient, m_pImgBk);
	}

	MemPaintPng(memDC, rcClient, pImage);
	CString strText;
	GetWindowText(strText);
	MemDrawText(memDC, rcClient, strText, m_textAlignMode);

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	ReleaseDC(pDC);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

BOOL CPngButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	InitImage();

	return TRUE;
}

LRESULT CPngButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if(message == BM_SETCHECK || message == WM_SETTEXT)
	{
		SetRedraw(FALSE);
		LRESULT ret = __super::WindowProc(message, wParam, lParam);
		SetRedraw(TRUE);

		return ret;
	}

	return __super::WindowProc(message, wParam, lParam);
}
