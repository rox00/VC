// SendFileListTree.cpp : implementation file
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "SendFileListTree.h"


// CSendFileListTree

IMPLEMENT_DYNAMIC(CSendFileListTree, CTreeCtrl)

CSendFileListTree::CSendFileListTree()
{
	m_bMouseMove = FALSE;
}

CSendFileListTree::~CSendFileListTree()
{
}


BEGIN_MESSAGE_MAP(CSendFileListTree, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CSendFileListTree message handlers



void CSendFileListTree::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CTreeCtrl::OnPaint() for painting messages

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
	::SetTextColor(memDC.GetSafeHdc(), RGB(0,0,0));

	////【zy 2014-10-17 ↓】  画背景
	memDC.FillSolidRect(rcClient, RGB(0,75,133));


	//CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	HTREEITEM	hItem			= GetFirstVisibleItem();
	int			vCnt			= GetVisibleCount();

	CRect 		itemLineRect	= CRect(0, 0, 0, 0);
	CString		itemText		= _T("");
	CString		str				= _T("发送:0%");
	while(hItem != NULL && vCnt>=0)
	{
		itemText = GetItemText(hItem);
		GetItemRect(hItem, &itemLineRect, FALSE );
		memDC.FillSolidRect(itemLineRect, RGB(0,155,213));
	
		CPoint mousePt;
		GetCursorPos(&mousePt);
		ScreenToClient(&mousePt);
		if(m_bMouseMove)	//当鼠标在客户区处于移动状态时才画经过的底图
		{
			if(itemLineRect.PtInRect(mousePt))		//鼠标经过，是画全行还是单个Item
				memDC.FillSolidRect(itemLineRect, RGB(0,200,213));
		}


		LPTRANSFILEINFO pLogfileInfo = (LPTRANSFILEINFO)GetItemData(hItem);

		if(pLogfileInfo)
		{
			int width = 0;

			CString nameTemp = _T("");
			tstring name = PathFindFileName(pLogfileInfo->GetApplicationName().c_str());
			if(name.size() > 8)
				nameTemp.Format(_T("%8.8s..."), name.c_str());
			else
				nameTemp = name.c_str();

			CString rateStr = _T("");
			if (pLogfileInfo->rateTemp / (1024 * 1024 * 1024))
				rateStr.Format(_T("%.1fGB/s"), pLogfileInfo->rateTemp / (1.0 *(1024 * 1024 * 1024)));
			else if (pLogfileInfo->rateTemp / (1024 * 1024))
				rateStr.Format(_T("%.2fMB/s"), pLogfileInfo->rateTemp / (1.0 *(1024 * 1024)));
			else if (pLogfileInfo->rateTemp / (1024))
				rateStr.Format(_T("%.3fKB/s"), pLogfileInfo->rateTemp / (1.0 *(1024)));
			else
				rateStr.Format(_T("%dB/s"), pLogfileInfo->rateTemp);

			CString transByteStr = _T("");
			if (pLogfileInfo->transBytesTemp / (1024 * 1024 * 1024))
				transByteStr.Format(_T("%.3fGB"), pLogfileInfo->transBytesTemp / (1.0 *(1024 * 1024 * 1024)));
			else if (pLogfileInfo->transBytesTemp / (1024 * 1024))
				transByteStr.Format(_T("%.3fMB"), pLogfileInfo->transBytesTemp / (1.0 *(1024 * 1024)));
			else if (pLogfileInfo->transBytesTemp / (1024))
				transByteStr.Format(_T("%.3fKB"), pLogfileInfo->transBytesTemp / (1.0 *(1024)));
			else
				transByteStr.Format(_T("%dB"), pLogfileInfo->transBytesTemp);

			CString totalByteStr = _T("");
			if (pLogfileInfo->GetTotalSize() / (1024 * 1024 * 1024))
				totalByteStr.Format(_T("%.3fGB"), pLogfileInfo->GetTotalSize() / (1.0 *(1024 * 1024 * 1024)));
			else if (pLogfileInfo->GetTotalSize() / (1024 * 1024))
				totalByteStr.Format(_T("%.3fMB"), pLogfileInfo->GetTotalSize() / (1.0 *(1024 * 1024)));
			else if (pLogfileInfo->GetTotalSize() / (1024))
				totalByteStr.Format(_T("%.3fKB"), pLogfileInfo->GetTotalSize() / (1.0 *(1024)));
			else
				totalByteStr.Format(_T("%dB"), pLogfileInfo->GetTotalSize());

			double totaltrans	= pLogfileInfo->GetTotalSize() / (1.0 *(1024 * 1024));
			double alreadytrans = pLogfileInfo->transBytesTemp / (1.0 *(1024 * 1024));
			double percent = alreadytrans * 1.0 / totaltrans;
			if (_tcscmp(itemText, SENDFILELISTTREE_SENDSTR) == 0)
			{
				str.Format(_T("发送 %s：(%s/%s) \n    (%s)"), nameTemp, transByteStr, totalByteStr, rateStr);
			}
			else if (_tcscmp(itemText, SENDFILELISTTREE_RECVSTR) == 0)
			{
				str.Format(_T("接收 %s：(%s/%s) \n    (%s)"), nameTemp, transByteStr, totalByteStr, rateStr);
			}


			width = int((itemLineRect.Width() * percent));

			CRect percentRect = itemLineRect;
			percentRect.right = percentRect.left + width;
			memDC.FillSolidRect(percentRect, RGB(0,200,0));


			memDC.DrawText(str, CRect(itemLineRect.left + 2, itemLineRect.top, itemLineRect.right, itemLineRect.bottom), DT_VCENTER|DT_LEFT);

			CRect rectSendOrRecv	= CRect(itemLineRect.right - 60, itemLineRect.bottom - 20, itemLineRect.right - 30, itemLineRect.bottom);
			CRect rectCancel		= CRect(itemLineRect.right - 30, itemLineRect.bottom - 20, itemLineRect.right,		itemLineRect.bottom);


			CString fileName = PathFindFileName(pLogfileInfo->GetApplicationName().c_str());

			if(rectCancel.PtInRect(mousePt))
				memDC.FillSolidRect(rectCancel, RGB(255,0,0));
			memDC.DrawText(_T("取消"), rectCancel, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

			if(_tcscmp(itemText, SENDFILELISTTREE_RECVSTR) == 0)
			{
				if(pLogfileInfo->GetTransFlag() <= tagTRANSFILEINFO::TRANS_FLAG_TRANSBEGIN)
				{
					if(rectSendOrRecv.PtInRect(mousePt))
						memDC.FillSolidRect(rectSendOrRecv, RGB(255,0,0));
				}
				else
					memDC.SetTextColor(RGB(128,128,128));

				memDC.DrawText(_T("接收"), rectSendOrRecv, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				memDC.SetTextColor(RGB(0, 0, 0));
			}

		}


		hItem = GetNextVisibleItem( hItem );
		vCnt--;
	}


	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
}

BOOL CSendFileListTree::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	return CTreeCtrl::OnEraseBkgnd(pDC);
}

void CSendFileListTree::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CTreeCtrl::OnNcPaint() for painting messages
}

void CSendFileListTree::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMouseMove == false)
	{
		//要求接收鼠标离开消息
		TRACKMOUSEEVENT		csTME;
		csTME.cbSize	= sizeof(csTME);
		csTME.dwFlags	= TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&csTME);

		m_bMouseMove = true;
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
	Invalidate();
}

void CSendFileListTree::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnMouseLeave();
	m_bMouseMove = false;
	Invalidate();
}

void CSendFileListTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	HTREEITEM	hItem	= GetFirstVisibleItem();
	int			vCnt	= GetVisibleCount();

	CRect 		itemLineRect	= CRect(0, 0, 0, 0);
	CString		itemText		= _T("");
	CString		str				= _T("进度:0%");
	while(hItem != NULL && vCnt>=0)
	{
		itemText = GetItemText(hItem);
		GetItemRect(hItem, &itemLineRect, FALSE );
		LPTRANSFILEINFO pTransfileInfo = (LPTRANSFILEINFO)GetItemData(hItem);

		if(pTransfileInfo)
		{
			CRect rectSendOrRecv	= CRect(itemLineRect.right - 60, itemLineRect.bottom - 25, itemLineRect.right - 30, itemLineRect.bottom);
			CRect rectCancel		= CRect(itemLineRect.right - 30, itemLineRect.bottom - 25, itemLineRect.right,		itemLineRect.bottom);

			/////如果是准备传输状态，才可修改为正在传输状态

			if(rectSendOrRecv.PtInRect(point) && pTransfileInfo->GetTransFlag() == TRANSFILEINFO::TRANS_FLAG_TRANSBEGIN)
			{
				if(_tcscmp(itemText, SENDFILELISTTREE_RECVSTR) == 0)
				{
					tstring str  = PathFindFileName(pTransfileInfo->GetApplicationName().c_str());
					CFileDialog filedlg(FALSE, NULL, str.c_str(), OFN_PATHMUSTEXIST|OFN_READONLY|OFN_OVERWRITEPROMPT, NULL, this);
					if(IDOK == filedlg.DoModal())
					{
						CString filePath = filedlg.GetPathName();
						TRANSFILEINFO::InitTransFileInfoByPath(pTransfileInfo, filePath.GetBuffer());
						pTransfileInfo->SafeModifyTransFlag(TRANSFILEINFO::TRANS_FLAG_TRANSING);
					}
				}
				break;
			}

			if(rectCancel.PtInRect(point) && 
				(pTransfileInfo->GetTransFlag() == TRANSFILEINFO::TRANS_FLAG_TRANSBEGIN || pTransfileInfo->GetTransFlag() == TRANSFILEINFO::TRANS_FLAG_TRANSING))
			{
				pTransfileInfo->SafeModifyTransFlag(TRANSFILEINFO::TRANS_FLAG_TRANSCANCEL);
				break;
			}
		}

		hItem = GetNextVisibleItem( hItem );
		vCnt--;
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}
