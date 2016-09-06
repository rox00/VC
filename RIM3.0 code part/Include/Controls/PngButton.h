//********************************************************************
//	PNGBUTTON.H 文件注释
//	文件名 : PNGBUTTON.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:14
//	文件描述: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"
// CPngButton

class CPngButton : public CButton, public CPngBase
{
	DECLARE_DYNAMIC(CPngButton)

public:
	CPngButton();
	virtual ~CPngButton();

protected:
	DECLARE_MESSAGE_MAP()
	bool										m_bLButtonDown;
	bool										m_bMouseMove;


	Image*										m_pImgParentBk;
	Image*										m_pImgBk;

	Image*										m_pImgNormal;
	Image*										m_pImgPush;
	Image*										m_pImgOver;
	Image*										m_pImgDisable;


protected:
	afx_msg HBRUSH								CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void 								OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void 								PreSubclassWindow();
	virtual void 								DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void 								OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 								OnMouseLeave();
	afx_msg void 								OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 								OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void 								OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL								OnEraseBkgnd(CDC* pDC);

	void										InitImage();
	void										DestoryImage();
	void										DrawButton(Image *pImage);
	virtual LRESULT								WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	BOOL										m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	DWORD										m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF									m_textColor;			//文本颜色RGB(255,0,0)

	CString										m_pngParentBkPath;		//父背景图片
	CString										m_pngBkPath;			//背景图片

	CString										m_normalPngPath;		//初始化显示图片		有效性：非强制有效
	CString										m_pushPngPath;			//按下时显示图片		有效性：非强制有效	
	CString										m_overPngPath;			//鼠标经过时显示图片	有效性：非强制有效
	CString										m_disablePngPath;		//禁用时显示图片		有效性：非强制有效

};


