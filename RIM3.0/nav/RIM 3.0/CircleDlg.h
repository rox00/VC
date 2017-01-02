//********************************************************************
//	CIRCLEDLG.H 文件注释
//	文件名		: CIRCLEDLG.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/5 9:05
//	文件描述	: 用于显示圆环窗口
//*********************************************************************	
#pragma once


// CCircleDlg dialog

class CCircleDlg : public CDialog
{
	DECLARE_DYNAMIC(CCircleDlg)

public:
	CCircleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCircleDlg();

// Dialog Data
	enum { IDD = IDD_CIRCLEDLG };

protected:
	virtual void							DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg LRESULT							OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();

	virtual BOOL 							OnInitDialog();
	afx_msg void 							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 							OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL 							OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void 							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT							OnNcHitTest(CPoint point);
	afx_msg void							OnSize(UINT nType, int cx, int cy);
	afx_msg void							OnMouseLeave();
	afx_msg void 							OnDestroy();
	virtual BOOL 							PreTranslateMessage(MSG* pMsg);
	afx_msg void							OnMove(int x, int y);

	//************************************
	// Method:    RepositionSubMoudle
	// FullName:  CCircleDlg::RepositionSubMoudle
	// Access:    protected 
	// Returns:   void
	// Qualifier:							调整子系统按钮的位置
	// Parameter: float offAngle			整个子系统队列偏移的角度  （所有子系统之间以固定角度间隔）
	//************************************
	void									RepositionSubMoudle(float offAngle = 0);
	//************************************
	// Method:    OpenSubSys
	// FullName:  CCircleDlg::OpenSubSys
	// Access:    protected 
	// Returns:   void
	// Qualifier:							打开子系统模块
	//************************************
	void									OpenSubSys(LPSUBMOUDLEINFO pSubMoudleInfo);
	//************************************
	// Method:    Init
	// FullName:  CCircleDlg::Init
	// Access:    protected 
	// Returns:   void
	// Qualifier:							本对话框的初始化函数，仅可调用一次
	//************************************
	void									Init();
	//************************************
	// Method:    InitMoudleShowArray
	// FullName:  CCircleDlg::InitMoudleShowArray
	// Access:    protected 
	// Returns:   void
	// Qualifier:							初始化显示数组  m_moudleShowArray
	//************************************
	void									ReInitMoudleShowArray();

	BOOL									m_bDisplayChange;
	Image*									m_pImgLogoTemp;			//用于绘制圆环中心区域的Logo图片
	Image*									m_pImgCircleTemp;		//用于绘制圆环区域的图片
	vector<LPSUBMOUDLEINFO>					m_moudleShowArrayTemp;	//只用来预存配置文件的数组指针，不需手动析构

	bool									m_bMouseMove;			//用来判断鼠标是否移出本窗口
	bool									m_bShowMainWnd;			//用于判断发送显示和隐藏主窗口

	CRgn									m_wndCircleRgn;			//圆环在本窗口中所占的区域
	CRgn									m_wndLogoRgn;			//Logo在本窗口中所占在区域
	CRgn									m_wndRoleRgn;			//环道在本窗口中所占的区域
	CRgn									m_wndRoleRgnAlpha;		//比m_wndRoleRgn向内圆延伸2个像素
	CRgn									m_wndUnRoleRgn;			//圆环区域与环道的异或，即除了环道及环道外边框的象素区域

	SUBMOUDLEINFO							*m_pSubMoudleInfoHover;	//鼠标当前停留的按钮
	double									m_offSetAngle;			//整个子系统图标队列偏移的角度 顺时针为正，逆时针为负
	BOOL									m_bDrag;				//当前是否处于拖动状态
	CPoint									m_dragPt;				//当前拖动的鼠标坐标


	float									m_startAngle;			//摆放按钮的起始角度
	float									m_stepAngle;			//每个按钮之间的间隔角度
	float									m_angleMax;				//最在的摆放按钮的角度

	HCURSOR									m_handDo;				//抓起图标
	HCURSOR									m_handUndo;				//未抓起图标
	CToolTipCtrl							m_pngToolTipCtrl;

	CString									m_showText;
public:
	BOOL									m_bAlreadyLogon;		//是否已经登录成功

	BYTE									m_wndAlphaValue;		//兼容XP获取不准确的问题
	void									SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	void									ReUpdateFromConfig();
	void									DoLogon(BOOL bLogon);
};
