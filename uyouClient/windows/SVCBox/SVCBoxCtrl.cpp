
// SVCBoxCtrl.cpp : Implementation of the CSVCBoxCtrl ActiveX Control class.

#include "stdafx.h"
#include "SVCBox.h"
#include "SVCBoxCtrl.h"
#include "SVCBoxPropPage.h"
#include "afxdialogex.h"
#include <atlbase.h>
#include <atlstr.h>
#include <atlconv.h>
#include <atldef.h>
#include <atlcom.h>
#include <atlenc.h>
#include "Channel/IMEAppProtocol.h"
#include "UpgradeInfo.h"
#include "ConsoleUtil.h"
#include "../Common/Common.h"

#define CONFIG_FILE "./upgradeconfig.ini"

#ifdef _DEBUG
#pragma comment( lib, "libprotobufUD.lib" )
#else
#pragma comment( lib, "libprotobuf.lib" )
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CSVCBoxCtrl, COleControl)

// Message map

BEGIN_MESSAGE_MAP(CSVCBoxCtrl, COleControl)
	ON_MESSAGE(WM_FGNOTIFY, OnFgNotify)

	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_NOTIFY_CONNECT_STATE_CHANGED, OnConnectStateChanged)
	ON_MESSAGE(WM_NOTIFY_DEVICE_MESSAGE_RECEIVED, OnDeviceMessageReceived)
	ON_MESSAGE(WM_NOTIFY_VIDEO_READY, OnNotifyVideoReady)
	ON_MESSAGE(WM_NOTIFY_RESTART, OnNotifyReStart)
	ON_MESSAGE(WM_NOTIFY_COPYDATA, OnNotifyCopyData)
	ON_WM_INPUTLANGCHANGEREQUEST()
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CSVCBoxCtrl, COleControl)
	INTERFACE_PART(CSVCBoxCtrl, __uuidof(IDispatch), Dispatch)
END_INTERFACE_MAP()

// Dispatch map
//宿主调用control的命令
BEGIN_DISPATCH_MAP(CSVCBoxCtrl, COleControl)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "Start", dispidStart, Start, VT_UI4, VTS_UI4 VTS_UI4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SendOperateCmd", dispidSendOperateCmd, SendOperateCmd, VT_UI4, VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4 VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SVCDowndloadApp", dispidSVCDowndloadApp, SVCDowndloadApp, VT_UI4, VTS_BSTR VTS_BSTR VTS_UI4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SetIMEStatus", dispidSetIMEStatus, SetIMEStatus, VT_UI4, VTS_UI4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "GetIMEStatus", dispidGetIMEStatus, GetIMEStatus, VT_UI4, VTS_NONE)	
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SetScreenResolution", dispidSetScreenResolution, SetScreenResolution, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "ResetDisplayMode", dispidResetDisplayMode, ResetDisplayMode, VT_EMPTY, VTS_NONE)	
	DISP_FUNCTION_ID(CSVCBoxCtrl, "GetUpgradeInfo", dispidGetUpgradeInfo, GetUpgradeInfo, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "GetDeviceScreenSize", dispidGetDeviceScreenSize, GetDeviceScreenSize, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "GetSDCardSpace", dispidGetSDCardSpace, GetSDCardSpace, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "openApk", dispidopenApk, openApk, VT_EMPTY, VTS_BSTR VTS_BSTR)	
	DISP_FUNCTION_ID(CSVCBoxCtrl, "LoadKeyMapping", dispidLoadKeyMapping, LoadKeyMapping, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SetKeyMappingOpen", dispidSetKeyMappingOpen, SetKeyMappingOpen, VT_UI4, VTS_I2)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SaveKeyMappingConfig", dispidSaveKeyMappingConfig, SaveKeyMappingConfig, VT_UI4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SetRunningApp", dispidSetRunningApp, SetRunningApp, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "PushUpgradeFileToBox", dispidPushUpgradeFileToBox, PushUpgradeFileToBox, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "InputChar", dispidInputChar, InputChar, VT_UI4, VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "UpdatePCVersion", dispidUpdatePCVersion, UpdatePCVersion, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "GetSystemDPI", dispidGetSystemDPI, GetSystemDPI, VT_UI4, VTS_PI4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "GetWorkAreaRect", dispidGetWorkAreaRect, GetWorkAreaRect, VT_UI4, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "GetAndroidVolume", dispidGetAndroidVolume, GetAndroidVolume, VT_UI4, VTS_PI4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "RebootBox", dispidRebootBox, RebootBox, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "OpenTaskManager", dispidOpenTaskManager, OpenTaskManager, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "CallAndroidBack", dispidCallAndroidBack, CallAndroidBackKey, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "CallAndroidHomeKey", dispidCallAndroidHomeKey, CallAndroidHomeKey, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SetVolume", dispidSetVolume, SetVolume, VT_UI4, VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "Exit", dispidExit, Exit, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "ClearTouchedKey", dispidClearTouchedKey, ClearTouchedKey, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "InstallAPP", dispidInstallAPP, InstallAPP, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "UpdateVedioRect", dispidUpdateVedioRect, UpdateVedioRect, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "SetFlag", dispidSetFlag, SetFlag, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "UpdateDeviceSize", dispidUpdateDeviceSize, UpdateDeviceSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxCtrl, "UpdateCollsepRc", dispidUpdateCollsepRc, UpdateCollsepRc, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_DISPATCH_MAP()

// Event map
//由control发给宿主的消息
BEGIN_EVENT_MAP(CSVCBoxCtrl, COleControl)
	EVENT_CUSTOM_ID("ConnectStateChanged", eventidConnectStateChanged, ConnectStateChanged, VTS_UI4)
	EVENT_CUSTOM_ID("NotifyRunningApp", eventidNotifyRunningApp, NotifyRunningApp, VTS_BSTR VTS_BSTR)
	EVENT_CUSTOM_ID("NotifyDeviceScreenSize", eventidNotifyDeviceScreenSize, NotifyDeviceScreenSize, VTS_UI4 VTS_UI4)
	EVENT_CUSTOM_ID("HintInputMethodStatus", eventidHintInputMethodStatus, HintInputMethodStatus, VTS_UI2)
	EVENT_CUSTOM_ID("NotifySdCardSpace", eventidNotifySdCardSpace, NotifySdCardSpace, VTS_UI4 VTS_UI4)
	EVENT_CUSTOM_ID("NotifySystemVolume", eventidNotifySystemVolume, NotifySystemVolume, VTS_I4)	
	EVENT_CUSTOM_ID("NotifyUpgrade", eventidNotifyUpgrade, NotifyUpgrade, VTS_BSTR)			
	EVENT_CUSTOM_ID("NotifyVideoReady", eventidNotifyVideoReady, NotifyVideoReady, VTS_NONE)
	EVENT_CUSTOM_ID("NotifyUpdateException", eventidNotifyUpdateException, NotifyUpdateException, VTS_NONE)
	EVENT_CUSTOM_ID("NotifyUpgrageFinish", eventidNotifyUpgrageFinish, NotifyUpgrageFinish, VTS_NONE)
END_EVENT_MAP()

// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSVCBoxCtrl, 1)
	PROPPAGEID(CSVCBoxPropPage::guid)
END_PROPPAGEIDS(CSVCBoxCtrl)

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSVCBoxCtrl, "SVCBOX.SVCBoxCtrl.1",
	0x9175a113, 0xdd75, 0x4e95, 0x8a, 0x27, 0xd0, 0x91, 0x7a, 0xc9, 0xde, 0x10)

// Type library ID and version

IMPLEMENT_OLETYPELIB(CSVCBoxCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID IID_DSVCBox = { 0x2273438B, 0xC05A, 0x432D, { 0x94, 0x3E, 0x14, 0xC2, 0x5, 0x91, 0x5C, 0x12 } };
const IID IID_DSVCBoxEvents = { 0xBA088AF6, 0xF635, 0x447C, { 0x99, 0xE7, 0x83, 0x72, 0xF6, 0x7B, 0x1E, 0x99 } };

// Control type information

static const DWORD _dwSVCBoxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSVCBoxCtrl, IDS_SVCBOX, _dwSVCBoxOleMisc)

// CSVCBoxCtrl::CSVCBoxCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSVCBoxCtrl

BOOL CSVCBoxCtrl::CSVCBoxCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SVCBOX,
			IDB_SVCBOX,
			afxRegApartmentThreading,
			_dwSVCBoxOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}
// CSVCBoxCtrl::CSVCBoxCtrl - Constructor
CSVCBoxCtrl::CSVCBoxCtrl()
	: m_Channel(g_szEXEPath)
	, m_KeyboardMapper(g_szEXEPath)
	, m_strCurrentRunningApp("com.blue.uyou.gamelauncher")
	, m_strSDCardPath("")
	, m_inputValue(0)
	, m_szCableName("")
	, m_bAutoReconnect(FALSE)
	, m_startCnt(0)
	, m_bExit(FALSE)
	, m_hwndVedio(NULL)
	, m_fCoordinateXMappingRatio(0)
	, m_fCoordinateYMappingRatio(0)
	, m_vedioScreenRect()
	, m_CollapseScreenRect()
	, m_CollapseVedioClientRect()
{
	InitializeIIDs(&IID_DSVCBox, &IID_DSVCBoxEvents);
	m_className = CA2T(this->GetRuntimeClass()->m_lpszClassName);	
	InputManager::GetInstance().GetTouchScreen().SetTouchMappingFunction(boost::bind(&CSVCBoxCtrl::ClientToDeviceScreen, this, _1, _2));
	InputManager::GetInstance().SetDeviceScreenToClientFunc(boost::bind(&CSVCBoxCtrl::DeviceScreenToScreen, this, _1, _2));

	{	//提升调用此控件进程的权限
		HANDLE htoke;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &htoke))
		{
			return;
		}
		LUID luid;
		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			return;
		}
		TOKEN_PRIVILEGES NewState;
		NewState.PrivilegeCount = 1;
		NewState.Privileges[0].Luid = luid;
		NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(htoke, FALSE, &NewState, sizeof(NewState), NULL, NULL))
		{
			return;
		}
	}
}
// CSVCBoxCtrl::~CSVCBoxCtrl - Destructor
CSVCBoxCtrl::~CSVCBoxCtrl()
{	
	Common::LogHelper::Close();	
}
// CSVCBoxCtrl::AboutBox - Display an "About" box to the user
void CSVCBoxCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_SVCBOX);
	dlgAbout.DoModal();
}

int CSVCBoxCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxEnableControlContainer();

	m_Status = ConnectStatus::DISCONNECT;
	std::string path = Common::SystemHelper::GetCurrentPath();
	path += "\\log";
	CreateDirectoryA(path.c_str(), NULL);

	//初始化Box Manager的stub
	m_BoxFinderStub.CreateDispatch(__uuidof(BoxFinder));
	
	return 0;
}

void CSVCBoxCtrl::OnDestroy()
{	
	m_Vedio.Release();
	InputManager::GetInstance().DetachScreenFunc();
	COleControl::OnDestroy();	
}

void CSVCBoxCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);
}

ULONG CSVCBoxCtrl::Start(ULONG bAutoReconnect, ULONG hwndVedio)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_bAutoReconnect	= bAutoReconnect;
	m_hwndVedio			= (HWND)hwndVedio;
	::PostMessage(m_hWnd, WM_NOTIFY_RESTART, 0, 0);
	return 0;
}

BOOL CSVCBoxCtrl::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pCopyDataStruct = (COPYDATASTRUCT*)lParam;
	if (pCopyDataStruct)
	{
		COPYDATASTRUCT	cpData;
		cpData.dwData	= pCopyDataStruct->dwData;
		cpData.cbData	= pCopyDataStruct->cbData;
		cpData.lpData	= new BYTE[pCopyDataStruct->cbData];
		memcpy(cpData.lpData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

		m_copyDataVec.push_back(cpData);
		::PostMessage(m_hWnd, WM_NOTIFY_COPYDATA, 0, 0);
	}

	return TRUE;
}

ULONG CSVCBoxCtrl::SendOperateCmd(ULONG ulCmdType, ULONG ulPara1, ULONG ulPara2, ULONG ulPara3, BSTR data)
{	//用户从界面上选择不同的操作，调用到这里
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_Status == ConnectStatus::CONNECTED)
	{
		IMEAppProtocol::EM_OPER_TYPE OperationCode;
		switch (ulCmdType)
		{
		case IMEAppProtocol::EMV_OPER_HOME:
		case IMEAppProtocol::EMV_OPER_RETURN:
		case IMEAppProtocol::EMV_OPER_TASKMGR:
		case IMEAppProtocol::EMV_OPER_INPUT_CHAR:
		case IMEAppProtocol::EMV_OPER_INPUT_KEY:
		case IMEAppProtocol::EMV_OPER_SDCARD_SPACE:
		case IMEAppProtocol::EMV_OPER_GETXY:
		case IMEAppProtocol::EMV_OPER_DOWNLOADAPP:
		case IMEAppProtocol::EMV_OPER_INPUTM:
		case IMEAppProtocol::EMV_OPER_DOWNLOADAPPPROG:
		case IMEAppProtocol::EMV_OPER_DOWNLOADPAUSE:
		case IMEAppProtocol::EMV_OPER_DOWNLOADFINISH:
		case IMEAppProtocol::EMV_OPER_HEARTBEAT:
		case IMEAppProtocol::EMV_OPER_APPRUNNING:
		case IMEAppProtocol::EMV_OPER_OPENAPP:
		case IMEAppProtocol::EMV_OPER_UPGRADE:
		case IMEAppProtocol::EMV_PC_TO_ANDROID_PUSH_SUCESS:
		case IMEAppProtocol::EMV_OPER_OPENRECENTAPPLICATIONS:
		case IMEAppProtocol::EMV_OPER_GETMEDIASTORAGEDIRECTORY:
		case IMEAppProtocol::EMV_OPER_GETVOLUME:
		case IMEAppProtocol::EMV_OPER_SETVOLUME:
		case IMEAppProtocol::EMV_OPER_REBOOT_DEVICE:
			OperationCode = (IMEAppProtocol::EM_OPER_TYPE)ulCmdType;
			break;
		default:
			return 0;			
		};
		if (ulCmdType == IMEAppProtocol::EMV_OPER_APPRUNNING)
		{
			//InputManager::GetInstance().ResetMouseEvent();
		}
		_bstr_t b = data;
		CHAR* lpszData = b;
		//用r-value，避免内存复制
		try
		{
			string&& strPacket = IMEAppProtocol::GetInstance().ToString(IMEAppProtocol::IMEAppMessage(OperationCode, lpszData));
			TRACE("Send Cmd String is %s \n", strPacket.c_str());
			std::wstring s = CA2W(strPacket.c_str());
			Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, s.c_str());

			m_Channel.SendInputMethodOperationMessage(boost::asio::buffer(strPacket.data(), strPacket.size()));
		}
		catch (...)
		{
			return -1;
		}		
		return 0;
	}	
	return -1;
}

ULONG CSVCBoxCtrl::SVCDowndloadApp(LPCTSTR strUrl, LPCTSTR strExtend, ULONG ulExtent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
		
	return 0;
}

ULONG CSVCBoxCtrl::SetIMEStatus(ULONG ulIMEOpen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}

ULONG CSVCBoxCtrl::GetIMEStatus(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}

LRESULT CSVCBoxCtrl::OnConnectStateChanged(WPARAM wParam, LPARAM lParam)
{
	ConnectStatus status = (ConnectStatus)wParam;
	switch (status)
	{
		case ConnectStatus::DISCONNECT:
		{
			if (m_Status != status && m_startCnt == 0)
			{	//正在Start的情况下就不需要再次上报断连状态了
				m_Status = status;
				InputManager::GetInstance().DetachScreenFunc();
				m_CollapseVedioClientRect = CRect(0,0,0,0);
				ConnectStateChanged(0);

				if (m_bAutoReconnect)	//断线重连
				{
					::PostMessage(m_hWnd, WM_NOTIFY_RESTART, 0, 0);
				}
			}
		}
		break;
		case ConnectStatus::CONNECTING:
			if (m_Status != status)
			{	
				m_Status = status;
			}
			break;
		case ConnectStatus::CONNECTED:
		{
			if (m_Status != status)
			{	//当SVCChannel初始化完成后，把InputManager和SVCChannel联系起来
				m_Status = status;

				InputManager::GetInstance().AttachScreenFunc(
					boost::bind(&SVCChannel::SendTouchScreenMessage, &m_Channel, _1),
					boost::bind(&SVCChannel::SendInputMethodOperationMessage, &m_Channel, _1)
					);


				string&& strPacket = IMEAppProtocol::GetInstance().ToString(IMEAppProtocol::IMEAppMessage(IMEAppProtocol::EMV_OPER_GETMEDIASTORAGEDIRECTORY, ""));
				TRACE("Send Cmd String is %s \n", strPacket.c_str());
				std::wstring s = CA2W(strPacket.c_str());
				Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, s.c_str());
				ClearTouchedKey();

				ConnectStateChanged(1);		//TODO: 可能还有其他状态		
			}

			//PushFileToBox(_T("D:\\APK\\设置颜色 app-release.apk"));
			//PushFileToBox(_T("D:\\APK\\com.UCMobile_175.apk"));
		}
		break;
		default:
			break;
	}

	return TRUE;
}

LRESULT CSVCBoxCtrl::OnDeviceMessageReceived(WPARAM wParam, LPARAM lParam)
{
	auto MessageType = static_cast<IMEAppProtocol::EM_OPER_TYPE>(wParam);
	auto lpszData = reinterpret_cast<LPCSTR>(lParam);
	
	auto ParsedData = IMEAppProtocol::GetInstance().ParseMessage(MessageType, lpszData);
	if (!ParsedData){
		ATLTRACE(_T("接收到的设备消息体结构解析失败"));
		return 0;
	}
	Json::Value value;
	static Json::Value result;
	LPCSTR data;
	CHAR configValue[1024] = { 0 };
	switch(MessageType)
	{
	case IMEAppProtocol::EMV_OPER_APPRUNNING:
		{	//注意，android端传来的是ansi/gbk编码
			LPCSTR lpszPackageName = ParsedData->at(0).c_str();
			m_strCurrentRunningApp = lpszPackageName;

			ClearTouchedKey();

			//实际上这里得到的都是ansi/gbk编码 但要强制转换成unicode的接口，以便通过编译
			NotifyRunningApp(CComBSTR(lpszPackageName), CComBSTR(ParsedData->at(1).c_str()));
		}
		break;
	case IMEAppProtocol::EMV_OPER_GETXY:
		NotifyDeviceScreenSize(boost::lexical_cast<ULONG>(ParsedData->at(0).c_str()),
			boost::lexical_cast<ULONG>(ParsedData->at(1).c_str()));
		break;
	case IMEAppProtocol::EMV_OPER_INPUTM:
		
		if (ParsedData->at(0).compare("false") == 0)
		{			
			m_inputValue = 0;
		}
		else if (ParsedData->at(0).compare("true") == 0)
		{			
			m_inputValue = 1;
		}
		m_KeyboardMapper.SetInputOpened(m_inputValue);
		HintInputMethodStatus(boost::lexical_cast<USHORT>(m_inputValue));
		break;
	case IMEAppProtocol::EMV_OPER_SDCARD_SPACE:
		ulAvailable = boost::lexical_cast<ULONG>(ParsedData->at(0).c_str());
		ulTotal = boost::lexical_cast<ULONG>(ParsedData->at(1).c_str());
		NotifySdCardSpace( ulAvailable, ulTotal);		
		break;
	case IMEAppProtocol::EMV_ANDROID_TO_PC_UPGRADEINFO:	//盒子通知PC端有升级包，控件通知主程序，提示是否升级
		data = ParsedData->at(0).c_str();
		value = Common::JsonHelper::readFromString(CA2T(data));
		{
			std::wstring message = CA2T(data);
			Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, message.c_str()); 
		}
		if (value.isMember("result"))
		{			
			result = value["result"];
			std::string sss = result.toStyledString();		
			TRACE("%s\n", sss.c_str());
			NotifyUpgrade(CComBSTR(sss.c_str()));
		}
		else{
			ATLTRACE("升级包解析失败");
		}
		
		break;
	case IMEAppProtocol::EMV_ANDROID_TO_PC_UPGRADEFINISH:		
		TRACE("EMV_ANDROID_TO_PC_UPGRADEFINISH\n");
		Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, _T("EMV_ANDROID_TO_PC_UPGRADEFINISH"));
		if (result.isMember("box"))
		{
			Json::Value boxInfo = result["box"];
			TRACE("%s\n", boxInfo.toStyledString().c_str());
			if (boxInfo != Json::nullValue 
				&& boxInfo.isMember("vervalue")
				&& boxInfo["vervalue"] != Json::nullValue)
			{						
				TRACE("%s = %s\n", ParsedData->at(0).c_str(), boxInfo["vervalue"].asCString());
				if (ParsedData->at(0).compare(boxInfo["vervalue"].asCString()) == 0)
				{//升级成功通知接口和查询升级包信息接口返回的版本信息相同，说明是同一次升级动作，升级成功后，删除Box内的升级包
					m_Channel.DeleteFile(m_strBoxPackageFile);
					if (result.isMember("pc"))
					{
						Json::Value pcInfo = result["pc"];
						if (pcInfo != NULL)
						{
							if (pcInfo.isMember("downloadurl"))
							{
								Json::Value url = pcInfo["downloadurl"];
								if (url != Json::nullValue)
								{
									std::string pcPath = pcInfo["downloadurl"].asCString();
									std::string fileName = pcPath.substr(pcPath.find_last_of("/") + 1);
									fileName = Common::SystemHelper::GetCurrentPath() + "\\download\\" + fileName;
									TRACE("%s\n", fileName.c_str());
									UpdatePCFile((LPSTR)fileName.c_str());
								}
								else{
									NotifyUpgrageFinish();
								}
							}
						}						
					}
				}
			}
		}

		break;
	case IMEAppProtocol::EMV_ANDROID_TO_PC_UPGRADE_EXCEPTION:
		//TODO 升级异常
		NotifyUpdateException();
		break;
	case IMEAppProtocol::EMV_OPER_GETMEDIASTORAGEDIRECTORY:
		data = ParsedData->at(0).c_str();
		m_strSDCardPath = data;
		TRACE("%s\n", data);
		break;
	case IMEAppProtocol::EMV_OPER_GETVOLUME:
		TRACE("EMV_OPER_GETVOLUME %s\n", ParsedData->at(0).c_str());		
		NotifySystemVolume(boost::lexical_cast<ULONG>(ParsedData->at(0).c_str()));
		break;
	default:
		break;
	}

	return 0;
}

void CSVCBoxCtrl::SetScreenResolution(LPCTSTR resolution)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Common::SystemHelper::ScreenHelper::SetDisplayMode(resolution);		
}

void CSVCBoxCtrl::ResetDisplayMode()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Common::SystemHelper::ScreenHelper::ResetDisplayMode();		
}

//************************************
// Method:    GetUpgradeInfo
// 检测升级包
// FullName:  CSVCBoxCtrl::GetUpgradeInfo
// Access:    protected 
// Returns:   ULONG
// Qualifier:
// Parameter: BSTR version
//************************************
ULONG CSVCBoxCtrl::GetUpgradeInfo(BSTR version)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  在此添加调度处理程序代码
	Json::Value value;
	_bstr_t b = version;
	value["version"] = (LPSTR)b;
	SendOperateCmd(IMEAppProtocol::EMV_OPER_UPGRADE, 0, 0, 0, CComBSTR(value.toStyledString().c_str()));
	return 0;
}

//************************************
// Method:    GetDeviceScreenSize
// 查询设备实际屏幕尺寸，查询结果通过NotifyDeviceScreenSize返回
// FullName:  CSVCBoxCtrl::GetDeviceScreenSize
// Access:    protected 
// Returns:   NotifyDeviceScreenSize
// Qualifier:
//************************************
void CSVCBoxCtrl::GetDeviceScreenSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	// TODO:  在此添加调度处理程序代码
	SendOperateCmd(IMEAppProtocol::EMV_OPER_GETXY, 0, 0, 0, CComBSTR(""));
}

//************************************
// Method:    GetSDCardSpace
// 获取盒子SD卡上的剩余空间，查询通过NotifySdCardSpace返回
// FullName:  CSVCBoxCtrl::GetSDCardSpace
// Access:    protected 
// Returns:   void
// Qualifier:
//************************************
void CSVCBoxCtrl::GetSDCardSpace()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO:  在此添加调度处理程序代码
	SendOperateCmd(IMEAppProtocol::EMV_OPER_SDCARD_SPACE, 0, 0, 0, CComBSTR(""));
}

//************************************
// Method:    openApk
// 启动盒子上的APK
// FullName:  CSVCBoxCtrl::openApk
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: BSTR packageName
// Parameter: BSTR activityName
//************************************
void CSVCBoxCtrl::openApk(BSTR packageName, BSTR activityName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	Json::Value root;
	_bstr_t b1 = packageName;
	
	root["packageName"] = (LPSTR) b1;
	b1 = activityName;
	root["activityName"] = (LPSTR)b1;
	
	std::string out = root.toStyledString();
	
	SendOperateCmd(IMEAppProtocol::EMV_OPER_OPENAPP, 0, 0, 0, CComBSTR(out.c_str()));	
}

LRESULT CSVCBoxCtrl::OnNotifyVideoReady(WPARAM wParam, LPARAM lParam){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	NotifyVideoReady();
	return 0;
}

/******************键盘映射************************/
ULONG CSVCBoxCtrl::SetRunningApp(LPCTSTR packageName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_strCurrentRunningApp = CT2A(packageName);
	return 0;
}
ULONG CSVCBoxCtrl::SetKeyMappingOpen(SHORT open)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (open == 1)
	{
		BindMappingFunc();
	}
	else
	{		
		UnBindMappingFunc();				
	}
	ClearTouchedKey();
	return 0;
}
ULONG CSVCBoxCtrl::LoadKeyMapping(LPCTSTR packageName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//	InputManager::GetInstance().ResetMouseEvent();
	InputManager::GetInstance().ResetMouseHandleStatus();
	ClearTouchedKey();
	m_KeyboardMapper.Load(CT2A(packageName), m_vedioScreenRect, TRUE);

	return S_OK;
}
ULONG CSVCBoxCtrl::SaveKeyMappingConfig(LPCTSTR configFile, LPCTSTR content)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	String msg;
	msg = _T("SaveKeyMappingConfig ");
	msg += content;
	Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, msg.c_str());
	return m_KeyboardMapper.Save(CT2A(configFile), CT2A(content));	
}
/*************************************************/
ULONG CSVCBoxCtrl::PushUpgradeFileToBox(LPCTSTR boxFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	DWORD fileSize =0;
	
	hFind = FindFirstFile(boxFile, &fileInfo);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		fileSize = fileInfo.nFileSizeLow;
	}
	FindClose(hFind);
	if (ulAvailable - 100 < fileSize / 1024 / 1024)
	{
		MessageBox(_T("剩余空间不足，无法升级，请先清理盒子"), _T("系统提示"), MB_ICONERROR | MB_OK);
		return -1;
	}
	ATLTRACE("push file to box %s\n", CT2A(boxFile));

	PushFileToBox(boxFile);

	BOOL b		= ::DeleteFile(boxFile);
	return 0;
}

ULONG CSVCBoxCtrl::InputChar(LONG value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (m_inputValue && m_Status== ConnectStatus::CONNECTED)
	{
		InputManager::GetInstance().CharInputHandler(value);
	}
	return 0;
}

ULONG CSVCBoxCtrl::UpdatePCVersion(LPCTSTR filePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
		
	UpdatePCFile(CT2A(filePath));
	return 0;
}

ULONG CSVCBoxCtrl::GetSystemDPI(LONG* dpiValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	*dpiValue = Common::SystemHelper::ScreenHelper::GetSystemDPI();
	return 0;
}

ULONG CSVCBoxCtrl::GetWorkAreaRect(LONG* left, LONG* top, LONG* right, LONG* bottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	SIZE sz = Common::SystemHelper::ScreenHelper::getFullScreenSize();
	*left = 0;
	*top = 0;
	*right = sz.cx;
	*bottom = sz.cy;
	return 0;
}

ULONG CSVCBoxCtrl::GetAndroidVolume(LONG* volume)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TRACE("GetAndroidVolume \n");
	SendOperateCmd(IMEAppProtocol::EMV_OPER_GETVOLUME, 0, 0, 0, CComBSTR(""));
	*volume = 0;
	return 0;
}

ULONG CSVCBoxCtrl::RebootBox()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Channel.RebootDevice();
	return 0;
}

ULONG CSVCBoxCtrl::OpenTaskManager()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SendOperateCmd(IMEAppProtocol::EMV_OPER_OPENRECENTAPPLICATIONS, 0, 0, 0, CComBSTR(""));
	return 0;
}

ULONG CSVCBoxCtrl::CallAndroidBackKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	SendOperateCmd(IMEAppProtocol::EMV_OPER_RETURN, 0, 0, 0, CComBSTR(""));
	return 0;
}

ULONG CSVCBoxCtrl::CallAndroidHomeKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SendOperateCmd(IMEAppProtocol::EMV_OPER_HOME, 0, 0, 0, CComBSTR(""));	
	return 0;
}

ULONG CSVCBoxCtrl::SetVolume(LONG value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHAR sValue[10] = { 0 };
	sprintf_s(sValue, 10, "%d", value);
	
	// TODO:  在此添加调度处理程序代码
	SendOperateCmd(IMEAppProtocol::EMV_OPER_SETVOLUME, 0, 0, 0, CComBSTR(sValue));
	return 0;
}

void CSVCBoxCtrl::OnClose()
{
	InputManager::GetInstance().ClearMapping();
	COleControl::OnClose(0);
}

ULONG CSVCBoxCtrl::Exit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_bExit		= TRUE;
	return 0;
}

void CSVCBoxCtrl::ClearTouchedKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_Status == ConnectStatus::CONNECTED)
	{
		InputManager::GetInstance().ClearMapping();
	}	
	m_KeyboardMapper.ClearKeyDownList();
}

ULONG CSVCBoxCtrl::InstallAPP(BSTR fileName){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::wstring ss			= _bstr_t(fileName);
	std::wstring message	= _T("install app") + ss;

	BOOL	ret				=  m_Channel.InstallAPP(ss.c_str());

	Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, message.c_str());
	return ret;
}

String CSVCBoxCtrl::UpdatePCFile(LPSTR downloadFile)
{
	std::wstring message = CA2W(downloadFile);

	message =  _T("----开始升级PC端软件") + message;

	Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, message.c_str());
	std::string cmd = Common::SystemHelper::GetCurrentPath();
	cmd += "\\Update.exe";
	cmd = "\"" + cmd;
	cmd += "\" \"";

	cmd += downloadFile;
	cmd += "\"";

	//int ret = Common::SystemHelper::WinExec(cmd.c_str());	
	std::wstring wcmd = CA2T(cmd.c_str());

	String ret = ConsoleUtil::Execute(wcmd.c_str());
	return ret;
}

void CSVCBoxCtrl::PushFileToBox(LPCTSTR downladFile){
	TCHAR		drive[_MAX_DRIVE];
	TCHAR		dir[_MAX_DIR];
	TCHAR		fName[_MAX_FNAME];
	TCHAR		ext[_MAX_EXT];
	CString		destPath;

	_tsplitpath(downladFile, drive, dir, fName, ext);

	if (m_strSDCardPath == _T(""))
	{
		m_strSDCardPath = (_T("/data/media/0/"));
	}

	destPath				= destPath + m_strSDCardPath + fName + ext;
	m_strBoxPackageFile		= destPath;
	TRACE(destPath);
	Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, _T("start push file to box"));
	std::wstring ret		= m_Channel.PushFileToDevice(downladFile, destPath);
	std::wstring message	= _T("push file to box finish. file is ");
	message					+= CT2W(downladFile);
	Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, message.c_str());
	Json::Value value;
	value["filePath"]		= (char*)CT2A(destPath.GetBuffer());
	SendOperateCmd(IMEAppProtocol::EMV_PC_TO_ANDROID_PUSH_SUCESS, 0, 0, 0, CComBSTR(value.toStyledString().c_str()));
}

void CSVCBoxCtrl::BindMappingFunc()
{
	InputManager::GetInstance().GetInputMethodOperationDevice().SetTouchMappingFunction(
		boost::bind(&KeyboardMapper::KeyMappingFunction, &m_KeyboardMapper, _1, _2));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetMouseMappingFunction(
		boost::bind(&KeyboardMapper::MouseMappingFunction, &m_KeyboardMapper, _1, _2));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetIsDirectKeyFunction(
		boost::bind(&KeyboardMapper::IsDirectKey, &m_KeyboardMapper, _1));
}

void CSVCBoxCtrl::UnBindMappingFunc()
{
	InputManager::GetInstance().GetInputMethodOperationDevice().SetTouchMappingFunction(
		static_cast<InputManager::InputMethodOperationInputDevice::MappingFuncType>(nullptr));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetMouseMappingFunction(
		static_cast<InputManager::InputMethodOperationInputDevice::MappingFuncType>(nullptr));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetIsDirectKeyFunction(
		static_cast<std::function<bool(UINT&)>>(nullptr));

	InputManager::GetInstance().ClearMapping();
}

void CSVCBoxCtrl::ReStart()
{
	if (m_Status != ConnectStatus::DISCONNECT)
		return;
	::PostMessage(m_hWnd, WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::CONNECTING, 0);

	m_BoxFinderStub.DetachDevice(m_szCableName, (long)GetSafeHwnd());

	m_szCableName = _T("");
	while (TRUE)
	{
		if (m_bExit)
			break;

		DWORD nCableId	= 0xffff;
		m_szCableName	= m_BoxFinderStub.GetCable(&nCableId);
		BOOL bBreak		= FALSE;

		if (!m_szCableName.IsEmpty()&&  m_Vedio.Play(CWnd::FromHandle(m_hwndVedio), this, m_szCableName, nCableId))
		{	//异步调用，当成功找到android设备的时候会收到通知
				m_BoxFinderStub.AttachDevice(m_szCableName, (long)m_hWnd);
				break;
		}

		boost::timer tm;
		while (tm.elapsed() < 0.1)//找不到或连不上cable，等一下继续找
		{
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Sleep(10);
		}
	}
}

LRESULT CSVCBoxCtrl::OnNotifyReStart(WPARAM wParam, LPARAM lParam)
{
	if (m_startCnt == 0)	//每次只允许一个启动运行
	{
		InterlockedIncrement(&m_startCnt);
		ReStart();
		InterlockedDecrement(&m_startCnt);
	}

	return TRUE;
}

LRESULT CSVCBoxCtrl::OnNotifyCopyData(WPARAM wParam, LPARAM lParam)
{
	while (m_copyDataVec.size())
	{
		COPYDATASTRUCT cpData = m_copyDataVec.front();
		m_copyDataVec.pop_front();

		LPCTSTR lpcopyStr					= (LPCTSTR)cpData.lpData;
		DWORD	copyLen						= cpData.cbData;

		String  cablename					= _T("");
		String  deviceid					= _T("");

		{	//查找显卡名称及ADB设备的ID
			////from in
			TCHAR *pTempCh					= new TCHAR[copyLen];
			memset(pTempCh, 0, copyLen);
			memcpy(pTempCh, lpcopyStr, copyLen);
			TCHAR *pCharPos					= NULL;
			TCHAR *pCharTemp				= _tcstok_s(pTempCh, _T("::"), &pCharPos);
			while (pCharTemp)
			{
				TCHAR *pChar				= NULL;
				String::size_type copySize	= 0;
				int subSize					= 0;

				String subStr				= _T("");
				const TCHAR findStr[][20]	={ _T("cablename="), _T("deviceid=") };
				if (pChar = _tcsstr(pCharTemp, findStr[0]))
				{
					subStr					= pChar;
					subSize					= _tcsclen(findStr[0]);
					subStr					= subStr.substr(subSize, subStr.size() - subSize);

					cablename				= subStr;
				}
				else if (pChar = _tcsstr(pCharTemp, findStr[1]))
				{
					subStr					= pChar;
					subSize					= _tcsclen(findStr[1]);
					subStr					= subStr.substr(subSize, subStr.size() - subSize);

					deviceid				= subStr;
				}

				pCharTemp = _tcstok_s(NULL, _T(","), &pCharPos);
			}
			delete[] pTempCh;
		}

		delete[] cpData.lpData;	//析构分配内存

		if (deviceid.length() == 0					||
			cablename.length() == 0					||
			cablename.compare(m_szCableName) != 0
			)
		{	//传进来的信息与的发现的设备信息不一致
			::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);	//改变连接状态
			continue;
		}

		//下面这句仅用于调试，正式版请去掉
		//AfxMessageBox(deviceid.c_str());

		if (m_Channel.Initialize(GetSafeHwnd(), deviceid.c_str()))
		{
			::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::CONNECTED, 0);	//改变连接状态
			InputManager::GetInstance().Initialize(m_hwndVedio);
		}
		else
		{
			::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);	//改变连接状态
			continue;
		}
	}

	return TRUE;
}

void CSVCBoxCtrl::ClientToDeviceScreen(int &x, int &y)
{	//注意，这个函数可能是在其他线程里面被调用的

	POINT pt;
	pt.x = x + m_vedioScreenRect.left;
	pt.y = y + m_vedioScreenRect.top;
	//::ClientToScreen(m_hwndVedio, &pt);

	x = (pt.x - m_vedioScreenRect.left) * m_fCoordinateXMappingRatio + 0.5;
	y = (pt.y - m_vedioScreenRect.top) * m_fCoordinateYMappingRatio + 0.5;
}

void CSVCBoxCtrl::DeviceScreenToScreen(int &x, int &y)
{	//注意，这个函数可能是在其他线程里面被调用的

	if (m_fCoordinateXMappingRatio > 0 && m_fCoordinateYMappingRatio > 0)
	{
		POINT pt;

		pt.x = x/m_fCoordinateXMappingRatio + m_vedioScreenRect.left + 0.5;
		pt.y = y/m_fCoordinateYMappingRatio + m_vedioScreenRect.top + 0.5;

		x = pt.x;
		y = pt.y;
	}
}

BOOL CSVCBoxCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return COleControl::OnEraseBkgnd(pDC);
}

void CSVCBoxCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 COleControl::OnPaint()
}

void CSVCBoxCtrl::OnNcPaint()
{
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 COleControl::OnNcPaint()
}

void CSVCBoxCtrl::UpdateVedioRect(LONG l, LONG t, LONG r, LONG b)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		// TODO:  在此添加调度处理程序代码
		//一切皆为屏幕像素坐标
		CRect rcTemp = CRect(l, t, r, b);
		m_vedioScreenRect = rcTemp;
		if (m_vedioScreenRect.Width())
			m_fCoordinateXMappingRatio = g_currentDeviceWidth * 1.0 / m_vedioScreenRect.Width();
		if (m_vedioScreenRect.Height())
			m_fCoordinateYMappingRatio = g_currentDeviceHeight * 1.0 / m_vedioScreenRect.Height();

		if (m_hwndVedio)
		{
			CWnd::FromHandle(m_hwndVedio)->ScreenToClient(rcTemp);
			m_Vedio.ReSize(rcTemp);
			if (InputHookHelper::GetInstance().GetAttachWnd())
			{	//重置了尺寸  要重新设置折叠按钮的区域位置
				CWnd *pWnd = CWnd::FromHandle(InputHookHelper::GetInstance().GetAttachWnd());

				////计算相对于承载视频窗口的客户位置
				CRect	CollapseVedioClientRect = m_CollapseScreenRect;
				CWnd::FromHandle(m_hwndVedio)->ScreenToClient(CollapseVedioClientRect);

				if (!m_CollapseVedioClientRect.EqualRect(CollapseVedioClientRect))
				{	////中断或者尺寸更改都会与m_CollapseClientRect clospclientRc 不同
					////计算视频窗口的客户区域
					CRect   rcClient;
					pWnd->GetClientRect(&rcClient);	////客户区域大小 

					////计算折叠按钮在视频窗口上的客户区域
					CRect	clospclientRc = m_CollapseScreenRect;
					pWnd->ScreenToClient(clospclientRc);	////把按钮屏幕坐标转成客户坐标

					HRGN hrgn	= CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
					HRGN hrgn1	= CreateRectRgn(clospclientRc.left, clospclientRc.top, clospclientRc.right, clospclientRc.bottom);

					CombineRgn(hrgn, hrgn, hrgn1, RGN_XOR);
					::SetWindowRgn(InputHookHelper::GetInstance().GetAttachWnd(), hrgn, true);
					DeleteRgn(hrgn1);

					m_CollapseVedioClientRect = CollapseVedioClientRect;
				}
			}
		}
	}
	catch (...)
	{
	}
}

void CSVCBoxCtrl::UpdateDeviceSize(LONG deviceWidth, LONG deviceHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  在此添加调度处理程序代码
	g_currentDeviceWidth	= deviceWidth;
	g_currentDeviceHeight	= deviceHeight;

	if (m_vedioScreenRect.Width())
		m_fCoordinateXMappingRatio = g_currentDeviceWidth * 1.0 / m_vedioScreenRect.Width();
	if (m_vedioScreenRect.Height())
		m_fCoordinateYMappingRatio = g_currentDeviceHeight * 1.0 / m_vedioScreenRect.Height();
}

void CSVCBoxCtrl::SetFlag(LONG nFlag, LONG value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  在此添加调度处理程序代码

	switch (nFlag)
	{
		case BoxFlags_IsOpenDeviceControl:
			g_IsOpenDeviceControl = value;
			break;
		default:
			break;
	}
}
COLORREF CSVCBoxCtrl::GetWindowPixelColor(HWND hWnd, int x, int y)
{
	auto hDC = ::GetDC(hWnd);
	if (hDC == NULL)
		return -1;

	auto iColor = ::GetPixel(hDC, x, y);

	::ReleaseDC(hWnd, hDC);

	return iColor;
}
UINT CSVCBoxCtrl::GetWindowColorValue(HWND hWnd)
{
	UINT ColorValue = -1;

	POINT PixelList[] =
	{	//暂时取以下采样点
		{ 100, 100 },
		{ 200, 200 },
		{ 200, 100 },
		{ 100, 200 },
		{ 300, 300 },
		{ 100, 300 },
		{ 300, 100 },
		{ 200, 300 },
		{ 300, 200 },
	};

	for each(auto Pixel in PixelList)
	{
		auto GotColor = GetWindowPixelColor(hWnd, Pixel.x, Pixel.y);
		if (GotColor == -1)		//有一个点获取颜色失败，都认为失败
			return -1;

		//注意：Android的颜色渲染机制比较奇怪。当需要画(x, x, x)这样的颜色的时候，它会在每个x上加一个不等的扰动，一般扰动为整数，偶见负数，绝对值不超过10
		//所以这里的判断也会稍微复杂一点
		if (abs(GetRValue(GotColor) - GetGValue(GotColor)) >= 10 || abs(GetBValue(GotColor) - GetGValue(GotColor)) >= 10
			|| abs(GetBValue(GotColor) - GetRValue(GotColor)) >= 10)	//三分量必须全相等
			return -1;

		UINT GotColorValue = (GetRValue(GotColor) + GetGValue(GotColor) + GetBValue(GotColor)) / 3;

		//第一次成立
		if (ColorValue == -1)
			ColorValue = GotColorValue;
		else if (GotColorValue != ColorValue)		//测试点的颜色如果不一样，认为失败
			return -1;
	}

	return ColorValue;
}

LRESULT CSVCBoxCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO:  在此添加专用代码和/或调用基类

	if (message == WM_COPYDATA)
	{
		BOOL bResult	= FALSE;
		if (wParam == 0)
		{
			OnCopyData(wParam, lParam);
			bResult = TRUE;
		}
		else if (wParam == 1)
		{
			COPYDATASTRUCT* pCopyDataStruct = (COPYDATASTRUCT*)lParam;
			if (pCopyDataStruct)
			{
				COPYDATASTRUCT	cpData;
				cpData.dwData	= pCopyDataStruct->dwData;
				cpData.cbData	= pCopyDataStruct->cbData;
				cpData.lpData	= new BYTE[pCopyDataStruct->cbData];
				memcpy(cpData.lpData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

				int deviceNums	= -1;

				LPCTSTR lpcopyStr					= (LPCTSTR)cpData.lpData;
				DWORD	copyLen						= cpData.cbData;

				String  cablename					= _T("");
				String  deviceid					= _T("");

				{	//查找显卡名称及ADB设备的ID
					////from in
					TCHAR *pTempCh					= new TCHAR[copyLen];
					memset(pTempCh, 0, copyLen);
					memcpy(pTempCh, lpcopyStr, copyLen);
					TCHAR *pCharPos					= NULL;
					TCHAR *pCharTemp				= _tcstok_s(pTempCh, _T("::"), &pCharPos);
					while (pCharTemp)
					{
						TCHAR *pChar				= NULL;
						String::size_type copySize	= 0;
						int subSize					= 0;

						String subStr				= _T("");
						const TCHAR findStr[][20]	={ _T("testindex=")};
						if (pChar = _tcsstr(pCharTemp, findStr[0]))
						{
							subStr					= pChar;
							subSize					= _tcsclen(findStr[0]);
							subStr					= subStr.substr(subSize, subStr.size() - subSize);

							deviceNums				= _tstoi(subStr.c_str());
						}

						pCharTemp = _tcstok_s(NULL, _T(","), &pCharPos);
					}
					delete[] pTempCh;
				}
				
				HWND hAttachWnd = NULL;
				HWND hChild = ::GetWindow(m_hwndVedio, GW_CHILD);
				TCHAR  text[MAX_PATH] ={ 0 };
				while (hChild!=NULL)
				{
					::GetWindowText(hChild, text, MAX_PATH);
					if (StrStr(text, _T("Movie")))
					{
						hAttachWnd = hChild;
						break;
					}

					if (StrStr(text, _T("VideoRenderer")))
					{
						hAttachWnd = hChild;
						break;
					}
					hChild=::GetNextWindow(hChild, GW_HWNDNEXT);
				}


				UINT nIndex = 0;
				

				UINT nColorValue = GetWindowColorValue(hAttachWnd);
				nIndex = nColorValue / 10 - 1;		//除以10以后反向计算出index

				if (nColorValue == -1 || nIndex != deviceNums)
				{
					bResult = FALSE;
				}
				else
					bResult = TRUE;
			}
		}
		return bResult;
	}

	return COleControl::WindowProc(message, wParam, lParam);
}

LRESULT CSVCBoxCtrl::OnFgNotify(WPARAM wParam, LPARAM lParam)
{
	if (!m_Vedio.ProcessFgNotify(wParam, lParam))
	{
		::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);
	}
	return TRUE;
}



void CSVCBoxCtrl::UpdateCollsepRc(LONG screenLeft, LONG screenTop, LONG screenRight, LONG screenBottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_CollapseScreenRect = CRect(screenLeft, screenTop, screenRight, screenBottom);
}
