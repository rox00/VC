#include "StdAfx.h"
#include "AdbUtils.h"
#include "TSocket.h"
#include <ConsoleUtil.h>

#include <WinSock2.h>
#include <tcpmib.h>
#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")
AdbUtils::AdbUtils(LPCTSTR lpszAdbPathName)
	: m_strAdbPathName(lpszAdbPathName)
{
}

AdbUtils::~AdbUtils(void)
{
}

HRESULT AdbUtils::GetPortState(ULONG nPort, ULONG *nStateID)
{
	MIB_TCPTABLE TcpTable[100];
	DWORD nSize = sizeof(TcpTable);
	if (NO_ERROR == GetTcpTable(&TcpTable[0], &nSize, TRUE))
	{
		DWORD nCount = TcpTable[0].dwNumEntries;
		if (nCount > 0)
		{
			for (DWORD i=0; i<nCount; i++)
			{
				MIB_TCPROW TcpRow = TcpTable[0].table[i];
				DWORD temp1 = TcpRow.dwLocalPort;
				int temp2 = temp1 / 256 + (temp1 % 256) * 256;
				if (temp2 == nPort)
				{
					*nStateID = TcpRow.dwState;
					return S_OK;
				}
			}
		}
		return S_FALSE;
	}
	return S_FALSE;
}

BOOL AdbUtils::PortUsedTCP(ULONG uPort)
{
	MIB_TCPTABLE TcpTable[100];
	DWORD nSize = sizeof(TcpTable);
	if (NO_ERROR == GetTcpTable(&TcpTable[0], &nSize, TRUE))
	{
		DWORD nCount = TcpTable[0].dwNumEntries;
		if (nCount > 0)
		{
			for (DWORD i=0; i<nCount; i++)
			{
				MIB_TCPROW TcpRow = TcpTable[0].table[i];
				DWORD temp1 = TcpRow.dwLocalPort;
				int temp2 = temp1 / 256 + (temp1 % 256) * 256;
				if (temp2 == uPort)
				{
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	return FALSE;
}

/***********************************************************
功能：执行 "adb start-server"命令，启动adb
***********************************************************/
BOOL AdbUtils::StartServer()
{
	CString strCmd;
	strCmd.Format(_T("%s start-server"), m_strAdbPathName);
	BOOL bRet = ConsoleUtil::Execute(strCmd) != ConsoleUtil::ERROR_RETURN;	
	return (bRet);
}

/***********************************************************
功能：执行 "adb kill-server"命令，关闭adb
***********************************************************/
BOOL AdbUtils::KillServer()
{
	CString strCmd;
	strCmd.Format(_T("%s kill-server"), m_strAdbPathName);
	return (ConsoleUtil::Execute(strCmd) != ConsoleUtil::ERROR_RETURN);
}

/***********************************************************
功能：重新启动adb
***********************************************************/
BOOL AdbUtils::RestartServer()
{
	Common::SystemHelper::KillProcess(_T("adb.exe"));
	KillServer();
	return StartServer();
}

BOOL AdbUtils::DeleteFile(LPCTSTR fileName){
	CString strCmd;
	std::wstring strResult;

	strCmd.Format(_T("%s -s %s shell rm \"%s\""), m_strAdbPathName, m_strSerialno, fileName);
	//cmd = m_strAdbPathName + _T(" shell rm ") + fileName;
	Common::LogHelper::log(_T("adbutil"), L4CP_INFO_LOG_LEVEL, strCmd);
	strResult = ConsoleUtil::Execute(strCmd, 30 * 60 * 1000).c_str();
	
	return TRUE;
}

UTILSTYPE AdbUtils::GetUtilsType()
{
	return UTILSTYPE_ADB;
}

// check only one time
BOOL AdbUtils::HasDevice()
{
	CString strCmd, strResult;

	strCmd.Format(_T("%s devices"), m_strAdbPathName);
	strResult = ConsoleUtil::Execute(strCmd).c_str();
	if(strResult.Find(_T("error:")) != -1)
	{
		RestartServer();
		strResult = ConsoleUtil::Execute(strCmd).c_str();
		if(strResult.Find(_T("error:")) != -1)
		{
			return FALSE;
		}
	}

	auto nRet = strResult.Find(_T("device\r\n"), 16);
	return (nRet > 0);
}

/***********************************************************
功能: 给手机安装应用程序
***********************************************************/
BOOL AdbUtils::InstallApk(LPCTSTR strApkFilePath)
{
	CString strCmd, strResult;

	strCmd.Format(_T("%s -s %s install -r \"%s\""), m_strAdbPathName, m_strSerialno, strApkFilePath);
	strResult = ConsoleUtil::Execute(strCmd, 2*60*1000).c_str(); 	// 2分钟
	auto nRet = strResult.Find(_T("Success"));
	
	return (nRet > 0);
}

/***********************************************************
功能：升级手机应用程序
***********************************************************/
BOOL AdbUtils::UpgradeApk(LPCTSTR strApkFilePath)
{
	CString strCmd, strResult;

	strCmd.Format(_T("%s -s %s install -r \"%s\""), m_strAdbPathName, m_strSerialno, strApkFilePath);
	strResult = ConsoleUtil::Execute(strCmd).c_str();
	auto nRet = strResult.Find(_T("Success"));

	return (nRet > 0); 
}

/***********************************************************
功能：运行某个shell命令
***********************************************************/
CString AdbUtils::RunShellCommand(LPCTSTR strCommand, LPCTSTR strSerialNo, DWORD nWaitTime/* = INFINITE*/)
{
	if (!strSerialNo)
		strSerialNo = m_strSerialno.GetBuffer(0);

	CString strCmd, strResult;
	if ((CString)strSerialNo == _T(""))
		strCmd.Format(_T("%s shell %s"), m_strAdbPathName, strCommand);
	else
		strCmd.Format(_T("%s -s %s shell %s"), m_strAdbPathName, strSerialNo, strCommand);
	strResult = ConsoleUtil::Execute(strCmd, nWaitTime).c_str();
	
	return strResult;
}

UINT AdbUtils::GetState()
{
	CString strCmd, strResult;


	strCmd.Format(_T("%s get-state"), m_strAdbPathName);

	strResult	= ConsoleUtil::Execute(strCmd).c_str();
	if (StrStr(strResult, _T("device")))
	{
		return 1;
	}

	return 0;
}

/***********************************************************
功能：端口映射
***********************************************************/
UINT AdbUtils::ForwardPort(UINT uAndroidPort)
{
	CString strCmd, strResult;

	UINT	uPCPort = uAndroidPort;
	while(!CheckPort(uPCPort))
		uPCPort++;
	strCmd.Format(_T("%s -s %s forward tcp:%d tcp:%d"), m_strAdbPathName, m_strSerialno, uPCPort, uAndroidPort);

	strResult	= ConsoleUtil::Execute(strCmd).c_str();

	auto nRet0	= strResult.Find(_T("error"));
	auto nRet1	= strResult.Find(_T("not"));
	if (nRet0 != -1 || nRet1 != -1)
		return 0;

	return uPCPort;
}

/***********************************************************
功能：执行adb devices命令获取设备列表
***********************************************************/
BOOL AdbUtils::GetSerialNo(StringArray &serials)
{
	int					iFirstDevLinePos;
	CString				strCmd;
	CString				strTemp;
	CString				strResult;
	StringArray			tempSnVector;

	strCmd.Format(_T("%s devices"), m_strAdbPathName);
	strResult = ConsoleUtil::Execute(strCmd).c_str();
	if(strResult.Find(_T("error:")) != -1)	// adb 出错
	{
		RestartServer();
		strResult = ConsoleUtil::Execute(strCmd).c_str();
		if (strResult.Find(_T("error:")) != -1)
		{
			return FALSE;
		}
	}

	if ((iFirstDevLinePos = strResult.Find(_T("device\r\n"))) < 0)
		return TRUE;
	strTemp				= strResult.Left(iFirstDevLinePos - 2);
	iFirstDevLinePos	= strTemp.ReverseFind(_T('\n'));
	strResult			= strResult.Mid(iFirstDevLinePos+1);
	tempSnVector.clear();

	int			nPos		= 0;
	int			nTokenPos	= 0;
	CString		resToken;
	resToken	= strResult.Tokenize(_T("\r\n"), nTokenPos);
	while(!resToken.IsEmpty())
	{
		if ((nPos = resToken.Find(_T("device"))) > 0)
		{
			CString strSn(resToken.GetBuffer(0), nPos);
			strSn.TrimLeft(_T("\t "));
			strSn.TrimRight(_T("\t "));
			if (!strSn.IsEmpty())
				tempSnVector.push_back(strSn.GetBuffer());
		}
		
		resToken = strResult.Tokenize(_T("\r\n"), nTokenPos);
	}

	// 去重
	serials.clear();
	BOOL bRepeated = FALSE;
	for(auto iter = tempSnVector.begin(); iter != tempSnVector.end(); iter++)
	{
		auto& szCurSn = *iter;

		for (auto sn = serials.begin(), end = serials.end(); sn != end; sn++)
		{
			if (*sn == szCurSn)
			{
				bRepeated = TRUE;
				break;
			}
		}
		if (bRepeated)
		{ 
			bRepeated = FALSE;
			continue;
		}
		serials.push_back(szCurSn);
	}

	return TRUE;
}

/***********************************************************
功能：检查设备是否还在
***********************************************************/
BOOL AdbUtils::IsDeviceOnline(AndroidDevice *device)
{
	StringArray serials;
	if (GetSerialNo(serials) && serials.size() > 0)
	{
		for (auto sn=serials.begin(), end=serials.end(); sn != end; sn++)
		{
			if(*sn == device->GetSerialNo().GetBuffer())
				return TRUE;
		}
	}
	return FALSE;
}

/***********************************************************
功能：根据列表名获得设备品牌名称
***********************************************************/
BOOL AdbUtils::GetBrandName(StringArray &serials, StringArray &brands)
{
	CString strCmd1, strCmd2, strTemp, strResult;

	int iPos;
	brands.clear();
	for(auto iter = serials.begin(); iter != serials.end(); iter++)
	{
		CString strSn = iter->c_str();
		LPCTSTR lpszEmulatorPrefix = _T("emulator-");
		if (strSn.Left(_tcslen(lpszEmulatorPrefix)).Compare(lpszEmulatorPrefix) == 0)
		{
			brands.push_back(strSn.GetBuffer());
			continue;
		}
		strCmd1.Format(_T("%s -s %s shell getprop ro.product.brand"), m_strAdbPathName, strSn);
		strCmd2.Format(_T("%s -s %s shell getprop ro.product.model"), m_strAdbPathName, strSn);
		strTemp.Empty();
		strResult.Empty();
		// 获得品牌
		strResult = ConsoleUtil::Execute(strCmd1).c_str();
		if (strResult.Find(_T("error: device not found")) >= 0)	// 设备已经没有了
			continue;
		if (strResult.Find(_T("error:")) >= 0)	// adb 出错
		{
			RestartServer();
			strResult = ConsoleUtil::Execute(strCmd1).c_str();
			if(strResult.Find(_T("error:"))  >= 0)
			{
				return FALSE;
			}
		}

		if ((iPos = strResult.Find(_T("\r\r\n"))) == -1)	// 结果中没找到
			continue;
		strResult = strResult.Left(iPos); 
		if ((iPos = strResult.ReverseFind(_T('\n'))) != -1)
			strResult = strResult.Mid(iPos + 1);
		strResult.TrimLeft(_T("\t\r "));
		strResult.TrimRight(_T("\t\r "));
		if (strResult.IsEmpty())
			continue;
		strTemp = strResult + _T("  ");
		
		// 获得型号
		strResult = ConsoleUtil::Execute(strCmd2).c_str();
		if (strResult.Find(_T("error: device not found")) != -1)	// 设备已经没有了
			continue;
		if (strResult.Find(_T("error:")) != -1)	// adb 出错
		{
			RestartServer();
			strResult = ConsoleUtil::Execute(strCmd2).c_str();
			if(strResult.Find(_T("error:")) != -1)
			{
				return FALSE;
			}
		}

		if ((iPos = strResult.Find(_T("\r\r\n"))) == -1)	// 结果中没找到
			continue;
		strResult = strResult.Left(iPos); 
		if ((iPos = strResult.ReverseFind(_T('\n'))) != -1)
			strResult = strResult.Mid(iPos + 1);
		strResult.TrimLeft(_T("\t\r "));
		strResult.TrimRight(_T("\t\r "));
		if (strResult.IsEmpty())
			continue;
		strTemp = strTemp + strResult;

		brands.push_back(strTemp.GetBuffer());
	}
	return TRUE;
}

/***********************************************************
功能：测试端口是否可以listen
***********************************************************/
BOOL AdbUtils::CheckPort(UINT uPort)
{
	SOCKET sockRet = NULL;
	if (uPort < 65536)
	{
		if (!PortUsedTCP(uPort))
		{
			SOCKADDR_IN addr;
			BOOL  bDontLinger		= FALSE;
			SOCKET sock				= socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
			if (sock != INVALID_SOCKET)
			{
				setsockopt(sock, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
				addr.sin_family		= AF_INET;
				addr.sin_port		= htons((short)uPort);
				addr.sin_addr.s_addr= inet_addr("127.0.0.1");
				if (SOCKET_ERROR == bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr)))
				{
					INT AA = WSAGetLastError();
					closesocket(sock);
				}
				else
				{
					if (SOCKET_ERROR == listen(sock, SOMAXCONN))
					{
						closesocket(sock);
					}
					else
					{
						sockRet = sock;
					}
				}
			}
		}
	}

	if (sockRet)//表示建立成功，说明该端口未被占用。
	{
		if (sockRet != 0)
		{
			shutdown(sockRet, 0x02);
			closesocket(sockRet);
		}
		return TRUE;
	}
	else
		return FALSE;
}

std::wstring AdbUtils::PushFile(LPCTSTR fileName, LPCTSTR destPath){
	
	CString strCmd;
	std::wstring strResult;
	
	strCmd		= m_strAdbPathName + _T(" remount");
	strResult	= ConsoleUtil::Execute(strCmd, 1 * 60 * 1000);
	strCmd		= "";

	strCmd.Format(_T("%s -s %s push \"%s\" %s"), m_strAdbPathName, m_strSerialno, fileName, destPath);
	//cmd = m_strAdbPathName + _T(" -s ") + m_strSerialno +_T(" push \"") + CString(fileName) + _T("\" ") + CString(destPath);
	Common::LogHelper::log(_T("adbutil"), L4CP_INFO_LOG_LEVEL, strCmd);
	strResult	= ConsoleUtil::Execute(strCmd, 30 * 60 * 1000).c_str();
	return strResult;
}

BOOL AdbUtils::RebootDevice(){
	CString strCmd;
	strCmd.Format(_T("%s -s %s reboot"), m_strAdbPathName, m_strSerialno);
	BOOL bRet = ConsoleUtil::Execute(strCmd) != ConsoleUtil::ERROR_RETURN;
	return (bRet);
}


BOOL AdbUtils::GetIPStr(CString &ipStr)
{
	ipStr = _T("127.0.0.1");
	return TRUE;
}

void AdbUtils::SetCurrentSerialno(LPCTSTR szSerialno)
{
	m_strSerialno = szSerialno;
}

void AdbUtils::SetCurrentIP(LPCTSTR szIP)
{
}

