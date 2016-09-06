// ChatMonitorThread.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadDataProcess.h "
#include "NetData.h"


CThreadUDPDataProcess::CThreadUDPDataProcess()
{

}

CThreadUDPDataProcess::~CThreadUDPDataProcess()
{

}

UINT CThreadUDPDataProcess::ThreadProc()
{
	TRACE(_T("%s %d \n"), _T("CThreadUDPDataProcess"), GetCurrentThreadId());
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("CThreadUDPDataProcess 套接字初始化失败！"));
		return FALSE;
	}

	vector<tstring> ipArray;
	GetAvalibleIpAddress(ipArray);

	DWORD dwRet = 0;
	HANDLE handle[2];
	handle[0] = RIMConfig::GetInstance()->m_UDPReceiveQueue.GetQueueEvent();
	handle[1] = m_hExit;
	while(TRUE)	 
	{
		///第一个句柄代表队列里面有数据
		dwRet = WaitForMultipleObjects(2, handle, FALSE, INFINITE);
		if(dwRet != WAIT_OBJECT_0)
			break;


		CNetData		*pChatNetData	= NULL;
		RIMConfig::GetInstance()->m_UDPReceiveQueue.Pop((CQueueData**)&pChatNetData);
		BOOL			bUpdateUser		= FALSE;
		switch(pChatNetData->GetDataType())
		{
			case CNetData::UDP_ONLINE_NOTIFY:
			case CNetData::UDP_OFFLINE:
			{	//上线或下线消息 处理 
				BOOL bLocalCmputer = FALSE;
				for (UINT i = 0; i < ipArray.size(); ++i)
				{
					if (_tcscmp(ipArray[i].c_str(), pChatNetData->GetFromOrToIP().c_str()) == 0)
					{
						bLocalCmputer = TRUE;
						break;
					}
				}

				if (bLocalCmputer == FALSE)
				{//非本机的广播包
					tstring sourID = CA2T(string((char*)pChatNetData->GetBuffer(), pChatNetData->GetBufferLen()).c_str());

					LPUSERINFO lpFindUserInfo = NULL;
					for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
					{
						LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];

						if (pChatNetData->GetSourceTypeID() == lpUserInfo->userTypeID && _tcscmp(lpUserInfo->UserID.c_str(), sourID.c_str()) == 0)	//广播消息只判断来源
						{	//先判定哪个用户发过来的

							lpFindUserInfo = lpUserInfo;
							break;
						}
					}

					if (lpFindUserInfo)	//找到了
					{
						if (pChatNetData->GetDataType() == CNetData::UDP_ONLINE_NOTIFY)
						{
							bUpdateUser = (USERINFO::USER_STATE_OFFLINE == lpFindUserInfo->state);	//与变更状态相反就更新

							USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

							lpFindUserInfo->ipList.insert(pChatNetData->GetFromOrToIP());
							lpFindUserInfo->userUdpPortTemp	= UDP_CHATSERVER_PORT;
							lpFindUserInfo->state			= USERINFO::USER_STATE_ONLINE;		//上线
							lpFindUserInfo->stateUpdateClk	= clock();	//更新状态时间


							CNetData *pUDPData = new CNetData();

							/////////收到新上线用户 的通知后要进行消息应答
							string transBuf = CT2A(RIMConfig::GetInstance()->m_pLogonUser->UserID.c_str());
							pUDPData->MakeNETBuffer(
								CNetData::UDP_ONLINE_ANSWER,
								RIMConfig::GetInstance()->m_pLogonUser,
								lpFindUserInfo, pChatNetData->GetFromOrToIP(),
								(LPBYTE)transBuf.c_str()
								,
								transBuf.size()
								);

							RIMConfig::GetInstance()->m_UDPSendQueue.Push(pUDPData);

							USERINFO::IPListLock.unlock();
						}
						else
						{
							bUpdateUser = (USERINFO::USER_STATE_ONLINE == lpFindUserInfo->state);	//与变更状态相反就更新
							
							USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

							for (set<tstring>::iterator it = lpFindUserInfo->ipList.begin(); it != lpFindUserInfo->ipList.end(); ++it)
							{
								if(_tcscmp(pChatNetData->GetFromOrToIP().c_str(), (*it).c_str())	== 0)
								{	////删除用户IP列表中的 下线IP
									lpFindUserInfo->ipList.erase(it);
									break;
								}
							}

							if(lpFindUserInfo->ipList.size() == 0)	////该用户名下的IP列表为空说明该用户未在一台电脑上登录
								lpFindUserInfo->state			= USERINFO::USER_STATE_OFFLINE;		//下线
							lpFindUserInfo->stateUpdateClk	= clock();	//上线

							USERINFO::IPListLock.unlock();
						}
					}
					else if (pChatNetData->GetSourceTypeID() == USERINFO::USER_ID_UNKNOWN && pChatNetData->GetDataType() == CNetData::UDP_ONLINE_NOTIFY)	//没找到是匿名且是上线消息
					{	//上线消息未找到则新建用户  如果是下线消息 即使匿名用户也应当已经存在于用户列表中

						LPUSERINFO lpuserinfo				= new USERINFO();
						{	//新建用户

							lpuserinfo->userTypeID			= USERINFO::USER_ID_UNKNOWN;
							lpuserinfo->UserID				= sourID;
							lpuserinfo->UserName			= sourID;		////匿名用户ID与名称相同
							lpuserinfo->branch				= USERINFO::UNKNOWNBRANCHSTR;
							lpuserinfo->state				= USERINFO::USER_STATE_ONLINE;
							lpuserinfo->stateUpdateClk		= clock();		////更新状态时间

							lpuserinfo->ipList.insert(pChatNetData->GetFromOrToIP());
							lpuserinfo->userUdpPortTemp		= UDP_CHATSERVER_PORT;
							RIMConfig::GetInstance()->m_userArray.push_back(lpuserinfo);
						}
						CNetData *pUDPData = new CNetData();

						/////////收到新上线用户 的通知后要进行消息应答
						string transBuf = CT2A(RIMConfig::GetInstance()->m_pLogonUser->UserID.c_str());
						pUDPData->MakeNETBuffer(
							CNetData::UDP_ONLINE_ANSWER,
							RIMConfig::GetInstance()->m_pLogonUser,
							lpuserinfo, pChatNetData->GetFromOrToIP(),
							(LPBYTE)transBuf.c_str()
							,
							transBuf.size()
							);

						bUpdateUser = TRUE;	
						RIMConfig::GetInstance()->m_UDPSendQueue.Push(pUDPData);
					}

					//更新在线用户列表状态
					if(bUpdateUser)
						AfxGetApp()->m_pMainWnd->PostMessage(WM_UPDATEUSERLIST);
				}
			}
				break;
			case CNetData::UDP_ONLINE_ANSWER: //收到我上线的消息之后，应答给我的当前在线用户
			{	//非广播消息

				tstring sourID = CA2T(string((char*)pChatNetData->GetBuffer(), pChatNetData->GetBufferLen()).c_str());

				LPUSERINFO lpFindUserInfo = NULL;
				for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
				{
					LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];

					if (pChatNetData->GetSourceTypeID() == lpUserInfo->userTypeID && _tcscmp(lpUserInfo->UserID.c_str(), sourID.c_str()) == 0)	//广播消息只判断来源
					{	//先判定哪个用户发过来的

						lpFindUserInfo = lpUserInfo;
						break;
					}
				}

				if (lpFindUserInfo)	//找到了
				{
					bUpdateUser = (USERINFO::USER_STATE_OFFLINE == lpFindUserInfo->state);	//与变更状态相反就更新

					USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

					lpFindUserInfo->ipList.insert(pChatNetData->GetFromOrToIP());
					lpFindUserInfo->userUdpPortTemp		= UDP_CHATSERVER_PORT;
					lpFindUserInfo->state				= USERINFO::USER_STATE_ONLINE;		//上线
					lpFindUserInfo->stateUpdateClk		= clock();	//更新状态时间

					USERINFO::IPListLock.unlock();
				}
				else if (pChatNetData->GetSourceTypeID() == USERINFO::USER_ID_UNKNOWN)	//没找到是匿名且是应答消息
				{	//如果是应答消息 应当新建该匿名用户
					LPUSERINFO lpuserinfo		= new USERINFO();
					{	//新建用户

						lpuserinfo->userTypeID			= USERINFO::USER_ID_UNKNOWN;
						lpuserinfo->UserID				= sourID;
						lpuserinfo->UserName			= sourID;		////匿名用户ID与名称相同
						lpuserinfo->branch				= USERINFO::UNKNOWNBRANCHSTR;
						lpuserinfo->state				= USERINFO::USER_STATE_ONLINE;
						lpuserinfo->stateUpdateClk		= clock();	//更新状态时间

						lpuserinfo->ipList.insert(pChatNetData->GetFromOrToIP());
						lpuserinfo->userUdpPortTemp		= UDP_CHATSERVER_PORT;

						bUpdateUser = TRUE;	
						RIMConfig::GetInstance()->m_userArray.push_back(lpuserinfo);
					}
				}

				//更新在线用户列表状态
				if(bUpdateUser)
					AfxGetApp()->m_pMainWnd->PostMessage(WM_UPDATEUSERLIST);
			}

				break;
		}

		if (pChatNetData)
			delete pChatNetData;
	}

	return 0;
}

CThreadUDPDataProcess* CThreadUDPDataProcess::m_pDataProcessReciveThread = NULL;
CThreadUDPDataProcess* CThreadUDPDataProcess::GetInstance()
{
	if(m_pDataProcessReciveThread == NULL)
	{
		m_pDataProcessReciveThread = new CThreadUDPDataProcess();
	}

	return m_pDataProcessReciveThread;
}

void CThreadUDPDataProcess::ReleaseInstance()
{
	if(m_pDataProcessReciveThread)
	{
		delete m_pDataProcessReciveThread;
		m_pDataProcessReciveThread = NULL;
	}
}


CThreadTCPDataProcess::CThreadTCPDataProcess()
{

}

CThreadTCPDataProcess::~CThreadTCPDataProcess()
{

}

UINT CThreadTCPDataProcess::ThreadProc()
{
	TRACE(_T("%s %d \n"), _T("CThreadTCPDataProcess"), GetCurrentThreadId());

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("CThreadTCPDataProcess 套接字初始化失败！"));
		return FALSE;
	}

	vector<tstring> ipArray;
	GetAvalibleIpAddress(ipArray);

	DWORD dwRet = 0;
	HANDLE handle[2];
	handle[0] = RIMConfig::GetInstance()->m_TCPReceiveQueue.GetQueueEvent();
	handle[1] = m_hExit;
	while(TRUE)	 
	{
		///第一个句柄代表队列里面有数据
		dwRet = WaitForMultipleObjects(2, handle, FALSE, INFINITE);
		if(dwRet != WAIT_OBJECT_0)
			break;


		CNetData		*pChatNetData = NULL;
		RIMConfig::GetInstance()->m_TCPReceiveQueue.Pop((CQueueData**)&pChatNetData);
		bool bNeedDelete = TRUE;

		switch(pChatNetData->GetDataType())
		{
		case CNetData::TCP_CHATMSG: //处理好友发送过来的消息
			{	
				for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
				{
					LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];
					if(	pChatNetData->GetSourceTypeID() == lpUserInfo->userTypeID									&&	////匿名用户的ID是0XFFFFFFFF
						pChatNetData->GetDesTypeID() == RIMConfig::GetInstance()->m_pLogonUser->userTypeID			&&
						_tcscmp(pChatNetData->GetSourceID().c_str(), lpUserInfo->UserID.c_str()) == 0
						)
					{	//先判定哪个用户发过来的 并且仅是发给我的
						;
						PlaySound(RelativeToFullPath(VEDIO_RELATIVEPATH_MSG).c_str(), NULL, SND_FILENAME | SND_ASYNC);
						lpUserInfo->receiveMsgQueue.Push(pChatNetData);
						bNeedDelete = false;

						//////托盘图标通知消息  2代表聊天消息  3代表文件消息
						AfxGetApp()->m_pMainWnd->PostMessage(WM_FLICKERNOTIFYICON, 2, (LPARAM)lpUserInfo);
						break;
					}
				}

				//更新在线用户列表状态
			}
			break;
		}	

		if(bNeedDelete)
			delete pChatNetData;
	}

	return 0;
}

CThreadTCPDataProcess* CThreadTCPDataProcess::m_pDataProcessReciveThread = NULL;
CThreadTCPDataProcess* CThreadTCPDataProcess::GetInstance()
{
	if(m_pDataProcessReciveThread == NULL)
	{
		m_pDataProcessReciveThread = new CThreadTCPDataProcess();
	}

	return m_pDataProcessReciveThread;
}

void CThreadTCPDataProcess::ReleaseInstance()
{
	if(m_pDataProcessReciveThread)
	{
		delete m_pDataProcessReciveThread;
		m_pDataProcessReciveThread = NULL;
	}
}