//********************************************************************
//	PNGRADIOBUTTON.CPP 文件注释
//	文件名 : PNGRADIOBUTTON.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PngRadioButton.h"


// CPngRadioButton

IMPLEMENT_DYNAMIC(CPngRadioButton, CButton)

CPngRadioButton::CPngRadioButton()
{
	m_bFillOrTitleParentPng			= TRUE;

	m_pngParentBkPath				= _T("");		//父背景图片
	m_pngBkPath						= _T("");
	m_pngRadioUnSel					= _T("");
	m_pngRadioSel					= _T("");
	m_disablePngPath				= _T("");

	m_textOffsetX					= 2;
	m_textAlignMode					= DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_textColor 					= RGB(255, 255, 255);

	m_bLButtonDown					= FALSE;
	m_pImgParentBk					= NULL;
	m_pImgBk						= NULL;
	m_pImgDisable					= NULL;
	m_pImgRadioSel					= NULL;
	m_pImgRadioUnSel				= NULL;
}

CPngRadioButton::~CPngRadioButton()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngRadioButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(BM_SETCHECK, &CPngRadioButton::OnSetCheck)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CPngRadioButton message handlers
void CPngRadioButton::InitImage()
{
	if(m_pImgParentBk == NULL)		m_pImgParentBk		= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)			m_pImgBk			= SafeImageFrom(m_pngBkPath);
	if(m_pImgDisable == NULL)		m_pImgDisable		= SafeImageFrom(m_disablePngPath);
	if(m_pImgRadioSel == NULL)		m_pImgRadioSel		= SafeImageFrom(m_pngRadioSel);
	if(m_pImgRadioUnSel == NULL)	m_pImgRadioUnSel	= SafeImageFrom(m_pngRadioUnSel);
}

void CPngRadioButton::DestoryImage()
{
	if(m_pImgParentBk	== NULL)	{	delete m_pImgParentBk	;		m_pImgParentBk	= NULL;		}					
	if(m_pImgBk			== NULL)	{	delete m_pImgBk			;		m_pImgBk		= NULL;		}	
	if(m_pImgDisable	== NULL)	{	delete m_pImgDisable	;		m_pImgDisable	= NULL;		}	
	if(m_pImgRadioSel	== NULL)	{	delete m_pImgRadioSel	;		m_pImgRadioSel	= NULL;		}		
	if(m_pImgRadioUnSel	== NULL)	{	delete m_pImgRadioUnSel	;		m_pImgRadioUnSel= NULL;		}	
}

BOOL CPngRadioButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	InitImage();

	//CRect rcClient;
	//GetClientRect(&rcClient);

	//CDC			memDC;
	//CBitmap		memBmp;
	//memDC.CreateCompatibleDC(pDC);
	//memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	//CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	//memDC.SetBkMode(TRANSPARENT);
	//CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	//::SetTextColor(memDC.GetSafeHdc(), m_textColor);

	//if(IsWindowEnabled())
	//{
	//	::SetTextColor(memDC.GetSafeHdc(), m_textColor);
	//	{	//对于背景，设置了就画，未设置则不绘制背景
	//			MemPaintParentPng(memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);
	//			MemPaintPng(memDC, rcClient, m_pImgBk);

	//		pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	//	}
	//}
	//else
	//{
	//	::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
	//	MemPaintPng(memDC, rcClient, m_pImgDisable);
	//	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	//}

	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

void CPngRadioButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CButton::OnPaint() for painting messages

	InitImage();

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
 
 
 	if(GetCheck())
 	{
 		CRect paintRc = rcClient;
 		MemPaintPng(memDC, paintRc, m_pImgRadioSel);
 	}
 	else
 	{
 		CRect paintRc = rcClient;
 		MemPaintPng(memDC, paintRc, m_pImgRadioUnSel);
 	}
 
 	CString strText;
 	GetWindowText(strText);
	CRect textRc = rcClient;
	textRc.left += m_textOffsetX;
	MemDrawText(memDC, textRc, strText, m_textAlignMode);
 
 	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

HBRUSH CPngRadioButton::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CPngRadioButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetRedraw(FALSE);
	CButton::OnLButtonDown(nFlags, point);
	SetRedraw(TRUE);
}

void CPngRadioButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CButton::OnLButtonUp(nFlags, point);	//鼠标弹起时会发重绘消息 而Checkbox不会
	SetRedraw(TRUE);
}

void CPngRadioButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnMouseMove(nFlags, point);
}

void CPngRadioButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	//ModifyStyle(m_hWnd, 0, BS_OWNERDRAW | BS_AUTORADIOBUTTON, 0);

	CButton::PreSubclassWindow();
}

void CPngRadioButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CButton::OnLButtonDblClk(nFlags, point);
	SetRedraw(TRUE);
}


LRESULT CPngRadioButton::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
	SetRedraw(FALSE);
	CWnd::DefWindowProc(BM_SETCHECK, wParam, lParam);
	SetRedraw(TRUE);
	Invalidate();

	return TRUE;
}

LRESULT CPngRadioButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
