#pragma once

#include "IUtils.h"
//#include "UserThread.h"
//接口用String，方便其他不用MFC的类；内部实现用CString，实现简便
class NetUtils;
//class CThreadUDPMonitorRecive : public CUserThread
//{
//public:
//	virtual ~CThreadUDPMonitorRecive();
//
//	enum	{ RECIVEBUFLEN = 4096 };
//protected:
//	CThreadUDPMonitorRecive();
//
//	virtual UINT								ThreadProc();
//	int											SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds);
//
//	PhoneUtils*									m_pPhoneUtils;
//	LPBYTE										m_pBuf;
//
//	SOCKET										m_hChatMonitorSocket;
//	static CThreadUDPMonitorRecive*				m_pChatMonitorReciveThread;	//内部实例指针
//public:
//	virtual HRESULT								Begin(PhoneUtils* pPhoneUtils, BOOL bInitCom = FALSE);
//	static CThreadUDPMonitorRecive* 			GetInstance();
//	static void 								ReleaseInstance();
//};

class NetUtils : public IUtils
{
public:
	NetUtils();
	~NetUtils(void);

public:
	typedef std::vector<String> StringArray;

public:	
	BOOL									StartServer();
	BOOL									KillServer();
	BOOL									RestartServer();
	BOOL									HasDevice();

	BOOL									GetSerialNo(StringArray &serials);
	BOOL									GetIPStr(CString &ipStr);
	BOOL									IsDeviceOnline(AndroidDevice *device);
	BOOL									GetBrandName(IN StringArray &serials, OUT StringArray &brands);

	BOOL									InstallApk(LPCTSTR strApkFilePath);
	BOOL									UpgradeApk(LPCTSTR strApkFilePath);

	UINT									ForwardPort(UINT uAndroidPort);
	void									SetCurrentSerialno(LPCTSTR szSerialno);
	void									SetCurrentIP(LPCTSTR szIP);

	std::wstring							PushFile(LPCTSTR fileName, LPCTSTR destPath);

	BOOL									RebootDevice();

	BOOL									DeleteFile(LPCTSTR fileName);

	virtual UTILSTYPE						GetUtilsType();
protected:
	BOOL									CheckPort(UINT uPort);
private:
	CString									m_strSerialno;
	CString									m_ipStr;
};

