#pragma once

#include "PngBase.h"
#include "../PngScrollBar.h"

class CPngComListBox : public CListBox, public CPngBase
{
public:
	CPngComListBox();
	virtual ~CPngComListBox();

	void											InitImage();
	void											DestoryImage();


	CRect											m_vTopRect;
	CRect											m_vBotRect;
	CRect											m_vThumbRect;
	CRect											m_hLeftRect;
	CRect											m_hRightRect;
	CRect											m_hThumbRect;

protected:

	DECLARE_MESSAGE_MAP()

	CDC												m_NcMemDC;
	CBitmap											m_NcMemBmp;

	Image*											m_pImgListLeft;	
	Image*											m_pImgListTop;		
	Image*											m_pImgListRight;
	Image*											m_pImgListBottom;
	Image*											m_pImgListCenter;
	Image*											m_pImgListSel;

	//ScrollPng
	Image*											m_pImgScrollBk;
	Image*											m_pImgVScrollTop;
	Image*											m_pImgVScrollBot;
	Image*											m_pImgVScrollThumb;
	Image*											m_pImgHScrollLeft;
	Image*											m_pImgHScrollRight;
	Image*											m_pImgHScrollThumb;

	void 											DrawVScroll(CDC* pDC);
	void 											DrawClient(CDC* pDC);
	UINT											OnHitTest(CPoint point);
	BOOL											ModifyVScrollStyle(); //检测是否有垂直滚动条

	virtual void									DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void 									OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void									OnPaint();
	afx_msg BOOL 									OnEraseBkgnd(CDC* pDC);
	afx_msg	LRESULT									OnPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT									OnPrintClient(WPARAM wParam, LPARAM lParam);
	void											PrepareNcPaint();
	afx_msg void									OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL 									OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void 									OnNcPaint();
	virtual void 									PreSubclassWindow();
	afx_msg void 									OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void									OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);


	virtual BOOL									OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

public:

	DWORD											m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF										m_textColor;			//RGB(255,0,0)
	int												m_nTextOffsetX;			//文字区域 向左偏移像素数

	CString											m_listLeftPngPath;		//COMBOX  列表左边
	CString											m_listTopPngPath;		//COMBOX  列表顶部
	CString											m_listRightPngPath;		//COMBOX  列表右部
	CString											m_listBottomPngPath;	//COMBOX  列表底部
	CString											m_listCenterPngPath;	//COMBOX  列表底部
	CString											m_listSelPngPath;		//COMBOX  列表选中状态

	//ScrollPng
	CString											m_pngScrollBk;
	CString											m_pngVScrollTop;
	CString											m_pngVScrollBot;
	CString											m_pngVScrollThumb;
	CString											m_pngHScrollLeft;
	CString											m_pngHScrollRight;
	CString											m_pngHScrollThumb;

};

