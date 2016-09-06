//********************************************************************
//	PNGCHECKBOX.CPP 文件注释
//	文件名 : PNGCHECKBOX.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:13
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PngCheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CCheckBoxEx

IMPLEMENT_DYNAMIC(CPngCheckBox, CButton)

CPngCheckBox::CPngCheckBox()
{
	m_bFillOrTitleParentPng		= TRUE;// 父窗口的贴图模式 默认填充

	m_pngParentBkPath			= _T("");		//父背景图片
	m_pngBkPath					= _T("");
	m_pngChkUnSel				= _T("");
	m_pngChkSel					= _T("");
	m_disablePngPath			= _T("");

	m_textOffsetX				= 2;
	m_textAlignMode				= DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_textColor 				= RGB(255, 255, 255);

	m_bLButtonDown				= FALSE;
	m_pImgParentBk				= NULL;
	m_pImgBk					= NULL;
	m_pImgDisable				= NULL;
	m_pImgChkSel				= NULL;
	m_pImgChkUnSel				= NULL;
}

CPngCheckBox::~CPngCheckBox()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngCheckBox, CButton)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CPngCheckBox::OnNMCustomdraw)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CCheckBoxEx message handlers

void CPngCheckBox::InitImage()
{
	if(m_pImgParentBk == NULL)		m_pImgParentBk		= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)			m_pImgBk			= SafeImageFrom(m_pngBkPath);
	if(m_pImgDisable == NULL)		m_pImgDisable		= SafeImageFrom(m_disablePngPath);
	if(m_pImgChkSel == NULL)		m_pImgChkSel		= SafeImageFrom(m_pngChkSel);
	if(m_pImgChkUnSel == NULL)		m_pImgChkUnSel		= SafeImageFrom(m_pngChkUnSel);
}

void CPngCheckBox::DestoryImage()
{
	if(m_pImgParentBk		== NULL)	{	delete m_pImgParentBk	;		m_pImgParentBk	= NULL;		}					
	if(m_pImgBk				== NULL)	{	delete m_pImgBk			;		m_pImgBk		= NULL;		}	
	if(m_pImgDisable		== NULL)	{	delete m_pImgDisable	;		m_pImgDisable	= NULL;		}
	if(m_pImgChkSel			== NULL)	{	delete m_pImgChkSel		;		m_pImgChkSel	= NULL;		}
	if(m_pImgChkUnSel		== NULL)	{	delete m_pImgChkUnSel	;		m_pImgChkUnSel	= NULL;		}

}

void CPngCheckBox::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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

BOOL CPngCheckBox::OnEraseBkgnd(CDC* pDC)
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

HBRUSH CPngCheckBox::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CPngCheckBox::PreSubclassWindow()
{

	// TODO: Add your specialized code here and/or call the base class
	//ModifyStyle(m_hWnd, 0, BS_OWNERDRAW, 0);

	CButton::PreSubclassWindow();

}

void CPngCheckBox::OnPaint()
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

	//memDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dc, 0, 0, SRCCOPY);

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


// 	if(IsWindowEnabled())
// 	{
// 		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
// 	}
// 	else
// 	{
// 		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
// 	}


	CRect textRc = rcClient;
	if(GetCheck())
	{
		MemPaintPng(memDC, CRect(0, 0, SafeGetImageWidth(m_pImgChkSel),  SafeGetImageHeight(m_pImgChkSel)), m_pImgChkSel);
		textRc.left += SafeGetImageWidth(m_pImgChkSel) + m_textOffsetX;
	}
	else
	{
		if(m_pImgChkUnSel) MemPaintPng(memDC, CRect(0, 0, SafeGetImageWidth(m_pImgChkUnSel), SafeGetImageHeight(m_pImgChkUnSel)), m_pImgChkUnSel);
		textRc.left += SafeGetImageWidth(m_pImgChkUnSel) + m_textOffsetX;
	}

	CString strText;
	GetWindowText(strText);
	MemDrawText(memDC, textRc, strText, m_textAlignMode);

	

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CPngCheckBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButtonDown = FALSE;

	SetRedraw(FALSE);
	CButton::OnLButtonUp(nFlags, point);
	SetRedraw(TRUE);

	//只画选择框部分
	CDC* pDC = GetDC();

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

	if(m_pImgChkSel && m_pImgChkUnSel)
	{
		if(GetCheck())
		{
			CRect paintRc = CRect(0, 0, SafeGetImageWidth(m_pImgChkSel),  SafeGetImageHeight(m_pImgChkSel));
			MemPaintPng(memDC, paintRc, m_pImgChkSel);

			pDC->StretchBlt(0, 0, SafeGetImageWidth(m_pImgChkSel), SafeGetImageHeight(m_pImgChkSel), &memDC, 0, 0, paintRc.Width(), paintRc.Height(), SRCCOPY);
		}
		else
		{
			CRect paintRc = CRect(0, 0, SafeGetImageWidth(m_pImgChkUnSel),  SafeGetImageHeight(m_pImgChkUnSel));
			MemPaintPng(memDC, paintRc, m_pImgChkUnSel);

			pDC->StretchBlt(0, 0, SafeGetImageWidth(m_pImgChkUnSel), SafeGetImageHeight(m_pImgChkUnSel), &memDC, 0, 0, paintRc.Width(), paintRc.Height(), SRCCOPY);
		}
	}

	ReleaseDC(pDC);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CPngCheckBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButtonDown = TRUE;

	SetRedraw(FALSE);
	CButton::OnLButtonDown(nFlags, point);
	SetRedraw(TRUE);
	Invalidate(FALSE);
}

void CPngCheckBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetRedraw(FALSE);
	CButton::OnLButtonDblClk(nFlags, point);
	SetRedraw(TRUE);
}

void CPngCheckBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnMouseMove(nFlags, point);
}
void CPngCheckBox::OnSetFocus(CWnd* pOldWnd)
{
	CButton::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	Invalidate();
}

LRESULT CPngCheckBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if(message == BM_SETCHECK || message == WM_SETTEXT)
	{
		LRESULT ret = __super::WindowProc(message, wParam, lParam);

		Invalidate();
		return ret;
	}
	return __super::WindowProc(message, wParam, lParam);
}
