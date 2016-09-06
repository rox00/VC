//********************************************************************
//	PNGRADIOBUTTON.H �ļ�ע��
//	�ļ��� : PNGRADIOBUTTON.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	���� : ����
//	����ʱ��: 2014/12/3 11:15
//	�ļ�����: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CPngRadioButton

class CPngRadioButton : public CButton, public CPngBase
{
	DECLARE_DYNAMIC(CPngRadioButton)

public:
	CPngRadioButton();
	virtual ~CPngRadioButton();

protected:
	DECLARE_MESSAGE_MAP()

	void						InitImage();
	void						DestoryImage();

	afx_msg BOOL 				OnEraseBkgnd(CDC* pDC);
	afx_msg void 				OnPaint();
	afx_msg HBRUSH				CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void 				OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 				OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void				OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void				OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT				OnSetCheck(WPARAM wParam, LPARAM lParam);
	virtual void				PreSubclassWindow();
	virtual LRESULT				WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	BOOL						m_bLButtonDown;		//����������OnMouseMove�з�ֹ�ػ�

	Image*						m_pImgParentBk;
	Image*						m_pImgBk;
	Image*						m_pImgRadioUnSel;
	Image*						m_pImgRadioSel;
	Image*						m_pImgDisable;

public:
	BOOL						m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	int							m_textOffsetX;			//�ı�ˮƽƫ��
	DWORD						m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF					m_textColor;			//�ı���ɫ

	CString						m_pngParentBkPath;		//������ͼƬ
	CString						m_pngBkPath;			//����ͼƬ

	CString						m_pngRadioUnSel;		//δѡ��ʱ��״̬ͼƬ
	CString						m_pngRadioSel;			//ѡ��ʱ��״̬ͼƬ
	CString						m_disablePngPath;		//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч
};


