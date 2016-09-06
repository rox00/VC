//********************************************************************
// RIMTREECTRL.CPP �ļ�ע��
// �ļ��� ��: RIMTREECTRL.CPP
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\NAV\RIM 3.0/
// ���� ����: ����
// ����ʱ�� : 2015/4/30 10:11
// �ļ����� : 
//*********************************************************************
// RIMTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "RIMTreeCtrl.h"


// CRIMTreeCtrl

IMPLEMENT_DYNAMIC(CRIMTreeCtrl, CPngTreeCtrl)

CRIMTreeCtrl::CRIMTreeCtrl()
{
	m_hHoverItem = NULL;
}

CRIMTreeCtrl::~CRIMTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CRIMTreeCtrl, CPngTreeCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CRIMTreeCtrl message handlers



void CRIMTreeCtrl::OnPaint()
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
	memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SelectObject(GetFont());
	::SetTextColor(memDC.GetSafeHdc(), m_textColor);

	////��zy 2014-10-17 ����  ������
	if(IsWindowEnabled())
	{
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
		//���ڱ����������˾ͻ���δ�����򲻻��Ʊ���
		if(SafeGetImageWidth(m_pImgParentBk))
			MemPaintParentPng(this, memDC, rcClient, m_pImgParentBk, m_bFillOrTitleParentPng);

		if(SafeGetImageWidth(m_pImgBk))
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
	CRect 		itemTextRect= CRect(0, 0, 0, 0);
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

		BOOL bHasBtn = FALSE;	//�Ƿ���BTn
		if(ItemHasChildren(hItem) && ((GetStyle() & (TVS_LINESATROOT|TVS_HASBUTTONS))))
			bHasBtn = TRUE;

		BOOL bHasChk = FALSE;	//�Ƿ���CHECKBOX
		if(GetStyle() & TVS_CHECKBOXES)
			bHasChk = TRUE;

		BOOL bFullRowSelect = FALSE;	//�Ƿ�ѡ��ȫ��
		if(GetStyle() & TVS_FULLROWSELECT)
			bFullRowSelect = TRUE;

		CPoint	imagePt		= CPoint(itemTextRect.left, itemTextRect.top);
		int		imgWidth	= 16;
		int		imagHeight	= 16;
		int		normalCnt	= 0;
		int		stateCnt	= 0;
		if(pImageListNormal)
		{
			pImageListNormal->GetImageInfo(0, &imageInfo);
			normalCnt	= pImageListNormal->GetImageCount();
			imgWidth	= imageInfo.rcImage.right - imageInfo.rcImage.left;
			imagHeight	= imageInfo.rcImage.bottom - imageInfo.rcImage.top;
		}

		if(pImageListState)
			stateCnt	= pImageListState->GetImageCount();

		if ( !(GetItemState( hItem, selFlag ) & selFlag ))	//	δѡ��
		{
			CPoint mousePt;
			GetCursorPos(&mousePt);
			ScreenToClient(&mousePt);
			if(bHoverItem && rcClient.PtInRect(mousePt))	//������ڿͻ��������ƶ�״̬ʱ�Ż������ĵ�ͼ
			{
				if(bFullRowSelect && itemLineRect.PtInRect(mousePt))		//��꾭�����ǻ�ȫ�л��ǵ���Item
					MemPaintPng(memDC, itemLineRect, m_pImgItemOver);
				else if(itemTextRect.PtInRect(mousePt))
					MemPaintPng(memDC, itemTextRect, m_pImgItemOver);
			}

			GetItemImage(hItem, imagNormalIndex, imagSelIndex);
			if(pImageListNormal && imagNormalIndex >= 0 && imagNormalIndex < normalCnt)		//����̬ͼ��
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);
				pImageListNormal->Draw(&memDC, imagNormalIndex, imagePt, ILD_TRANSPARENT);
				//pImageListNormal->DrawEx(&memDC, imagNormalIndex, imagePt, imageSize, CLR_NONE, CLR_NONE, ILD_TRANSPARENT);

			}
			else if(pImageListNormal)
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);
				textRc.left = imagePt.x;	//û��ͼ��Ͱ��ı��ַ��������ƶ�
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
			if(pImageListState && imagSelIndex >= 0 && imagSelIndex < stateCnt)			//��״̬ͼ��
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);

				pImageListState->Draw(&memDC, imagSelIndex, imagePt, ILD_TRANSPARENT);
			}
			else if(pImageListNormal && imagNormalIndex >= 0 && imagNormalIndex < normalCnt)	//����̬ͼ��
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);

				pImageListNormal->Draw(&memDC, imagNormalIndex, imagePt, ILD_TRANSPARENT);
			}
			else if(pImageListNormal)
			{
				imagePt = CPoint(imagePt.x - imgWidth, itemTextRect.top + (itemTextRect.Height() - imagHeight)/2);
				textRc.left = imagePt.x;	//û��ͼ��Ͱ��ı��ַ��������ƶ�
			}

			itemText = GetItemText(hItem);
			textRc.left += m_textOffsetX;
			MemDrawText(memDC, textRc, itemText, m_textAlignMode);
		}

		if(bHasChk)
		{
			if(GetCheck(hItem))
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - m_pImgChkSel->GetHeight())/2);
				MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + m_pImgChkSel->GetWidth(), imagePt.y + m_pImgChkSel->GetHeight()), m_pImgChkSel);
			}
			else
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - m_pImgChkUnSel->GetHeight())/2);
				MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + m_pImgChkUnSel->GetWidth(), imagePt.y + m_pImgChkUnSel->GetHeight()), m_pImgChkUnSel);
			}
		}

		if(bHasBtn)
		{
			if((GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED))
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - m_pImgButtonSel->GetHeight())/2);
				MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + m_pImgButtonSel->GetWidth(), imagePt.y + m_pImgButtonSel->GetHeight()), m_pImgButtonSel);
			}
			else
			{
				imagePt = CPoint(imagePt.x - min(imgWidth, imagHeight), itemTextRect.top + (itemTextRect.Height() - m_pImgButtonSel->GetHeight())/2);
				MemPaintPng(memDC, CRect(imagePt.x, imagePt.y, imagePt.x + m_pImgButtonUnSel->GetWidth(), imagePt.y + m_pImgButtonUnSel->GetHeight()), m_pImgButtonUnSel);
			}
		}


		hItem = GetNextVisibleItem( hItem );
		vCnt--;
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	m_VScroll.UpdateScrollBar();
	m_HScroll.UpdateScrollBar();

	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CRIMTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_hHoverItem != HitTest(point))
	{
		ITEMHOVER itemhover;
		itemhover.hdr.hwndFrom			= GetSafeHwnd();
		itemhover.hdr.idFrom			= ::GetDlgCtrlID(GetSafeHwnd());
		itemhover.hdr.code				= TVN_ITEMHOVERCHANGE;
		itemhover.m_hHoverOldItem		= m_hHoverItem;
		itemhover.m_hHoverNewItem		= HitTest(point);
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, itemhover.hdr.idFrom, LPARAM(&itemhover));

		m_hHoverItem = itemhover.m_hHoverNewItem;
	}
	
	CPngTreeCtrl::OnMouseMove(nFlags, point);
}


void CRIMTreeCtrl::OnMouseLeave()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CPngTreeCtrl::OnMouseLeave();

	{
		ITEMHOVER itemhover;
		itemhover.hdr.hwndFrom			= GetSafeHwnd();
		itemhover.hdr.idFrom			= ::GetDlgCtrlID(GetSafeHwnd());
		itemhover.hdr.code				= TVN_ITEMHOVERCHANGE;
		itemhover.m_hHoverOldItem		= m_hHoverItem;
		itemhover.m_hHoverNewItem		= NULL;
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, itemhover.hdr.idFrom, LPARAM(&itemhover));

		m_hHoverItem = itemhover.m_hHoverNewItem;
	}
	
}
