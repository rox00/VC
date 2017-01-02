//********************************************************************
//	PNGTOOLTIPCTRL.H 文件注释
//	文件名 : PNGTOOLTIPCTRL.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	作者 : 张永
//	创建时间: 2014/12/3 11:12
//	文件描述: 
//*********************************************************************	

#pragma once

#include "PngBase.h"

// CPngToolTipCtrl

class CPngToolTipCtrl : public CToolTipCtrl, public CPngBase
{
	DECLARE_DYNAMIC(CPngToolTipCtrl)

public:
	CPngToolTipCtrl();
	virtual ~CPngToolTipCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void							PreSubclassWindow();
	afx_msg void							OnPaint();
	afx_msg HBRUSH							CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL							OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT							OnAdjustRect(WPARAM wParam, LPARAM lParam);

	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
public:

	COLORREF								m_textColor;
	Image*									m_pImgBk;
};


