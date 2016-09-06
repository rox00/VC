//********************************************************************
//	PNGLISTCTRL.H �ļ�ע��
//	�ļ���		: PNGLISTCTRL.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	����		: ����
//	����ʱ��	: 2014/12/29 15:30
//	�ļ�����	: Ŀǰ��֧�����֣�����Ҫͼ�굽ʱ�ٽ�����չ������ϵԭ����
//*********************************************************************	
#pragma once

#include "Common/PngBase.h"

#include "Common\\PngControlScroll.h"
#include "PngHeaderCtrl.h"
// CPngListCtrl

class CPngListCtrl : public CListCtrl, public CPngBase
{
	DECLARE_DYNAMIC(CPngListCtrl)

public:
	CPngListCtrl();
	virtual ~CPngListCtrl();


	BOOL								m_bScrollInit;
	CPngHeaderCtrl						m_Header;

	CPngControlScroll					m_HScroll;
	CPngControlScroll					m_VScroll;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH 						CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg HBRUSH 						OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 						OnEraseBkgnd(CDC* pDC);
	afx_msg void 						OnPaint();
	afx_msg int							OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void						OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT 					OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT 					OnPrint(WPARAM wParam, LPARAM lParam);
protected:
	virtual void						PreSubclassWindow();
	afx_msg void 						OnNcPaint();
	afx_msg void 						OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 						OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void 						OnSize(UINT nType, int cx, int cy);
	void								InitImage();
	void								DestoryImage();

	Image*								m_pImgParentBk;
	Image*								m_pImgBk;
	Image*								m_pImgDisable;

	//Header
	Image*								m_pImgHeaderBk;
	Image*								m_pImgHeaderItemOverBk;
	Image*								m_pImgHeaderItemPushBk;
	Image*								m_pImgHeaderDisable;

	//ScrollPng
	Image*								m_pImgScrollBk;
	Image*								m_pImgVScrollTop;
	Image*								m_pImgVScrollBot;
	Image*								m_pImgVScrollThumb;
	Image*								m_pImgHScrollLeft;
	Image*								m_pImgHScrollRight;
	Image*								m_pImgHScrollThumb;	


public:
	BOOL								m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	CString								m_pngParentBkPath;		//������ͼƬ
	CString								m_pngBkPath;			//����ͼƬ
	CString								m_disablePngPath;		//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч

	//Header
	int									m_headerSeparatorWidth;	//�ָ����Ŀ��
	COLORREF							m_headerSeparatorColor;	//�ָ�������ɫ
	int									m_headerTextOffsetX;	//�ı�ˮƽƫ��
	DWORD								m_headerTextAlignMode;	//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF							m_headerTextColor;		//������ɫ

	CString								m_pngHeaderBk;			//ͷ�ؼ�����ı���ͼƬ
	CString								m_pngHeaderItemOverBk;	//ͷ�ؼ���꾭��ʱ�ı���ͼƬ
	CString								m_pngHeaderItemPushBk;	//ͷ�ؼ���갴��ʱ�ı���ͼƬ
	CString								m_pngHeaderDisable;		//ͷ�ؼ�������ʱ�ı���ͼƬ

	//ScrollPng
	CString								m_pngScrollBk;			//�������ı��� ˮƽ�봹ֱ
	CString								m_pngVScrollTop;		//��ֱ���������ϼ�ͷ
	CString								m_pngVScrollBot;		//��ֱ���������¼�ͷ
	CString								m_pngVScrollThumb;		//��ֱ�������Ļ���
	CString								m_pngHScrollLeft;		//ˮƽ���������ϼ�ͷ
	CString								m_pngHScrollRight;		//ˮƽ���������¼�ͷ
	CString								m_pngHScrollThumb;		//ˮƽ�������Ļ���
};


