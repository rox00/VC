#pragma once

// SVCBoxMonitorCtrl.h : Declaration of the CSVCBoxMonitorCtrl ActiveX Control class.


// CSVCBoxMonitorCtrl : See SVCBoxMonitorCtrl.cpp for implementation.
#include "CVedioDlg.h"
#include "Input/InputManager.h"
#include "Input/KeyboardMapper.h"
#include <map>
using namespace std;

class CSVCBoxMonitorCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSVCBoxMonitorCtrl)
	DECLARE_INTERFACE_MAP()
	DECLARE_OLECREATE_EX(CSVCBoxMonitorCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSVCBoxMonitorCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSVCBoxMonitorCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSVCBoxMonitorCtrl)		// Type name and misc status

	// Message maps
	DECLARE_MESSAGE_MAP()
	// Dispatch maps
	DECLARE_DISPATCH_MAP()
	// Event maps
	DECLARE_EVENT_MAP()
// Constructor
public:
	CSVCBoxMonitorCtrl();
	~CSVCBoxMonitorCtrl();
// Implementation
	afx_msg void AboutBox();
// Dispatch and event IDs
public:
	enum 
	{									
		dispidUpdateCollsepRc		= 14L,
		dispidUpdateDeviceSize		= 13L,
		dispidSetFlag				= 12L,
		dispidUpdateVedioRect		= 11L,
		dispidClearTouchedKey		= 10L,
		dispidExit					= 9L,
		dispidCallAndroidHomeKey	= 8L,
		dispidCallAndroidBack		= 7L,
		dispidOpenTaskManager		= 6L,
		dispidSetRunningApp			= 5L,
		dispidSaveKeyMappingConfig	= 4L,
		dispidSetKeyMappingOpen		= 3L,
		dispidLoadKeyMapping		= 2L,
		dispidStart					= 1L,

		eventidNotifyConnectStateChanged = 1L
	};

protected:
	/*接口*/
	ULONG						Start(ULONG bAutoReconnect, ULONG hwndVedio, LPCTSTR strIP);
	ULONG						LoadKeyMapping(LPCTSTR packageName);
	ULONG						SetKeyMappingOpen(SHORT open);
	ULONG						SaveKeyMappingConfig(LPCTSTR configFile, LPCTSTR content);
	ULONG						SetRunningApp(LPCTSTR packageName);
	ULONG						OpenTaskManager();
	ULONG						CallAndroidBackKey();
	ULONG						CallAndroidHomeKey();
	ULONG						Exit();
	void						ClearTouchedKey();
	void						SetFlag(LONG nFlag, LONG value);
	void						UpdateVedioRect(LONG l, LONG t, LONG r, LONG b);
	void						UpdateDeviceSize(LONG deviceWidth, LONG deviceHeight);
	void						UpdateCollsepRc(LONG screenLeft, LONG screenTop, LONG screenRight, LONG screenBottom);
	/*事件*/
	void NotifyConnectStateChanged(ULONG ulCurrentState)
	{
		FireEvent(eventidNotifyConnectStateChanged, EVENT_PARAM(VTS_UI4), ulCurrentState);
	}

	/*消息*/
	afx_msg int					OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void				OnSize(UINT nType, int cx, int cy);
	afx_msg void				OnDestroy();
	afx_msg LRESULT				OnConnectStateChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT				OnNotifyReStart(WPARAM wParam, LPARAM lParam);
	afx_msg void				OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void				OnClose();
	afx_msg BOOL				OnEraseBkgnd(CDC* pDC);
	afx_msg void				OnPaint();
	afx_msg void				OnNcPaint();
	/*方法*/
	void						BindMappingFunc();
	void						UnBindMappingFunc();
	void						PushFileToBox(LPCTSTR downladFile);
	String						UpdatePCFile(LPSTR file);
	void						ReStart();
private:
	BOOL						m_bAutoReconnect;			//断线重连
	boost::thread_group			m_WorkingThreadGroup;									////线程组
	boost::thread				*m_pThreadConnectProc;
	list<COPYDATASTRUCT>		m_copyDataVec;				//接收copyData的数据队列
	ConnectStatus				m_Status;
	BOOL						m_bExit;					//判断是否已经退出
	HWND						m_hwndVedio;				//显示视频的窗口

	CRect						m_CollapseScreenRect;		//按钮的屏幕度座标
	CRect						m_CollapseVedioClientRect;	//按钮相对于主窗口的客户区域座标
	CRect						m_vedioScreenRect;
	double						m_fCoordinateXMappingRatio;	//相对于盒子的比率
	double						m_fCoordinateYMappingRatio;	//相对于盒子的比率

	CVedioDlg					m_vedioDlg;
	//CBoxView					m_Vedio;
	KeyboardMapper				m_KeyboardMapper;

	std::wstring				m_className;				//用于日志打印的类名
	CString						m_strSDCardPath;			//SD卡的路径名
	std::string					m_strCurrentRunningApp;		//当前正在运行的APP的名称
	ULONG						ulAvailable;				//剩余空间
	ULONG						ulTotal;					//总空间
	USHORT						m_inputValue;				//当前是否启用输入文字的模式

	CString						m_strBoxPackageFile;		//盒子升级包，在盒子中的存放路径
	CString						m_ipStr;

public:

	COLORREF					GetWindowPixelColor(HWND hWnd, int x, int y);
	UINT						GetWindowColorValue(HWND hWnd);
	void						StartConnectProc();

	void						ClientToDeviceScreen(int &x, int &y);
	void						DeviceScreenToScreen(int &x, int &y);
protected:

};
