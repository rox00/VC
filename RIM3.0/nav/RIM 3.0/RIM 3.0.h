
// RIM 3.0.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CRIM30App:
// 有关此类的实现，请参阅 RIM 3.0.cpp
//

class CRIM30App : public CWinAppEx
{
public:
	CRIM30App();
	~CRIM30App();
// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int				ExitInstance();
	BOOL					ChangeWndMessageFilter(UINT uMessage, BOOL bAllow);
	HMODULE					m_hinstRE;


	IRIM30MsgServer 		*m_pServer;
};

extern CRIM30App theApp;