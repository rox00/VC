//********************************************************************
//	PNGLISTBOX.H �ļ�ע��
//	�ļ���		: PNGLISTBOX.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	����		: ����
//	����ʱ��	: 2014/12/22 15:32
//	�ļ�����	: 
//*********************************************************************	
#pragma once

#include "Common\\PngControlScroll.h"

// CPngListBox

class CPngListBox : public CListBox, public CPngBase
{
	DECLARE_DYNAMIC(CPngListBox)

public:
	CPngListBox();
	virtual ~CPngListBox();

protected:
	CPngControlScroll						m_HScroll;
	CPngControlScroll						m_VScroll;
	BOOL									m_bScrollInit;

	DECLARE_MESSAGE_MAP()

	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH							CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void							OnPaint();
	afx_msg void							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void							OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void							OnMouseLeave();
	afx_msg void							OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void							OnTimer(UINT_PTR nIDEvent);
	virtual void							PreSubclassWindow();
	afx_msg int								OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void 							OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void 							OnSize(UINT nType, int cx, int cy);

	void									InitImage();
	void									DestoryImage();
	void									DrawClient(CDC* pDC);
	CRect									m_vTopRect;
	CRect									m_vBotRect;
	CRect									m_vThumbRect;
	CRect									m_hLeftRect;
	CRect									m_hRightRect;
	CRect									m_hThumbRect;

	BOOL									m_bMouseMove;

	Image*									m_pImgParentBk;
	Image*									m_pImgBk;
	Image*									m_pImgListOver;
	Image*									m_pImgListSel;
	Image*									m_pImgDisable;

	//ScrollPng
	Image*									m_pImgScrollBk;
	Image*									m_pImgVScrollTop;
	Image*									m_pImgVScrollBot;
	Image*									m_pImgVScrollThumb;
	Image*									m_pImgHScrollLeft;
	Image*									m_pImgHScrollRight;
	Image*									m_pImgHScrollThumb;
public:

	COLORREF								m_textColor;			//RGB(255,0,0)
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	int										m_nTextOffsetX;			//�������� ����ƫ��������

	BOOL									m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	CString									m_pngParentBkPath;		//������ͼƬ
	CString									m_pngBkPath;
	CString									m_pngOverPath;			//��꾭��ʱ��ͼƬ
	CString									m_pngSelPath;			//���ѡ��ʱ��ͼƬ
	CString									m_disablePngPath;		//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч


	//ScrollPng
	CString									m_pngScrollBk;			//�������ı��� ˮƽ�봹ֱ
	CString									m_pngVScrollTop;		//��ֱ���������ϼ�ͷ
	CString									m_pngVScrollBot;		//��ֱ���������¼�ͷ
	CString									m_pngVScrollThumb;		//��ֱ�������Ļ���
	CString									m_pngHScrollLeft;		//ˮƽ���������ϼ�ͷ
	CString									m_pngHScrollRight;		//ˮƽ���������¼�ͷ
	CString									m_pngHScrollThumb;		//ˮƽ�������Ļ���

	afx_msg void							OnNcPaint();
};


