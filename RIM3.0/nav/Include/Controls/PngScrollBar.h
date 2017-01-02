//********************************************************************
//	PNGSCROLLBAR.H �ļ�ע��
//	�ļ��� : PNGSCROLLBAR.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	���� : ����
//	����ʱ��: 2014/12/3 11:15
//	�ļ�����: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CPngScrollBar

class CPngScrollBar : public CScrollBar, public CPngBase
{
	DECLARE_DYNAMIC(CPngScrollBar)

public:
	CPngScrollBar();
	virtual ~CPngScrollBar();
	//�˴����ú������Ƽ��������ô�

	CRect						m_vTopRect;
	CRect						m_vBotRect;
	CRect						m_vThumbRect;
	CRect						m_hLeftRect;
	CRect						m_hRightRect;
	CRect						m_hThumbRect;

	void 						InitImage();
	void						DestoryImage();
	void 						DrawHScroll(CDC* pDC);
	void 						DrawVScroll(CDC* pDC);
	BOOL						SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
	int							SetScrollPos(int nPos, BOOL bRedraw = TRUE);
	void						SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
	UINT						OnHitTest(CPoint point);
protected:

	CDC							parentMemDC;
	CBitmap						parentMemBmp;

	DECLARE_MESSAGE_MAP()

	Image*						m_pImgParentBk;
	Image*						m_pImgScrollBk;
	Image*						m_pImgDisable;


	Image*						m_pImgVScrollTop;
	Image*						m_pImgVScrollBot;
	Image*						m_pImgVScrollThumb;
	Image*						m_pImgHScrollLeft;
	Image*						m_pImgHScrollRight;
	Image*						m_pImgHScrollThumb;


	afx_msg HBRUSH				OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 				OnEraseBkgnd(CDC* pDC);
	afx_msg void 				OnPaint();


	afx_msg int					OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void 				OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 				OnLButtonDblClk(UINT nFlags, CPoint point);

public:
	BOOL						m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	/////////////////���Žӿ�
	CString						m_pngParentBkPath;		//������ͼƬ
	CString						m_disablePngPath;		//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч

	//ScrollPng
	CString						m_pngScrollBk;			//����������ͼƬ
	CString						m_pngVScrollTop;		//��ֱ���������ϼ�ͷ
	CString						m_pngVScrollBot;		//��ֱ���������¼�ͷ
	CString						m_pngVScrollThumb;		//��ֱ�������Ļ���
	CString						m_pngHScrollLeft;		//ˮƽ���������ϼ�ͷ
	CString						m_pngHScrollRight;		//ˮƽ���������¼�ͷ
	CString						m_pngHScrollThumb;		//ˮƽ�������Ļ���
};


