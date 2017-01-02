//********************************************************************
//	PNGSTATIC.H 文件注释
//	文件名 : PNGSTATIC.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:15
//	文件描述: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CTransparentStatic

class CPngStatic : public CStatic, public CPngBase
{
	DECLARE_DYNAMIC(CPngStatic)

public:
	CPngStatic();
	virtual ~CPngStatic();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH							CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL							OnEraseBkgnd(CDC* pDC);
	afx_msg void							OnPaint();
	void									InitImage();
	void									DestoryImage();
	virtual void							PreSubclassWindow();
	virtual LRESULT							WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	Image*									m_pImgParentBk;


public:
	BOOL									m_bFillOrTitleParentPng;// 父窗口的贴图模式 默认填充

	int										m_textOffsetX;
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	CString									m_pngParentBkPath;		//父背景图片
	COLORREF								m_textColor;			//文本颜色RGB(255,0,0)
};


