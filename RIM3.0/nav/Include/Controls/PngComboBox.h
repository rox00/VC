#pragma once

#include "Common/PngBase.h"

#include "Common/PngComListBox.h"
#include "PNGGrayTipEdit.h"

class CPngComboBox : public CComboBox, public CPngBase
{
public:
	CPngComboBox();
	virtual ~CPngComboBox();

protected:
	void									InitImage();
	void									DestoryImage();

	afx_msg LRESULT							OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnCtlColorListEdit(WPARAM wParam, LPARAM lParam);
	virtual void							DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void 							OnPaint();
	afx_msg void 							OnDestroy();
	virtual void							PreSubclassWindow();
	afx_msg int								OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void 							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 							OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void 							OnCbnDropdown();
	afx_msg void 							OnCbnCloseup();
	afx_msg void 							OnNcPaint();
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	virtual LRESULT							WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void							MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);


	DECLARE_MESSAGE_MAP()

	CPNGGrayTipEdit							m_edit;
	CPngComListBox							m_listbox;

	BOOL									bCloseUPLast;	//刚刚是否关闭

	Image*									m_pImgComboLeft;
	Image*									m_pImgComboCenter; 
	Image*									m_pImgComboRight;	

	Image*									m_pImgListLeft;	
	Image*									m_pImgListTop;		
	Image*									m_pImgListRight;
	Image*									m_pImgListBottom;
	Image*									m_pImgListCenter;
	Image*									m_pImgListSel;

public:
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF								m_textColor;			//RGB(255,0,0)
	int										m_nTextOffsetX;			//文字区域 向左偏移像素数

	CString									m_leftPngPath;			//COMBOX  左边
	CString									m_centerPngPath;		//COMBOX  中间
	CString									m_rightPngPath;			//COMBOX  右边

	CString									m_listLeftPngPath;		//COMBOX  列表左边
	CString									m_listTopPngPath;		//COMBOX  列表顶部
	CString									m_listRightPngPath;		//COMBOX  列表右部
	CString									m_listBottomPngPath;	//COMBOX  列表底部
	CString									m_listCenterPngPath;	//COMBOX  列表底部
	CString									m_listSelPngPath;		//COMBOX  列表选中状态

	//ScrollPng
	CString									m_pngScrollBk;			//滚动条的背景 水平与垂直
	CString									m_pngVScrollTop;		//垂直滚动条的上箭头
	CString									m_pngVScrollBot;		//垂直滚动条的下箭头
	CString									m_pngVScrollThumb;		//垂直滚动条的滑块
	CString									m_pngHScrollLeft;		//水平滚动条的上箭头
	CString									m_pngHScrollRight;		//水平滚动条的下箭头
	CString									m_pngHScrollThumb;		//水平滚动条的滑块

	CString									m_editPngBkPath;		//Edit  编辑框背景

};

