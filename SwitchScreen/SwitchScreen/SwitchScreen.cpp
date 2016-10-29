
// SwitchScreen.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "SwitchScreen.h"
#include "SwitchScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSwitchScreenApp

BEGIN_MESSAGE_MAP(CSwitchScreenApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSwitchScreenApp 构造

CSwitchScreenApp::CSwitchScreenApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSwitchScreenApp 对象

CSwitchScreenApp theApp;


// CSwitchScreenApp 初始化

BOOL CSwitchScreenApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	{
		GetAllMonitors();

		if (PrimaryNum == 0)
			SwitchPrimaryScreen(1, PrimaryNum);
		else
			SwitchPrimaryScreen(0, PrimaryNum);
	}

	return FALSE;
	CSwitchScreenDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

void CSwitchScreenApp::GetAllMonitors()
{
	std::list<DISPLAY_DEVICE> devices;
	std::list<DEVMODE> modes;
	int devId = 0;
	bool ret = false;
	bool isPrimary = false;
	//list all DisplayDevices (Monitors)
	do
	{
		DISPLAY_DEVICE displayDevice;
		ZeroMemory(&displayDevice, sizeof(DISPLAY_DEVICE));
		displayDevice.cb = sizeof(displayDevice);
		ret = EnumDisplayDevices(NULL, devId, &displayDevice, 0);
		if (ret == true)
		{
			if ((displayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) == DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
			{
				devices.push_back(displayDevice);
				isPrimary = ((displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) == DISPLAY_DEVICE_PRIMARY_DEVICE);
				if (isPrimary)
					PrimaryNum = devId;
			}
		}
		devId++;
	} while (ret);
	dev_list = devices;
	std::list<DISPLAY_DEVICE>::iterator it;
	for (it = dev_list.begin(); it != dev_list.end(); it++)
	{
		DEVMODE deviceMode;
		deviceMode.dmSize = sizeof(DEVMODE);
		deviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_POSITION | DM_DISPLAYFREQUENCY | DM_DISPLAYFLAGS; // | DM_DISPLAYORIENTATION;
		EnumDisplaySettings(it->DeviceName, (int)ENUM_REGISTRY_SETTINGS, &deviceMode);
		modes.push_back(deviceMode);
	}
	dev_mode_list = modes;
}

void CSwitchScreenApp::SwitchPrimaryScreen(int newPrimary, int oldPrimary)
{
	MoveNewPrimary(newPrimary, oldPrimary);
	MoveOldPrimary(newPrimary, oldPrimary);
	CommitChange();
}

void CSwitchScreenApp::MoveNewPrimary(int newPrimary, int oldPrimary)
{
	int index = 0;
	std::list<DISPLAY_DEVICE>::iterator it1;
	for (it1 = dev_list.begin(); it1 != dev_list.end(); it1++)
	{
		if (index == newPrimary)
			break;
		index++;
	}
	index = 0;
	std::list<DEVMODE>::iterator it2;
	for (it2 = dev_mode_list.begin(); it2 != dev_mode_list.end(); it2++)
	{
		if (index == newPrimary)
			break;
		index++;
	}
	it2->dmPosition.x = 0;
	it2->dmPosition.y = 0;
	DEVMODE deviceMode = *it2;

	int ret = ChangeDisplaySettingsEx(it1->DeviceName, &deviceMode, NULL, CDS_SET_PRIMARY | CDS_UPDATEREGISTRY | CDS_NORESET, NULL);
}
void CSwitchScreenApp::MoveOldPrimary(int newPrimary, int oldPrimary)
{
	int index = 0;
	std::list<DISPLAY_DEVICE>::iterator it1;
	for (it1 = dev_list.begin(); it1 != dev_list.end(); it1++)
	{
		if (index == oldPrimary)
			break;
		index++;
	}
	index = 0;
	std::list<DEVMODE>::iterator it2;
	for (it2 = dev_mode_list.begin(); it2 != dev_mode_list.end(); it2++)
	{
		if (index == newPrimary)
			break;
		index++;
	}
	index = 0;
	std::list<DEVMODE>::iterator it3;
	for (it3 = dev_mode_list.begin(); it3 != dev_mode_list.end(); it3++)
	{
		if (index == oldPrimary)
			break;
		index++;
	}
	if (newPrimary < oldPrimary)
		it3->dmPosition.x = it2->dmPelsWidth;
	else
		it3->dmPosition.x = 0 - it2->dmPelsWidth;
	it3->dmPosition.y = 0;
	DEVMODE deviceMode = *it3;
	int ret = ChangeDisplaySettingsEx(it1->DeviceName, &deviceMode, NULL, CDS_UPDATEREGISTRY | CDS_NORESET, NULL);
}

void CSwitchScreenApp::CommitChange()
{
	ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);
}
int CSwitchScreenApp::SetCloneView(int mode)
{
	/*UINT32 PathArraySize = 0;
	UINT32 ModeArraySize = 0;
	DISPLAYCONFIG_PATH_INFO* PathArray;
	DISPLAYCONFIG_MODE_INFO* ModeArray;
	DISPLAYCONFIG_TOPOLOGY_ID CurrentTopology;
	//Determine the size of the path array that is required to hold all valid paths
	GetDisplayConfigBufferSizes(QDC_ALL_PATHS, &PathArraySize, &ModeArraySize); //retrieve the sizes of the DISPLAYCONFIG_PATH_INFO and DISPLAYCONFIG_MODE_INFO buffers that are required
	//Allocate memory for path and mode information arrays
	PathArray = (DISPLAYCONFIG_PATH_INFO*)malloc(PathArraySize * sizeof(DISPLAYCONFIG_PATH_INFO));
	memset(PathArray, 0, PathArraySize * sizeof(DISPLAYCONFIG_PATH_INFO));
	ModeArray = (DISPLAYCONFIG_MODE_INFO*)malloc(ModeArraySize * sizeof(DISPLAYCONFIG_MODE_INFO));
	ZeroMemory(ModeArray, ModeArraySize * sizeof(DISPLAYCONFIG_MODE_INFO));

	//Request all of the path information
	LONG ret = QueryDisplayConfig(QDC_DATABASE_CURRENT,&PathArraySize, PathArray, &ModeArraySize, ModeArray, &CurrentTopology); //obtain the path and mode information for all posible paths
	// Above CurrentTopology variable will aquire the current display setting (ie Extend, Duplicate etc)
	free(PathArray);
	free(ModeArray);
	*/
	//Set the new topology.
	SetDisplayConfig(0, NULL, 0, NULL, mode | SDC_APPLY); //change to the clone topology
	return 0;
}

int CSwitchScreenApp::ChangeScreenOrientation(int num, int rotation)
{
	int index = 0;
	std::list<DEVMODE>::iterator it;
	for (it = dev_mode_list.begin(); it != dev_mode_list.end(); it++)
	{
		if (index == num)
			break;
		index++;
	}
	DWORD dwTemp = it->dmPelsHeight;
	switch (rotation)
	{
	case 0:
		if (it->dmDisplayOrientation == DMDO_DEFAULT)
			it->dmDisplayOrientation = DMDO_90;
		else if (it->dmDisplayOrientation == DMDO_90)
			it->dmDisplayOrientation = DMDO_DEFAULT;
		it->dmPelsHeight= it->dmPelsWidth;
		it->dmPelsWidth = dwTemp;
		break;
	case 1:
		if (it->dmDisplayOrientation == DMDO_DEFAULT)
			it->dmDisplayOrientation = DMDO_90;
		else if (it->dmDisplayOrientation == DMDO_90)
			it->dmDisplayOrientation = DMDO_DEFAULT;
		it->dmPelsHeight= it->dmPelsWidth;
		it->dmPelsWidth = dwTemp;
		break;
	}
	DEVMODE deviceMode;
	ZeroMemory(&deviceMode, sizeof(deviceMode));
	deviceMode.dmSize = sizeof(deviceMode);
	deviceMode = *it;
	index = 0;
	std::list<DISPLAY_DEVICE>::iterator it1;
	for (it1 = dev_list.begin(); it1 != dev_list.end(); it1++)
	{
		if (index == num)
			break;
		index++;
	}
	//long lRet = ChangeDisplaySettingsEx(it1->DeviceName, &deviceMode, NULL, CDS_UPDATEREGISTRY | CDS_NORESET, NULL);
	//CommitChange();
	long lRet = ChangeDisplaySettingsEx(it1->DeviceName, &deviceMode, NULL, CDS_UPDATEREGISTRY, NULL);
	return lRet;
}