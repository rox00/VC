//********************************************************************
//	PNGLISTBOX.H 文件注释
//	文件名		: PNGLISTBOX.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者		: 张永
//	创建时间	: 2014/12/22 15:32
//	文件描述	: 
//*********************************************************************	
#pragma once

#include "Common\\PngControlScroll.h"

// CPngListBox

class CPngListBox : public CListBox, public CPngBase
{
	DECLARE_DYNAMIC(CPngListBox)

public:
	CPngListBox();
	virtual ~CPngListBox();

protected:
	CPngControlScroll						m_HScroll;
	CPngControlScroll						m_VScroll;
	BOOL									m_bScrollInit;

	DECLARE_MESSAGE_MAP()

	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH							CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void							OnPaint();
	afx_msg void							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void							OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void							OnMouseLeave();
	afx_msg void							OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void							OnTimer(UINT_PTR nIDEvent);
	virtual void							PreSubclassWindow();
	afx_msg int								OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void 							OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void 							OnSize(UINT nType, int cx, int cy);

	void									InitImage();
	void									DestoryImage();
	void									DrawClient(CDC* pDC);
	CRect									m_vTopRect;
	CRect									m_vBotRect;
	CRect									m_vThumbRect;
	CRect									m_hLeftRect;
	CRect									m_hRightRect;
	CRect									m_hThumbRect;

	BOOL									m_bMouseMove;

	Image*									m_pImgParentBk;
	Image*									m_pImgBk;
	Image*									m_pImgListOver;
	Image*									m_pImgListSel;
	Image*									m_pImgDisable;

	//ScrollPng
	Image*									m_pImgScrollBk;
	Image*									m_pImgVScrollTop;
	Image*									m_pImgVScrollBot;
	Image*									m_pImgVScrollThumb;
	Image*									m_pImgHScrollLeft;
	Image*									m_pImgHScrollRight;
	Image*									m_pImgHScrollThumb;
public:

	COLORREF								m_textColor;			//RGB(255,0,0)
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	int										m_nTextOffsetX;			//文字区域 向左偏移像素数

	BOOL									m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	CString									m_pngParentBkPath;		//父背景图片
	CString									m_pngBkPath;
	CString									m_pngOverPath;			//鼠标经过时的图片
	CString									m_pngSelPath;			//鼠标选中时的图片
	CString									m_disablePngPath;		//禁用时显示图片		有效性：非强制有效


	//ScrollPng
	CString									m_pngScrollBk;			//滚动条的背景 水平与垂直
	CString									m_pngVScrollTop;		//垂直滚动条的上箭头
	CString									m_pngVScrollBot;		//垂直滚动条的下箭头
	CString									m_pngVScrollThumb;		//垂直滚动条的滑块
	CString									m_pngHScrollLeft;		//水平滚动条的上箭头
	CString									m_pngHScrollRight;		//水平滚动条的下箭头
	CString									m_pngHScrollThumb;		//水平滚动条的滑块

	afx_msg void							OnNcPaint();
};


