//********************************************************************
//	PNGTREECTRL.H �ļ�ע��
//	�ļ��� : PNGTREECTRL.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	���� : ����
//	����ʱ��: 2014/12/3 11:15
//	�ļ�����: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CPngTreeCtrl

#include "Common\\PngControlScroll.h"
#include "Common\\PngToolTipCtrl.h"

class CPngTreeCtrl : public CTreeCtrl, public CPngBase
{
	DECLARE_DYNAMIC(CPngTreeCtrl)

public:
	CPngTreeCtrl();
	virtual ~CPngTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	CPngControlScroll					m_HScroll;
	CPngControlScroll					m_VScroll;
	BOOL								m_bScrollInit;
	CPngToolTipCtrl						m_pngToolTipCtrl;

	void								InitImage();
	void								DestoryImage();
	afx_msg void 						OnPaint();
	afx_msg BOOL 						OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH						CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void						OnMouseMove(UINT nFlags, CPoint point);
	virtual void 						PreSubclassWindow();
	virtual BOOL						PreTranslateMessage(MSG* pMsg);
	//Scroll
	afx_msg BOOL						OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void 						OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void 						OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void						OnNcPaint();
	afx_msg int							OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void						OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void 						OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 						OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void 						OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void 						OnSize(UINT nType, int cx, int cy);

	Image*								m_pImgParentBk;
	Image*								m_pImgBk;

	Image*								m_pImgButtonSel;
	Image*								m_pImgButtonUnSel;
	Image*								m_pImgChkSel;
	Image*								m_pImgChkUnSel;
	Image*								m_pImgItemOver;
	Image*								m_pImgItemSel;
	Image*								m_pImgDisable;

	//ScrollPng
	Image*								m_pImgScrollBk;
	Image*								m_pImgVScrollTop;
	Image*								m_pImgVScrollBot;
	Image*								m_pImgVScrollThumb;
	Image*								m_pImgHScrollLeft;
	Image*								m_pImgHScrollRight;
	Image*								m_pImgHScrollThumb;

	//tooltippng
	Image*								m_pImgToolTip;

public:
	BOOL								m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����


	int									m_textOffsetX;			//�ı�ˮƽƫ��
	DWORD								m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF							m_textColor;			//������ɫ


	CString								m_pngParentBkPath;		//������ͼƬ
	CString								m_pngBkPath;			//����ͼƬ

	CString								m_pngItemBtnSel;		//HasButton���
	CString								m_pngItemBtnUnSel;		//HasButton���
	CString								m_pngItemChkSel;		//chekbox���
	CString								m_pngItemChkUnSel;		//chekbox���
	CString								m_pngItemOver;			//��꾭��ʱ��ͼƬ
	CString								m_pngItemSel;			//ѡ��ʱ��ͼƬ
	CString								m_disablePngPath;		//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч

	//ScrollPng
	CString								m_pngScrollBk;			//�������ı��� ˮƽ�봹ֱ
	CString								m_pngVScrollTop;		//��ֱ���������ϼ�ͷ
	CString								m_pngVScrollBot;		//��ֱ���������¼�ͷ
	CString								m_pngVScrollThumb;		//��ֱ�������Ļ���
	CString								m_pngHScrollLeft;		//ˮƽ���������ϼ�ͷ
	CString								m_pngHScrollRight;		//ˮƽ���������¼�ͷ
	CString								m_pngHScrollThumb;		//ˮƽ�������Ļ���

	//tooltippng
	CString								m_pngTooltipBk;			//������ʾ���ı���
};


