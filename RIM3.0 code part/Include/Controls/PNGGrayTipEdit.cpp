	//********************************************************************
//	PNGGRAYTIPEDIT.CPP 文件注释
//	文件名 : PNGGRAYTIPEDIT.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:14
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PNGGrayTipEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEditCtrl

CPNGGrayTipEdit::CPNGGrayTipEdit()
{
	m_pBkBrush					= NULL;
	m_bFillOrTitleParentPng		= TRUE;// 父窗口的贴图模式 默认填充

	m_textColor 				= RGB(255, 255, 255);
	m_GrayTipText				= _T("");
	m_bEditChange				= TRUE;
	m_curCaretPt				= CPoint(0, 0);

	m_pngParentBkPath			= _T("");		//父背景图片
	m_pngBkPath					= _T("");
	m_disablePngPath			= _T("");


	m_pImgParentBk				= NULL;
	m_pImgBk					= NULL;
	m_pImgDisable				= NULL;
}

CPNGGrayTipEdit::~CPNGGrayTipEdit()
{
	DeleteBrush();
	DestoryImage();
}

BEGIN_MESSAGE_MAP(CPNGGrayTipEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_CONTROL_REFLECT_EX(EN_CHANGE, &CPNGGrayTipEdit::OnEnChange)
	ON_CONTROL_REFLECT_EX(EN_SETFOCUS, &CPNGGrayTipEdit::OnEnSetfocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_MOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEditCtrl message handlers
void CPNGGrayTipEdit::DeleteBrush() 
{
	if(m_pBkBrush && m_pBkBrush->GetSafeHandle())
	{
		m_pBkBrush->DeleteObject();
		delete m_pBkBrush;
		m_pBkBrush = NULL;
	}
}

HBRUSH CPNGGrayTipEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkMode(TRANSPARENT); 
	pDC->SetTextColor(m_textColor);

	////【zy 2014-12-30 ↓】	//返回位图画刷  MAKE  Brush  在此为了适应UNICODE版本在ONPAINT中会绘制黑色背景的问题
	if(m_pBkBrush == NULL)
	{
		Bitmap* pResizeBitmap = NULL;
		InitImage();
		if(IsWindowEnabled())
		{//构造背景位图
			CRect rcClient;
			GetClientRect(rcClient);
			pResizeBitmap = CutBitmapFromSourRc(m_pImgBk, rcClient, rcClient);	
			if(pResizeBitmap == NULL)
			{//构造父背景位图
				CRect parentRect;
				GetParent()->GetClientRect(parentRect);

				CRect desRect;
				GetWindowRect(desRect);
				GetParent()->ScreenToClient(desRect);
				pResizeBitmap = CutBitmapFromSourRc(m_pImgParentBk, parentRect, desRect);
			}
		}
		else
		{//构造背景位图
			CRect rcClient;
			GetClientRect(rcClient);
			pResizeBitmap = CutBitmapFromSourRc(m_pImgDisable, rcClient, rcClient);	
		}

		if(pResizeBitmap)
		{
			HBITMAP hbitmap;
			pResizeBitmap->GetHBITMAP(Color(255,0,0,0), &hbitmap);
			m_pBkBrush = new CBrush(CBitmap::FromHandle(hbitmap));
			delete pResizeBitmap;

			return *m_pBkBrush;
		}
	}
	else
	{
		return *m_pBkBrush;
	}
	////【zy 2014-12-30 ↓】


	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	return NULL;
}

void CPNGGrayTipEdit::Redraw()
{
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE);
}

void CPNGGrayTipEdit::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CEdit::PreSubclassWindow();
}

void CPNGGrayTipEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CEdit::OnPaint() for painting messages
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

	CString wndText;
	GetWindowText(wndText);
	if (wndText.GetLength())
	{
		CWnd::DefWindowProc(WM_PAINT, WPARAM(memDC.m_hDC), 0);		//这里仅贴背景透明的文字
	}
	else
	{
		CRect grayRect;
		GetRect(grayRect);

		::SetTextColor(memDC.GetSafeHdc(), RGB(170, 170, 170));
		MemDrawText(memDC, grayRect, m_GrayTipText, DT_SINGLELINE | DT_LEFT);
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CPNGGrayTipEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	Invalidate(FALSE);
	CEdit::OnLButtonUp(nFlags, point);
}

BOOL CPNGGrayTipEdit::OnEnChange()
{
	m_bEditChange = true;
	Invalidate(FALSE);
	return FALSE;
}

BOOL CPNGGrayTipEdit::OnEnSetfocus()
{
	// TODO: Add your control notification handler code here

	Invalidate(FALSE);
	return FALSE;
}

void CPNGGrayTipEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnLButtonDown(nFlags, point);

	//CDC* pDC = GetDC();

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

	//MemPaintParentPng(this, memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);

	//if(IsWindowEnabled())
	//{
	//	//对于背景，设置了就画，未设置则不绘制背景
	//	MemPaintPng(memDC, rcClient, m_pImgBk);
	//}
	//else
	//{
	//	::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
	//	MemPaintPng(memDC, rcClient, m_pImgDisable);
	//}

	//CWnd::DefWindowProc(WM_PAINT, WPARAM(memDC.m_hDC), 0);//

	//pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	//ReleaseDC(pDC);

	//memDC.SelectObject(pOldBmp);
	//memDC.SelectObject(pOldFont);
	//memDC.DeleteDC();
	//memBmp.DeleteObject();
}

void CPNGGrayTipEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnMouseMove(nFlags, point);
	if(::GetFocus() == m_hWnd)
		Invalidate(FALSE);
}

BOOL CPNGGrayTipEdit::OnEraseBkgnd(CDC* pDC)
{
	InitImage();
	pDC->SetBkMode(TRANSPARENT);
	return TRUE;
}


void CPNGGrayTipEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate(FALSE);
}

void CPNGGrayTipEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnLButtonDblClk(nFlags, point);
	Invalidate(FALSE);
}

void CPNGGrayTipEdit::InitImage()
{
	if(m_pImgParentBk == NULL)	m_pImgParentBk	= SafeImageFrom(m_pngParentBkPath);
	if(m_pImgBk == NULL)		m_pImgBk		= SafeImageFrom(m_pngBkPath);
	if(m_pImgDisable == NULL)	m_pImgDisable	= SafeImageFrom(m_disablePngPath);
}

void CPNGGrayTipEdit::DestoryImage()
{
	if(m_pImgParentBk		== NULL)	{	delete m_pImgParentBk	;		m_pImgParentBk	= NULL;		}					
	if(m_pImgBk				== NULL)	{	delete m_pImgBk			;		m_pImgBk		= NULL;		}	
	if(m_pImgDisable		== NULL)	{	delete m_pImgDisable	;		m_pImgDisable	= NULL;		}	
}

LRESULT CPNGGrayTipEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	LRESULT ret = NULL;
	ret = __super::WindowProc(message, wParam, lParam);

 	if(message == EM_SETSEL)
 		Invalidate(FALSE);

	return ret;
}

void CPNGGrayTipEdit::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call __super::OnNcPaint() for painting messages
}

void CPNGGrayTipEdit::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	DeleteBrush();
}

void CPNGGrayTipEdit::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	// TODO: Add your message handler code here
	DeleteBrush();
}
