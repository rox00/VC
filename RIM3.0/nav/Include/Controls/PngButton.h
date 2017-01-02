//********************************************************************
//	PNGBUTTON.H �ļ�ע��
//	�ļ��� : PNGBUTTON.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	���� : ����
//	����ʱ��: 2014/12/3 11:14
//	�ļ�����: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"
// CPngButton

class CPngButton : public CButton, public CPngBase
{
	DECLARE_DYNAMIC(CPngButton)

public:
	CPngButton();
	virtual ~CPngButton();

protected:
	DECLARE_MESSAGE_MAP()
	bool										m_bLButtonDown;
	bool										m_bMouseMove;


	Image*										m_pImgParentBk;
	Image*										m_pImgBk;

	Image*										m_pImgNormal;
	Image*										m_pImgPush;
	Image*										m_pImgOver;
	Image*										m_pImgDisable;


protected:
	afx_msg HBRUSH								CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void 								OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void 								PreSubclassWindow();
	virtual void 								DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void 								OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 								OnMouseLeave();
	afx_msg void 								OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 								OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void 								OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL								OnEraseBkgnd(CDC* pDC);

	void										InitImage();
	void										DestoryImage();
	void										DrawButton(Image *pImage);
	virtual LRESULT								WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	BOOL										m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	DWORD										m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF									m_textColor;			//�ı���ɫRGB(255,0,0)

	CString										m_pngParentBkPath;		//������ͼƬ
	CString										m_pngBkPath;			//����ͼƬ

	CString										m_normalPngPath;		//��ʼ����ʾͼƬ		��Ч�ԣ���ǿ����Ч
	CString										m_pushPngPath;			//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч	
	CString										m_overPngPath;			//��꾭��ʱ��ʾͼƬ	��Ч�ԣ���ǿ����Ч
	CString										m_disablePngPath;		//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч

};


