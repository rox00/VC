//********************************************************************
//	PNGLISTCTRL.H 文件注释
//	文件名		: PNGLISTCTRL.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者		: 张永
//	创建时间	: 2014/12/29 15:30
//	文件描述	: 目前仅支持文字，如需要图标到时再进行扩展，请联系原作者
//*********************************************************************	
#pragma once

#include "Common/PngBase.h"

#include "Common\\PngControlScroll.h"
#include "PngHeaderCtrl.h"
// CPngListCtrl

class CPngListCtrl : public CListCtrl, public CPngBase
{
	DECLARE_DYNAMIC(CPngListCtrl)

public:
	CPngListCtrl();
	virtual ~CPngListCtrl();


	BOOL								m_bScrollInit;
	CPngHeaderCtrl						m_Header;

	CPngControlScroll					m_HScroll;
	CPngControlScroll					m_VScroll;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH 						CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg HBRUSH 						OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 						OnEraseBkgnd(CDC* pDC);
	afx_msg void 						OnPaint();
	afx_msg int							OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void						OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT 					OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT 					OnPrint(WPARAM wParam, LPARAM lParam);
protected:
	virtual void						PreSubclassWindow();
	afx_msg void 						OnNcPaint();
	afx_msg void 						OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 						OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void 						OnSize(UINT nType, int cx, int cy);
	void								InitImage();
	void								DestoryImage();

	Image*								m_pImgParentBk;
	Image*								m_pImgBk;
	Image*								m_pImgDisable;

	//Header
	Image*								m_pImgHeaderBk;
	Image*								m_pImgHeaderItemOverBk;
	Image*								m_pImgHeaderItemPushBk;
	Image*								m_pImgHeaderDisable;

	//ScrollPng
	Image*								m_pImgScrollBk;
	Image*								m_pImgVScrollTop;
	Image*								m_pImgVScrollBot;
	Image*								m_pImgVScrollThumb;
	Image*								m_pImgHScrollLeft;
	Image*								m_pImgHScrollRight;
	Image*								m_pImgHScrollThumb;	


public:
	BOOL								m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	CString								m_pngParentBkPath;		//父背景图片
	CString								m_pngBkPath;			//背景图片
	CString								m_disablePngPath;		//禁用时显示图片		有效性：非强制有效

	//Header
	int									m_headerSeparatorWidth;	//分割条的宽度
	COLORREF							m_headerSeparatorColor;	//分割条的颜色
	int									m_headerTextOffsetX;	//文本水平偏移
	DWORD								m_headerTextAlignMode;	//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF							m_headerTextColor;		//字体颜色

	CString								m_pngHeaderBk;			//头控件整体的背景图片
	CString								m_pngHeaderItemOverBk;	//头控件鼠标经过时的背景图片
	CString								m_pngHeaderItemPushBk;	//头控件鼠标按下时的背景图片
	CString								m_pngHeaderDisable;		//头控件被禁用时的背景图片

	//ScrollPng
	CString								m_pngScrollBk;			//滚动条的背景 水平与垂直
	CString								m_pngVScrollTop;		//垂直滚动条的上箭头
	CString								m_pngVScrollBot;		//垂直滚动条的下箭头
	CString								m_pngVScrollThumb;		//垂直滚动条的滑块
	CString								m_pngHScrollLeft;		//水平滚动条的上箭头
	CString								m_pngHScrollRight;		//水平滚动条的下箭头
	CString								m_pngHScrollThumb;		//水平滚动条的滑块
};


