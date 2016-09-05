#include "stdafx.h"
#include "NetUtils.h"
#include "TSocket.h"
#include <ConsoleUtil.h>

#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "Iphlpapi.lib")
#include <vector>
using namespace std;
//
//CThreadUDPMonitorRecive::CThreadUDPMonitorRecive()
//{
//	m_pBuf = NULL;
//	m_pPhoneUtils = NULL;
//
//	m_hChatMonitorSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if (m_hChatMonitorSocket == INVALID_SOCKET)
//	{
//		MessageBox(NULL, _T("CThreadUDPMonitorRecive 套接子创建失败"), _T("RIM3.0 错误"), MB_OK);
//		return;
//	}
//
//	ULONG reuse = true;
//	if (setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == SOCKET_ERROR)
//	{
//		MessageBox(NULL, _T("CThreadUDPMonitorRecive SO_REUSEADDR 设置错误\n"), _T("RIM3.0 错误"), MB_OK);
//		closesocket(m_hChatMonitorSocket);
//		return;
//	}
//
//	SOCKADDR_IN addr;
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(17992);
//	addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	int ret = ::bind(m_hChatMonitorSocket, (sockaddr*)&addr, sizeof(addr));
//	if (ret)
//	{
//		MessageBox(NULL, _T("CThreadUDPMonitorRecive 套接子绑定失败\n"), _T("RIM3.0 错误"), MB_OK);
//		closesocket(m_hChatMonitorSocket);
//		return;
//	}
//
//	int nScktOpt = 5000;
//	//设置接收超时
//	if (setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nScktOpt, sizeof(nScktOpt)) == SOCKET_ERROR)
//	{
//		MessageBox(NULL, _T("CThreadUDPMonitorRecive 设置接收超时\n"), _T("RIM3.0 错误"), MB_OK);
//		closesocket(m_hChatMonitorSocket);
//		WSACleanup();
//		return;
//	}
//	//设置发送超时
//	nScktOpt = 5000;
//	if (setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nScktOpt, sizeof(nScktOpt)))
//	{
//		MessageBox(NULL, _T("CThreadUDPMonitorRecive 设置发送超时\n"), _T("RIM3.0 错误"), MB_OK);
//		closesocket(m_hChatMonitorSocket);
//		return;
//	}
//
//	BOOL bBroadCast = TRUE;
//	setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadCast, sizeof(bBroadCast));
//
//	m_pBuf = new BYTE[RECIVEBUFLEN];
//	ZeroMemory(m_pBuf, RECIVEBUFLEN);
//}
//
//int	CThreadUDPMonitorRecive::SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds)
//{
//	assert(sock != INVALID_SOCKET);
//	TIMEVAL timeOut;
//	timeOut.tv_sec	= 0;
//	timeOut.tv_usec = dwMilliseconds * 1000;
//	fd_set readFD;
//	FD_ZERO(&readFD);
//	FD_SET(sock, &readFD);
//	int iRetVal = select(int(sock), &readFD, NULL, NULL, &timeOut);
//
//	return iRetVal;
//}
//
//CThreadUDPMonitorRecive::~CThreadUDPMonitorRecive()
//{
//	if (m_pBuf)
//		delete[] m_pBuf;
//}
//
//UINT CThreadUDPMonitorRecive::ThreadProc()
//{
//	TRACE(_T("%s %d \n"), _T("CThreadUDPMonitorRecive"), GetCurrentThreadId());
//
//	if (m_hChatMonitorSocket)
//	{
//		SOCKADDR_IN fromAddr;
//		int addrLen = sizeof(SOCKADDR_IN);
//
//		DWORD dwRet = 0;
//		while (TRUE)
//		{
//			///第一个句柄代表队列里面有数据
//			dwRet = WaitForSingleObject(m_hExit, 0);
//			if (dwRet == WAIT_OBJECT_0)
//				break;
//
//			if (SelectSocketRead(m_hChatMonitorSocket, 100) > 0)
//			{
//				ZeroMemory(m_pBuf, RECIVEBUFLEN);
//				ZeroMemory(&fromAddr, addrLen);
//				int rcvLen = recvfrom(m_hChatMonitorSocket, (char*)m_pBuf, RECIVEBUFLEN, 0, (LPSOCKADDR)&fromAddr, &addrLen);
//
//				if (rcvLen <= 0)
//				{
//					DWORD error = WSAGetLastError();
//					if (error == WSAEMSGSIZE)	//如果缓冲区太小
//					{
//						continue;
//					}
//
//					AfxMessageBox(_T("CThreadUDPMonitorRecive 套接字接收失败！"));
//					break;
//				}
//				else
//				{
//					string str((char*)m_pBuf, rcvLen);
//
//					if (string::npos != str.find("answer:ok:"))
//					{
//						str = str.substr(strlen("answer:ok:"));
//						if (m_pPhoneUtils)
//						{
//							StringArray serialArray;
//							m_pPhoneUtils->GetSerialNo(serialArray);
//							if (serialArray.size() == 0)
//							{
//								m_pPhoneUtils->SetCurrentSerialno(CA2T(str.c_str()));
//								string addr = inet_ntoa(fromAddr.sin_addr);
//								m_pPhoneUtils->SetCurrentIP(CA2T(addr.c_str()));
//							}
//						}
//					}
//				}
//
//				ZeroMemory(m_pBuf, RECIVEBUFLEN);
//			}
//			else
//			{
//				Sleep(100);
//			}
//		}
//		closesocket(m_hChatMonitorSocket);
//	}
//
//	return 0;
//}
//
//CThreadUDPMonitorRecive* CThreadUDPMonitorRecive::m_pChatMonitorReciveThread = NULL;
//CThreadUDPMonitorRecive* CThreadUDPMonitorRecive::GetInstance()
//{
//	if (m_pChatMonitorReciveThread == NULL)
//	{
//		m_pChatMonitorReciveThread = new CThreadUDPMonitorRecive();
//	}
//
//	return m_pChatMonitorReciveThread;
//}
//
//void CThreadUDPMonitorRecive::ReleaseInstance()
//{
//	if (m_pChatMonitorReciveThread)
//	{
//		delete m_pChatMonitorReciveThread;
//		m_pChatMonitorReciveThread = NULL;
//	}
//}
//
//HRESULT CThreadUDPMonitorRecive::Begin(PhoneUtils* pPhoneUtils, BOOL bInitCom /*= FALSE*/)
//{
//	m_pPhoneUtils = pPhoneUtils;
//	return  CUserThread::Begin(bInitCom);
//}

NetUtils::NetUtils()
	: m_strSerialno(_T(""))
	, m_ipStr(_T(""))
{
	//CThreadUDPMonitorRecive::GetInstance()->Begin(this);
}

NetUtils::~NetUtils(void)
{
	//CThreadUDPMonitorRecive::GetInstance()->End();
	//CThreadUDPMonitorRecive::GetInstance()->ReleaseInstance();
}

bool GetAvalibleBroadcastAddress(vector<String> &broadCastIpArray)
{
	broadCastIpArray.clear();

	// 	CStringArray ipAddresses;

	ULONG ulTalbeSize = 0;
	DWORD res = ::GetIpAddrTable(NULL, &ulTalbeSize, TRUE);
	MIB_IPADDRTABLE *pIpAddTalbe = (MIB_IPADDRTABLE*)new BYTE[ulTalbeSize];
	DWORD ret = ::GetIpAddrTable(pIpAddTalbe, &ulTalbeSize, TRUE);
	if (NO_ERROR == ret)
	{
		DWORD ipAddress = 0;
		for (UINT i = 0; i < pIpAddTalbe->dwNumEntries; i++)
		{
			ipAddress = pIpAddTalbe->table[i].dwAddr;
			if (0 == ipAddress)
				continue;

			ipAddress |= ~(pIpAddTalbe->table[i].dwMask);
			ipAddress = ntohl(ipAddress);

			TCHAR ch[MAX_PATH] ={ 0 };
			_stprintf_s(ch, MAX_PATH, _T("%d.%d.%d.%d"),
				(ipAddress >> 24) & 0x000000ff,
				(ipAddress >> 16) & 0x000000ff,
				(ipAddress >> 8) & 0x000000ff,
				ipAddress & 0x000000ff);

			if (_tcscmp(ch, _T("127.255.255.255")) == 0)
				continue;

			broadCastIpArray.push_back(ch);
		}

		if (pIpAddTalbe)
			delete[] pIpAddTalbe;
		pIpAddTalbe = NULL;

		return true;
	}
	else{

		if (pIpAddTalbe)
			delete[] pIpAddTalbe;
		pIpAddTalbe = NULL;
		return false;
	}
}

/***********************************************************
功能：执行 "adb start-server"命令，启动adb
***********************************************************/
BOOL NetUtils::StartServer()
{
	//CString strCmd;
	//strCmd.Format(_T("%s start-server"), m_strAdbPathName);
	//BOOL bRet = ConsoleUtil::Execute(strCmd) != ConsoleUtil::ERROR_RETURN;	
	//return (bRet);
	return TRUE;
}

/***********************************************************
功能：执行 "adb kill-server"命令，关闭adb
***********************************************************/
BOOL NetUtils::KillServer()
{
	//CString strCmd;
	//strCmd.Format(_T("%s kill-server"), m_strAdbPathName);
	//return (ConsoleUtil::Execute(strCmd) != ConsoleUtil::ERROR_RETURN);
	return TRUE;
}

/***********************************************************
功能：重新启动adb
***********************************************************/
BOOL NetUtils::RestartServer()
{
	//Common::SystemHelper::KillProcess(_T("adb.exe"));
	//KillServer();
	//return StartServer();
	return TRUE;
}

BOOL NetUtils::DeleteFile(LPCTSTR fileName){
	//CString strCmd;
	//std::wstring strResult;

	//CString cmd;

	//
	//cmd = m_strAdbPathName + _T(" shell rm ") + fileName;
	//Common::LogHelper::log(_T("adbutil"), L4CP_INFO_LOG_LEVEL, cmd);
	//strResult = ConsoleUtil::Execute(cmd, 30 * 60 * 1000).c_str();
	//
	//return TRUE;
	return FALSE;
}

UTILSTYPE NetUtils::GetUtilsType()
{
	return UTILSTYPE_NET;
}

// check only one time
BOOL NetUtils::HasDevice()
{
	if (m_strSerialno.GetLength() == 0)
	{
		SOCKET chatMonitorSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (chatMonitorSocket == INVALID_SOCKET)
		{
			MessageBox(NULL, _T("CThreadUDPMonitorSend 套接子创建失败"), _T("RIM3.0 错误"), MB_OK);
			return FALSE;
		}

		string connectStr = "ask:connect";
		BOOL bBroadCast = TRUE;
		setsockopt(chatMonitorSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadCast, sizeof(bBroadCast));

		vector<tstring>  ipList;
		GetAvalibleBroadcastAddress(ipList);
		ipList.push_back(_T("255.255.255.255"));
		for (UINT i = 0; i < ipList.size(); i++)
		{
			SOCKADDR_IN toAddr;
			toAddr.sin_family		= AF_INET;
			toAddr.sin_addr.s_addr	= inet_addr(CT2A(ipList[i].c_str()));
			toAddr.sin_port			= htons(17990);

			int ret = sendto(chatMonitorSocket, connectStr.c_str(), connectStr.length(), 0, (LPSOCKADDR)&toAddr, sizeof(SOCKADDR_IN));
			if (ret == SOCKET_ERROR)
			{
				DWORD error = WSAGetLastError();

				AfxMessageBox(_T("CThreadUDPMonitorSend 套接字发送失败！"));
				break;
			}
		}
		ipList.clear();
		////【zy 2015-02-07 ↑】
	}

	return m_strSerialno.GetLength() > 0;
}

/***********************************************************
功能: 给手机安装应用程序
***********************************************************/
BOOL NetUtils::InstallApk(LPCTSTR strApkFilePath)
{
	//CString strCmd, strResult;

	//strCmd.Format(_T("%s install -r \"%s\""), m_strAdbPathName, strApkFilePath);
	//strResult = ConsoleUtil::Execute(strCmd, 2*60*1000).c_str(); 	// 2分钟
	//auto nRet = strResult.Find(_T("Success"));
	//
	//return (nRet > 0);
	return FALSE;
}

/***********************************************************
功能：升级手机应用程序
***********************************************************/
BOOL NetUtils::UpgradeApk(LPCTSTR strApkFilePath)
{
	//CString strCmd, strResult;

	//strCmd.Format(_T("%s -s %s install -r \"%s\""), m_strAdbPathName, m_strSerialno, strApkFilePath);
	//strResult = ConsoleUtil::Execute(strCmd).c_str();
	//auto nRet = strResult.Find(_T("Success"));

	//return (nRet > 0); 
	return FALSE;
}

/***********************************************************
功能：端口映射
***********************************************************/
UINT NetUtils::ForwardPort(UINT uAndroidPort)
{
	CString strCmd;
	UINT uPCPort = uAndroidPort;
	while(!CheckPort(uPCPort))
		uPCPort++;
	//strCmd.Format(_T("%s -s %s forward tcp:%d tcp:%d"), m_strAdbPathName, m_strSerialno, uPCPort, uAndroidPort);
	//ConsoleUtil::Execute(strCmd);

	return uPCPort;
}

/***********************************************************
功能：执行adb devices命令获取设备列表
***********************************************************/
BOOL NetUtils::GetSerialNo(StringArray &serials)
{
	//int iFirstDevLinePos;
	//CString strCmd, strTemp, strResult;
	//StringArray tempSnVector;
	//strCmd.Format(_T("%s devices"), m_strAdbPathName);
	//strResult = ConsoleUtil::Execute(strCmd).c_str();
	//if(strResult.Find(_T("error:")) != -1)	// adb 出错
	//{
	//	RestartServer();
	//	strResult = ConsoleUtil::Execute(strCmd).c_str();
	//	if (strResult.Find(_T("error:")) != -1)
	//	{
	//		return FALSE;
	//	}
	//}

	//if ((iFirstDevLinePos = strResult.Find(_T("device\r\n"))) < 0)
	//	return TRUE;
	//strTemp = strResult.Left(iFirstDevLinePos - 2);
	//iFirstDevLinePos = strTemp.ReverseFind(_T('\n'));

	//strResult = strResult.Mid(iFirstDevLinePos+1);
	//tempSnVector.clear();

	//int nPos = 0, nTokenPos = 0;
	//CString resToken;
	//resToken = strResult.Tokenize(_T("\r\n"), nTokenPos);
	//while(!resToken.IsEmpty())
	//{
	//	if ((nPos = resToken.Find(_T("device"))) > 0)
	//	{
	//		CString strSn(resToken.GetBuffer(0), nPos);
	//		strSn.TrimLeft(_T("\t "));
	//		strSn.TrimRight(_T("\t "));
	//		if (!strSn.IsEmpty())
	//			tempSnVector.push_back(strSn.GetBuffer());
	//	}
	//	
	//	resToken = strResult.Tokenize(_T("\r\n"), nTokenPos);
	//}

	//// 去重
	//serials.clear();
	//BOOL bRepeated = FALSE;
	//for(auto iter = tempSnVector.begin(); iter != tempSnVector.end(); iter++)
	//{
	//	auto& szCurSn = *iter;

	//	for (auto sn = serials.begin(), end = serials.end(); sn != end; sn++)
	//	{
	//		if (*sn == szCurSn)
	//		{
	//			bRepeated = TRUE;
	//			break;
	//		}
	//	}
	//	if (bRepeated)
	//	{ 
	//		bRepeated = FALSE;
	//		continue;
	//	}
	//	serials.push_back(szCurSn);
	//}


	if (m_strSerialno.GetLength())
	{
		String serialNo = _T("");
		serialNo = m_strSerialno.GetBuffer();
		serials.push_back(serialNo);
	}
	return TRUE;
}

/***********************************************************
功能：检查设备是否还在
***********************************************************/
BOOL NetUtils::IsDeviceOnline(AndroidDevice *device)
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
BOOL NetUtils::GetBrandName(StringArray &serials, StringArray &brands)
{
	//CString strCmd1, strCmd2, strTemp, strResult;

	//int iPos;
	//brands.clear();
	//for(auto iter = serials.begin(); iter != serials.end(); iter++)
	//{
	//	CString strSn = iter->c_str();
	//	LPCTSTR lpszEmulatorPrefix = _T("emulator-");
	//	if (strSn.Left(_tcslen(lpszEmulatorPrefix)).Compare(lpszEmulatorPrefix) == 0)
	//	{
	//		brands.push_back(strSn.GetBuffer());
	//		continue;
	//	}
	//	strCmd1.Format(_T("%s -s %s shell getprop ro.product.brand"), m_strAdbPathName, strSn);
	//	strCmd2.Format(_T("%s -s %s shell getprop ro.product.model"), m_strAdbPathName, strSn);
	//	strTemp.Empty();
	//	strResult.Empty();
	//	// 获得品牌
	//	strResult = ConsoleUtil::Execute(strCmd1).c_str();
	//	if (strResult.Find(_T("error: device not found")) >= 0)	// 设备已经没有了
	//		continue;
	//	if (strResult.Find(_T("error:")) >= 0)	// adb 出错
	//	{
	//		RestartServer();
	//		strResult = ConsoleUtil::Execute(strCmd1).c_str();
	//		if(strResult.Find(_T("error:"))  >= 0)
	//		{
	//			return FALSE;
	//		}
	//	}

	//	if ((iPos = strResult.Find(_T("\r\r\n"))) == -1)	// 结果中没找到
	//		continue;
	//	strResult = strResult.Left(iPos); 
	//	if ((iPos = strResult.ReverseFind(_T('\n'))) != -1)
	//		strResult = strResult.Mid(iPos + 1);
	//	strResult.TrimLeft(_T("\t\r "));
	//	strResult.TrimRight(_T("\t\r "));
	//	if (strResult.IsEmpty())
	//		continue;
	//	strTemp = strResult + _T("  ");
	//	
	//	// 获得型号
	//	strResult = ConsoleUtil::Execute(strCmd2).c_str();
	//	if (strResult.Find(_T("error: device not found")) != -1)	// 设备已经没有了
	//		continue;
	//	if (strResult.Find(_T("error:")) != -1)	// adb 出错
	//	{
	//		RestartServer();
	//		strResult = ConsoleUtil::Execute(strCmd2).c_str();
	//		if(strResult.Find(_T("error:")) != -1)
	//		{
	//			return FALSE;
	//		}
	//	}

	//	if ((iPos = strResult.Find(_T("\r\r\n"))) == -1)	// 结果中没找到
	//		continue;
	//	strResult = strResult.Left(iPos); 
	//	if ((iPos = strResult.ReverseFind(_T('\n'))) != -1)
	//		strResult = strResult.Mid(iPos + 1);
	//	strResult.TrimLeft(_T("\t\r "));
	//	strResult.TrimRight(_T("\t\r "));
	//	if (strResult.IsEmpty())
	//		continue;
	//	strTemp = strTemp + strResult;

	//	brands.push_back(strTemp.GetBuffer());
	//}
	//return TRUE;
	return false;
}

/***********************************************************
功能：测试端口是否可以listen
***********************************************************/

BOOL NetUtils::CheckPort(UINT uPort)
{
	bool bRes = false;
	auto sock = CTSocket::CreateServer( uPort );
	if(sock)//表示建立成功，说明该端口未被占用。
	{
		CTSocket::Close( sock );
		return TRUE;
	}
	else
		return FALSE;
}

std::wstring NetUtils::PushFile(LPCTSTR fileName, LPCTSTR destPath){
	
	//CString strCmd;
	//std::wstring strResult;
	//
	//CString cmd;

	//cmd = m_strAdbPathName + _T(" ") + _T("remount");
	//
	//strResult = ConsoleUtil::Execute(cmd, 1 * 60 * 1000);
	//
	//cmd = "";

	//cmd = m_strAdbPathName + _T(" push \"") + CString(fileName) + _T("\" ") + CString(destPath);	
	//Common::LogHelper::log(_T("adbutil"), L4CP_INFO_LOG_LEVEL, cmd);
	//strResult = ConsoleUtil::Execute(cmd, 30 * 60 * 1000).c_str();
	//DeleteFileA(fileName);
	//return strResult;
	return L" ";
}

BOOL NetUtils::RebootDevice(){
	//CString strCmd;
	//strCmd.Format(_T("%s reboot"), m_strAdbPathName);
	//BOOL bRet = ConsoleUtil::Execute(strCmd) != ConsoleUtil::ERROR_RETURN;
	//return (bRet);
	return FALSE;
}

BOOL NetUtils::GetIPStr(CString &ipStr)
{
	ipStr = m_ipStr;
	return m_ipStr.GetLength() > 0;
}

void NetUtils::SetCurrentSerialno(LPCTSTR szSerialno)
{
	m_strSerialno = szSerialno;
}

void NetUtils::SetCurrentIP(LPCTSTR szIP)
{
	m_ipStr = szIP;
}
