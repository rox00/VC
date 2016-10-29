
// SwitchScreen.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <list>

// CSwitchScreenApp: 
// 有关此类的实现，请参阅 SwitchScreen.cpp
//

class CSwitchScreenApp : public CWinApp
{
public:
	CSwitchScreenApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

	void						GetAllMonitors();
	void						SwitchPrimaryScreen(int newPrimary, int oldPrimary);
	void						MoveNewPrimary(int newPrimary, int oldPrimary);
	void						MoveOldPrimary(int newPrimary, int oldPrimary);
	void						CommitChange();
	int							ChangeScreenOrientation(int num, int rotation);
	int							SetCloneView(int mode);
	std::list<DISPLAY_DEVICE>	dev_list;
	std::list<DEVMODE>			dev_mode_list;

	int							PrimaryNum;
};

extern CSwitchScreenApp theApp;