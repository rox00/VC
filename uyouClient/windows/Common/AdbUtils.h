#pragma once

#include <vector>
#include <types.h>
#include "AndroidDevice.h"
#include "Common.h"

#define ADB_EXE_RELATIVE_PATH  _T("\\adb.exe")

//接口用String，方便其他不用MFC的类；内部实现用CString，实现简便

class AdbUtils
{
public:
	AdbUtils(LPCTSTR lpszAdbPathName);
	~AdbUtils(void);

public:
	typedef std::vector<String> StringArray;
	static HRESULT				GetPortState(ULONG nPort, ULONG *nStateID);
	UINT						GetState();
	static BOOL					PortUsedTCP(ULONG uPort);

public:	
	BOOL						StartServer();
	BOOL						KillServer();
	BOOL						RestartServer();
	BOOL						HasDevice();	

	BOOL						GetSerialNo(StringArray &serials);
	BOOL						IsDeviceOnline(AndroidDevice *device);
	BOOL						GetBrandName(IN StringArray &serials, OUT StringArray &brands);

	BOOL						InstallApk(LPCTSTR strApkFilePath);
	BOOL						UpgradeApk(LPCTSTR strApkFilePath);

	CString						RunShellCommand(LPCTSTR strCommand, LPCTSTR strSerialNo, DWORD nWaitTime = INFINITE);
	UINT						ForwardPort(UINT uAndroidPort);
	void						SetCurrentSerialno(LPCTSTR szSerialno) { m_strSerialno = szSerialno;}
	std::wstring				PushFile(LPCTSTR fileName, LPCTSTR destPath);
	BOOL						RebootDevice();
	BOOL						DeleteFile(LPCTSTR fileName);
protected:
	BOOL						CheckPort(UINT uPort);
private:
	CString						m_strSerialno;
	const CString				m_strAdbPathName;
};

