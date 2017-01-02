//********************************************************************
//	PNGRADIOBUTTON.H 文件注释
//	文件名 : PNGRADIOBUTTON.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CPngRadioButton

class CPngRadioButton : public CButton, public CPngBase
{
	DECLARE_DYNAMIC(CPngRadioButton)

public:
	CPngRadioButton();
	virtual ~CPngRadioButton();

protected:
	DECLARE_MESSAGE_MAP()

	void						InitImage();
	void						DestoryImage();

	afx_msg BOOL 				OnEraseBkgnd(CDC* pDC);
	afx_msg void 				OnPaint();
	afx_msg HBRUSH				CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void 				OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 				OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void				OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void				OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT				OnSetCheck(WPARAM wParam, LPARAM lParam);
	virtual void				PreSubclassWindow();
	virtual LRESULT				WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	BOOL						m_bLButtonDown;		//暂留，用在OnMouseMove中防止重绘

	Image*						m_pImgParentBk;
	Image*						m_pImgBk;
	Image*						m_pImgRadioUnSel;
	Image*						m_pImgRadioSel;
	Image*						m_pImgDisable;

public:
	BOOL						m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	int							m_textOffsetX;			//文本水平偏移
	DWORD						m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF					m_textColor;			//文本颜色

	CString						m_pngParentBkPath;		//父背景图片
	CString						m_pngBkPath;			//背景图片

	CString						m_pngRadioUnSel;		//未选中时的状态图片
	CString						m_pngRadioSel;			//选中时的状态图片
	CString						m_disablePngPath;		//禁用时显示图片		有效性：非强制有效
};


