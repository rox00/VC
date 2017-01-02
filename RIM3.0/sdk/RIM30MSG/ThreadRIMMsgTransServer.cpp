//********************************************************************
// THREADRIMMSGTRANSSERVER.CPP �ļ�ע��
// �ļ��� ��: THREADRIMMSGTRANSSERVER.CPP
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// ���� ����: ����
// ����ʱ�� : 2015/4/7 14:05
// �ļ����� : 
//*********************************************************************

#include "stdafx.h"
#include "ThreadRIMMsgTransServer.h"
#include <winnt.h>

// CThreadRIMMsgTransClient message handlers


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRIM30MsgTransServer g_RIM30MsgTransServer;
IRIM30MsgServer* GetRIM30Server()
{
	return &g_RIM30MsgTransServer;
}

///////////////////////////////////////////////////////////////////////////CRIM30MsgTransServer//////////////////////////////////////////////
UINT CRIM30MsgTransServer::__ThreadProc(LPVOID lpParam)
{
	ASSERT(lpParam);
	CRIM30MsgTransServer *pThread = (CRIM30MsgTransServer*)lpParam;

	if(pThread->m_bInitCom) 
		CoInitialize(NULL);

	UINT uiRet = pThread->ThreadProc();

	if(pThread->m_bInitCom)
		CoUninitialize();

#ifdef DEBUG
	if(!pThread->m_strName.IsEmpty())
	{
		CString strMsg = _T("Thread \"");
		strMsg +=	pThread->m_strName;
		strMsg +=	_T("\" Is Exited");

		TRACE(strMsg);
	}
#endif

	return uiRet;
}

CRIM30MsgTransServer::CRIM30MsgTransServer(void)
{
	m_pThread			= NULL;
	m_hSendDataThread	= INVALID_HANDLE_VALUE;
	m_hExit				= NULL;
	m_bInitCom			= FALSE;

	for(int i = 0; i < 64; i++)
		m_threadHandles[i] = INVALID_HANDLE_VALUE;
	m_completionPort	= INVALID_HANDLE_VALUE;

	m_serverGuid		= CreateNewGuid();
	//m_serverGuid		= RIM30GUID_INITPROCESS;
	//m_serverGuid		= RIM30GUID_NAVIGATOR_STR;
}

CRIM30MsgTransServer::~CRIM30MsgTransServer(void)
{
	if (m_subSockInfoVec.size())
	{
		m_subSockInfoVec.clear();
	}

	
	{////��zy 2015-06-08 ����Ϊ�˽����ϵͳ ����BUGδ�رյ�����
		LPDWORD pProcessIDs = new DWORD[4096];
		DWORD   IDsCount	= 0;
		clock_t clkBegin	= clock();

		while (m_subSockInfoVec.size() || (m_job.QueryBasicProcessIdList(4096, pProcessIDs, &IDsCount), IDsCount))
		{
			Sleep(1);

			if(clock() - clkBegin > 2000)
			{
				m_job.Terminate();
				TCHAR ch[MAX_PATH] = {0};
				_stprintf_s(ch, MAX_PATH, _T("RIM3.0 CRIM30MsgTransServer::ǿ�ƹر���ҵ��"));
				OutputDebugString(ch);
				break;
			}
		}

		if(pProcessIDs) {	delete [] pProcessIDs; pProcessIDs = NULL;	}
	}
}

string CRIM30MsgTransServer::CreateNewGuid()
{
	char ch[MAX_PATH] = { 0 };
	GUID guid;

	if (S_OK == CoCreateGuid(&guid))
	{
		sprintf_s(ch, MAX_PATH,
			("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
	}

	return ch;
}

int CRIM30MsgTransServer::SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds)
{
	assert(sock != INVALID_SOCKET);
	TIMEVAL timeOut;
	timeOut.tv_sec	= 0;
	timeOut.tv_usec = dwMilliseconds * 1000;
	fd_set readFD;
	FD_ZERO(&readFD);
	FD_SET(sock, &readFD);
	int iRetVal		= select(int(sock), &readFD, NULL, NULL, &timeOut);

	return iRetVal;
}

int CRIM30MsgTransServer::SelectSocketWrite(SOCKET sock, __in DWORD dwMilliseconds)
{
	assert(sock != INVALID_SOCKET);
	TIMEVAL timeOut;
	timeOut.tv_sec	= 0;
	timeOut.tv_usec = dwMilliseconds * 1000;
	fd_set writeFD;
	FD_ZERO(&writeFD);
	FD_SET(sock, &writeFD);
	int iRetVal		= select(int(sock), NULL, &writeFD, NULL, &timeOut);

	return iRetVal;
}

bool CRIM30MsgTransServer::SendPackByRemainLen(LPSUBSOCKINFO pSockInfo)
{	/////����pSockInfo->pSendOverlapped->bytesSENDƫ�ƺ���һ�����ݣ������͸ð�����remainLen��������
	if(pSockInfo == NULL)
		return false;

	RIM30TCPBLOCK		blockTemp;
	DWORD				unitPackLen			= sizeof(RIM30TCPBLOCK);
	const int			unitDatalen			= RIM30TCPBLOCKDATALEN;		
	DWORD				SendBytes			= 0;

	{	////�����ѷ���������λ�ֽ��� �Լ� ������λʣ��δ���ֽ��� �Զ�������岢����
		ZeroMemory(&(pSockInfo->pSendOverlapped->overlapped), sizeof(OVERLAPPED));
		int dataReserveSend = pSockInfo->pSendOverlapped->sendTolLength - pSockInfo->pSendOverlapped->bytesSEND;
		if (dataReserveSend > 0)
		{////����blockTemp
			blockTemp._fileSize	= pSockInfo->pSendOverlapped->sendTolLength;
			blockTemp._offset	= pSockInfo->pSendOverlapped->bytesSEND;
			if (dataReserveSend < unitDatalen)
			{
				blockTemp._len	= dataReserveSend;
				memcpy(blockTemp._buf, pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND, dataReserveSend);
			}
			else
			{
				blockTemp._len	= unitDatalen;
				memcpy(blockTemp._buf, pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND, unitDatalen);
			}

			pSockInfo->pSendOverlapped->cacheBuf.buf = (char*)&blockTemp + pSockInfo->pSendOverlapped->packSEND;	/////����һ������
			pSockInfo->pSendOverlapped->cacheBuf.len = unitPackLen - pSockInfo->pSendOverlapped->packSEND;			/////һ�����ݵĳ���

			if (pSockInfo->socket != INVALID_SOCKET)
			{
				if (WSASend(pSockInfo->socket, &(pSockInfo->pSendOverlapped->cacheBuf), 1, &SendBytes, 0,
					&(pSockInfo->pSendOverlapped->overlapped), NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != ERROR_IO_PENDING)
					{
						pSockInfo->bDelete = true;
						pSockInfo->deleteInfo = "WSASend";
						//MessageBox(NULL, _T("RIM3.0 WSASend() failed with error\n"), _T("����"), MB_OK);
						OutputDebugString(_T("RIM3.0 CRIM30MsgTransServer::WSASend() failed with error\n"));
						return false;
					}
					else
						InterlockedIncrement(&pSockInfo->IOTotalCnt);
				}
				else
					InterlockedIncrement(&pSockInfo->IOTotalCnt);
			}
			
		}
	}

	return true;
}

bool CRIM30MsgTransServer::SendPack(LPRIM30MSG pMSG, LPSUBSOCKINFO pSockInfo)
{	/////����pSockInfo->pSendOverlapped->bytesSENDƫ�ƺ���һ�����ݣ������͸ð�����remainLen��������

	if(pMSG == NULL || pSockInfo == NULL)
		return false;

	memcpy(pSockInfo->pSendOverlapped->buffer, pMSG, pMSG->GetTotalLength());	////������Ϣ�����ͻ�����
	pSockInfo->pSendOverlapped->sendTolLength	= pMSG->GetTotalLength();			////�������ݵ��ܳ���
	pSockInfo->pSendOverlapped->packSEND	= 0;	
	pSockInfo->pSendOverlapped->bytesSEND	= 0;								////��ǰ���͵��ֽ���

	if (pSockInfo->socket != INVALID_SOCKET && false == CRIM30MsgTransServer::SendPackByRemainLen(pSockInfo))
	{
		//MessageBox(NULL, _T("SendPack ��ʼ�����������ݰ�ʱ�쳣\n"), _T("RIM3.0 ����"), MB_OK);
		{	////�����������Ѿ����������
			pSockInfo->pSendOverlapped->packSEND		= 0;
			pSockInfo->pSendOverlapped->bytesSEND		= 0;
			pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////��ǰ��ʼ�����ֽڵ�BUFλ��
			pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
			pSockInfo->pSendOverlapped->sendTolLength	= 0;
		}
		return false;
	}
	return true;
}

bool CRIM30MsgTransServer::ProcessMsg(LPRIM30MSG pMsg, LPSUBSOCKINFO pSockInfo, CRIM30MsgTransServer *pServer)
{
	ASSERT(pMsg && pSockInfo && pServer);
	if(RIMConfig::GetInstance()->m_pLogonUser == NULL)
		return false;

	LPRIM30MSG			pSendMsgTemp		= NULL;			////��Ҫѹ�뷢����Ϣ�Ķ���
	LPSUBSOCKINFO		pSendSockInfo		= pSockInfo;	////ָ��Ӧ��������Ϣ���׽���Ϣ


	if(pMsg->GetMsgID() == RIM30MSG_ONLINE_ASK)				////��ϵͳ��������
	{	////������Ϣ  ����һ��ϵͳӦ�������ߣ�
		if(memcmp(pMsg->GetSendGuid().c_str(), pMsg->GetDataBuf(), pMsg->GetSendGuid().size()) == 0 && pMsg->GetDataLength() == 68)
		{
			string		userID				= CT2A(RIMConfig::GetInstance()->m_pLogonUser->UserID.c_str());
			DWORD		userTypeID			= RIMConfig::GetInstance()->m_pLogonUser->userTypeID;

			{	////������Ϣ ������
				const int	totalLen		= 64 + 256 + 1;
				BYTE		ch[totalLen]	= { 0 };
				LPBYTE		pChar			= ch;

				memcpy_s(pChar, 64,  pServer->m_serverGuid.c_str(),  pServer->m_serverGuid.size());	
				pChar += 64;
				memcpy_s(pChar, 256, userID.c_str(), userID.size());	//�û�ID
				pChar += 256;
				*pChar = (userTypeID == USERINFO::USER_ID_UNKNOWN);		//�Ƿ��Ǳ�Я��

				pSendMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pMsg->GetSendGuid().c_str(), RIM30MSG_ONLINE_ANSWER, 
					RIM30MSG::DATATYPE_BINARY, ch, totalLen);

				pSendSockInfo->tcpSendQueue.Push(pSendMsgTemp, true);
				pSendMsgTemp = NULL;
			}

			memset(pSockInfo->guid, 0, sizeof(pSockInfo->guid));
			memcpy(pSockInfo->guid, pMsg->GetSendGuid().c_str(), pMsg->GetSendGuid().size());
			pSockInfo->processID = *((DWORD*)(pMsg->GetDataBuf() + 64));
		}
	}
	else if (strlen(pSockInfo->guid))
	{	////�������ߵ�ϵͳ��������Ϣ
		if (pMsg->GetMsgID() == RIM30MSG_HEART_PACK)	////��������Ϣ
		{	//��������Ϣ 
			pSendMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pSockInfo->guid, RIM30MSG_HEART_PACK, 0, NULL, 0);
		}
		else if (strcmp(pMsg->GetRecvGuid().c_str(), pServer->m_serverGuid.c_str()) == 0)
		{	////��������������Ϣ 
			switch (pMsg->GetMsgID())
			{
			case RIM30MSG_OFFLINE_ASK:					////��ϵͳ��������
				{
					char ch[MAX_PATH] ={ 0 };
					sprintf_s(ch, MAX_PATH, "OK");
					pSendMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pMsg->GetSendGuid().c_str(), RIM30MSG_OFFLINE_ANSWER,
						RIM30MSG::DATATYPE_BINARY, (LPBYTE)ch, strlen(ch));
				}
				break;
			case RIM30MSG_SHUTDOWN_ASK:					////�ر�����
				break;
			case RIM30MSG_SHUTDOWN_ANSWER:				////�ر�Ӧ��
				{
				}
				break;
			case RIM30MSG_MEASUREDF_START_ANSWER:		////������������
				{
					int i = 0;
					i++;
				}
				break;
			case RIM30MSG_QUERYSERVERADDR_ASK:			////��������Ӧ��
				{
					if (pMsg->GetDataLength() == 8)
					{
						LPDWORD pBuffer		= (LPDWORD)pMsg->GetDataBuf();

						DWORD	subSysID	= *pBuffer;
						DWORD	issueTYPE	= *(pBuffer + 1);
						vector<LPWEBADDRINFO> infoArray;

						{	////���ҷ��������ķ����ַ
							bool bGoOn = true;
							for (map<DWORD, vector<LPWEBADDRINFO>>::iterator it = RIMConfig::GetInstance()->m_webAddrInfoMap.begin();
								bGoOn && it != RIMConfig::GetInstance()->m_webAddrInfoMap.end(); ++it)
							{
								if (it->first == subSysID)
								{
									if (issueTYPE != -1)
									{	////��-1���Ҷ�Ӧ����Ϣ
										for (UINT i = 0; bGoOn && i < it->second.size(); ++i)
										{
											if (it->second.at(i)->nSubModeID == issueTYPE)
											{
												infoArray.push_back(it->second.at(i));
												bGoOn = false;
											}
										}
									}
									else
									{	////-1����ȫ��
										for (UINT i = 0; bGoOn && i < it->second.size(); ++i)
										{
											infoArray.push_back(it->second.at(i));
										}
									}

									bGoOn = false;
								}
							}
						}

						{	////������Ϣ ������
							const int	totalLen		= 4 + 4 + (4 + 256 + 256) * infoArray.size();
							DWORD		totalCnt		= infoArray.size();
							LPBYTE		pBuffer			= new BYTE[totalLen];
							memset(pBuffer, 0, totalLen);
							LPBYTE		pChar			= pBuffer;

							memcpy_s(pChar, 4, &subSysID, sizeof(DWORD));		pChar += 4;		////SUBSYSTEMID		��ϵͳID
							memcpy_s(pChar, 4, &totalCnt, sizeof(DWORD));		pChar += 4;		////�����͵ĸ���

							for (UINT i = 0; i < infoArray.size(); ++i)
							{
								LPWEBADDRINFO lpwebaddrinfo = infoArray.at(i);
								memcpy_s(pChar, 4, &(lpwebaddrinfo->nSubModeID), sizeof(DWORD));
								pChar += 4;	////sName			��������
								memcpy_s(pChar, 256, lpwebaddrinfo->sName.c_str(), lpwebaddrinfo->sName.size());
								pChar += 256;	////sName			��������
								memcpy_s(pChar, 256, lpwebaddrinfo->sLocation.c_str(), lpwebaddrinfo->sLocation.size());
								pChar += 256;	////sLocation		�����ַ
							}

							pSendMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pMsg->GetSendGuid().c_str(), RIM30MSG_QUERYSERVERADDR_ANSWER,
								RIM30MSG::DATATYPE_BINARY, pBuffer, totalLen);

							if (pBuffer) { delete[] pBuffer;	pBuffer = NULL; };
						}
					}
				}
				break;
			}
		}
		else if (pMsg->GetRecvGuid().length())
		{	////��ϵͳ������ϵͳ����Ϣ 
			pSendMsgTemp = RIM30MSG::MakeMsg(pMsg->GetSendGuid().c_str(), pMsg->GetRecvGuid().c_str(), pMsg->GetMsgID(), pMsg->GetDataType(), pMsg->GetDataBuf(), pMsg->GetDataLength());
			for(UINT i = 0; i < pServer->m_subSockInfoVec.size(); ++i)
			{
				if(strcmp(pMsg->GetRecvGuid().c_str(), pServer->m_subSockInfoVec.at(i)->guid) == 0)
				{	////////����Ҫ�������ݵ���Ӧ��SOCKET  ת������
					pSendSockInfo = pServer->m_subSockInfoVec.at(i);
					break;
				}
			}
		}
	}
	
	if (pSendMsgTemp)
	{
		if(pSendSockInfo)
			pSendSockInfo->tcpSendQueue.Push(pSendMsgTemp);
		else
			MessageBox(NULL, _T("��Ϣ���������쳣��ָ�벻Ӧ��Ϊ��\n"), _T("RIM3.0 ����"), MB_OK);
	}

	if(pSendSockInfo)
	{	////���ݷ�����Ϣ���н�����Ϣ����
		pSendSockInfo->sendLock.Lock();
		if(pSendSockInfo->pSendOverlapped->sendTolLength == 0)
		{
			LPRIM30MSG	pMsgInfo	= NULL;
			pSendSockInfo->tcpSendQueue.Pop((LPRIM30MSG*)&pMsgInfo);
			if(pMsgInfo)
				SendPack(pMsgInfo, pSendSockInfo);
			RIM30MSG::FreeMsg(pMsgInfo);
		}
		pSendSockInfo->sendLock.Unlock();
	}

	return true;
}

HRESULT CRIM30MsgTransServer::Begin(short serverPort /*= 0*/)
{
	ASSERT(m_hExit == NULL);

	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(m_hExit == NULL)
		return E_POINTER;

	m_serverPort	= serverPort;
	m_bInitCom		= FALSE;
	m_pThread		= AfxBeginThread(CRIM30MsgTransServer::__ThreadProc, LPVOID(this), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

	if(m_pThread == NULL)
		return E_POINTER;

	return NOERROR;
}

void CRIM30MsgTransServer::End()
{
	if(m_pThread && m_hExit)
	{
		{	////�ȹر����ӽ����߳�
			SetEvent(m_hExit);
			// 		::PostThreadMessage(m_pThread->m_nThreadID, WM_QUIT, 0, 0);

			WaitForSingleObject(m_pThread->m_hThread, INFINITE);
			WaitForSingleObject(m_hSendDataThread, INFINITE);		CloseHandle(m_hSendDataThread);
			delete m_pThread;
			m_pThread = NULL;

			CloseHandle(m_hExit);
			m_hExit = NULL;
		}

		{	////�ȳֲ��ر�������ϵͳEXE
			CloseAllSubSystem();
		}

		{	////�ر�������ɶ˿��߳�
			int cnt = 0;
			while(m_threadHandles[cnt] != INVALID_HANDLE_VALUE) cnt++;

			while(WAIT_TIMEOUT == WaitForMultipleObjects(cnt, m_threadHandles, TRUE, 100))
			{
				PostQueuedCompletionStatus(m_completionPort, 0xFFFFFFFF, NULL, NULL);
			}
			cnt = 0;
			while(m_threadHandles[cnt] != INVALID_HANDLE_VALUE) {	CloseHandle(m_threadHandles[cnt]);	cnt++;	}
			CloseHandle(m_completionPort); m_completionPort = INVALID_HANDLE_VALUE;
		}

		{	////�����ϵͳSOCKET��Ϣ
			m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

			LPSUBSOCKINFO		pSockInfoTemp		= NULL;
			for(vector<LPSUBSOCKINFO>::iterator it = m_subSockInfoVec.begin(); it != m_subSockInfoVec.end(); ++it)
			{
				pSockInfoTemp = *it;

				closesocket(pSockInfoTemp->socket);	pSockInfoTemp->socket = INVALID_SOCKET;
				delete pSockInfoTemp;
			}
			m_subSockInfoVec.clear();

			m_socketInfolock.unlock();
		}

		{	////�����ϵͳ����������Ϣ
			LPUSERMSGINFO lpreservemsginfo = NULL;
			while (m_userSendQueue.GetQueueCount())	{ m_userSendQueue.Pop((LPRIM30MSG*)&lpreservemsginfo); delete lpreservemsginfo; }
		}
	}
}

BOOL CRIM30MsgTransServer::SetPriority(int nPriority)
{
	if(m_pThread)
		return m_pThread->SetThreadPriority(nPriority);

	return FALSE;
}

//	�����̣߳�ѭ�������ɶ˿�״̬����ȡPerIoData�е�����
UINT __stdcall CRIM30MsgTransServer::ServerWorkerThread(LPVOID pVoid)
{
	CRIM30MsgTransServer *pServer				= (CRIM30MsgTransServer*) pVoid;	 
	HANDLE				&CompletionPort			= pServer->m_completionPort;		// ��ɶ˿ھ��

	DWORD				BytesTransferred		= 0;								// ���ݴ�����ֽ���
	LPSUBSOCKINFO		pSockInfo				= NULL;								// �׽��־���ṹ��
	LPIOOVERLAPPED		pIOOverlapped			= NULL;								// I/O�����ṹ��
	DWORD				RecvBytes				= 0;								// ���ͺͽ��յ�����
	DWORD				Flags					= 0;								// WSARecv()�����еı�ʶλ

	const int			unitDatalen				= RIM30TCPBLOCKDATALEN;				//һ����ʵ���ݵĳ���
	const DWORD			unitPackLen				= sizeof(RIM30TCPBLOCK);
	LPSUBSOCKINFO		pSockInfoTemp			= NULL;
	TCHAR				szTemp[MAX_PATH]		= {0};
	bool				bDecrementIO			= false;							//�Ƿ���Ҫ���ٽ��ռ���

	while (TRUE)
	{
		// �����ɶ˿ڵ�״̬
		if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (LPDWORD)&pSockInfo, (LPOVERLAPPED *)&pIOOverlapped, INFINITE) == 0)
		{
			DWORD dwIOError = GetLastError();
			if (dwIOError != WAIT_TIMEOUT) // It was not an Time out event we wait for ever (INFINITE) 
			{
				if (pSockInfo != NULL)
				{
					pServer->m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_READ);
					if (dwIOError == ERROR_NETNAME_DELETED)
					{
						bDecrementIO = true;
						pSockInfo->bDelete = true;
						pSockInfo->deleteInfo = "dwIOError == ERROR_NETNAME_DELETED";
					}
					else
						MessageBox(NULL, _T("���������쳣\n"), _T("RIM3.0 ����"), MB_OK);
					pServer->m_socketInfolock.unlock();
				}
				else
					MessageBox(NULL, _T("���������쳣\n"), _T("RIM3.0 ����"), MB_OK);
			}
		}
		else if (pSockInfo == NULL)
		{	// ������ݴ������ˣ����˳�
			if (BytesTransferred == 0xFFFFFFFF)
			{	////�߳��˳���ʶ
				break;
			}
			else
			{	////��ҵ��ʶ
				switch (BytesTransferred)
				{
				case JOB_OBJECT_MSG_END_OF_JOB_TIME:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB����ҵ����ʱ������n"));
					break;

				case JOB_OBJECT_MSG_END_OF_PROCESS_TIME:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB����ҵ ����(Id=%d) ����ʱ������\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB����ҵ�а�������Ľ���\n"));
					break;

				case JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB����ҵĿǰ���������Ѿ�Ϊ0��\n"));
					break;

				case JOB_OBJECT_MSG_NEW_PROCESS:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB���½��� (Id=%d) ������ҵ\n"), pIOOverlapped);

					break;

				case JOB_OBJECT_MSG_EXIT_PROCESS:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB������ (Id=%d) ��ֹ\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB������ (Id=%d) �쳣��ֹ\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB������ (Id=%d) �����ڴ�����\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_JOB_MEMORY_LIMIT:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB������ (Id=%d) ������ҵ�ڴ�����\n"), pIOOverlapped);
					break;

				default:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB��δ֪��֪ͨ: %d\n"), BytesTransferred);
					break;
				}

				OutputDebugString(szTemp);
			}
		}
		else
		{
			pServer->m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_READ);
			if (BytesTransferred == 0)
			{
				bDecrementIO = true;
				pSockInfo->bDelete = true;
				pSockInfo->deleteInfo = "BytesTransferred == 0";
			}
			else if (pIOOverlapped->bRecvOrSend)
			{

				bDecrementIO = true;

				pSockInfo->recvTotalByte += BytesTransferred;
				ZeroMemory(&(pIOOverlapped->overlapped), sizeof(OVERLAPPED));								////��0Ϊ����׼��

				if (pIOOverlapped->cacheBuf.len > unitPackLen)
				{
					MessageBox(NULL, _T("ServerWorkerThread ���л����������쳣\n"), _T("RIM3.0 ����"), MB_OK);
					{	////���¿�ʼ����
						pIOOverlapped->bytesRECV	= 0;													////�Ѿ������˶��ٸ��ֽ���
						pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;		////��ǰ��ʼ�����ֽڵ�BUFλ��
						pIOOverlapped->cacheBuf.len	= unitPackLen;											////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
					}
				}

				Flags = 0;

				if (BytesTransferred > unitPackLen)
				{
					MessageBox(NULL, _T("ServerWorkerThread ���ճ����쳣\n"), _T("RIM3.0 ����"), MB_OK);
				}
				else if (BytesTransferred < pIOOverlapped->cacheBuf.len)
				{	//���յĳ��Ȳ���һ��BUFBLOCK��������

					int remainLen = pIOOverlapped->cacheBuf.len - BytesTransferred;							////���ж��ٸ��ֽڴչ� unitRecvLen

					ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
					pIOOverlapped->bytesRECV	+= BytesTransferred;										////�Ѿ������˶��ٸ��ֽ���
					pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;			////��ǰ��ʼ�����ֽڵ�BUFλ��
					pIOOverlapped->cacheBuf.len	= remainLen;												////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen

					///////IO����
					if (pSockInfo->socket != INVALID_SOCKET && WSARecv(pSockInfo->socket, &(pIOOverlapped->cacheBuf), 1, &RecvBytes, &Flags, &(pIOOverlapped->overlapped), NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != ERROR_IO_PENDING)
						{
							pSockInfo->bDelete = true;
							pSockInfo->deleteInfo = "WSARecv";
							OutputDebugString(_T("RIM3.0 WSARecv() ����ʣ������մ���\n"));
							//MessageBox(NULL, _T("WSARecv() ����ʣ������մ���\n"), _T("RIM3.0 ����"), MB_OK);
						}
						else
							InterlockedIncrement(&pSockInfo->IOTotalCnt);
					}
					else
						InterlockedIncrement(&pSockInfo->IOTotalCnt);
				}
				else
				{
					pIOOverlapped->bytesRECV	+= BytesTransferred;										////�Ѿ������˶��ٸ��ֽ���
					ASSERT(pIOOverlapped->bytesRECV >= unitPackLen);


					LPRIM30TCPBLOCK pBlock		= LPRIM30TCPBLOCK(pIOOverlapped->buffer + (pIOOverlapped->bytesRECV - unitPackLen));
					BOOL			bEnd		= ((pBlock->_len + pBlock->_offset) == pBlock->_fileSize);	////û���յ��ļ���β�������������

					int blockLen = pBlock->_len;
					memcpy(pBlock, pBlock->_buf, pBlock->_len);												////ȡ��ʵ����
					pIOOverlapped->bytesRECV	= pIOOverlapped->bytesRECV - unitPackLen + blockLen;		////�Ѿ������˶��ٸ��ֽ���

					if (bEnd)
					{
						LPRIM30MSG pMsg = (LPRIM30MSG)pIOOverlapped->buffer;
						///////////********������Ϣ **********/////////////////////
						/*
						�����ж���Ϣ�Ƿ����ĸ�socket��Ȼ���͹�ȥ���Ǹ�socket�����Ӧһ��pIOOverlapped������֮ǰ������Read״̬����δ���

						*/

						ProcessMsg(pMsg, pSockInfo, pServer);
						///////////********������� **********/////////////////////

						{	////���¿�ʼ����
							ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
							pIOOverlapped->bytesRECV	= 0;													////�Ѿ������˶��ٸ��ֽ���
							pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;		////��ǰ��ʼ�����ֽڵ�BUFλ��
							pIOOverlapped->cacheBuf.len	= unitPackLen;											////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
						}
					}
					else
					{	////δ����β��������
						ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
						pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;		////��ǰ��ʼ�����ֽڵ�BUFλ��
						pIOOverlapped->cacheBuf.len	= unitPackLen;											////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
					}

					///////IO����
					if (pSockInfo->socket != INVALID_SOCKET && WSARecv(pSockInfo->socket, &(pIOOverlapped->cacheBuf), 1, &RecvBytes, &Flags, &(pIOOverlapped->overlapped), NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != ERROR_IO_PENDING)
						{
							//MessageBox(NULL, _T("WSARecv() ����������մ���\n"), _T("RIM3.0 ����"), MB_OK);
							pSockInfo->bDelete = true;
							pSockInfo->deleteInfo = "WSARecv";
							OutputDebugString(_T("RIM3.0 WSARecv() ����������մ���n"));
						}
						else
							InterlockedIncrement(&pSockInfo->IOTotalCnt);
					}
					else
						InterlockedIncrement(&pSockInfo->IOTotalCnt);
				}
			}
			else if(pIOOverlapped->bRecvOrSend == FALSE)
			{	////����
				bDecrementIO = true;

				pSockInfo->sendTotalByte += BytesTransferred;

				ZeroMemory(&(pSockInfo->pSendOverlapped->overlapped), sizeof(OVERLAPPED));			////��0Ϊ����׼��

				if (BytesTransferred > unitPackLen)
				{
					MessageBox(NULL, _T("ServerWorkerThread ���ͳ����쳣\n"), _T("RIM3.0 ����"), MB_OK);
					{	////�����������Ѿ����������
						pSockInfo->pSendOverlapped->packSEND		= 0;
						pSockInfo->pSendOverlapped->bytesSEND		= 0;
						pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////��ǰ��ʼ�����ֽڵ�BUFλ��
						pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
						pSockInfo->pSendOverlapped->sendTolLength	= 0;
					}
				}
				else if (BytesTransferred < pSockInfo->pSendOverlapped->cacheBuf.len)
				{	////һ������û����
					pSockInfo->pSendOverlapped->packSEND	+= BytesTransferred;					////���������˶����ֽ���
					if (false == CRIM30MsgTransServer::SendPackByRemainLen(pSockInfo))
					{
						MessageBox(NULL, _T("ServerWorkerThread ���ͷ��������ݰ�ʱ�쳣\n"), _T("RIM3.0 ����"), MB_OK);
						{	////�����������Ѿ����������
							pSockInfo->pSendOverlapped->packSEND		= 0;
							pSockInfo->pSendOverlapped->bytesSEND		= 0;
							pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////��ǰ��ʼ�����ֽڵ�BUFλ��
							pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
							pSockInfo->pSendOverlapped->sendTolLength	= 0;
						}
					}
				}
				else
				{
					pSockInfo->pSendOverlapped->packSEND			+= BytesTransferred;			////���������˶����ֽ���
					if (pSockInfo->pSendOverlapped->packSEND != unitPackLen)
					{
						MessageBox(NULL, _T("ServerWorkerThread ���ͳ����쳣\n"), _T("RIM3.0 ����"), MB_OK);
						{	////�����������Ѿ����������
							pSockInfo->pSendOverlapped->packSEND		= 0;
							pSockInfo->pSendOverlapped->bytesSEND		= 0;
							pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////��ǰ��ʼ�����ֽڵ�BUFλ��
							pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
							pSockInfo->pSendOverlapped->sendTolLength	= 0;
						}
					}

					if (pSockInfo->pSendOverlapped->sendTolLength - pIOOverlapped->bytesSEND > unitPackLen)
					{
						pSockInfo->pSendOverlapped->bytesSEND		+= unitDatalen;
						pSockInfo->pSendOverlapped->packSEND		= 0;							////��������һ������
						if (false == CRIM30MsgTransServer::SendPackByRemainLen(pSockInfo))
						{
							OutputDebugString(_T("ServerWorkerThread ������һ���������ݰ�ʱ�쳣\n"));
							////MessageBox(NULL, _T("ServerWorkerThread ������һ���������ݰ�ʱ�쳣\n"), _T("RIM3.0 ����"), MB_OK);
							{	////�����������Ѿ����������
								pSockInfo->pSendOverlapped->packSEND		= 0;
								pSockInfo->pSendOverlapped->bytesSEND		= 0;
								pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////��ǰ��ʼ�����ֽڵ�BUFλ��
								pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
								pSockInfo->pSendOverlapped->sendTolLength	= 0;
							}
						}
					}
					else
					{
						pSockInfo->pSendOverlapped->bytesSEND = pSockInfo->pSendOverlapped->sendTolLength;
						{	////�����������Ѿ����������
							pSockInfo->pSendOverlapped->packSEND		= 0;
							pSockInfo->pSendOverlapped->bytesSEND		= 0;
							pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////��ǰ��ʼ�����ֽڵ�BUFλ��
							pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////ʣ�໹�ж��ٸ��ֽڴչ� unitRecvLen
							pSockInfo->pSendOverlapped->sendTolLength	= 0;
						}

						{	////���ݷ�����Ϣ���н�����Ϣ����
							pSockInfo->sendLock.Lock();
							if(pSockInfo->pSendOverlapped->sendTolLength == 0)
							{
								LPRIM30MSG	pMsgInfo	= NULL;
								pSockInfo->tcpSendQueue.Pop((LPRIM30MSG*)&pMsgInfo);
								if(pMsgInfo)
									SendPack(pMsgInfo, pSockInfo);
								RIM30MSG::FreeMsg(pMsgInfo);
							}
							pSockInfo->sendLock.Unlock();
						}
					}
				}
			}

			pServer->m_socketInfolock.unlock();
		}

		////ɾ��SOCKET��Դ
		if (pSockInfo && pSockInfo->bDelete)
		{	////���������IO�����ڴ��������ز�Ϊ0��
			pServer->m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

			if(bDecrementIO)		{	InterlockedDecrement(&pSockInfo->IOTotalCnt); bDecrementIO	= false;	}
			
			if (pSockInfo->IOTotalCnt == 0)
			{	////�ܽ���˵������ lockread֮�� Ҳ����lockWrite֮��
				for (vector<LPSUBSOCKINFO>::iterator it = pServer->m_subSockInfoVec.begin(); it != pServer->m_subSockInfoVec.end(); ++it)
				{
					pSockInfoTemp = *it;
					if (pSockInfo == pSockInfoTemp)
					{
						TCHAR ch[MAX_PATH] ={ 0 };
						_stprintf_s(ch, MAX_PATH, _T("\nRIM3.0 CRIM30MsgTransServer::�ر�һ������ socket:%d. ���ռ���:%d ����%d������, %d, %s\n"),
							DWORD(pSockInfoTemp->socket), pSockInfo->IOTotalCnt, pServer->m_subSockInfoVec.size() - 1, clock(), pSockInfoTemp->deleteInfo.c_str());
						OutputDebugString(ch);

						CancelIo((HANDLE)pSockInfo->socket);
						closesocket(pSockInfo->socket);	pSockInfo->socket = INVALID_SOCKET;
						delete pSockInfoTemp;
						pServer->m_subSockInfoVec.erase(it);
						break;
					}
				}
			}

			pServer->m_socketInfolock.unlock();
		}
		else
		{	////���������IO�����ڴ���ֻ�����μ�����1
			if(bDecrementIO)		{	InterlockedDecrement(&pSockInfo->IOTotalCnt); bDecrementIO	= false;	}
		}
	}
	return 0;
}

UINT __stdcall CRIM30MsgTransServer::ThreadSendData(void *pDt)
{
	CRIM30MsgTransServer *pServer			= (CRIM30MsgTransServer*)pDt;

	DWORD dwRet = 0;

	while (TRUE)
	{
		dwRet = WaitForSingleObject(pServer->m_hExit, 0);
		if (dwRet == WAIT_OBJECT_0)
			break;

		LPUSERMSGINFO	pUserMsgInfo	= NULL;
		if (pServer->m_userSendQueue.GetQueueCount())
		{	////�Ȱ���Ϣѹ�뵽�����б���
			//while(pServer->m_userSendQueue.GetQueueCount())	////��ѭ�� �ر�ʱ�����ж��Ƿ��˳��߳�
			{
				pServer->m_userSendQueue.Pop((LPRIM30MSG*)&pUserMsgInfo);
				if ((clock() - pUserMsgInfo->clk) > 60 * 1000)
				{
					delete	pUserMsgInfo;	pUserMsgInfo = NULL;
				}
				else
				{
					LPRIM30MSG		pMsg				= pUserMsgInfo->pMsg;
					bool			bProcessd			= false;
					pServer->m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_READ);
					for (UINT i = 0; i < pServer->m_subSockInfoVec.size(); ++i)
					{
						LPSUBSOCKINFO	pSockInfoTemp		= pServer->m_subSockInfoVec.at(i);
						if (pUserMsgInfo->processID == pSockInfoTemp->processID)
						{////////�������ݵ���Ӧ��SOCKET

							pSockInfoTemp->sendLock.Lock();
							if(pSockInfoTemp->pSendOverlapped->sendTolLength == 0)
							{
								LPRIM30MSG pMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pSockInfoTemp->guid, pMsg->GetMsgID(), pMsg->GetDataType(), pMsg->GetDataBuf(), pMsg->GetDataLength());
								SendPack(pMsgTemp, pSockInfoTemp);
								RIM30MSG::FreeMsg(pMsgTemp);

								delete	pUserMsgInfo;	pUserMsgInfo = NULL;
							}
							pSockInfoTemp->sendLock.Unlock();							
							////���������ݰ�����һ�δ����Ҫɾ��
							break;
						}
						else if (pUserMsgInfo->processID == 0xFFFFFFFF && strcmp(pMsg->GetRecvGuid().c_str(), pSockInfoTemp->guid) == 0)
						{
							pSockInfoTemp->sendLock.Lock();
							if(pSockInfoTemp->pSendOverlapped->sendTolLength == 0)
							{
								SendPack(pMsg, pSockInfoTemp);
								delete	pUserMsgInfo;	pUserMsgInfo = NULL;
							}
							pSockInfoTemp->sendLock.Unlock();	
							break;
						}
					}
					pServer->m_socketInfolock.unlock();

					if(pUserMsgInfo)	/////˵����δ����
						pServer->m_userSendQueue.Push((LPRIM30MSG)pUserMsgInfo);
				}
			}
		}
		else
			Sleep(1);
	}

	return true;
}

UINT CRIM30MsgTransServer::ThreadProc()
{

	SOCKET 			servSocket				= INVALID_SOCKET;	// �����׽���
	SOCKET 			clientSocket			= INVALID_SOCKET;	// ��ͻ��˽���ͨ�ŵ��׽���
	LPSUBSOCKINFO	pSockInfo				= NULL;				// �׽��־���ṹ��
	DWORD 			RecvBytes				= 0;				// ���յ����ֽ���
	DWORD 			Flags					= 0;				// WSARecv()������ָ���ı�ʶλ
	UINT 			ThreadID				= 0;				// �����̱߳��
	DWORD 			dwRet					= 0;				// ��������ֵ
	SOCKADDR_IN		serverAddr;									// ��������ַ
	SYSTEM_INFO		SystemInfo;									// ��ȡϵͳ��Ϣ��������Ҫ���ڻ�ȡCPU������
	WSADATA			wsaData;									// Windows Socket��ʼ����Ϣ

	// ��ʼ��Windows Sockets����
	if ((dwRet = WSAStartup(0x0202, &wsaData)) != 0)
	{
		MessageBox(NULL, _T("RIM3.0 WSAStartup failed with error\n"), _T("RIM3.0 ����"), MB_OK);
		OutputDebugString(_T("RIM3.0 WSAStartup failed with error\n"));
		return -1;
	}
	// �����µ���ɶ˿�
	if ((m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		MessageBox(NULL, _T("RIM3.0 CreateIoCompletionPort failed with error\n"), _T("RIM3.0 ����"), MB_OK);
		OutputDebugString(_T("RIM3.0 CreateIoCompletionPort failed with error\n"));
		return -1;
	}

	{	/////����ҵ����ɶ˿������
		BOOL bRet = FALSE;
		bRet = m_job.Create(NULL, RIM30MsgServer_JOBNAME);
		bRet = m_job.SetEndOfJobInfo(JOB_OBJECT_POST_AT_END_OF_JOB);
		if(FALSE == m_job.AssociateCompletionPort(m_completionPort, ((UINT_PTR)NULL)))
		{
			MessageBox(NULL, _T("RIM3.0 m_job.AssociateCompletionPort failed with error\n"), _T("RIM3.0 ����"), MB_OK);
			OutputDebugString(_T("RIM3.0 m_job.AssociateCompletionPort failed with error\n"));
			return -1;
		}
	}
	// ��ȡϵͳ��Ϣ
	GetSystemInfo(&SystemInfo);   
	// ����CPU���������߳�
	for(UINT i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		HANDLE ThreadHandle;
		// �����̣߳�����ServerWorkerThread()����            
		if ((ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, ServerWorkerThread, (LPVOID)this, 0, &ThreadID)) == NULL)
		{
			MessageBox(NULL, _T("RIM3.0 CreateThread() failed with error\n"), _T("RIM3.0 ����"), MB_OK);
			OutputDebugString(_T("RIM3.0 CreateThread() failed with error\n"));
			return -1;
		}      
		m_threadHandles[i] = ThreadHandle;
	}
	m_hSendDataThread = (HANDLE)_beginthreadex(NULL, 0, ThreadSendData, (LPVOID)this, 0, NULL);

	// ���������׽���
	if ((servSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("RIM3.0 WSASocket() failed with error\n"), _T("RIM3.0 ����"), MB_OK);
		OutputDebugString(_T("RIM3.0 WSASocket() failed with error\n"));
		return -1;
	}

	ULONG reuse = true;
	if(setsockopt( servSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CRIM30MsgTransServer SO_REUSEADDR ���ô���\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(servSocket);
		return -1;
	}

	// �󶨵����ص�ַ��9990�˿�
	serverAddr.sin_family		= AF_INET;
	serverAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	serverAddr.sin_port			= htons(m_serverPort);
	
	int ret = bind(servSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret)
	{
		MessageBox(NULL, _T("CRIM30MsgTransServer �׽��Ӱ�ʧ��\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(servSocket);
		return -1;
	}

	//int nScktOpt = 5000;
	////���ý��ճ�ʱ
	//if(setsockopt(Listen, SOL_SOCKET, SO_RCVTIMEO, (char *)&nScktOpt, sizeof(nScktOpt)) == SOCKET_ERROR)
	//{
	//	MessageBox(NULL, _T("CRIM30MsgTransServer ���ý��ճ�ʱ\n"), _T("RIM3.0 ����"), MB_OK);
	//	closesocket(Listen);
	//	WSACleanup();
	//	return;
	//}
	////���÷��ͳ�ʱ
	//nScktOpt = 5000;
	//if(setsockopt(Listen, SOL_SOCKET, SO_SNDTIMEO,(char *)&nScktOpt, sizeof(nScktOpt)))
	//{
	//	MessageBox(NULL, _T("CRIM30MsgTransServer ���÷��ͳ�ʱ\n"), _T("RIM3.0 ����"), MB_OK);
	//	closesocket(Listen);
	//	return;
	//}

	if(SOCKET_ERROR == listen(servSocket, SOMAXCONN))
	{
		MessageBox(NULL, _T("CRIM30MsgTransServer ����ʧ��\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(servSocket);
		return -1;
	}

	//return -1;
	// �����˿ڴ򿪣��Ϳ�ʼ������ѭ����һ��socket���ϣ�WSAAccept�ʹ���һ��socket��
	// ���socket ����ɶ˿�����
	while(servSocket != INVALID_SOCKET)
	{
		dwRet = WaitForSingleObject(m_hExit, 0);
		if(dwRet == WAIT_OBJECT_0)
			break;

		if(CRIM30MsgTransServer::SelectSocketRead(servSocket, 1) > 0)
		{
			// �ȴ��ͻ�����
			if ((clientSocket = WSAAccept(servSocket, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
			{
				MessageBox(NULL, _T("CRIM30MsgTransServer �ȴ��ͻ�����ʧ��\n"), _T("RIM3.0 ����"), MB_OK);
				break;
			}

			{// ���䲢�����׽��־���ṹ��
				pSockInfo			= new SUBSOCKINFO;  
				pSockInfo->socket	= clientSocket;

				// ΪI/O�����ṹ������ڴ�ռ�
				pSockInfo->pRecvOverlapped->bRecvOrSend 	= TRUE;
				pSockInfo->pSendOverlapped->bRecvOrSend 	= FALSE;

				// ��ʼ��I/O�����ṹ��
				ZeroMemory(&(pSockInfo->pSendOverlapped->overlapped), sizeof(OVERLAPPED));
				pSockInfo->pSendOverlapped->bytesSEND		= 0;
				pSockInfo->pSendOverlapped->bytesRECV		= 0;
				pSockInfo->pSendOverlapped->cacheBuf.len	= 0;						////һ��BLOCK
				pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer;
				pSockInfo->pSendOverlapped->sendTolLength	= 0;						// ���������ܳ���
				pSockInfo->pSendOverlapped->packSEND		= 0;						// һ���������͵��ֽ�����һ�����ݿ���δ���꣩

				// ��ʼ��I/O�����ṹ��
				ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
				pSockInfo->pRecvOverlapped->bytesSEND		= 0;
				pSockInfo->pRecvOverlapped->bytesRECV		= 0;
				pSockInfo->pRecvOverlapped->cacheBuf.len	= sizeof(RIM30TCPBLOCK);	////Ҫ�����һ��BLOCK
				pSockInfo->pRecvOverlapped->cacheBuf.buf	= pSockInfo->pRecvOverlapped->buffer;

				m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);
				m_subSockInfoVec.push_back(pSockInfo);
				TCHAR ch[MAX_PATH] ={ 0 };
				_stprintf_s(ch, MAX_PATH, _T("RIM3.0 CRIM30MsgTransServer::���յ�һ������ socket:%d. ��%d������\n"), DWORD(clientSocket), m_subSockInfoVec.size());
				OutputDebugString(ch);
				m_socketInfolock.unlock();
			}

			// ����ͻ��˽���ͨ�ŵ��׽���Accept����ɶ˿�CompletionPort�����
			if (CreateIoCompletionPort((HANDLE)pSockInfo->socket, m_completionPort, (DWORD)pSockInfo, 0) == NULL)
			{
				MessageBox(NULL, _T("RIM3.0 CreateIoCompletionPort ʧ��\n"), _T("RIM3.0 ����"), MB_OK);
				OutputDebugString(_T("RIM3.0 CreateIoCompletionPort failed with error\n"));
				break;
			}


			// �������ݣ��ŵ�PerIoData�У���perIoData��ͨ�������߳��е�ServerWorkerThread����ȡ��,
			if (pSockInfo->socket != INVALID_SOCKET)
			{
				if (WSARecv(pSockInfo->socket, &(pSockInfo->pRecvOverlapped->cacheBuf), 1, &RecvBytes, &Flags, &(pSockInfo->pRecvOverlapped->overlapped), NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != ERROR_IO_PENDING)
					{
						pSockInfo->bDelete = true;
						pSockInfo->deleteInfo = "WSARecv";
						MessageBox(NULL, _T("WSARecv() failed with error\n"), _T("RIM3.0 ����"), MB_OK);
					}
					else
						InterlockedIncrement(&pSockInfo->IOTotalCnt);
				}
				else
					InterlockedIncrement(&pSockInfo->IOTotalCnt);
			}
		}
		else
		{
			Sleep(1);
		}
	}

	closesocket(servSocket);
	return 0;
}

#ifdef DEBUG

CString	CRIM30MsgTransServer::GetName()
{
	return m_strName;
}

void CRIM30MsgTransServer::GetName(LPCTSTR lpName)
{
	m_strName = lpName;
}


#endif

bool CRIM30MsgTransServer::IsProgramExist(LPCTSTR lpszExeName, DWORD processID)  
{  
	bool			bReturn = false;
	LPCTSTR 		lpszFile;
	HANDLE 			hProcessSnap;
	PROCESSENTRY32	pe32;  

	if( !lpszExeName || !lpszExeName[0] ) return 0;

	lpszFile = _tcsrchr(lpszExeName, _T('\\'));
	if( lpszFile != 0 )
		lpszExeName =   lpszFile + 1;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hProcessSnap == INVALID_HANDLE_VALUE )  return 0;

	memset(&pe32, 0, sizeof(PROCESSENTRY32));
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if( Process32First(hProcessSnap, &pe32) )
	{
		do
		{
			lpszFile = _tcsrchr(pe32.szExeFile, _T('\\'));
			lpszFile = lpszFile ? lpszFile + 1 : pe32.szExeFile;
			if( _tcsicmp(lpszFile, lpszExeName) == 0 && (pe32.th32ProcessID == processID))
			{
				bReturn = true;
				break;
			}
		} while( Process32Next(hProcessSnap,&pe32) );
	}
	CloseHandle(hProcessSnap);

	return bReturn;
}

BOOL CRIM30MsgTransServer::OpenSubSystem(string exePath /*= ""*/, LPRIM30MSG pInitMsg/* = NULL*/)
{
	ASSERT(m_completionPort != INVALID_HANDLE_VALUE);
	if(m_completionPort == INVALID_HANDLE_VALUE)
		return FALSE;
	
	if(exePath.size())
	{
		STARTUPINFOA si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

		{
			SECURITY_ATTRIBUTES SecAttr;
			SECURITY_DESCRIPTOR SecDesc;

			SecAttr.nLength = sizeof(SecAttr);
			SecAttr.bInheritHandle = FALSE;
			SecAttr.lpSecurityDescriptor = &SecDesc;

			InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl(&SecDesc, TRUE, 0, FALSE);


			if(CreateProcessA(exePath.c_str(), NULL, NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB , NULL, NULL, &si, &pi))
			{
				m_job.AssignProcess(pi.hProcess);

				ResumeThread(pi.hThread);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				clock_t clk = clock();
				while(1)
				{
					if(IsProgramExist(CA2T(exePath.c_str()), pi.dwProcessId))
						break;

					if(clock() - clk > 3000)
						break;
				}

				if (pInitMsg)	m_userSendQueue.Push((LPRIM30MSG)new USERMSGINFO(pi.dwProcessId, pInitMsg));

				return TRUE;
			}
		}
	}

	return FALSE;
}

void CRIM30MsgTransServer::CloseAllSubSystem()
{
	if(m_completionPort == INVALID_HANDLE_VALUE)
		return;

	{	////��������ϵͳSOCKET���͹ر���Ϣ
		m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_READ);

		LPSUBSOCKINFO		pSockInfoTemp		= NULL;
		for(vector<LPSUBSOCKINFO>::iterator it = m_subSockInfoVec.begin(); it != m_subSockInfoVec.end(); ++it)
		{
			pSockInfoTemp = *it;

			LPRIM30MSG pMsg = RIM30MSG::MakeMsg(m_serverGuid.c_str(), pSockInfoTemp->guid, RIM30MSG_SHUTDOWN_ASK, RIM30MSG::DATATYPE_BINARY, (LPBYTE)pSockInfoTemp->guid, 64);
			SendPack(pMsg, pSockInfoTemp);
			RIM30MSG::FreeMsg(pMsg);
		}

		m_socketInfolock.unlock();
	}

	clock_t clkBegin = clock();
	while (m_subSockInfoVec.size())
	{
		Sleep(1);

		if(clock() - clkBegin > 1000)
		{
			TCHAR ch[MAX_PATH] = {0};
			_stprintf_s(ch, MAX_PATH, _T("RIM3.0 CRIM30MsgTransServer::����%d�����ؽ���û�йرգ�"), m_subSockInfoVec.size());
			OutputDebugString(ch);
			break;
		}
	}
}

UINT CRIM30MsgTransServer::GetSubSystemCnt()
{
	/*����������ϵͳ���ڱ�������������Ϊm_subProcessIDs.size()*/
	/*�������ϵͳ��Զ������������Ϊm_subSockInfoVec.size()*/
	//return m_subProcessIDs.size();
	return m_subSockInfoVec.size();
}

void CRIM30MsgTransServer::PushSendMsg(LPRIM30MSG pMsg)
{
	if(pMsg == NULL)
		return;

	m_userSendQueue.Push((LPRIM30MSG)new USERMSGINFO(pMsg));
}

std::string CRIM30MsgTransServer::GetSubSystemGuid(UINT index)
{
	string strTemp = "";
	m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_READ);
	if(index >= 0 && index < m_subSockInfoVec.size())
		strTemp = m_subSockInfoVec[index]->guid;
	m_socketInfolock.unlock();

	return strTemp;
}
