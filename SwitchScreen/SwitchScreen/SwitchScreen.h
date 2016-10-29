
// SwitchScreen.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <list>

// CSwitchScreenApp: 
// �йش����ʵ�֣������ SwitchScreen.cpp
//

class CSwitchScreenApp : public CWinApp
{
public:
	CSwitchScreenApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

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