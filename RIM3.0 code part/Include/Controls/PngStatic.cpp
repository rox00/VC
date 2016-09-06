//********************************************************************
//	PNGSTATIC.CPP 文件注释
//	文件名 : PNGSTATIC.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: \
//*********************************************************************	

#include "stdafx.h"
#include "PngStatic.h"


// CTransparentStatic

IMPLEMENT_DYNAMIC(CPngStatic, CStatic)

CPngStatic::CPngStatic()
{
	m_bFillOrTitleParentPng		= TRUE;

	m_textColor					= RGB(255, 255, 255);
	m_textOffsetX				= 0;
	m_textAlignMode				= DT_SINGLELINE | DT_VCENTER | DT_LEFT;

	m_pngParentBkPath			= _T("");
	m_pImgParentBk				= NULL;
}

CPngStatic::~CPngStatic()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTransparentStatic message handlers

void CPngStatic::InitImage()
{
	if(m_pImgParentBk == NULL)		m_pImgParentBk		= SafeImageFrom(m_pngParentBkPath);
}

void CPngStatic::DestoryImage()
{
	if(m_pImgParentBk		== NULL)	{	delete m_pImgParentBk		;		m_pImgParentBk		= NULL;		}					
}

void CPngStatic::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CStatic::PreSubclassWindow();
}

HBRUSH CPngStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{

	pDC->SetBkMode(TRANSPARENT); 
	pDC->SetTextColor(m_textColor);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL CPngStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	InitImage();

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

	if(IsWindowEnabled())
	{
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
		{	//对于背景，设置了就画，未设置则不绘制背景
			MemPaintParentPng(this, memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);

		}
	}
	else
	{
// 		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
// 		MemPaintPng(memDC, rcClient, m_pImgDisable);
// 		pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	}

	CString strText;
	GetWindowText(strText);
	CRect textRc = rcClient;
	textRc.left += m_textOffsetX;
	MemDrawText(memDC, textRc, strText, m_textAlignMode);

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();

	return TRUE;
}

LRESULT CPngStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	LRESULT ret =  CStatic::WindowProc(message, wParam, lParam);

	if(message == WM_SETTEXT)
		Invalidate();
	return ret;
}

void CPngStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call __super::OnPaint() for painting messages

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

	//if(IsWindowEnabled())
	//{
	//	//对于背景，设置了就画，未设置则不绘制背景
	//		MemPaintPng(memDC, rcClient, m_pImgBk);
	//}
	//else
	//{
	//	::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
	//	MemPaintPng(memDC, rcClient, m_pImgDisable);
	//}

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
