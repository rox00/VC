
// myffmpeg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CmyffmpegApp: 
// �йش����ʵ�֣������ myffmpeg.cpp
//

class CmyffmpegApp : public CWinApp
{
public:
	CmyffmpegApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CmyffmpegApp theApp;