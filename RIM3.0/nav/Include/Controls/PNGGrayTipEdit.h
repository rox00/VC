//********************************************************************
//	PNGGRAYTIPEDIT.H 文件注释
//	文件名 : PNGGRAYTIPEDIT.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:14
//	文件描述: 
//*********************************************************************	

#pragma once
#include "Common/PngBase.h"

class CPNGGrayTipEdit : public CEdit, public CPngBase
{
// Construction
public:
	CPNGGrayTipEdit();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CPNGGrayTipEdit();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEditCtrl)
	bool								m_bEditChange;		
	CPoint								m_curCaretPt;	
	CBrush*								m_pBkBrush;		//为了适应UNICODE版本，返回位图画刷

	HBRUSH								CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void								InitImage();
	void								DestoryImage();
	void								Redraw();
	virtual void						PreSubclassWindow();
	afx_msg void						OnPaint();
	afx_msg void 						OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL 						OnEnChange();
	afx_msg void 						OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 						OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL 						OnEraseBkgnd(CDC* pDC);
	//以下为了解决光标  caret 位置问题
	afx_msg BOOL 						OnEnSetfocus();
	afx_msg void 						OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void						OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual LRESULT						WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void						OnNcPaint();
	afx_msg void 						OnSize(UINT nType, int cx, int cy);
	afx_msg void 						OnMove(int x, int y);
	void								DeleteBrush();
	
	Image*								m_pImgParentBk;
	Image*								m_pImgBk;
	Image*								m_pImgDisable;

public:
	BOOL								m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	COLORREF							m_textColor;			//文本颜色
	CString								m_GrayTipText;			//没有文本时的灰色提示文本字符串

	CString								m_pngParentBkPath;		//父背景图片
	CString								m_pngBkPath;			//背景图片
	CString								m_disablePngPath;		//禁用时显示图片		有效性：非强制有效


};

