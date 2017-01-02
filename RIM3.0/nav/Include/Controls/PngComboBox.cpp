
#include "stdafx.h"
#include "PngComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPngComboBox::CPngComboBox()
{
	bCloseUPLast			= FALSE;

	m_textColor				= RGB(255, 255, 255);
	m_textAlignMode			= DT_SINGLELINE | DT_VCENTER | DT_LEFT;

	m_leftPngPath			= _T("");			//COMBOX  左边
	m_centerPngPath			= _T("");			//COMBOX  中间
	m_rightPngPath			= _T("");			//COMBOX  右边

	m_listLeftPngPath		= _T("");			//COMBOX  列表左边
	m_listTopPngPath		= _T("");			//COMBOX  列表顶部
	m_listRightPngPath		= _T("");			//COMBOX  列表右部
	m_listBottomPngPath		= _T("");			//COMBOX  列表底部
	m_listCenterPngPath		= _T("");			//COMBOX  列表除边框外的背景图
	m_listSelPngPath		= _T("");			//COMBOX  列表选中状态

	m_pngScrollBk			= _T("");
	m_pngVScrollTop			= _T("");
	m_pngVScrollBot			= _T("");
	m_pngVScrollThumb		= _T("");
	m_pngHScrollLeft		= _T("");
	m_pngHScrollRight		= _T("");
	m_pngHScrollThumb		= _T("");

	m_editPngBkPath			= _T("");

	m_nTextOffsetX			= 4;


	m_pImgComboLeft			= NULL;
	m_pImgComboCenter		= NULL; 
	m_pImgComboRight		= NULL;	

	m_pImgListLeft			= NULL;	
	m_pImgListTop			= NULL;		
	m_pImgListRight			= NULL;
	m_pImgListBottom		= NULL;
	m_pImgListCenter		= NULL;
	m_pImgListSel			= NULL;

}


//----------------------------------------------------------------------

CPngComboBox::~CPngComboBox()
{
	DestoryImage();
}


BEGIN_MESSAGE_MAP(CPngComboBox, CComboBox)
	//{{AFX_MSG_MAP(CBitComboBox)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTLCOLOREDIT, OnCtlColorListEdit)
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CPngComboBox::OnCbnDropdown)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &CPngComboBox::OnCbnCloseup)
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitComboBox message handlers

void CPngComboBox::InitImage()
{
	if(m_pImgComboLeft == NULL)		m_pImgComboLeft		= SafeImageFrom(m_leftPngPath);
	if(m_pImgComboCenter == NULL)	m_pImgComboCenter	= SafeImageFrom(m_centerPngPath);
	if(m_pImgComboRight == NULL)	m_pImgComboRight	= SafeImageFrom(m_rightPngPath);

	if(m_pImgListLeft == NULL)		m_pImgListLeft		= SafeImageFrom(m_listLeftPngPath);
	if(m_pImgListTop == NULL)		m_pImgListTop		= SafeImageFrom(m_listTopPngPath);
	if(m_pImgListRight == NULL)		m_pImgListRight		= SafeImageFrom(m_listRightPngPath);
	if(m_pImgListBottom == NULL)	m_pImgListBottom	= SafeImageFrom(m_listBottomPngPath);
	if(m_pImgListCenter == NULL)	m_pImgListCenter	= SafeImageFrom(m_listCenterPngPath);
	if(m_pImgListSel == NULL)		m_pImgListSel		= SafeImageFrom(m_listSelPngPath);

	m_listbox.m_listLeftPngPath		= m_listLeftPngPath;		//COMBOX  列表左边
	m_listbox.m_listTopPngPath		= m_listTopPngPath;		//COMBOX  列表顶部
	m_listbox.m_listRightPngPath	= m_listRightPngPath;	//COMBOX  列表右部
	m_listbox.m_listBottomPngPath	= m_listBottomPngPath;		//COMBOX  列表底部
	m_listbox.m_listCenterPngPath	= m_listCenterPngPath;
	m_listbox.m_listSelPngPath		= m_listSelPngPath;			//COMBOX  列表选中状态

	//scroll
 	m_listbox.m_pngScrollBk			= m_pngScrollBk;
 	m_listbox.m_pngVScrollTop		= m_pngVScrollTop;
 	m_listbox.m_pngVScrollBot		= m_pngVScrollBot;
 	m_listbox.m_pngVScrollThumb		= m_pngVScrollThumb;
 	m_listbox.m_pngHScrollLeft		= m_pngHScrollLeft;
 	m_listbox.m_pngHScrollRight		= m_pngHScrollRight;
 	m_listbox.m_pngHScrollThumb		= m_pngHScrollThumb;

	m_listbox.m_textAlignMode		= m_textAlignMode;
	m_listbox.m_textColor			= m_textColor;
	m_listbox.m_nTextOffsetX		= m_nTextOffsetX;

	if(m_edit.GetSafeHwnd())
		m_edit.SetMargins(m_nTextOffsetX, 0);
	m_edit.m_textColor				= m_textColor;
	m_edit.m_pngBkPath				= m_editPngBkPath;

	m_listbox.InitImage();	//算是优化速度
}


void CPngComboBox::DestoryImage()
{
	if(m_pImgComboLeft			== NULL)	{	delete m_pImgComboLeft		;		m_pImgComboLeft		= NULL;		}					
	if(m_pImgComboCenter		== NULL)	{	delete m_pImgComboCenter	;		m_pImgComboCenter	= NULL;		}	
	if(m_pImgComboRight			== NULL)	{	delete m_pImgComboRight		;		m_pImgComboRight	= NULL;		}	

	if(m_pImgListLeft			== NULL)	{	delete m_pImgListLeft		;		m_pImgListLeft		= NULL;		}
	if(m_pImgListTop			== NULL)	{	delete m_pImgListTop		;		m_pImgListTop		= NULL;		}
	if(m_pImgListRight			== NULL)	{	delete m_pImgListRight		;		m_pImgListRight		= NULL;		}
	if(m_pImgListBottom			== NULL)	{	delete m_pImgListBottom		;		m_pImgListBottom	= NULL;		}
	if(m_pImgListCenter			== NULL)	{	delete m_pImgListCenter		;		m_pImgListCenter	= NULL;		}
	if(m_pImgListSel			== NULL)	{	delete m_pImgListSel		;		m_pImgListSel		= NULL;		}
}

LRESULT CPngComboBox::OnCtlColorListEdit(WPARAM wParam, LPARAM lParam) 
{//这样才会响应焦点

	if (this->m_edit.m_hWnd == 0) {
		HWND hWnd = (HWND)lParam;

		if (::GetDlgCtrlID(hWnd) == 1001) 
		{
			m_edit.SubclassWindow(hWnd);
		}
	}


	return DefWindowProc(WM_CTLCOLOREDIT, wParam, lParam);
}

LRESULT CPngComboBox::OnCtlColorListBox(WPARAM wParam, LPARAM lParam) 
{
	// Here we need to get a reference to the listbox of the combobox
	// (the dropdown part). We can do it using 
	//TRACE("OnCtlColorListBox()\n");
	if (this->m_listbox.m_hWnd == 0) {
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd) 
		{
			m_listbox.SubclassWindow(hWnd);
		}
	}

	//return (LRESULT)::GetStockObject(NULL_BRUSH);
	
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

void CPngComboBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	InitImage();
	if(m_pImgComboLeft && m_pImgComboRight)
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CRect leftRc			= CRect(rcClient.left, rcClient.top, rcClient.left + SafeGetImageWidth(m_pImgComboLeft), rcClient.Height());
		CRect rightRc			= CRect(rcClient.right- SafeGetImageWidth(m_pImgComboRight), rcClient.top, rcClient.right, rcClient.Height());
		CRect centerRc			= CRect(leftRc.right, rcClient.top, rightRc.left, rcClient.Height());

		CDC			memDC;
		CBitmap		memBmp;
		memDC.CreateCompatibleDC(&dc);
		memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		memDC.SelectObject(&memBmp);
		memDC.SetBkMode(TRANSPARENT);
		memDC.SelectObject(GetFont());
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
		if(IsWindowEnabled())
		{
			::SetTextColor(memDC.GetSafeHdc(), m_textColor);
		}
		else
		{
			::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
		}


		MemPaintPng(memDC, leftRc, m_pImgComboLeft);
		MemPaintPng(memDC, centerRc, m_pImgComboCenter);
		//MemPaintPng(memDC, rightRc, m_pImgComboRight);

		RECT rcText;
		rcText.left		= rcClient.left;		//测试时减一个像素效果好些，原则上不应减
		rcText.right	= rcClient.right;
		rcText.top		= rcClient.top; 
		rcText.bottom	= rcClient.bottom;
		RECT rcPaint;
		rcPaint.left	= rcClient.left		+ leftRc.Width();
		rcPaint.right	= rcClient.right	- rightRc.Width();
		rcPaint.top		= rcClient.top		+ leftRc.Width(); 
		rcPaint.bottom	= rcClient.bottom	- leftRc.Width();

		//Set the normal/highlight color when its repainted 
		if (GetCurSel() != CB_ERR)
		{
			MemPaintPng(memDC, rcPaint, m_pImgListSel);
		}
		else
		{
			MemPaintPng(memDC, rcPaint, m_pImgListCenter);
		}

		CString strText;
		GetWindowText(strText);
		rcText.left += m_nTextOffsetX;
		MemDrawText(memDC, rcText, strText, m_textAlignMode);

		MemPaintPng(memDC, rightRc, m_pImgComboRight);		//防止文字过长
		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	}
	
}

void CPngComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	//有年Listbox对象这里不再执行
	if( lpDrawItemStruct->itemID == -1 )
		return;

	InitImage();

	///****************************画Combox*********************
	if(m_pImgListLeft && m_pImgListTop && m_pImgListRight && m_pImgListBottom)
	{
		if(lpDrawItemStruct->hwndItem == GetSafeHwnd())
		{		
			CDC* pDC = GetDC();

			CRect rcListClient;
			if(m_listbox.GetSafeHwnd())
				m_listbox.GetClientRect(&rcListClient);

			CRect listLeftRc		= CRect(rcListClient.left, rcListClient.top, SafeGetImageWidth(m_pImgListLeft), rcListClient.bottom);
			CRect listTopRc			= CRect(rcListClient.left, rcListClient.top, rcListClient.right, SafeGetImageHeight(m_pImgListTop));
			CRect listRightRc		= CRect(rcListClient.right - SafeGetImageWidth(m_pImgListRight), rcListClient.top, rcListClient.right, rcListClient.bottom);
			CRect listBottomRc		= CRect(rcListClient.left, rcListClient.bottom - SafeGetImageHeight(m_pImgListBottom), rcListClient.right, rcListClient.bottom);

			CRect rcClient;
			GetClientRect(&rcClient);

			CRect leftRc			= CRect(rcClient.left, rcClient.top, rcClient.left + SafeGetImageWidth(m_pImgComboLeft), rcClient.Height());
			CRect rightRc			= CRect(rcClient.right- SafeGetImageWidth(m_pImgComboRight), rcClient.top, rcClient.right, rcClient.Height());
			CRect centerRc			= CRect(leftRc.right, rcClient.top, rightRc.left, rcClient.Height());


			CDC			memDC;
			CBitmap		memBmp;
			memDC.CreateCompatibleDC(pDC);
			memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
			memDC.SelectObject(&memBmp);
			memDC.SetBkMode(TRANSPARENT);
			memDC.SelectObject(GetFont());
			::SetTextColor(memDC.GetSafeHdc(), m_textColor);

			MemPaintPng(memDC, leftRc, m_pImgComboLeft);
			MemPaintPng(memDC, centerRc, m_pImgComboCenter);
			//MemPaintPng(memDC, rightRc, m_pImgComboRight);

			RECT rcText;
			rcText.left		= rcClient.left;		//测试时减一个像素效果好些，原则上不应减
			rcText.right	= rcClient.right;
			rcText.top		= rcClient.top; 
			rcText.bottom	= rcClient.bottom;
			RECT rcPaint;
			rcPaint.left	= rcClient.left		+ leftRc.Width();
			rcPaint.right	= rcClient.right	- rightRc.Width();
			rcPaint.top		= rcClient.top		+ leftRc.Width(); 
			rcPaint.bottom	= rcClient.bottom	- leftRc.Width();

			//Set the normal/highlight color when its repainted 
			if(GetFocus() && GetFocus()->m_hWnd == m_hWnd )
			{
				MemPaintPng(memDC, rcPaint, m_pImgListSel);
			}
			else
			{
				MemPaintPng(memDC, rcPaint, m_pImgListCenter);
			}

			CString strText;
			GetWindowText(strText);
			rcText.left += m_nTextOffsetX;
			MemDrawText(memDC, rcText, strText, m_textAlignMode);

			MemPaintPng(memDC, rightRc, m_pImgComboRight);		//防止文字过长
			pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
			ReleaseDC(pDC);
			return;
		}
		else
		{
			///****************************画 Listbox(如果没有采用m_listbox.SubclassWindow(hWnd)方式这里将包括画listbox及combox)*********************
			CDC	dc;	
			dc.Attach(lpDrawItemStruct->hDC);

			CRect rcItem		= lpDrawItemStruct->rcItem;
			CRect rcItemPaint	= CRect(0, 0, rcItem.Width(), rcItem.Height());

			CDC			memDC;
			CBitmap		memBmp;
			memDC.CreateCompatibleDC(&dc);
			memBmp.CreateCompatibleBitmap(&dc, rcItemPaint.Width(), rcItemPaint.Height());
			memDC.SelectObject(&memBmp);
			memDC.SetBkMode(TRANSPARENT);
			memDC.SelectObject(GetFont());
			::SetTextColor(memDC.GetSafeHdc(), m_textColor);

			if(lpDrawItemStruct->itemState & ODS_FOCUS)
			{
				MemPaintPng(memDC, rcItemPaint, m_pImgListSel);
			}
			else
			{
				MemPaintPng(memDC, rcItemPaint, m_pImgListCenter);
			}
			//

			CRect rcListClient;
			m_listbox.GetClientRect(&rcListClient);

			CRect listLeftRc	= CRect(rcListClient.left, rcListClient.top, SafeGetImageWidth(m_pImgListLeft), rcListClient.bottom);
			CRect listTopRc		= CRect(rcListClient.left, rcListClient.top, rcListClient.right, SafeGetImageHeight(m_pImgListTop));
			CRect listRightRc	= CRect(rcListClient.right - SafeGetImageWidth(m_pImgListRight), rcListClient.top, rcListClient.right, rcListClient.bottom);
			CRect listBottomRc	= CRect(rcListClient.left, rcListClient.bottom - SafeGetImageHeight(m_pImgListBottom), rcListClient.right, rcListClient.bottom);

			////////////绘制Listbox上面的文字   
			CString		sItemText;
			// Copy the text of the item to a string
			this->GetLBText(lpDrawItemStruct->itemID, sItemText);
			// Draw the text after the images left postion
			rcItemPaint.left += m_nTextOffsetX;
			MemDrawText(memDC, rcItemPaint, sItemText, m_textAlignMode);

			CRect desDcRc		= CRect(rcItem.left + listLeftRc.Width(), rcItem.top, rcItem.right - listRightRc.Width(), rcItem.bottom);
			CRect sourceRc		= CRect(rcItem.left + listLeftRc.Width(), 0, rcItem.right - listRightRc.Width(), rcItemPaint.bottom);
			if( lpDrawItemStruct->itemID == 0 )
			{
				desDcRc.top		+= listTopRc.Height();
				sourceRc.top	+= listTopRc.Height();
			}

			if( lpDrawItemStruct->itemID == GetCount()-1 )
			{
				desDcRc.bottom -= listBottomRc.Height();
				sourceRc.bottom -= listBottomRc.Height();
			}

			dc.StretchBlt(desDcRc.left, desDcRc.top, desDcRc.Width(), desDcRc.Height(), &memDC,
				sourceRc.left, sourceRc.top, sourceRc.Width(), sourceRc.Height(),SRCCOPY);

			dc.Detach();
		}
	}

}

void CPngComboBox::OnDestroy() 
{
	if (m_listbox.GetSafeHwnd() != NULL)
		m_listbox.UnsubclassWindow();

	if (m_edit.GetSafeHwnd() != NULL)
		m_edit.UnsubclassWindow();

	CComboBox::OnDestroy();	
}

void CPngComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//SetRedraw(FALSE);
	CComboBox::OnLButtonDown(nFlags, point);
	//SetRedraw(TRUE);
	//RedrawWindow();
}

void CPngComboBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

 	CComboBox::OnLButtonUp(nFlags, point);
	//RedrawWindow();
}

void CPngComboBox::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CComboBox::PreSubclassWindow();

	if(AfxGetThreadState()->m_pWndInit == NULL)
	{
		DWORD style = GetStyle();

		if(((style & CBS_OWNERDRAWFIXED) || (style & CBS_OWNERDRAWVARIABLE)) && (style & CBS_HASSTRINGS))
		{
		}
		else
		{
			::MessageBox(NULL, _T("使用CPngComboBox请选择\nCBS_OWNERDRAWFIXED & CBS_HASSTRINGS 或\nCBS_OWNERDRAWVARIABLE & CBS_HASSTRINGS 风格!"), _T("警告！"), MB_OK|MB_ICONWARNING);
		}


		// TODO:  在此添加您专用的创建代码
		COMBOBOXINFO	comboInfo;

		//获取控件信息
		comboInfo.cbSize = sizeof(COMBOBOXINFO);
		GetComboBoxInfo(&comboInfo);

		//子类化列表框
		if (comboInfo.hwndItem != NULL && IsWindow(comboInfo.hwndItem))
		{
			m_edit.SubclassWindow(comboInfo.hwndItem);
		}

		//子类化列表框
		if (comboInfo.hwndList != NULL && IsWindow(comboInfo.hwndList))
		{
			m_listbox.SubclassWindow(comboInfo.hwndList);
			m_listbox.ModifyStyle(WS_BORDER, 0);
		}

	}

}

int CPngComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	// TODO:  在此添加您专用的创建代码
	if(AfxGetThreadState()->m_pWndInit == NULL)
	{
		DWORD style = GetStyle();

		if(((style & CBS_OWNERDRAWFIXED) || (style & CBS_OWNERDRAWVARIABLE)) && (style & CBS_HASSTRINGS))
		{
		}
		else
		{
			::MessageBox(NULL, _T("使用CPngComboBox请选择\nCBS_OWNERDRAWFIXED & CBS_HASSTRINGS 或\nCBS_OWNERDRAWVARIABLE & CBS_HASSTRINGS 风格!"), _T("警告！"), MB_OK|MB_ICONWARNING);
		}


		// TODO:  在此添加您专用的创建代码
		COMBOBOXINFO	comboInfo;

		//获取控件信息
		comboInfo.cbSize = sizeof(COMBOBOXINFO);
		GetComboBoxInfo(&comboInfo);

		//子类化列表框
		if (comboInfo.hwndItem != NULL && IsWindow(comboInfo.hwndItem))
		{
			m_edit.SubclassWindow(comboInfo.hwndItem);
		}

		//子类化列表框
		if (comboInfo.hwndList != NULL && IsWindow(comboInfo.hwndList))
		{
			m_listbox.SubclassWindow(comboInfo.hwndList);
			m_listbox.ModifyStyle(WS_BORDER, 0);
		}

	}

	return 0;
}

void CPngComboBox::OnCbnDropdown()
{
	// TODO: Add your control notification handler code here

// 	if (m_listbox.GetSafeHwnd())
//    	{
// 		m_listbox.InitImage();
// 
//    		CRect comWndRect;
//    		CRect listWndRect;
//    		GetWindowRect(comWndRect);
//    		m_listbox.GetWindowRect(listWndRect);
//    		if (listWndRect.left >= comWndRect.left && listWndRect.right <= comWndRect.right)
// 		{
// 			m_listbox.PrepareNcPaint();
//    			m_listbox.ShowWindow(SW_SHOW);	//在这直接把窗口显示出来，速度更快  每次显示会先画桌面白背景
// 		}
//    		else
//    		{
//    			listWndRect.OffsetRect(comWndRect.left - listWndRect.left, comWndRect.bottom + 1 - listWndRect.top);
//    			m_listbox.MoveWindow(listWndRect);
// 			m_listbox.PrepareNcPaint();
// 			m_listbox.ShowWindow(SW_SHOW);
//    		}
//    		//m_listbox.SetRedraw(FALSE);
// 
// 		m_listbox.UpdateScroll(TRUE);
//    	}

}

void CPngComboBox::OnCbnCloseup()
{
	// TODO: Add your control notification handler code here
// 	if (m_listbox.GetSafeHwnd())
// 	{
// 		m_listbox.ShowWindow(SW_HIDE);
// 		m_listbox.UpdateScroll(FALSE);
// 	}
// 	Invalidate();

	if(m_edit.m_hWnd == NULL)
	{
		SetRedraw(FALSE);
		bCloseUPLast = TRUE;
	}
}

void CPngComboBox::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CComboBox::OnNcPaint() for painting messages

	InitImage();

	if(m_pImgComboLeft && m_pImgComboRight)
	{
		CDC *pDC = GetDC();

		CRect rcClient;
		GetClientRect(&rcClient);

		CRect leftRc = CRect(rcClient.left, rcClient.top, rcClient.left + SafeGetImageWidth(m_pImgComboLeft), rcClient.Height());
		CRect rightRc = CRect(rcClient.right - SafeGetImageWidth(m_pImgComboRight), rcClient.top, rcClient.right, rcClient.Height());
		CRect centerRc = CRect(leftRc.right, rcClient.top, rightRc.left, rcClient.Height());

		CDC			memDC;
		CBitmap		memBmp;
		memDC.CreateCompatibleDC(pDC);
		memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
		CBitmap* pOldBmp = (CBitmap*)memDC.SelectObject(&memBmp);
		memDC.SetBkMode(TRANSPARENT);
		CFont* pOldFont = (CFont*)memDC.SelectObject(GetFont());
		::SetTextColor(memDC.GetSafeHdc(), m_textColor);
		if (IsWindowEnabled())
		{
			::SetTextColor(memDC.GetSafeHdc(), m_textColor);
		}
		else
		{
			::SetTextColor(memDC.GetSafeHdc(), RGB(128, 128, 128));
		}


		MemPaintPng(memDC, leftRc, m_pImgComboLeft);
		MemPaintPng(memDC, centerRc, m_pImgComboCenter);

		RECT rcText;
		rcText.left = rcClient.left;		//测试时减一个像素效果好些，原则上不应减
		rcText.right = rcClient.right;
		rcText.top = rcClient.top;
		rcText.bottom = rcClient.bottom;
		RECT rcPaint;
		rcPaint.left = rcClient.left + leftRc.Width();
		rcPaint.right = rcClient.right - rightRc.Width();
		rcPaint.top = rcClient.top + leftRc.Width();
		rcPaint.bottom = rcClient.bottom - leftRc.Width();

		//Set the normal/highlight color when its repainted 
		if (GetFocus() && GetFocus()->m_hWnd == m_hWnd)
		{
			MemPaintPng(memDC, rcPaint, m_pImgListSel);
		}
		else
		{
			MemPaintPng(memDC, rcPaint, m_pImgListCenter);
		}

		CString strText;
		GetWindowText(strText);
		rcText.left += m_nTextOffsetX;
		MemDrawText(memDC, rcText, strText, m_textAlignMode);

		MemPaintPng(memDC, rightRc, m_pImgComboRight);		//防止文字过长
		pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		memDC.SelectObject(pOldFont);
		memDC.DeleteDC();
		memBmp.DeleteObject();

		ReleaseDC(pDC);
	}
}

BOOL CPngComboBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	return CComboBox::OnEraseBkgnd(pDC);
}

LRESULT CPngComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	LRESULT ret = NULL;
	if(message == WM_KILLFOCUS || message == WM_SETFOCUS /*|| message == WM_KEYDOWN*/)
	{
		SetRedraw(FALSE);
		ret = __super::WindowProc(message, wParam, lParam);
		SetRedraw(TRUE);
		RedrawWindow();
	}
	else if(message == WM_KEYDOWN || message == WM_CHAR)
	{
		SetRedraw(FALSE);
		ret = __super::WindowProc(message, wParam, lParam);
		SetRedraw(TRUE);
		RedrawWindow();
	}
	else if (bCloseUPLast)
	{
		bCloseUPLast = FALSE;	//必须要先置FALS
		SetRedraw(TRUE);
		ret = __super::WindowProc(message, wParam, lParam);
	}
	else
	{
		ret = __super::WindowProc(message, wParam, lParam);
	}

	return ret;
}

void CPngComboBox::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{

	// TODO:  Add your code to determine the size of specified item
}
