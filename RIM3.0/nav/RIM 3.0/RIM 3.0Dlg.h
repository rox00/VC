//********************************************************************
//	RIM 3.0DLG.H 文件注释
//	文件名		: RIM 3.0DLG.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/5 9:04
//	文件描述	: 用于显示背景的窗口
//*********************************************************************	
// RIM30Dlg.h : 头文件
//

#pragma once

#include "LogonWndDlg.h"
#include "CircleDlg.h"
#include "DockDlg.h"
#include "MainListDlg.h"

// CRIM30Dlg 对话框
class CRIM30Dlg : public CDialog
{
// 构造
public:
	CRIM30Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~CRIM30Dlg();	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RIM30_DIALOG };

	protected:
	virtual void							DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	static CRIM30Dlg*						m_pSizeWnd;
	static LRESULT CALLBACK					ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK					WndProcHook(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK					MouseHook(int code, WPARAM wParam, LPARAM lParam);



	////【zy 2014-11-20 ↓】显示部分
	/************************************************************************************************************************************************
	显示通过操作类型主要分为三种模式：
	SHOWMODE_DOCK为在桌面客户区停靠， 仅控制所显示的窗口的隐藏和显示。
	SHOWMODE_MENU为通过右键菜单响应命令的显示模式，控制所有窗口显示和隐藏。
	SHOWMODE_MAIN主要为双击椭圆区来判定是否显示主界面。
	SHOWMODE_MAIN的特殊性：m_bShowMainImg用来记录主界面是否显示，用于避免SHOWMODE_DOCK与SHOWMODE_MENU两种模式在显示与隐藏之间产生误差。

	m_clientBound m_clientcircleBound主要用于在停靠时通过m_bShowMainImg来判定主界面的移动区域及鼠标可在主界面的活动范围
	************************************************************************************************************************************************/
	
	enum WNDDOCK{							//用于停靠功能
		DOCK_NORMAL		,					//
		DOCK_LEFT		,
		DOCK_TOP		,
		DOCK_RIGHT		,
		DOCK_BOTTOM 	,
	};
	WNDDOCK									m_dockType;

 	enum SHOWOPERA{							//不同操作下 不同的显示模式
 		SHOWOPERA_DOCK	,					//实时检测停靠功能更新显示界面
 		SHOWOPERA_MENU	,					//菜单控制显示与隐藏
		SHOWOPERA_MAIN	,					//主要区别为主界面的显示问题 非圆环部分
		SHOWOPERA_LOGON	,					//登录更新显示
 	};

	BOOL									m_bShowMainImg;								//判断主窗口是否在显示
	CRect									m_clientRect;								//整个圆环矩形与主分录界面的矩形的合集，相对于客户区的位置，主要用来计算移动时的界问题
	CRect									m_clientcircleRect;							//圆环矩形相对于客户区的位置，主要用于计算在隐藏主窗口时，移动时的边界问题

	////【zy 2014-11-20 ↑】


	BOOL									m_bBalloonShow;
	HICON									m_hIcon;
	NOTIFYICONDATA							m_icondata;									//托盘图标

	// 生成的消息映射函数
	virtual BOOL 							OnInitDialog();
	afx_msg void 							OnPaint();
	afx_msg HCURSOR							OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL							OnEraseBkgnd(CDC* pDC);
	afx_msg void							OnSize(UINT nType, int cx, int cy);
	afx_msg void							OnMove(int x, int y);
	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void 							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void 							OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void							OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void							OnMouseLeave();
	afx_msg void							OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void							OnDestroy();
	afx_msg LRESULT							OnNcHitTest(CPoint point);
	afx_msg void							OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void							OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void							OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void							OnClose();
	afx_msg void							OnTimer(UINT_PTR nIDEvent);

	//响应最大化，最小化，关闭等，用于在所有窗口显示间进行切换
	virtual BOOL							OnCommand(WPARAM wParam, LPARAM lParam);

	//************************************
	// Method:    RepositionSubDlg
	// FullName:  CRIM30Dlg::RepositionSubDlg
	// Access:    protected 
	// Returns:   void
	// Qualifier:							调整子窗口的位置
	// Parameter: bool bRedraw				调整后，是否重画子窗口
	//************************************
	void									RepositionSubDlg(bool bRedraw = false);		//调整子窗口的位置
	//************************************
	// Method:    PtInWindow
	// FullName:  CRIM30Dlg::PtInWindow
	// Access:    protected 
	// Returns:   BOOL
	// Qualifier:							判断当前鼠标位置 是否在当前界面可见像素所在的区域内
	// Parameter: CPoint scrPoint			鼠标屏幕位置。
	//************************************
	BOOL									PtInWindow(CPoint scrPoint);
	//************************************
	// Method:    PtInDock
	// FullName:  CRIM30Dlg::PtInDock
	// Access:    protected 
	// Returns:   BOOL						TRUE：存在于该区域内   FALSE:存在于该区域外
	// Qualifier:							判断当前鼠标位置 是否在停靠对话框所在的区域内
	// Parameter: CPoint scrPoint			鼠标屏幕位置。
	//************************************
	BOOL									PtInDock(CPoint scrPoint);

	//************************************
	// Method:    OnDockBound
	// FullName:  CRIM30Dlg::OnDockBound
	// Access:    protected 
	// Returns:   LRESULT					默认返回TRUE
	// Qualifier:							对话框停靠显示间进行切换
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	//************************************
	afx_msg LRESULT							OnShowDockBound(WPARAM wParam, LPARAM lParam);

	//************************************
	// Method:    OnShowMianWnd
	// FullName:  CRIM30Dlg::OnShowMianWnd
	// Access:    protected 
	// Returns:   LRESULT					默认返回TRUE
	// Qualifier:							响应WM_SHOWMAINWND消息 在主窗口显示与隐藏之间进行切换
	// Parameter: WPARAM wParam				保留
	// Parameter: LPARAM lParam				保留
	//************************************
	afx_msg LRESULT							OnShowMianWnd(WPARAM wParam, LPARAM lParam);
	//************************************
	// Method:    OnAlredyLogon
	// FullName:  CRIM30Dlg::OnAlredyLogon
	// Access:    protected 
	// Returns:   LRESULT					默认返回TRUE
	// Qualifier:							响应WM_ALREDYLOGON消息 判断是否登录
	// Parameter: WPARAM wParam				0  登录失败  1  登录成功  
	// Parameter: LPARAM lParam				保留
	//************************************
	afx_msg LRESULT							OnAlredyLogon(WPARAM wParam, LPARAM lParam);


	//************************************
	// Method:    UpdateView
	// FullName:  CRIM30Dlg::UpdateView
	// Access:    protected 
	// Returns:   void
	// Qualifier:							更新Layerd主界面显示, 由于主界面需要上下拉伸，固主矩形区域分为三部分绘制
	// Parameter: Image * pImageMain		登录界面主矩形区域上部分所要绘制的图片
	// Parameter: Image * pImageSizeMid		登录界面主矩形区域中间部分所要绘制的图片
	// Parameter: Image * pImageSizeBttom	登录界面主矩形区域下部分所要绘制的图片
	// Parameter: Image * pImageCircle		圆环界面区域所要绘制的图片
	// Parameter: Image * pImageLogo		圆环区域中心部分所要绘制的Logo图片
	// Parameter: byte Alpha				整体更新图片的透明度
	//************************************
	void									UpdateView(byte Alpha = 255);

	//************************************
	// Method:    ShowDock
	// FullName:  CRIM30Dlg::ShowDock
	// Access:    protected 
	// Returns:   void
	// Qualifier:							用于在不同显示模式之间执行切换显示
	// Parameter: SHOWOPERA shwoOpera		执行该操作时，所进行的操作类型
	// Parameter: int nCmdShow				显示方式。 SW_SHOW、SW_HIDE
	//************************************
	void									ShowMode(SHOWOPERA shwoOpera, BOOL bShow);
	
	//************************************
	// Method:    OnIconMsg
	// FullName:  CRIM30Dlg::OnIconMsg
	// Access:    protected 
	// Returns:   LRESULT
	// Qualifier:							用于响应托盘鼠标的的鼠标事件
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	//************************************
	afx_msg LRESULT							OnIconMsg(WPARAM wParam, LPARAM lParam);


	afx_msg LRESULT							OnUpdateConfig(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnUpdateUserList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnUpdateStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnFlickerNotifyIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnRepositionSubdlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT							OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);

	void									Init();
	bool									Exit();
protected:

	Image*									m_pImgLogonTemp;		//登录界面图片绘制在主界面矩形的上部
	Image*									m_pImgSizeMidTemp;		//用于拉伸时绘制中间部分的PNG图片
	Image*									m_pImgSizebottomTemp;	//用于拉伸时绘制底部的PNG图片
	Image*									m_pImgMainListTemp;		//用于绘制登录后主界面矩形上部的PNG图片
	Image*									m_pImgLogoTemp;			//用于绘制圆环中心区域的Logo图片
	Image*									m_pImgCircleTemp;		//用于绘制圆环区域的图片
	Image*									m_pImgChkSelTemp;		//用于绘制下部按钮选中状态
	Image*									m_pImgChkUnSelTemp;		//用于绘制下部按钮未选中状态


	BOOL									m_bAlreadyLogon;		//是否已经登录成功
	BYTE									m_wndAlphaValue;		//本窗口的透明度值 因为本窗口使用UpdateView的窗口，固需要记录更新透明值

	CDockDlg								m_dockDlg;				//停靠对话框
	CLogonWndDlg							m_logonDlg;				//登录对话框
	CCircleDlg								m_circleDlg;			//圆环对话框
	CMainListDlg							m_mainListDlg;			//用户及站点列表对话框


	HRGN									m_hRgnCircle;			//大圆相对于客户区的区域
	HRGN									m_hRgnMain;				//登录及用户列表所在的相对于客户区的区域
	HRGN									m_hRgnLogo;				//logo相对于客户区的区域
	CRgn									m_MainPixelRgn;			//当前显示的像素区域 包括间隙。

	int										m_selMode;				//判断当前选中的是监测站还是联系人界面  0代表站点区域， 1代表用户列表 2代表最近使用列表
	Rect									m_rcStation;			//站点区域列表		绘图区域
	Rect									m_rcUser;				//用户列表			选项的绘图区域
	Rect									m_rcRecent;				//最近使用			选项的绘图区域
public:
};

