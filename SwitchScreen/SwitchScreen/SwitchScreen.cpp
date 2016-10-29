
// SwitchScreen.cpp : ����Ӧ�ó��������Ϊ��
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


// CSwitchScreenApp ����

CSwitchScreenApp::CSwitchScreenApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSwitchScreenApp ����

CSwitchScreenApp theApp;


// CSwitchScreenApp ��ʼ��

BOOL CSwitchScreenApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
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
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
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