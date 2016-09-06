//********************************************************************
//	DOCKDLG.H 文件注释
//	文件名		: DOCKDLG.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/5 9:05
//	文件描述	: 用于显示停靠的窗口
//*********************************************************************	
#pragma once


// CDockDlg dialog

class CDockDlg : public CDialog
{
	DECLARE_DYNAMIC(CDockDlg)

public:
	CDockDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDockDlg();

// Dialog Data
	enum { IDD = IDD_DOCKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	Image*									m_pImgLogoTemp;				//用于绘制圆环中心区域的Logo图片
	bool									m_bMouseMove;			//用来判断鼠标是否移出本窗口
	BOOL									m_bDisplayChange;


	afx_msg void							OnMove(int x, int y);
	LRESULT									OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);
	virtual BOOL							OnInitDialog();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();
	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void							OnMouseLeave();
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
public:

	BYTE									m_wndAlphaValue;		//当前窗口的透明度
	//************************************
	// Method:    UpdateView
	// FullName:  CDockDlg::UpdateView
	// Access:    public 
	// Returns:   void
	// Qualifier:							更新Layerd主界面显示
	// Parameter: Image * pImageLogo		Logo图片背景
	// Parameter: byte Alpha				透明度值 默认255即不透明
	//************************************
	void									UpdateView(Image* pImageLogo, byte Alpha = 255);
};
