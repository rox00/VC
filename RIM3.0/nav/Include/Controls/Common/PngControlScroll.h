//********************************************************************
//	PNGCONTROLSCROLL.H 文件注释
//	文件名 : PNGCONTROLSCROLL.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	作者 : 张永
//	创建时间: 2014/12/3 11:12
//	文件描述: 
//*********************************************************************	

#pragma once

#include "PngBase.h"
/////////////////////////////////////////////////////////////////////////////
// CPngControlScroll window
//本控件只可作为控件的滚动条
class CPngControlScroll : public CStatic, public CPngBase
{
	// Construction
public:
	CPngControlScroll();
	virtual ~CPngControlScroll();

	void 										DrawVScroll(CDC* pDC);
	void 										DrawHScroll(CDC* pDC);
protected:
	CWnd*										m_pCtrl;
	UINT										m_nScrollStyle;

	DECLARE_DYNAMIC(CPngControlScroll)

	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH								OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void								OnPaint();
	afx_msg BOOL								OnEraseBkgnd(CDC* pDC);
	afx_msg int									OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void								OnNcPaint();


public:
	virtual BOOL 								InitScrollBar(CWnd *pCtrl, UINT nStyle);
	void 										UpdateScrollBar();

	BOOL										m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	Image*										m_pImgParentBk;

	Image*										m_pImgScrollBk;
	Image*										m_pImgVScrollTop;
	Image*										m_pImgVScrollBot;
	Image*										m_pImgVScrollThumb;
	Image*										m_pImgHScrollLeft;
	Image*										m_pImgHScrollRight;
	Image*										m_pImgHScrollThumb;

};
