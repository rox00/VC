//********************************************************************
//	PNGHEADERCTRL.H �ļ�ע��
//	�ļ���		: PNGHEADERCTRL.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	����		: ����
//	����ʱ��	: 2014/12/29 15:29
//	�ļ�����	: Ŀǰ��֧�����֣�����Ҫͼ�굽ʱ�ٽ�����չ������ϵԭ����
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
	int										m_separatorWidth;		//�ָ����Ŀ��
	COLORREF								m_separatorColor;		//�ָ�������ɫ

	int										m_textOffsetX;			//�ı�ˮƽƫ��
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF								m_textColor;			//������ɫ

	Image*									m_pImgBk;
	Image*									m_pImgHoverBk;
	Image*									m_pImgPushBk;
	Image*									m_pImgDisable;
};


