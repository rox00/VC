
// SVCBoxMonitorCtrl.cpp : Implementation of the CSVCBoxMonitorCtrl ActiveX Control class.

#include "stdafx.h"
#include "SVCBoxMonitor.h"
#include "SVCBoxMonitorCtrl.h"
#include "SVCBoxMonitorPropPage.h"
#include "afxdialogex.h"
#include <atlbase.h>
#include <atlstr.h>
#include <atlconv.h>
#include <atldef.h>
#include <atlcom.h>
#include <atlenc.h>
#include "Channel/IMEAppProtocol.h"
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

IMPLEMENT_DYNCREATE(CSVCBoxMonitorCtrl, COleControl)

// Message map

BEGIN_MESSAGE_MAP(CSVCBoxMonitorCtrl, COleControl)

	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_NOTIFY_CONNECT_STATE_CHANGED, OnConnectStateChanged)
	ON_MESSAGE(WM_NOTIFY_RESTART, OnNotifyReStart)
	ON_WM_INPUTLANGCHANGEREQUEST()
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CSVCBoxMonitorCtrl, COleControl)
	INTERFACE_PART(CSVCBoxMonitorCtrl, __uuidof(IDispatch), Dispatch)
END_INTERFACE_MAP()

// Dispatch map
//��������control������
BEGIN_DISPATCH_MAP(CSVCBoxMonitorCtrl, COleControl)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "Start", dispidStart, Start, VT_UI4, VTS_UI4 VTS_UI4 VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "LoadKeyMapping", dispidLoadKeyMapping, LoadKeyMapping, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "SetKeyMappingOpen", dispidSetKeyMappingOpen, SetKeyMappingOpen, VT_UI4, VTS_I2)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "SaveKeyMappingConfig", dispidSaveKeyMappingConfig, SaveKeyMappingConfig, VT_UI4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "SetRunningApp", dispidSetRunningApp, SetRunningApp, VT_UI4, VTS_BSTR)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "OpenTaskManager", dispidOpenTaskManager, OpenTaskManager, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "CallAndroidBack", dispidCallAndroidBack, CallAndroidBackKey, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "CallAndroidHomeKey", dispidCallAndroidHomeKey, CallAndroidHomeKey, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "Exit", dispidExit, Exit, VT_UI4, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "ClearTouchedKey", dispidClearTouchedKey, ClearTouchedKey, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "UpdateVedioRect", dispidUpdateVedioRect, UpdateVedioRect, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "SetFlag", dispidSetFlag, SetFlag, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "UpdateDeviceSize", dispidUpdateDeviceSize, UpdateDeviceSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSVCBoxMonitorCtrl, "UpdateCollsepRc", dispidUpdateCollsepRc, UpdateCollsepRc, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_DISPATCH_MAP()

// Event map
//��control������������Ϣ
BEGIN_EVENT_MAP(CSVCBoxMonitorCtrl, COleControl)
	EVENT_CUSTOM_ID("NotifyConnectStateChanged", eventidNotifyConnectStateChanged, NotifyConnectStateChanged, VTS_UI4)
END_EVENT_MAP()

// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSVCBoxMonitorCtrl, 1)
	PROPPAGEID(CSVCBoxMonitorPropPage::guid)
END_PROPPAGEIDS(CSVCBoxMonitorCtrl)

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSVCBoxMonitorCtrl, "SVCBoxMonitor.SVCBoxMonitorCtrl.1",
	0x6bb1915f, 0x3d5e, 0x43ad, 0xb3, 0x8a, 0xb4, 0xec, 0xe5, 0x1c, 0xb, 0xa9 )

// Type library ID and version

IMPLEMENT_OLETYPELIB(CSVCBoxMonitorCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID IID_DSVCBoxMonitor ={ 0x6f20c68b, 0x855e, 0x481e, { 0x9e, 0xc4, 0xb9, 0x38, 0xed, 0x23, 0x4e, 0xfe } };
const IID IID_DSVCBoxMonitorEvents ={ 0xc8d375e0, 0x900b, 0x4427, { 0xaa, 0x44, 0xfd, 0x41, 0xbc, 0x68, 0xf5, 0x3b } };

// Control type information

static const DWORD _dwSVCBoxMonitorOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSVCBoxMonitorCtrl, IDS_SVCBoxMonitor, _dwSVCBoxMonitorOleMisc)

// CSVCBoxMonitorCtrl::CSVCBoxMonitorCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSVCBoxMonitorCtrl

BOOL CSVCBoxMonitorCtrl::CSVCBoxMonitorCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SVCBoxMonitor,
			IDB_SVCBoxMonitor,
			afxRegApartmentThreading,
			_dwSVCBoxMonitorOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}
// CSVCBoxMonitorCtrl::CSVCBoxMonitorCtrl - Constructor
CSVCBoxMonitorCtrl::CSVCBoxMonitorCtrl()
	: m_KeyboardMapper(g_szEXEPath)
	, m_strCurrentRunningApp("com.blue.uyou.gamelauncher")
	, m_strSDCardPath("")
	, m_inputValue(0)
	, m_bAutoReconnect(FALSE)
	, m_bExit(FALSE)
	, m_hwndVedio(NULL)
	, m_fCoordinateXMappingRatio(0)
	, m_fCoordinateYMappingRatio(0)
	, m_vedioScreenRect()
	, m_CollapseScreenRect()
	, m_CollapseVedioClientRect()
	, m_ipStr(_T(""))
	, m_pThreadConnectProc(NULL)
{
	InitializeIIDs(&IID_DSVCBoxMonitor, &IID_DSVCBoxMonitorEvents);
	m_className = CA2T(this->GetRuntimeClass()->m_lpszClassName);	
	InputManager::GetInstance().GetTouchScreen().SetTouchMappingFunction(boost::bind(&CSVCBoxMonitorCtrl::ClientToDeviceScreen, this, _1, _2));
	InputManager::GetInstance().SetDeviceScreenToClientFunc(boost::bind(&CSVCBoxMonitorCtrl::DeviceScreenToScreen, this, _1, _2));
}
// CSVCBoxMonitorCtrl::~CSVCBoxMonitorCtrl - Destructor
CSVCBoxMonitorCtrl::~CSVCBoxMonitorCtrl()
{	
	Common::LogHelper::Close();	
}
// CSVCBoxMonitorCtrl::AboutBox - Display an "About" box to the user
void CSVCBoxMonitorCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_SVCBoxMonitor);
	dlgAbout.DoModal();
}

int CSVCBoxMonitorCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxEnableControlContainer();

	m_Status = ConnectStatus::DISCONNECT;
	std::string path = Common::SystemHelper::GetCurrentPath();
	path += "\\log";
	CreateDirectoryA(path.c_str(), NULL);

	//��ʼ��Box Manager��stub
	
	return 0;
}

void CSVCBoxMonitorCtrl::OnDestroy()
{	
	//m_Vedio.Release();
	//InputManager::GetInstance().DetachScreenFunc();
	COleControl::OnDestroy();	
}
void CSVCBoxMonitorCtrl::OnClose()
{
	InputManager::GetInstance().ClearMapping();
	COleControl::OnClose(0);
}

ULONG CSVCBoxMonitorCtrl::Start(ULONG bAutoReconnect, ULONG hwndVedio, LPCTSTR strIP)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_bAutoReconnect	= bAutoReconnect;
	m_hwndVedio			= (HWND)hwndVedio;
	m_ipStr				= strIP;
	::PostMessage(m_hWnd, WM_NOTIFY_RESTART, 0, 0);
	return 0;
}

LRESULT CSVCBoxMonitorCtrl::OnConnectStateChanged(WPARAM wParam, LPARAM lParam)
{
	ConnectStatus status = (ConnectStatus)wParam;
	switch (status)
	{
		case ConnectStatus::DISCONNECT:
		{
			if (m_Status != status && !m_WorkingThreadGroup.is_thread_in(m_pThreadConnectProc))
			{	//����Start������¾Ͳ���Ҫ�ٴ��ϱ�����״̬��
				m_Status = status;

				m_vedioDlg.Stop();
				InputManager::GetInstance().DetachScreenFunc();
				m_CollapseVedioClientRect = CRect(0,0,0,0);
				NotifyConnectStateChanged(0);



				if (m_bAutoReconnect)	//��������
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
			{	//��SVCChannel��ʼ����ɺ󣬰�InputManager��SVCChannel��ϵ����
				m_Status = status;

				InputManager::GetInstance().AttachScreenFunc(
					boost::bind(&CVedioDlg::SendTouchScreenMessage, &m_vedioDlg, _1),
					boost::bind(&CVedioDlg::SendInputMethodOperationMessage, &m_vedioDlg, _1)
					);


				string&& strPacket = IMEAppProtocol::GetInstance().ToString(IMEAppProtocol::IMEAppMessage(IMEAppProtocol::EMV_OPER_GETMEDIASTORAGEDIRECTORY, ""));
				TRACE("Send Cmd String is %s \n", strPacket.c_str());
				std::wstring s = CA2W(strPacket.c_str());
				Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, s.c_str());
				ClearTouchedKey();

				NotifyConnectStateChanged(1);		//TODO: ���ܻ�������״̬		
			}

			//PushFileToBox(_T("D:\\APK\\������ɫ app-release.apk"));
			//PushFileToBox(_T("D:\\APK\\com.UCMobile_175.apk"));
		}
		break;
		default:
			break;
	}

	return TRUE;
}

/******************����ӳ��************************/
ULONG CSVCBoxMonitorCtrl::SetRunningApp(LPCTSTR packageName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_strCurrentRunningApp = CT2A(packageName);
	return 0;
}
ULONG CSVCBoxMonitorCtrl::SetKeyMappingOpen(SHORT open)
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
ULONG CSVCBoxMonitorCtrl::LoadKeyMapping(LPCTSTR packageName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//	InputManager::GetInstance().ResetMouseEvent();
	InputManager::GetInstance().ResetMouseHandleStatus();
	ClearTouchedKey();
	m_KeyboardMapper.Load(CT2A(packageName), m_vedioScreenRect, TRUE);

	return S_OK;
}
ULONG CSVCBoxMonitorCtrl::SaveKeyMappingConfig(LPCTSTR configFile, LPCTSTR content)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	String msg;
	msg = _T("SaveKeyMappingConfig ");
	msg += content;
	Common::LogHelper::log(m_className.c_str(), L4CP_INFO_LOG_LEVEL, msg.c_str());
	return m_KeyboardMapper.Save(CT2A(configFile), CT2A(content));	
}
/*************************************************/
ULONG CSVCBoxMonitorCtrl::OpenTaskManager()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	InputManager::GetInstance().SendKeyMessage(KEYFUC_TASKMGR, KEYACTION_FUNC);
	//SendOperateCmd(IMEAppProtocol::EMV_OPER_OPENRECENTAPPLICATIONS, 0, 0, 0, CComBSTR(""));
	return 0;
}

ULONG CSVCBoxMonitorCtrl::CallAndroidBackKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	InputManager::GetInstance().SendKeyMessage(KEYFUC_RETURN, KEYACTION_FUNC);
	//SendOperateCmd(IMEAppProtocol::EMV_OPER_RETURN, 0, 0, 0, CComBSTR(""));
	return 0;
}

ULONG CSVCBoxMonitorCtrl::CallAndroidHomeKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	InputManager::GetInstance().SendKeyMessage(KEYFUC_HOME, KEYACTION_FUNC);
	//SendOperateCmd(IMEAppProtocol::EMV_OPER_HOME, 0, 0, 0, CComBSTR(""));	
	return 0;
}

ULONG CSVCBoxMonitorCtrl::Exit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_bExit		= TRUE;
	m_vedioDlg.Stop();
	return 0;
}

void CSVCBoxMonitorCtrl::ClearTouchedKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_Status == ConnectStatus::CONNECTED)
	{
		InputManager::GetInstance().ClearMapping();
	}	
	m_KeyboardMapper.ClearKeyDownList();
}

String CSVCBoxMonitorCtrl::UpdatePCFile(LPSTR downloadFile)
{
	std::wstring message = CA2W(downloadFile);

	message =  _T("----��ʼ����PC�����") + message;

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

void CSVCBoxMonitorCtrl::BindMappingFunc()
{
	InputManager::GetInstance().GetInputMethodOperationDevice().SetTouchMappingFunction(
		boost::bind(&KeyboardMapper::KeyMappingFunction, &m_KeyboardMapper, _1, _2));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetMouseMappingFunction(
		boost::bind(&KeyboardMapper::MouseMappingFunction, &m_KeyboardMapper, _1, _2));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetIsDirectKeyFunction(
		boost::bind(&KeyboardMapper::IsDirectKey, &m_KeyboardMapper, _1));
}

void CSVCBoxMonitorCtrl::UnBindMappingFunc()
{
	InputManager::GetInstance().GetInputMethodOperationDevice().SetTouchMappingFunction(
		static_cast<InputManager::InputMethodOperationInputDevice::MappingFuncType>(nullptr));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetMouseMappingFunction(
		static_cast<InputManager::InputMethodOperationInputDevice::MappingFuncType>(nullptr));

	InputManager::GetInstance().GetInputMethodOperationDevice().SetIsDirectKeyFunction(
		static_cast<std::function<bool(UINT&)>>(nullptr));

	InputManager::GetInstance().ClearMapping();
}

void CSVCBoxMonitorCtrl::ReStart()
{
	if (m_bExit)
		return;
	::PostMessage(m_hWnd, WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::CONNECTING, 0);

	m_vedioDlg.Stop();

	try
	{
		m_vedioDlg.StartVedio(this, CT2A(m_ipStr), 12580);

		m_vedioDlg.WaitForConnect();
		if (m_vedioDlg.IsRunning())
		{
			::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::CONNECTED, 0);	//�ı�����״̬
			//if (m_Channel.Initialize(GetSafeHwnd(), m_ipStr))
			//{
			//	::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::CONNECTED, 0);	//�ı�����״̬
			//}
			//else
			//{
			//	::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);	//�ı�����״̬
			//}
		}
		else
		{
			m_vedioDlg.Stop();
			::PostMessage(GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);	//�ı�����״̬
		}
	}
	catch (...)
	{

	}

}

void CSVCBoxMonitorCtrl::StartConnectProc()
{
	ReStart();
	m_WorkingThreadGroup.remove_thread(m_pThreadConnectProc);
}

LRESULT CSVCBoxMonitorCtrl::OnNotifyReStart(WPARAM wParam, LPARAM lParam)
{
	if (m_bExit)
		return TRUE;

	if (!IsWindow(m_vedioDlg.GetSafeHwnd()))
	{	////�ȴ�������
		m_vedioDlg.Create(CVedioDlg::IDD, CWnd::FromHandle(m_hwndVedio));

		CRect rcTemp = m_vedioScreenRect;
		CWnd::FromHandle(m_hwndVedio)->ScreenToClient(rcTemp);
		m_vedioDlg.MoveWindow(rcTemp);
		m_vedioDlg.ShowWindow(SW_SHOW);

		InputManager::GetInstance().Initialize(m_hwndVedio);
	}

	if (!m_WorkingThreadGroup.is_thread_in(m_pThreadConnectProc))
	{	////��֤�߳��Ѿ�ִ�����
		m_pThreadConnectProc = m_WorkingThreadGroup.create_thread(std::bind(&CSVCBoxMonitorCtrl::StartConnectProc, this));
	}

	return TRUE;
}

void CSVCBoxMonitorCtrl::ClientToDeviceScreen(int &x, int &y)
{	//ע�⣬��������������������߳����汻���õ�

	POINT pt;
	pt.x = x + m_vedioScreenRect.left;
	pt.y = y + m_vedioScreenRect.top;
	//::ClientToScreen(m_hwndVedio, &pt);

	x = (pt.x - m_vedioScreenRect.left) * m_fCoordinateXMappingRatio + 0.5;
	y = (pt.y - m_vedioScreenRect.top) * m_fCoordinateYMappingRatio + 0.5;
}

void CSVCBoxMonitorCtrl::DeviceScreenToScreen(int &x, int &y)
{	//ע�⣬��������������������߳����汻���õ�

	if (m_fCoordinateXMappingRatio > 0 && m_fCoordinateYMappingRatio > 0)
	{
		POINT pt;

		pt.x = x/m_fCoordinateXMappingRatio + m_vedioScreenRect.left + 0.5;
		pt.y = y/m_fCoordinateYMappingRatio + m_vedioScreenRect.top + 0.5;

		x = pt.x;
		y = pt.y;
	}
}

void CSVCBoxMonitorCtrl::UpdateVedioRect(LONG l, LONG t, LONG r, LONG b)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		// TODO:  �ڴ���ӵ��ȴ���������
		//һ�н�Ϊ��Ļ��������
		CRect rcTemp = CRect(l, t, r, b);
		m_vedioScreenRect = rcTemp;
		if (m_vedioScreenRect.Width())
			m_fCoordinateXMappingRatio = g_currentDeviceWidth * 1.0 / m_vedioScreenRect.Width();
		if (m_vedioScreenRect.Height())
			m_fCoordinateYMappingRatio = g_currentDeviceHeight * 1.0 / m_vedioScreenRect.Height();

		if (m_hwndVedio)
		{
			CWnd::FromHandle(m_hwndVedio)->ScreenToClient(rcTemp);
			m_vedioDlg.MoveWindow(rcTemp);
			//m_Vedio.ReSize(rcTemp);
			if (InputHookHelper::GetInstance().GetAttachWnd())
			{	//�����˳ߴ�  Ҫ���������۵���ť������λ��
				CWnd *pWnd = CWnd::FromHandle(InputHookHelper::GetInstance().GetAttachWnd());

				////��������ڳ�����Ƶ���ڵĿͻ�λ��
				CRect	CollapseVedioClientRect = m_CollapseScreenRect;
				CWnd::FromHandle(m_hwndVedio)->ScreenToClient(CollapseVedioClientRect);

				if (!m_CollapseVedioClientRect.EqualRect(CollapseVedioClientRect))
				{	////�жϻ��߳ߴ���Ķ�����m_CollapseClientRect clospclientRc ��ͬ
					////������Ƶ���ڵĿͻ�����
					CRect   rcClient;
					pWnd->GetClientRect(&rcClient);	////�ͻ������С 

					////�����۵���ť����Ƶ�����ϵĿͻ�����
					CRect	clospclientRc = m_CollapseScreenRect;
					pWnd->ScreenToClient(clospclientRc);	////�Ѱ�ť��Ļ����ת�ɿͻ�����

					HRGN hrgn	= CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
					HRGN hrgn1	= CreateRectRgn(clospclientRc.left, clospclientRc.top, clospclientRc.right, clospclientRc.bottom);

					CombineRgn(hrgn, hrgn, hrgn1, RGN_XOR);
					::SetWindowRgn(InputHookHelper::GetInstance().GetAttachWnd(), hrgn, true);
					::DeleteObject(hrgn1);
					
					m_CollapseVedioClientRect = CollapseVedioClientRect;
				}
			}
		}
	}
	catch (...)
	{
	}
}

void CSVCBoxMonitorCtrl::UpdateDeviceSize(LONG deviceWidth, LONG deviceHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  �ڴ���ӵ��ȴ���������
	g_currentDeviceWidth	= deviceWidth;
	g_currentDeviceHeight	= deviceHeight;

	if (m_vedioScreenRect.Width())
		m_fCoordinateXMappingRatio = g_currentDeviceWidth * 1.0 / m_vedioScreenRect.Width();
	if (m_vedioScreenRect.Height())
		m_fCoordinateYMappingRatio = g_currentDeviceHeight * 1.0 / m_vedioScreenRect.Height();
}

void CSVCBoxMonitorCtrl::SetFlag(LONG nFlag, LONG value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  �ڴ���ӵ��ȴ���������

	switch (nFlag)
	{
		case BoxFlags_IsOpenDeviceControl:
			g_IsOpenDeviceControl = value;
			break;
		case BoxFlags_IsHorizontalMode:
			g_IsHorizontalMode = value;
			break;
		case  BoxFlags_ScreenShot:
			g_bScreenShot = value;
			break;
		default:
			break;
	}
}
COLORREF CSVCBoxMonitorCtrl::GetWindowPixelColor(HWND hWnd, int x, int y)
{
	auto hDC = ::GetDC(hWnd);
	if (hDC == NULL)
		return -1;

	auto iColor = ::GetPixel(hDC, x, y);

	::ReleaseDC(hWnd, hDC);

	return iColor;
}
UINT CSVCBoxMonitorCtrl::GetWindowColorValue(HWND hWnd)
{
	UINT ColorValue = -1;

	POINT PixelList[] =
	{	//��ʱȡ���²�����
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
		if (GotColor == -1)		//��һ�����ȡ��ɫʧ�ܣ�����Ϊʧ��
			return -1;

		//ע�⣺Android����ɫ��Ⱦ���ƱȽ���֡�����Ҫ��(x, x, x)��������ɫ��ʱ��������ÿ��x�ϼ�һ�����ȵ��Ŷ���һ���Ŷ�Ϊ������ż������������ֵ������10
		//����������ж�Ҳ����΢����һ��
		if (abs(GetRValue(GotColor) - GetGValue(GotColor)) >= 10 || abs(GetBValue(GotColor) - GetGValue(GotColor)) >= 10
			|| abs(GetBValue(GotColor) - GetRValue(GotColor)) >= 10)	//����������ȫ���
			return -1;

		UINT GotColorValue = (GetRValue(GotColor) + GetGValue(GotColor) + GetBValue(GotColor)) / 3;

		//��һ�γ���
		if (ColorValue == -1)
			ColorValue = GotColorValue;
		else if (GotColorValue != ColorValue)		//���Ե����ɫ�����һ������Ϊʧ��
			return -1;
	}

	return ColorValue;
}
void CSVCBoxMonitorCtrl::UpdateCollsepRc(LONG screenLeft, LONG screenTop, LONG screenRight, LONG screenBottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_CollapseScreenRect = CRect(screenLeft, screenTop, screenRight, screenBottom);
}
