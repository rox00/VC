
// RIM 3.0.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRIM30App:
// �йش����ʵ�֣������ RIM 3.0.cpp
//

class CRIM30App : public CWinAppEx
{
public:
	CRIM30App();
	~CRIM30App();
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int				ExitInstance();
	BOOL					ChangeWndMessageFilter(UINT uMessage, BOOL bAllow);
	HMODULE					m_hinstRE;


	IRIM30MsgServer 		*m_pServer;
};

extern CRIM30App theApp;