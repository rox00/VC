//********************************************************************
//	PNGTOOLTIPCTRL.CPP �ļ�ע��
//	�ļ��� : PNGTOOLTIPCTRL.CPP
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	���� : ����
//	����ʱ��: 2014/12/3 11:12
//	�ļ�����: 
//*********************************************************************	

#include "stdafx.h"
#include "PngToolTipCtrl.h"


// CPngToolTipCtrl

IMPLEMENT_DYNAMIC(CPngToolTipCtrl, CToolTipCtrl)

CPngToolTipCtrl::CPngToolTipCtrl()
{
	m_textAlignMode		= DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_textColor 		= RGB(255, 255, 255);

	m_pImgBk			= NULL;
}

CPngToolTipCtrl::~CPngToolTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CPngToolTipCtrl, CToolTipCtrl)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(TTM_ADJUSTRECT, &CPngToolTipCtrl::OnAdjustRect)
END_MESSAGE_MAP()



// CPngToolTipCtrl message handlers

void CPngToolTipCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CToolTipCtrl::OnPaint() for painting messages

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

	////��zy 2014-10-17 ����  ������
	if(IsWindowEnabled())
	{
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
		{	//���ڱ����������˾ͻ���δ�����򲻻��Ʊ���
				MemPaintPng(memDC, rcClient, m_pImgBk);
		}
	}


	CString strText;
	GetWindowText(strText);
	CRect textRc = rcClient;
	//textRc.left += m_textOffsetX;
	MemDrawText(memDC, textRc, strText, m_textAlignMode);
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

HBRUSH CPngToolTipCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
	return NULL;
}

BOOL CPngToolTipCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	return CToolTipCtrl::OnEraseBkgnd(pDC);
}

void CPngToolTipCtrl::PreSubclassWindow()
{
	// TODO:  �ڴ����ר�ô����/����û���

	ModifyStyle(WS_BORDER, 0);
	CToolTipCtrl::PreSubclassWindow();
}

LRESULT CPngToolTipCtrl::OnAdjustRect(WPARAM wParam, LPARAM lParam)
{
 	if (wParam == 1)
 	{
 		CRect rect = (*(LPRECT)lParam);		//xp  ����
 		SetRedraw(FALSE);
 		::SetWindowPos(m_hWnd, HWND_TOPMOST, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
 		SetRedraw(TRUE);
 		return 1;
 	}

	return 0;
}
