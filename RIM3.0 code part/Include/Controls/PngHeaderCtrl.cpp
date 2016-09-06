//********************************************************************
//	PNGHEADERCTRL.CPP �ļ�ע��
//	�ļ���		: PNGHEADERCTRL.CPP
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	����		: ����
//	����ʱ��	: 2014/12/29 15:30
//	�ļ�����	: Ŀǰ��֧�����֣�����Ҫͼ�굽ʱ�ٽ�����չ������ϵԭ����
//*********************************************************************	
// E:\My Project\visual studio 2008\myskinDlg\myskinDlg\Controls\PngHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PngHeaderCtrl.h"


// CPngHeaderCtrl

IMPLEMENT_DYNAMIC(CPngHeaderCtrl, CHeaderCtrl)

CPngHeaderCtrl::CPngHeaderCtrl()
{
	m_divideSelIndex				= -1;

	m_separatorWidth				= 2;
	m_separatorColor				= RGB(255, 255, 255);


	m_textOffsetX					= 2;
	m_textAlignMode					= DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	m_textColor 					= RGB(255, 255, 255);

	m_pImgBk						= NULL;
	m_pImgHoverBk					= NULL;
	m_pImgDisable					= NULL;
}

CPngHeaderCtrl::~CPngHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CPngHeaderCtrl, CHeaderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CPngHeaderCtrl message handlers



HBRUSH CPngHeaderCtrl::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

HBRUSH CPngHeaderCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CHeaderCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CPngHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default


//  	CRect rcClient;
//  	GetClientRect(rcClient);
//  	pDC->FillSolidRect(rcClient, RGB(0,255,255));
//  	pDC->SetBkMode(TRANSPARENT);
	return TRUE;
	//return CHeaderCtrl::OnEraseBkgnd(pDC);
}

void CPngHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CHeaderCtrl::OnPaint() for painting messages
  	//DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0); 
  	//return;

	CRect rcClient;
	GetClientRect(rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);

	if(IsWindowEnabled())
	{
		//���ڱ����������˾ͻ���δ�����򲻻��Ʊ���
		MemPaintPng(memDC, CRect(0, 0, rcClient.Width(), rcClient.Height()), m_pImgBk);
	}
	else
	{
		::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
		MemPaintPng(memDC, CRect(0, 0, rcClient.Width(), rcClient.Height()), m_pImgDisable);
	}

	HDITEM hdItem;
	TCHAR  lpBuffer[MAX_PATH];

	hdItem.mask = HDI_TEXT | HDI_HEIGHT | HDI_STATE;
	hdItem.pszText = lpBuffer;
	hdItem.cchTextMax = MAX_PATH;

	CPen sparetorPen(PS_SOLID, m_separatorWidth, m_separatorColor);
	CPen *pOldPen = (CPen*)memDC.SelectObject(sparetorPen);

	{
		CPoint curPt;
		GetCursorPos(&curPt);
		ScreenToClient(&curPt);
		HDHITTESTINFO hdhittestinfo;
		hdhittestinfo.pt = curPt;
		HitTest(&hdhittestinfo);

		if(!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))	//���δ����ʱ��¼������Item
			m_divideSelIndex = hdhittestinfo.iItem;
	}

	int colCount; 
	colCount = GetItemCount();						//�õ��м����е�Ԫ 
	for(int i = 0; i<colCount;i ++) 
	{//������Ŀ�ı�
		CRect tRect;
		GetItemRect(i,&tRect);						//�õ�Item�ĳߴ�

		memset(lpBuffer, 0, MAX_PATH);
		GetItem(i, &hdItem);

		int R = 0,G = 255,B = 0;
		CRect nRect(tRect);							//�����ߴ絽�µ������� 

		CPoint pt[2] = {
			CPoint(nRect.left-1, 2),
			CPoint(nRect.left-1, rcClient.Height()-2)
		};
		memDC.Polyline(pt, 2);

		if(m_divideSelIndex == i)
		{	//��꾭��ʱ��ͼƬ
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)	//��갴��
				MemPaintPng(memDC, nRect, m_pImgPushBk);
			else
				MemPaintPng(memDC, nRect, m_pImgHoverBk);
		}

		tRect.left += m_textOffsetX;
		memDC.DrawText(lpBuffer, &tRect, m_textAlignMode);//�����ı�
	} 

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

LRESULT CPngHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 

// 	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
// 	RECT *prc = hdl.prc;			//�����ڵľ������򣬰���ͷ�ؼ���ռ����
// 	WINDOWPOS *pwpos = hdl.pwpos;	//ͷ�ؼ���λ��
// 
// 	//��ͷ�߶�Ϊԭ��1.5�������Ҫ��̬�޸ı�ͷ�߶ȵĻ�����1.5���һ��ȫ�ֱ��� 
// 	int nHeight = (int)(pwpos->cy * m_multiHeight);
// 	pwpos->cy = nHeight; 
// 	prc->top = nHeight; 
// 
// 	m_updateRc.left		= pwpos->x;
// 	m_updateRc.top		= pwpos->y;
// 	m_updateRc.right	= pwpos->cx + pwpos->x;
// 	m_updateRc.bottom	= pwpos->y + pwpos->cy;
// 	//m_updateRc.OffsetRect(-pwpos->x, pwpos->y);
	return lResult;
}

void CPngHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	__super::OnMouseMove(nFlags, point);
	////��zy 2015-01-05 ����  �϶�ͷ�ؼ�ʱҪʵʱˢ��

	if(MK_LBUTTON & nFlags)	//��갴��
	{
		if(m_divideSelIndex >= 0)
		{
			RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
		}
	}
	////��zy 2015-01-05 ����
}
