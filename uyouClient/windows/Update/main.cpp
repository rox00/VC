
#include <windows.h>
#include "stdio.h"
#include <string>
#include "../Common/Common.h"
#ifdef _DEBUG
#pragma comment(lib, "CommonD.lib")
#else
#pragma comment(lib, "Common.lib")
#endif

#define MAINPROCESS _T("UYouMain.exe")

BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	while (true)
	{
		MSG msg;
		PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		CHAR str[100];
		::GetWindowTextA(hwnd, str, sizeof(str));
		std::string sss = str;

		if (sss.find("U游播放器") != std::string::npos)
		{
			//关闭播放器
			::SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		else{
			break;
		}
	}
	
	return 1;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd){
	
	EnumWindows(lpEnumFunc, NULL);
	int r = 0;
	if (strcmp(lpCmdLine, "") == 0)
	{
		SetProcessDPIAware();
		DEVMODE devMode;
		memset(&devMode, 0, sizeof(DEVMODE));
		devMode.dmSize = sizeof(devMode);
		::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

		DEVMODE mode;
		memset(&mode, 0, sizeof(DEVMODE));
		mode.dmSize = sizeof(DEVMODE);
		memcpy(&mode, &devMode, sizeof(DEVMODE));
		
		mode.dmLogPixels = 96;
		mode.dmFields = DM_LOGPIXELS;
		if (ChangeDisplaySettings(&mode, 0) != DISP_CHANGE_SUCCESSFUL)
		{
			ChangeDisplaySettings(&devMode, 0);
		}
		r = WinExec("UGameMain.exe", SW_SHOW);
	}
	else{
		Common::SystemHelper::KillProcess(MAINPROCESS);
		Common::SystemHelper::KillProcess(_T("BoxManager.exe"));
		Common::SystemHelper::KillProcess(_T("adb.exe"));
		r = WinExec(lpCmdLine, SW_HIDE);		
	}
		
	return r;
}