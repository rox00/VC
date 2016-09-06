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
		AfxMessageBox(_T("CThreadUDPDataProcess �׽��ֳ�ʼ��ʧ�ܣ�"));
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
		///��һ����������������������
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
			{	//���߻�������Ϣ ���� 
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
				{//�Ǳ����Ĺ㲥��
					tstring sourID = CA2T(string((char*)pChatNetData->GetBuffer(), pChatNetData->GetBufferLen()).c_str());

					LPUSERINFO lpFindUserInfo = NULL;
					for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
					{
						LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];

						if (pChatNetData->GetSourceTypeID() == lpUserInfo->userTypeID && _tcscmp(lpUserInfo->UserID.c_str(), sourID.c_str()) == 0)	//�㲥��Ϣֻ�ж���Դ
						{	//���ж��ĸ��û���������

							lpFindUserInfo = lpUserInfo;
							break;
						}
					}

					if (lpFindUserInfo)	//�ҵ���
					{
						if (pChatNetData->GetDataType() == CNetData::UDP_ONLINE_NOTIFY)
						{
							bUpdateUser = (USERINFO::USER_STATE_OFFLINE == lpFindUserInfo->state);	//����״̬�෴�͸���

							USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

							lpFindUserInfo->ipList.insert(pChatNetData->GetFromOrToIP());
							lpFindUserInfo->userUdpPortTemp	= UDP_CHATSERVER_PORT;
							lpFindUserInfo->state			= USERINFO::USER_STATE_ONLINE;		//����
							lpFindUserInfo->stateUpdateClk	= clock();	//����״̬ʱ��


							CNetData *pUDPData = new CNetData();

							/////////�յ��������û� ��֪ͨ��Ҫ������ϢӦ��
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
							bUpdateUser = (USERINFO::USER_STATE_ONLINE == lpFindUserInfo->state);	//����״̬�෴�͸���
							
							USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

							for (set<tstring>::iterator it = lpFindUserInfo->ipList.begin(); it != lpFindUserInfo->ipList.end(); ++it)
							{
								if(_tcscmp(pChatNetData->GetFromOrToIP().c_str(), (*it).c_str())	== 0)
								{	////ɾ���û�IP�б��е� ����IP
									lpFindUserInfo->ipList.erase(it);
									break;
								}
							}

							if(lpFindUserInfo->ipList.size() == 0)	////���û����µ�IP�б�Ϊ��˵�����û�δ��һ̨�����ϵ�¼
								lpFindUserInfo->state			= USERINFO::USER_STATE_OFFLINE;		//����
							lpFindUserInfo->stateUpdateClk	= clock();	//����

							USERINFO::IPListLock.unlock();
						}
					}
					else if (pChatNetData->GetSourceTypeID() == USERINFO::USER_ID_UNKNOWN && pChatNetData->GetDataType() == CNetData::UDP_ONLINE_NOTIFY)	//û�ҵ�����������������Ϣ
					{	//������Ϣδ�ҵ����½��û�  �����������Ϣ ��ʹ�����û�ҲӦ���Ѿ��������û��б���

						LPUSERINFO lpuserinfo				= new USERINFO();
						{	//�½��û�

							lpuserinfo->userTypeID			= USERINFO::USER_ID_UNKNOWN;
							lpuserinfo->UserID				= sourID;
							lpuserinfo->UserName			= sourID;		////�����û�ID��������ͬ
							lpuserinfo->branch				= USERINFO::UNKNOWNBRANCHSTR;
							lpuserinfo->state				= USERINFO::USER_STATE_ONLINE;
							lpuserinfo->stateUpdateClk		= clock();		////����״̬ʱ��

							lpuserinfo->ipList.insert(pChatNetData->GetFromOrToIP());
							lpuserinfo->userUdpPortTemp		= UDP_CHATSERVER_PORT;
							RIMConfig::GetInstance()->m_userArray.push_back(lpuserinfo);
						}
						CNetData *pUDPData = new CNetData();

						/////////�յ��������û� ��֪ͨ��Ҫ������ϢӦ��
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

					//���������û��б�״̬
					if(bUpdateUser)
						AfxGetApp()->m_pMainWnd->PostMessage(WM_UPDATEUSERLIST);
				}
			}
				break;
			case CNetData::UDP_ONLINE_ANSWER: //�յ������ߵ���Ϣ֮��Ӧ����ҵĵ�ǰ�����û�
			{	//�ǹ㲥��Ϣ

				tstring sourID = CA2T(string((char*)pChatNetData->GetBuffer(), pChatNetData->GetBufferLen()).c_str());

				LPUSERINFO lpFindUserInfo = NULL;
				for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
				{
					LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];

					if (pChatNetData->GetSourceTypeID() == lpUserInfo->userTypeID && _tcscmp(lpUserInfo->UserID.c_str(), sourID.c_str()) == 0)	//�㲥��Ϣֻ�ж���Դ
					{	//���ж��ĸ��û���������

						lpFindUserInfo = lpUserInfo;
						break;
					}
				}

				if (lpFindUserInfo)	//�ҵ���
				{
					bUpdateUser = (USERINFO::USER_STATE_OFFLINE == lpFindUserInfo->state);	//����״̬�෴�͸���

					USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

					lpFindUserInfo->ipList.insert(pChatNetData->GetFromOrToIP());
					lpFindUserInfo->userUdpPortTemp		= UDP_CHATSERVER_PORT;
					lpFindUserInfo->state				= USERINFO::USER_STATE_ONLINE;		//����
					lpFindUserInfo->stateUpdateClk		= clock();	//����״̬ʱ��

					USERINFO::IPListLock.unlock();
				}
				else if (pChatNetData->GetSourceTypeID() == USERINFO::USER_ID_UNKNOWN)	//û�ҵ�����������Ӧ����Ϣ
				{	//�����Ӧ����Ϣ Ӧ���½��������û�
					LPUSERINFO lpuserinfo		= new USERINFO();
					{	//�½��û�

						lpuserinfo->userTypeID			= USERINFO::USER_ID_UNKNOWN;
						lpuserinfo->UserID				= sourID;
						lpuserinfo->UserName			= sourID;		////�����û�ID��������ͬ
						lpuserinfo->branch				= USERINFO::UNKNOWNBRANCHSTR;
						lpuserinfo->state				= USERINFO::USER_STATE_ONLINE;
						lpuserinfo->stateUpdateClk		= clock();	//����״̬ʱ��

						lpuserinfo->ipList.insert(pChatNetData->GetFromOrToIP());
						lpuserinfo->userUdpPortTemp		= UDP_CHATSERVER_PORT;

						bUpdateUser = TRUE;	
						RIMConfig::GetInstance()->m_userArray.push_back(lpuserinfo);
					}
				}

				//���������û��б�״̬
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
		AfxMessageBox(_T("CThreadTCPDataProcess �׽��ֳ�ʼ��ʧ�ܣ�"));
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
		///��һ����������������������
		dwRet = WaitForMultipleObjects(2, handle, FALSE, INFINITE);
		if(dwRet != WAIT_OBJECT_0)
			break;


		CNetData		*pChatNetData = NULL;
		RIMConfig::GetInstance()->m_TCPReceiveQueue.Pop((CQueueData**)&pChatNetData);
		bool bNeedDelete = TRUE;

		switch(pChatNetData->GetDataType())
		{
		case CNetData::TCP_CHATMSG: //������ѷ��͹�������Ϣ
			{	
				for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
				{
					LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];
					if(	pChatNetData->GetSourceTypeID() == lpUserInfo->userTypeID									&&	////�����û���ID��0XFFFFFFFF
						pChatNetData->GetDesTypeID() == RIMConfig::GetInstance()->m_pLogonUser->userTypeID			&&
						_tcscmp(pChatNetData->GetSourceID().c_str(), lpUserInfo->UserID.c_str()) == 0
						)
					{	//���ж��ĸ��û��������� ���ҽ��Ƿ����ҵ�
						;
						PlaySound(RelativeToFullPath(VEDIO_RELATIVEPATH_MSG).c_str(), NULL, SND_FILENAME | SND_ASYNC);
						lpUserInfo->receiveMsgQueue.Push(pChatNetData);
						bNeedDelete = false;

						//////����ͼ��֪ͨ��Ϣ  2����������Ϣ  3�����ļ���Ϣ
						AfxGetApp()->m_pMainWnd->PostMessage(WM_FLICKERNOTIFYICON, 2, (LPARAM)lpUserInfo);
						break;
					}
				}

				//���������û��б�״̬
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