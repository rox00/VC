//********************************************************************
//	PNGSCROLLBAR.H 文件注释
//	文件名 : PNGSCROLLBAR.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CPngScrollBar

class CPngScrollBar : public CScrollBar, public CPngBase
{
	DECLARE_DYNAMIC(CPngScrollBar)

public:
	CPngScrollBar();
	virtual ~CPngScrollBar();
	//此处公用函数不推荐做其它用处

	CRect						m_vTopRect;
	CRect						m_vBotRect;
	CRect						m_vThumbRect;
	CRect						m_hLeftRect;
	CRect						m_hRightRect;
	CRect						m_hThumbRect;

	void 						InitImage();
	void						DestoryImage();
	void 						DrawHScroll(CDC* pDC);
	void 						DrawVScroll(CDC* pDC);
	BOOL						SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
	int							SetScrollPos(int nPos, BOOL bRedraw = TRUE);
	void						SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
	UINT						OnHitTest(CPoint point);
protected:

	CDC							parentMemDC;
	CBitmap						parentMemBmp;

	DECLARE_MESSAGE_MAP()

	Image*						m_pImgParentBk;
	Image*						m_pImgScrollBk;
	Image*						m_pImgDisable;


	Image*						m_pImgVScrollTop;
	Image*						m_pImgVScrollBot;
	Image*						m_pImgVScrollThumb;
	Image*						m_pImgHScrollLeft;
	Image*						m_pImgHScrollRight;
	Image*						m_pImgHScrollThumb;


	afx_msg HBRUSH				OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 				OnEraseBkgnd(CDC* pDC);
	afx_msg void 				OnPaint();


	afx_msg int					OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void 				OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 				OnLButtonDblClk(UINT nFlags, CPoint point);

public:
	BOOL						m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	/////////////////开放接口
	CString						m_pngParentBkPath;		//父背景图片
	CString						m_disablePngPath;		//禁用时显示图片		有效性：非强制有效

	//ScrollPng
	CString						m_pngScrollBk;			//滚动条背景图片
	CString						m_pngVScrollTop;		//垂直滚动条的上箭头
	CString						m_pngVScrollBot;		//垂直滚动条的下箭头
	CString						m_pngVScrollThumb;		//垂直滚动条的滑块
	CString						m_pngHScrollLeft;		//水平滚动条的上箭头
	CString						m_pngHScrollRight;		//水平滚动条的下箭头
	CString						m_pngHScrollThumb;		//水平滚动条的滑块
};


