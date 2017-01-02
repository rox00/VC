// ChatMonitorThread.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadUDPMonitor.h"
#include "NetData.h"

CThreadUDPMonitorRecive::CThreadUDPMonitorRecive()
{
	m_pBuf = NULL;

	m_hChatMonitorSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_hChatMonitorSocket == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("CThreadUDPMonitorRecive �׽��Ӵ���ʧ��"), _T("RIM3.0 ����"), MB_OK);
		return;
	}

	ULONG reuse = true;
	if(setsockopt( m_hChatMonitorSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CThreadUDPMonitorRecive SO_REUSEADDR ���ô���\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(UDP_CHATSERVER_PORT);
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	int ret = bind(m_hChatMonitorSocket, (sockaddr*)&addr, sizeof(addr));
	if(ret)
	{
		MessageBox(NULL, _T("CThreadUDPMonitorRecive �׽��Ӱ�ʧ��\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	int nScktOpt = 5000;
	//���ý��ճ�ʱ
	if(setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nScktOpt, sizeof(nScktOpt)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CThreadUDPMonitorRecive ���ý��ճ�ʱ\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		WSACleanup();
		return;
	}
	//���÷��ͳ�ʱ
	nScktOpt = 5000;
	if(setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_SNDTIMEO,(char *)&nScktOpt, sizeof(nScktOpt)))
	{
		MessageBox(NULL, _T("CThreadUDPMonitorRecive ���÷��ͳ�ʱ\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	BOOL bBroadCast = TRUE;
	setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadCast, sizeof(bBroadCast));

	m_pBuf = new BYTE[RECIVEBUFLEN];
	ZeroMemory(m_pBuf, RECIVEBUFLEN);
}

CThreadUDPMonitorRecive::~CThreadUDPMonitorRecive()
{
	if (m_pBuf)
		delete[] m_pBuf;
}

UINT CThreadUDPMonitorRecive::ThreadProc()
{
	TRACE(_T("%s %d \n"), _T("CThreadUDPMonitorRecive"), GetCurrentThreadId());

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("CThreadUDPMonitorRecive �׽��ֳ�ʼ��ʧ�ܣ�"));
		return FALSE;
	}

	if(m_hChatMonitorSocket)
	{
		SOCKADDR_IN fromAddr;
		int addrLen = sizeof(SOCKADDR_IN);

		DWORD dwRet = 0;
		while(TRUE)
		{
			///��һ����������������������
			dwRet = WaitForSingleObject(m_hExit, 0);
			if(dwRet == WAIT_OBJECT_0)
				break;

			if(SelectSocketRead(m_hChatMonitorSocket, 100) > 0 && RIMConfig::GetInstance()->m_pLogonUser)
			{
				ZeroMemory(&fromAddr, addrLen);
				int rcvLen = recvfrom(m_hChatMonitorSocket, (char*)m_pBuf, RECIVEBUFLEN, 0, (LPSOCKADDR)&fromAddr, &addrLen);

				if(rcvLen <= 0)
				{
					DWORD error = WSAGetLastError();
					if(error == WSAEMSGSIZE)	//���������̫С
					{
						continue;
					}

					AfxMessageBox(_T("CThreadUDPMonitorRecive �׽��ֽ���ʧ�ܣ�"));
					break;
				}
				else
				{

					BOOL			bPush			= FALSE;	//�ж��Ƿ�ѹ����Ϣ����
					CNetData		*pChatSendData	= new CNetData();
					if(	pChatSendData->ParseNETBuffer(m_pBuf, rcvLen, (LPSOCKADDR)&fromAddr))
					{
						if(pChatSendData->GetDataType() == CNetData::UDP_ONLINE_NOTIFY || pChatSendData->GetDataType() == CNetData::UDP_OFFLINE)
						{	////ֻ����������Ϣ��  ���޶�Ŀ�ķ��ͷ��û���
							RIMConfig::GetInstance()->m_UDPReceiveQueue.Push(pChatSendData);
						}
						else if(pChatSendData->GetDesTypeID() == RIMConfig::GetInstance()->m_pLogonUser->userTypeID)
							RIMConfig::GetInstance()->m_UDPReceiveQueue.Push(pChatSendData);

					}
					else
						delete pChatSendData;
				}

				ZeroMemory(m_pBuf, RECIVEBUFLEN);
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return 0;
}

CThreadUDPMonitorRecive* CThreadUDPMonitorRecive::m_pChatMonitorReciveThread = NULL;
CThreadUDPMonitorRecive* CThreadUDPMonitorRecive::GetInstance()
{
	if(m_pChatMonitorReciveThread == NULL)
	{
		m_pChatMonitorReciveThread = new CThreadUDPMonitorRecive();
	}

	return m_pChatMonitorReciveThread;
}

void CThreadUDPMonitorRecive::ReleaseInstance()
{
	if(m_pChatMonitorReciveThread)
	{
		delete m_pChatMonitorReciveThread;
		m_pChatMonitorReciveThread = NULL;
	}
}



CThreadUDPMonitorSend::CThreadUDPMonitorSend()
{
	m_chatMonitorSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_chatMonitorSocket == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("CThreadUDPMonitorSend �׽��Ӵ���ʧ��"), _T("RIM3.0 ����"), MB_OK);
		return;
	}

	BOOL bBroadCast = TRUE;
	setsockopt(m_chatMonitorSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadCast, sizeof(bBroadCast));
}

CThreadUDPMonitorSend::~CThreadUDPMonitorSend()
{
}

UINT CThreadUDPMonitorSend::ThreadProc()
{
	TRACE(_T("%s %d \n"), _T("CThreadUDPMonitorSend"), GetCurrentThreadId());

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("CThreadUDPMonitorSend �׽��ֳ�ʼ��ʧ�ܣ�"));
		return FALSE;
	}

	
	if(m_chatMonitorSocket)
	{
		DWORD dwRet = 0;
		HANDLE handle[2];
		handle[0] = RIMConfig::GetInstance()->m_UDPSendQueue.GetQueueEvent();
		handle[1] = m_hExit;
		while(TRUE)	 
		{
			///��һ����������������������
			dwRet = WaitForMultipleObjects(2, handle, FALSE, INFINITE);
			if(dwRet != WAIT_OBJECT_0)
				break;

			CQueueData *pNetData = NULL;
			RIMConfig::GetInstance()->m_UDPSendQueue.Pop(&pNetData);

			if(pNetData)
			{
				CNetData *pChatSendData = (CNetData *)pNetData;

				SOCKADDR_IN toAddr;
				toAddr.sin_family		= AF_INET;
				toAddr.sin_addr.s_addr	= inet_addr(CT2A(pChatSendData->GetFromOrToIP().c_str()));
				toAddr.sin_port			= htons(pChatSendData->GetFromOrToPort());
				int ret = sendto(m_chatMonitorSocket, (char*)pChatSendData->GetBuffer(), pChatSendData->GetBufferLen(), 0, (LPSOCKADDR)&toAddr, sizeof(SOCKADDR_IN));

				if (ret == SOCKET_ERROR)
				{
					DWORD error = WSAGetLastError();

					AfxMessageBox(_T("CThreadUDPMonitorSend �׽��ַ���ʧ�ܣ�"));
					break;
				}
				else
				{

				}
			}

			delete pNetData;
		}
	}
	

	return 0;
}


CThreadUDPMonitorSend* CThreadUDPMonitorSend::m_pChatMonitorSendThread = NULL;
CThreadUDPMonitorSend* CThreadUDPMonitorSend::GetInstance()
{
	if(m_pChatMonitorSendThread == NULL)
	{
		m_pChatMonitorSendThread = new CThreadUDPMonitorSend();
	}

	return m_pChatMonitorSendThread;
}

void CThreadUDPMonitorSend::ReleaseInstance()
{
	if(m_pChatMonitorSendThread)
	{
		delete m_pChatMonitorSendThread;
		m_pChatMonitorSendThread = NULL;
	}
}

