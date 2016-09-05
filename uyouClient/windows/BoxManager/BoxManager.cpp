// BoxManager.cpp : WinMain µÄÊµÏÖ


#include "stdafx.h"
#include "resource.h"
#include "BoxManager_i.h"
#include "xdlldata.h"

using namespace ATL;

CString g_szEXEPath;

class CBoxManagerModule : public ATL::CAtlExeModuleT< CBoxManagerModule >
{
public :
	DECLARE_LIBID(LIBID_BoxManagerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BOXMANAGER, "{2E09BFAF-7602-4583-A1D2-9F7CC868FDDC}")
};

CBoxManagerModule _AtlModule;

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR lpCmdLine, int nShowCmd)
{
	g_szEXEPath = GetCommandLine();
	g_szEXEPath.Trim(_T('\"'));
	g_szEXEPath = g_szEXEPath.Left(g_szEXEPath.ReverseFind(_T('\\')) + 1);
	return _AtlModule.WinMain(nShowCmd);
}

