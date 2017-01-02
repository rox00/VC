//********************************************************************
//	PNGSTATIC.H �ļ�ע��
//	�ļ��� : PNGSTATIC.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	���� : ����
//	����ʱ��: 2014/12/3 11:15
//	�ļ�����: 
//*********************************************************************	

#pragma once

#include "Common/PngBase.h"

// CTransparentStatic

class CPngStatic : public CStatic, public CPngBase
{
	DECLARE_DYNAMIC(CPngStatic)

public:
	CPngStatic();
	virtual ~CPngStatic();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH							CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL							OnEraseBkgnd(CDC* pDC);
	afx_msg void							OnPaint();
	void									InitImage();
	void									DestoryImage();
	virtual void							PreSubclassWindow();
	virtual LRESULT							WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	Image*									m_pImgParentBk;


public:
	BOOL									m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	int										m_textOffsetX;
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	CString									m_pngParentBkPath;		//������ͼƬ
	COLORREF								m_textColor;			//�ı���ɫRGB(255,0,0)
};


