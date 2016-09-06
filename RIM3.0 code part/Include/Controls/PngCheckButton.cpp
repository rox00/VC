//********************************************************************
//	PNGCHECKBUTTON.CPP 文件注释
//	文件名 : PNGCHECKBUTTON.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:13
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PngCheckButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CCheckBoxEx

IMPLEMENT_DYNAMIC(CPngCheckButton, CButton)

CPngCheckButton::CPngCheckButton()
{
	m_bFillOrTitleParentPng			= TRUE;// 父窗口的贴图模式 默认填充

	m_pngParentBkPath				= _T("");
	m_pngBkPath						= _T("");
	m_pngChkUnSel					= _T("");
	m_pngChkSel						= _T("");
	m_disablePngPath				= _T("");

	m_textOffsetX					= 2;
	m_textAlignMode					= DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_textColor 					= RGB(255, 255, 255);

	m_bLButtonDown					= FALSE;
	m_pImgParentBk					= NULL;
	m_pImgBk						= NULL;
	m_pImgDisable					= NULL;
	m_pImgChkSel					= NULL;
	m_pImgChkUnSel					= NULL;
}

CPngCheckButton::~CPngCheckButton()
{
}


BEGIN_MESSAGE_MAP(CPngCheckButton, CButton)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CPngCheckButton::OnNMCustomdraw)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CCheckBoxEx message handlers

void CPngCheckButton::InitImage()
{
	if(m_pImgParentBk == NULL)		m_pImgParentBk		= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)			m_pImgBk			= SafeImageFrom(m_pngBkPath);
	if(m_pImgDisable == NULL)		m_pImgDisable		= SafeImageFrom(m_disablePngPath);
	if(m_pImgChkSel == NULL)		m_pImgChkSel		= SafeImageFrom(m_pngChkSel);
	if(m_pImgChkUnSel == NULL)		m_pImgChkUnSel		= SafeImageFrom(m_pngChkUnSel);
}

void CPngCheckButton::DestoryImage()
{
	if(m_pImgParentBk			== NULL)	{	delete m_pImgParentBk	;		m_pImgParentBk	= NULL;		}					
	if(m_pImgBk					== NULL)	{	delete m_pImgBk			;		m_pImgBk		= NULL;		}	
	if(m_pImgDisable			== NULL)	{	delete m_pImgDisable	;		m_pImgDisable	= NULL;		}
	if(m_pImgChkSel				== NULL)	{	delete m_pImgChkSel		;		m_pImgChkSel	= NULL;		}
	if(m_pImgChkUnSel			== NULL)	{	delete m_pImgChkUnSel	;		m_pImgChkUnSel	= NULL;		}
}

void CPngCheckButton::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	DWORD drawState = pNMCD->dwDrawStage;

	switch(drawState)
	{
	case CDDS_PREERASE:
		*pResult = CDRF_SKIPDEFAULT;

	case  CDDS_PREPAINT:
		*pResult = CDRF_SKIPDEFAULT;
	case  CDDS_POSTPAINT:
		*pResult = CDRF_SKIPDEFAULT;

		break;
	}
}

BOOL CPngCheckButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	InitImage();
	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

HBRUSH CPngCheckButton::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here


	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CPngCheckButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	//ModifyStyle(m_hWnd, 0, BS_OWNERDRAW, 0);
	CButton::PreSubclassWindow();
}

void CPngCheckButton::OnPaint()
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
	////【zy 2014-10-17 ↑】

	////【zy 2014-10-17 ↓】		//画选中状态及文字

	if(IsWindowEnabled())
	{
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
	}
	else
	{
		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
	}

	if(GetCheck())
	{
		CRect paintRc = rcClient;
		MemPaintPng(memDC, paintRc, m_pImgChkSel);
	}
	else
	{
		CRect paintRc = rcClient;
		MemPaintPng(memDC, paintRc, m_pImgChkUnSel);
	}

	CString strText;
	GetWindowText(strText);
	CRect textRc = rcClient;
	textRc.left += m_textOffsetX;
	MemDrawText(memDC, textRc, strText, m_textAlignMode);
	////【zy 2014-10-17 ↑】

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CPngCheckButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButtonDown = FALSE;

	SetRedraw(FALSE);
	CButton::OnLButtonUp(nFlags, point);
	SetRedraw(TRUE);


	CDC* pDC = GetDC();

	////【zy 2014-10-17 ↓】  画背景
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
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
	////【zy 2014-10-17 ↑】

	////【zy 2014-10-17 ↓】		//画选中状态及文字
	if(IsWindowEnabled())
	{
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
	}
	else
	{
		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
	}

	if(GetCheck())
	{
		CRect paintRc = rcClient;
		MemPaintPng(memDC, paintRc, m_pImgChkSel);
	}
	else
	{
		CRect paintRc = rcClient;
		MemPaintPng(memDC, paintRc, m_pImgChkUnSel);
	}

	CString strText;
	GetWindowText(strText);
	CRect textRc = rcClient;
	textRc.left += m_textOffsetX;
	MemDrawText(memDC, textRc, strText, m_textAlignMode);
	////【zy 2014-10-17 ↑】

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	ReleaseDC(pDC);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CPngCheckButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButtonDown = TRUE;

	SetRedraw(FALSE);
	CButton::OnLButtonDown(nFlags, point);
	SetRedraw(TRUE);
}

void CPngCheckButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CButton::OnLButtonDblClk(nFlags, point);
	SetRedraw(TRUE);
}

void CPngCheckButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnMouseMove(nFlags, point);
}



LRESULT CPngCheckButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
