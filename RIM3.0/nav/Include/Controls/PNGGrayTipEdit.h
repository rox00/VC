//********************************************************************
//	PNGGRAYTIPEDIT.H �ļ�ע��
//	�ļ��� : PNGGRAYTIPEDIT.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	���� : ����
//	����ʱ��: 2014/12/3 11:14
//	�ļ�����: 
//*********************************************************************	

#pragma once
#include "Common/PngBase.h"

class CPNGGrayTipEdit : public CEdit, public CPngBase
{
// Construction
public:
	CPNGGrayTipEdit();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CPNGGrayTipEdit();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEditCtrl)
	bool								m_bEditChange;		
	CPoint								m_curCaretPt;	
	CBrush*								m_pBkBrush;		//Ϊ����ӦUNICODE�汾������λͼ��ˢ

	HBRUSH								CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void								InitImage();
	void								DestoryImage();
	void								Redraw();
	virtual void						PreSubclassWindow();
	afx_msg void						OnPaint();
	afx_msg void 						OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL 						OnEnChange();
	afx_msg void 						OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 						OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL 						OnEraseBkgnd(CDC* pDC);
	//����Ϊ�˽�����  caret λ������
	afx_msg BOOL 						OnEnSetfocus();
	afx_msg void 						OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void						OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual LRESULT						WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void						OnNcPaint();
	afx_msg void 						OnSize(UINT nType, int cx, int cy);
	afx_msg void 						OnMove(int x, int y);
	void								DeleteBrush();
	
	Image*								m_pImgParentBk;
	Image*								m_pImgBk;
	Image*								m_pImgDisable;

public:
	BOOL								m_bFillOrTitleParentPng;// �����ڵ���ͼģʽ Ĭ�����

	COLORREF							m_textColor;			//�ı���ɫ
	CString								m_GrayTipText;			//û���ı�ʱ�Ļ�ɫ��ʾ�ı��ַ���

	CString								m_pngParentBkPath;		//������ͼƬ
	CString								m_pngBkPath;			//����ͼƬ
	CString								m_disablePngPath;		//����ʱ��ʾͼƬ		��Ч�ԣ���ǿ����Ч


};

