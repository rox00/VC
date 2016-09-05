#pragma once
#include "../Common/Common.h"
#include "AndroidDevice.h"
#include <vector>
#include <types.h>
enum UTILSTYPE
{
	UTILSTYPE_ADB,
	UTILSTYPE_NET
};
typedef std::vector<String> StringArray;

__interface IUtils
{
	virtual BOOL			StartServer()													= 0;
	virtual BOOL			KillServer()													= 0;
	virtual BOOL			RestartServer()													= 0;
	virtual BOOL			HasDevice()														= 0;

	virtual BOOL			GetSerialNo(StringArray &serials)								= 0;
	virtual BOOL			GetIPStr(CString &ipStr)										= 0;

	virtual BOOL			IsDeviceOnline(AndroidDevice *device)							= 0;
	virtual BOOL			GetBrandName(IN StringArray &serials, OUT StringArray &brands)	= 0;

	virtual BOOL			InstallApk(LPCTSTR strApkFilePath)								= 0;
	virtual BOOL			UpgradeApk(LPCTSTR strApkFilePath)								= 0;

	virtual UINT			ForwardPort(UINT uAndroidPort)									= 0;
	virtual void			SetCurrentSerialno(LPCTSTR szSerialno)							= 0;
	virtual void			SetCurrentIP(LPCTSTR szIP)										= 0;

	virtual std::wstring	PushFile(LPCTSTR fileName, LPCTSTR destPath)					= 0;

	virtual BOOL			RebootDevice()													= 0;

	virtual BOOL			DeleteFile(LPCTSTR fileName)									= 0;

	virtual UTILSTYPE		GetUtilsType()													= 0;
};