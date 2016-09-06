//********************************************************************
//	PNGTREECTRL.H 文件注释
//	文件名 : PNGTREECTRL.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CPngTreeCtrl

#include "Common\\PngControlScroll.h"
#include "Common\\PngToolTipCtrl.h"

class CPngTreeCtrl : public CTreeCtrl, public CPngBase
{
	DECLARE_DYNAMIC(CPngTreeCtrl)

public:
	CPngTreeCtrl();
	virtual ~CPngTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	CPngControlScroll					m_HScroll;
	CPngControlScroll					m_VScroll;
	BOOL								m_bScrollInit;
	CPngToolTipCtrl						m_pngToolTipCtrl;

	void								InitImage();
	void								DestoryImage();
	afx_msg void 						OnPaint();
	afx_msg BOOL 						OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH						CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void						OnMouseMove(UINT nFlags, CPoint point);
	virtual void 						PreSubclassWindow();
	virtual BOOL						PreTranslateMessage(MSG* pMsg);
	//Scroll
	afx_msg BOOL						OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void 						OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void 						OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void						OnNcPaint();
	afx_msg int							OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void						OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void 						OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 						OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void 						OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void 						OnSize(UINT nType, int cx, int cy);

	Image*								m_pImgParentBk;
	Image*								m_pImgBk;

	Image*								m_pImgButtonSel;
	Image*								m_pImgButtonUnSel;
	Image*								m_pImgChkSel;
	Image*								m_pImgChkUnSel;
	Image*								m_pImgItemOver;
	Image*								m_pImgItemSel;
	Image*								m_pImgDisable;

	//ScrollPng
	Image*								m_pImgScrollBk;
	Image*								m_pImgVScrollTop;
	Image*								m_pImgVScrollBot;
	Image*								m_pImgVScrollThumb;
	Image*								m_pImgHScrollLeft;
	Image*								m_pImgHScrollRight;
	Image*								m_pImgHScrollThumb;

	//tooltippng
	Image*								m_pImgToolTip;

public:
	BOOL								m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充


	int									m_textOffsetX;			//文本水平偏移
	DWORD								m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF							m_textColor;			//字体颜色


	CString								m_pngParentBkPath;		//父背景图片
	CString								m_pngBkPath;			//背景图片

	CString								m_pngItemBtnSel;		//HasButton风格
	CString								m_pngItemBtnUnSel;		//HasButton风格
	CString								m_pngItemChkSel;		//chekbox风格
	CString								m_pngItemChkUnSel;		//chekbox风格
	CString								m_pngItemOver;			//鼠标经过时和图片
	CString								m_pngItemSel;			//选中时的图片
	CString								m_disablePngPath;		//禁用时显示图片		有效性：非强制有效

	//ScrollPng
	CString								m_pngScrollBk;			//滚动条的背景 水平与垂直
	CString								m_pngVScrollTop;		//垂直滚动条的上箭头
	CString								m_pngVScrollBot;		//垂直滚动条的下箭头
	CString								m_pngVScrollThumb;		//垂直滚动条的滑块
	CString								m_pngHScrollLeft;		//水平滚动条的上箭头
	CString								m_pngHScrollRight;		//水平滚动条的下箭头
	CString								m_pngHScrollThumb;		//水平滚动条的滑块

	//tooltippng
	CString								m_pngTooltipBk;			//工具提示条的背景
};


