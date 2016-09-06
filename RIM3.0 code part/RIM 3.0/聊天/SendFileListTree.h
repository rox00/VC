#pragma once


// CSendFileListTree
#define SENDFILELISTTREE_SENDSTR	_T("send")
#define SENDFILELISTTREE_RECVSTR	_T("recv")
class CSendFileListTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CSendFileListTree)

public:
	CSendFileListTree();
	virtual ~CSendFileListTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void 								OnPaint();
	afx_msg BOOL 								OnEraseBkgnd(CDC* pDC);
	afx_msg void 								OnNcPaint();
	afx_msg void 								OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 								OnMouseLeave();
	afx_msg void								OnLButtonDown(UINT nFlags, CPoint point);
};


