#pragma once

#include "Common/PngBase.h"

#include "Common/PngComListBox.h"
#include "PNGGrayTipEdit.h"

class CPngComboBox : public CComboBox, public CPngBase
{
public:
	CPngComboBox();
	virtual ~CPngComboBox();

protected:
	void									InitImage();
	void									DestoryImage();

	afx_msg LRESULT							OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnCtlColorListEdit(WPARAM wParam, LPARAM lParam);
	virtual void							DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void 							OnPaint();
	afx_msg void 							OnDestroy();
	virtual void							PreSubclassWindow();
	afx_msg int								OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void 							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 							OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void 							OnCbnDropdown();
	afx_msg void 							OnCbnCloseup();
	afx_msg void 							OnNcPaint();
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	virtual LRESULT							WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void							MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);


	DECLARE_MESSAGE_MAP()

	CPNGGrayTipEdit							m_edit;
	CPngComListBox							m_listbox;

	BOOL									bCloseUPLast;	//�ո��Ƿ�ر�

	Image*									m_pImgComboLeft;
	Image*									m_pImgComboCenter; 
	Image*									m_pImgComboRight;	

	Image*									m_pImgListLeft;	
	Image*									m_pImgListTop;		
	Image*									m_pImgListRight;
	Image*									m_pImgListBottom;
	Image*									m_pImgListCenter;
	Image*									m_pImgListSel;

public:
	DWORD									m_textAlignMode;		//DT_SINGLELINE | DT_VCENTER | DT_CENTER
	COLORREF								m_textColor;			//RGB(255,0,0)
	int										m_nTextOffsetX;			//�������� ����ƫ��������

	CString									m_leftPngPath;			//COMBOX  ���
	CString									m_centerPngPath;		//COMBOX  �м�
	CString									m_rightPngPath;			//COMBOX  �ұ�

	CString									m_listLeftPngPath;		//COMBOX  �б����
	CString									m_listTopPngPath;		//COMBOX  �б���
	CString									m_listRightPngPath;		//COMBOX  �б��Ҳ�
	CString									m_listBottomPngPath;	//COMBOX  �б�ײ�
	CString									m_listCenterPngPath;	//COMBOX  �б�ײ�
	CString									m_listSelPngPath;		//COMBOX  �б�ѡ��״̬

	//ScrollPng
	CString									m_pngScrollBk;			//�������ı��� ˮƽ�봹ֱ
	CString									m_pngVScrollTop;		//��ֱ���������ϼ�ͷ
	CString									m_pngVScrollBot;		//��ֱ���������¼�ͷ
	CString									m_pngVScrollThumb;		//��ֱ�������Ļ���
	CString									m_pngHScrollLeft;		//ˮƽ���������ϼ�ͷ
	CString									m_pngHScrollRight;		//ˮƽ���������¼�ͷ
	CString									m_pngHScrollThumb;		//ˮƽ�������Ļ���

	CString									m_editPngBkPath;		//Edit  �༭�򱳾�

};

