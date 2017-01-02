//********************************************************************
//	PNGHEADERCTRL.H 文件注释
//	文件名		: PNGHEADERCTRL.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者		: 张永
//	创建时间	: 2014/12/29 15:29
//	文件描述	: 目前仅支持文字，如需要图标到时再进行扩展，请联系原作者
//*********************************************************************	
#pragma once

#include "Common/PngBase.h"

// CPngHeaderCtrl

class CPngHeaderCtrl : public CHeaderCtrl, public CPngBase
{
	DECLARE_DYNAMIC(CPngHeaderCtrl)

public:
	CPngHeaderCtrl();
	virtual ~CPngHeaderCtrl();

protected:
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg HBRUSH 							CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg HBRUSH 							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();
	afx_msg void							OnMouseMove(UINT nFlags, CPoint point);

	afx_msg LRESULT							OnLayout( WPARAM wParam, LPARAM lParam );
	
	int										m_divideSelIndex;
public:
	int										m_separatorWidth;		//分割条的宽度
	COLORREF								m_separatorColor;		//分割条的颜色

	int										m_textOffsetX;			//文本水平偏移
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF								m_textColor;			//字体颜色

	Image*									m_pImgBk;
	Image*									m_pImgHoverBk;
	Image*									m_pImgPushBk;
	Image*									m_pImgDisable;
};


