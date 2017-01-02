//********************************************************************
// RIMTREECTRL.H �ļ�ע��
// �ļ��� ��: RIMTREECTRL.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\NAV\RIM 3.0/
// ���� ����: ����
// ����ʱ�� : 2015/4/30 10:11
// �ļ����� : 
//*********************************************************************
#pragma once
#include "InfoTipDlg.h"


// CRIMTreeCtrl
#define TVN_ITEMHOVERCHANGE	TVN_LAST + 1
typedef struct tagITEMHOVER
{
	NMHDR			hdr;
	HTREEITEM		m_hHoverOldItem;
	HTREEITEM		m_hHoverNewItem;
}ITEMHOVER, *LPITEMHOVER;

class CRIMTreeCtrl : public CPngTreeCtrl
{
	DECLARE_DYNAMIC(CRIMTreeCtrl)

public:
	CRIMTreeCtrl();
	virtual ~CRIMTreeCtrl();

	HTREEITEM							m_hHoverItem;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void						OnPaint();
	afx_msg void						OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void						OnMouseLeave();
};


