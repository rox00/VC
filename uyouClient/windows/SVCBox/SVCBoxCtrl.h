#pragma once

// SVCBoxCtrl.h : Declaration of the CSVCBoxCtrl ActiveX Control class.


// CSVCBoxCtrl : See SVCBoxCtrl.cpp for implementation.
#include "MainFrame.h"

#include "Channel/SVCChannel.h"
#include "Input/InputManager.h"
#include "Input/KeyboardMapper.h"
#include "CBoxFinder.h"

#include <map>
using namespace std;

class CSVCBoxCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSVCBoxCtrl)
	DECLARE_INTERFACE_MAP()
	DECLARE_OLECREATE_EX(CSVCBoxCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSVCBoxCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSVCBoxCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSVCBoxCtrl)		// Type name and misc status

	// Message maps
	DECLARE_MESSAGE_MAP()
	// Dispatch maps
	DECLARE_DISPATCH_MAP()
	// Event maps
	DECLARE_EVENT_MAP()
// Constructor
public:
	CSVCBoxCtrl();
	~CSVCBoxCtrl();
// Implementation
	afx_msg void AboutBox();
// Dispatch and event IDs
public:
	enum 
	{									
		dispidUpdateCollsepRc = 33L,
		dispidUpdateDeviceSize = 32L,
		dispidSetFlag = 31L,
		dispidUpdateVedioRect = 30L,
		dispidInstallAPP = 29L,
		dispidClearTouchedKey = 28L,
		dispidExit = 27L,
		dispidSetVolume = 26L,
		dispidCallAndroidHomeKey = 25L,
		dispidCallAndroidBack = 24L,
		dispidOpenTaskManager = 23L,
		dispidRebootBox = 22L,
		dispidGetAndroidVolume = 21L,
		dispidGetWorkAreaRect = 20L,
		dispidGetSystemDPI = 19L,
		dispidInputChar = 18L,
		dispidPushUpgradeFileToBox = 17L,
		dispidSetRunningApp = 16L,
		dispidSaveKeyMappingConfig = 15L,
		dispidSetKeyMappingOpen = 14L,
		dispidLoadKeyMapping = 13L,
		dispidUpdatePCVersion = 12L,
		dispidopenApk = 11L,
		dispidGetSDCardSpace = 10L,
		dispidGetDeviceScreenSize = 9L,
		dispidGetUpgradeInfo = 8L,
		dispidResetDisplayMode = 7L,
		dispidSetScreenResolution = 6L,
		dispidGetIMEStatus = 5L,
		dispidSetIMEStatus = 4L,
		dispidSVCDowndloadApp = 3L,
		dispidSendOperateCmd = 2L,
		dispidStart = 1L,

		eventidNotifyUpgrageFinish = 10L,
		eventidNotifyUpdateException = 9L,
		eventidNotifyVideoReady = 8L,
		eventidNotifyUpgrade = 7L,
		eventidNotifySystemVolume = 6L,
		eventidNotifySdCardSpace = 5L,
		eventidHintInputMethodStatus = 4L,
		eventidNotifyDeviceScreenSize = 3L,
		eventidNotifyRunningApp = 2L,
		eventidConnectStateChanged = 1L
	};

protected:
	/*接口*/
	ULONG						Start(ULONG bAutoReconnect, ULONG hwndVedio);
	ULONG						SendOperateCmd(ULONG ulCmdType, ULONG ulPara1, ULONG ulPara2, ULONG ulPara3, BSTR data);
	ULONG						SVCDowndloadApp(LPCTSTR strUrl, LPCTSTR strExtend, ULONG ulExtent);
	ULONG						SetIMEStatus(ULONG ulIMEOpen);
	ULONG						GetIMEStatus(void);
	void						SetScreenResolution(LPCTSTR resolution);
	void						ResetDisplayMode();
	ULONG						GetUpgradeInfo(BSTR version);
	void						GetDeviceScreenSize();
	void						GetSDCardSpace();
	void						openApk(BSTR packageName, BSTR activityName);
	ULONG						UpdatePCVersion(LPCTSTR filePath);
	ULONG						LoadKeyMapping(LPCTSTR packageName);
	ULONG						SetKeyMappingOpen(SHORT open);
	ULONG						SaveKeyMappingConfig(LPCTSTR configFile, LPCTSTR content);
	ULONG						SetRunningApp(LPCTSTR packageName);
	ULONG						PushUpgradeFileToBox(LPCTSTR boxFile);
	ULONG						InputChar(LONG value);
	ULONG						GetSystemDPI(LONG* dpiValue);
	ULONG						GetWorkAreaRect(LONG* left, LONG* top, LONG* right, LONG* bottom);
	ULONG						GetAndroidVolume(LONG* volume);
	ULONG						RebootBox();
	ULONG						OpenTaskManager();
	ULONG						CallAndroidBackKey();
	ULONG						CallAndroidHomeKey();
	ULONG						SetVolume(LONG value);
	ULONG						Exit();
	void						ClearTouchedKey();
	ULONG						InstallAPP(BSTR fileName);

	/*事件*/
	void ConnectStateChanged(ULONG ulCurrentState)
	{
		FireEvent(eventidConnectStateChanged, EVENT_PARAM(VTS_UI4), ulCurrentState);
	}

	void NotifyRunningApp(BSTR bstrPackageName, BSTR bstrAppName)
	{
		FireEvent(eventidNotifyRunningApp, EVENT_PARAM(VTS_BSTR VTS_BSTR), bstrPackageName, bstrAppName);
	}

	void NotifyDeviceScreenSize(ULONG ulWidth, ULONG ulHeight)
	{
		TRACE(_T("NotifyDeviceScreenSize %d %d \n"), ulWidth, ulHeight);
		FireEvent(eventidNotifyDeviceScreenSize, EVENT_PARAM(VTS_UI4 VTS_UI4), ulWidth, ulHeight);
	}

	void HintInputMethodStatus(USHORT bInputMethodShouldOpen)
	{
		TRACE(_T("HintInputMethodStatus %d \n"), bInputMethodShouldOpen);

		FireEvent(eventidHintInputMethodStatus, EVENT_PARAM(VTS_UI2), bInputMethodShouldOpen);
	}

	void NotifySdCardSpace(ULONG ulAvailable, ULONG ulTotal)
	{
		TRACE(_T("NotifySdCardSpace %d %d \n"), ulAvailable, ulTotal);
		FireEvent(eventidNotifySdCardSpace, EVENT_PARAM(VTS_UI4 VTS_UI4), ulAvailable, ulTotal);
	}

	void NotifySystemVolume(LONG volume)
	{
		TRACE(_T("NotifySystemVolume %d \n"), volume);
		FireEvent(eventidNotifySystemVolume, EVENT_PARAM(VTS_I4), volume);
	}

	void NotifyUpgrade(BSTR upgradeInfo)
	{
		TRACE(_T("NotifyUpgrade %s \n"), upgradeInfo);;
		FireEvent(eventidNotifyUpgrade, EVENT_PARAM(VTS_BSTR), upgradeInfo);
	}
	void NotifyVideoReady()
	{
		FireEvent(eventidNotifyVideoReady, EVENT_PARAM(VTS_NONE));
	}
	void NotifyUpdateException()
	{
		FireEvent(eventidNotifyUpdateException, EVENT_PARAM(VTS_NONE));
	}

	void NotifyUpgrageFinish()
	{
		FireEvent(eventidNotifyUpgrageFinish, EVENT_PARAM(VTS_NONE));
	}

	/*消息*/
	afx_msg int					OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void				OnSize(UINT nType, int cx, int cy);
	afx_msg void				OnDestroy();
	afx_msg LRESULT				OnConnectStateChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT				OnDeviceMessageReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT				OnNotifyWindowsVolume(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT				OnNotifyVideoReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT				OnNotifyReStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT				OnNotifyCopyData(WPARAM wParam, LPARAM lParam);
	afx_msg void				OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL				OnCopyData(WPARAM wParam, LPARAM lParam);
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
	volatile long				m_startCnt;
	list<COPYDATASTRUCT>		m_copyDataVec;				//接收copyData的数据队列
	ConnectStatus				m_Status;
	BOOL						m_bExit;					//判断是否已经退出
	HWND						m_hwndVedio;				//显示视频的窗口

	CRect						m_CollapseScreenRect;		//按钮的屏幕度座标
	CRect						m_CollapseVedioClientRect;	//按钮相对于主窗口的客户区域座标
	CRect						m_vedioScreenRect;
	double						m_fCoordinateXMappingRatio;	//相对于盒子的比率
	double						m_fCoordinateYMappingRatio;	//相对于盒子的比率

	CBoxView					m_Vedio;
	SVCChannel					m_Channel;
	KeyboardMapper				m_KeyboardMapper;

	std::wstring				m_className;				//用于日志打印的类名
	CString						m_strSDCardPath;			//SD卡的路径名
	std::string					m_strCurrentRunningApp;		//当前正在运行的APP的名称
	ULONG						ulAvailable;				//剩余空间
	ULONG						ulTotal;					//总空间
	USHORT						m_inputValue;				//当前是否启用输入文字的模式

	CString						m_strBoxPackageFile;		//盒子升级包，在盒子中的存放路径

	CBoxFinder					m_BoxFinderStub;
	CString						m_szCableName;

public:

	void ClientToDeviceScreen(int &x, int &y);
protected:
	void UpdateVedioRect(LONG l, LONG t, LONG r, LONG b);
	void SetFlag(LONG nFlag, LONG value);
	void UpdateDeviceSize(LONG deviceWidth, LONG deviceHeight);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT OnFgNotify(WPARAM wParam, LPARAM lParam);
	void DeviceScreenToScreen(int &x, int &y);
	UINT GetWindowColorValue(HWND hWnd);
	COLORREF GetWindowPixelColor(HWND hWnd, int x, int y);
	void UpdateCollsepRc(LONG screenLeft, LONG screenTop, LONG screenRight, LONG screenBottom);
};
