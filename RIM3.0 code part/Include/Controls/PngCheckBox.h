//********************************************************************
//	PNGCHECKBOX.H 文件注释
//	文件名 : PNGCHECKBOX.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:13
//	文件描述: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CCheckBoxEx

class CPngCheckBox : public CButton, public CPngBase
{
	DECLARE_DYNAMIC(CPngCheckBox)

public:
	CPngCheckBox();
	virtual ~CPngCheckBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void 							OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH							CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void 							OnPaint();
	afx_msg void 							OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void 							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 							OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 							OnSetFocus(CWnd* pOldWnd);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void							PreSubclassWindow();
	void									InitImage();
	void									DestoryImage();
	BOOL									m_bLButtonDown;		//暂留，用在OnMouseMove中防止重绘

	Image*									m_pImgParentBk;
	Image*									m_pImgBk;
	Image*									m_pImgChkUnSel;
	Image*									m_pImgChkSel;
	Image*									m_pImgDisable;


public:
	BOOL									m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	int										m_textOffsetX;			//文本的水平偏移
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF								m_textColor;			//文本颜色

	CString									m_pngParentBkPath;		//父背景图片
	CString									m_pngBkPath;			//背景图片

	CString									m_pngChkUnSel;			//未选中时的状态图片
	CString									m_pngChkSel;			//选中时的状态图片
	CString									m_disablePngPath;		//禁用时显示图片		有效性：非强制有效
};


