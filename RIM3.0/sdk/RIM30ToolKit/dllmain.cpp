// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

//////加载与DLL同目录的的Sqlite数据库文件
string RIM_RTK_BSD_DB_FILE = "";
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			TCHAR szPath[MAX_PATH] = {0};
			GetModuleFileName(hModule, szPath, MAX_PATH);
			*(_tcsrchr(szPath, _T('\\'))) = NULL;
			*(_tcsrchr(szPath, _T('\\'))) = NULL;
			RIM_RTK_BSD_DB_FILE = szPath;
			RIM_RTK_BSD_DB_FILE += _T("\\data\\BOSD.SDB");
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

