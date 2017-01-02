//********************************************************************
//	PNGCONTROLSCROLL.H �ļ�ע��
//	�ļ��� : PNGCONTROLSCROLL.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	���� : ����
//	����ʱ��: 2014/12/3 11:12
//	�ļ�����: 
//*********************************************************************	

#pragma once

#include "PngBase.h"
/////////////////////////////////////////////////////////////////////////////
// CPngControlScroll window
//���ؼ�ֻ����Ϊ�ؼ��Ĺ�����
class CPngControlScroll : public CStatic, public CPngBase
{
	// Construction
public:
	CPngControlScroll();
	virtual ~CPngControlScroll();

	void 										DrawVScroll(CDC* pDC);
	void 										DrawHScroll(CDC* pDC);
protected:
	CWnd*										m_pCtrl;
	UINT										m_nScrollStyle;

	DECLARE_DYNAMIC(CPngControlScroll)

	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH								OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void								OnPaint();
	afx_msg BOOL								OnEraseBkgnd(CDC* pDC);
	afx_msg int									OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void								OnNcPaint();


public:
	virtual BOOL 								InitScrollBar(CWnd *pCtrl, UINT nStyle);
	void 										UpdateScrollBar();

	BOOL										m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	Image*										m_pImgParentBk;

	Image*										m_pImgScrollBk;
	Image*										m_pImgVScrollTop;
	Image*										m_pImgVScrollBot;
	Image*										m_pImgVScrollThumb;
	Image*										m_pImgHScrollLeft;
	Image*										m_pImgHScrollRight;
	Image*										m_pImgHScrollThumb;

};
