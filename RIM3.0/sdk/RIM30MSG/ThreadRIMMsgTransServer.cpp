//********************************************************************
// THREADRIMMSGTRANSSERVER.CPP 文件注释
// 文件名 　: THREADRIMMSGTRANSSERVER.CPP
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// 作者 　　: 张永
// 创建时间 : 2015/4/7 14:05
// 文件描述 : 
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

	
	{////【zy 2015-06-08 ↓】为了解决子系统 由于BUG未关闭的问题
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
				_stprintf_s(ch, MAX_PATH, _T("RIM3.0 CRIM30MsgTransServer::强制关闭作业！"));
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
{	/////根据pSockInfo->pSendOverlapped->bytesSEND偏移后构造一包数据，并发送该包最后的remainLen长度数据
	if(pSockInfo == NULL)
		return false;

	RIM30TCPBLOCK		blockTemp;
	DWORD				unitPackLen			= sizeof(RIM30TCPBLOCK);
	const int			unitDatalen			= RIM30TCPBLOCKDATALEN;		
	DWORD				SendBytes			= 0;

	{	////根据已发送整包单位字节数 以及 整包单位剩余未发字节数 自动构造包体并发送
		ZeroMemory(&(pSockInfo->pSendOverlapped->overlapped), sizeof(OVERLAPPED));
		int dataReserveSend = pSockInfo->pSendOverlapped->sendTolLength - pSockInfo->pSendOverlapped->bytesSEND;
		if (dataReserveSend > 0)
		{////构造blockTemp
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

			pSockInfo->pSendOverlapped->cacheBuf.buf = (char*)&blockTemp + pSockInfo->pSendOverlapped->packSEND;	/////发送一包数据
			pSockInfo->pSendOverlapped->cacheBuf.len = unitPackLen - pSockInfo->pSendOverlapped->packSEND;			/////一包数据的长度

			if (pSockInfo->socket != INVALID_SOCKET)
			{
				if (WSASend(pSockInfo->socket, &(pSockInfo->pSendOverlapped->cacheBuf), 1, &SendBytes, 0,
					&(pSockInfo->pSendOverlapped->overlapped), NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != ERROR_IO_PENDING)
					{
						pSockInfo->bDelete = true;
						pSockInfo->deleteInfo = "WSASend";
						//MessageBox(NULL, _T("RIM3.0 WSASend() failed with error\n"), _T("错误"), MB_OK);
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
{	/////根据pSockInfo->pSendOverlapped->bytesSEND偏移后构造一包数据，并发送该包最后的remainLen长度数据

	if(pMSG == NULL || pSockInfo == NULL)
		return false;

	memcpy(pSockInfo->pSendOverlapped->buffer, pMSG, pMSG->GetTotalLength());	////拷贝消息到发送缓冲区
	pSockInfo->pSendOverlapped->sendTolLength	= pMSG->GetTotalLength();			////发送数据的总长度
	pSockInfo->pSendOverlapped->packSEND	= 0;	
	pSockInfo->pSendOverlapped->bytesSEND	= 0;								////当前发送的字节数

	if (pSockInfo->socket != INVALID_SOCKET && false == CRIM30MsgTransServer::SendPackByRemainLen(pSockInfo))
	{
		//MessageBox(NULL, _T("SendPack 初始发送完整数据包时异常\n"), _T("RIM3.0 错误"), MB_OK);
		{	////缓冲区数据已经发送完毕了
			pSockInfo->pSendOverlapped->packSEND		= 0;
			pSockInfo->pSendOverlapped->bytesSEND		= 0;
			pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////当前起始接收字节的BUF位置
			pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////剩余还有多少个字节凑够 unitRecvLen
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

	LPRIM30MSG			pSendMsgTemp		= NULL;			////将要压入发送消息的队列
	LPSUBSOCKINFO		pSendSockInfo		= pSockInfo;	////指向应当发送消息的套接信息


	if(pMsg->GetMsgID() == RIM30MSG_ONLINE_ASK)				////子系统上线请求
	{	////上线消息  首先一个系统应当先上线，
		if(memcmp(pMsg->GetSendGuid().c_str(), pMsg->GetDataBuf(), pMsg->GetSendGuid().size()) == 0 && pMsg->GetDataLength() == 68)
		{
			string		userID				= CT2A(RIMConfig::GetInstance()->m_pLogonUser->UserID.c_str());
			DWORD		userTypeID			= RIMConfig::GetInstance()->m_pLogonUser->userTypeID;

			{	////构造消息 并发送
				const int	totalLen		= 64 + 256 + 1;
				BYTE		ch[totalLen]	= { 0 };
				LPBYTE		pChar			= ch;

				memcpy_s(pChar, 64,  pServer->m_serverGuid.c_str(),  pServer->m_serverGuid.size());	
				pChar += 64;
				memcpy_s(pChar, 256, userID.c_str(), userID.size());	//用户ID
				pChar += 256;
				*pChar = (userTypeID == USERINFO::USER_ID_UNKNOWN);		//是否是便携版

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
	{	////处理上线的系统发来的消息
		if (pMsg->GetMsgID() == RIM30MSG_HEART_PACK)	////心跳包消息
		{	//心跳包消息 
			pSendMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pSockInfo->guid, RIM30MSG_HEART_PACK, 0, NULL, 0);
		}
		else if (strcmp(pMsg->GetRecvGuid().c_str(), pServer->m_serverGuid.c_str()) == 0)
		{	////发给导航器的消息 
			switch (pMsg->GetMsgID())
			{
			case RIM30MSG_OFFLINE_ASK:					////子系统下线请求
				{
					char ch[MAX_PATH] ={ 0 };
					sprintf_s(ch, MAX_PATH, "OK");
					pSendMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pMsg->GetSendGuid().c_str(), RIM30MSG_OFFLINE_ANSWER,
						RIM30MSG::DATATYPE_BINARY, (LPBYTE)ch, strlen(ch));
				}
				break;
			case RIM30MSG_SHUTDOWN_ASK:					////关闭请求
				break;
			case RIM30MSG_SHUTDOWN_ANSWER:				////关闭应答
				{
				}
				break;
			case RIM30MSG_MEASUREDF_START_ANSWER:		////启动测量请求
				{
					int i = 0;
					i++;
				}
				break;
			case RIM30MSG_QUERYSERVERADDR_ASK:			////启动测量应答
				{
					if (pMsg->GetDataLength() == 8)
					{
						LPDWORD pBuffer		= (LPDWORD)pMsg->GetDataBuf();

						DWORD	subSysID	= *pBuffer;
						DWORD	issueTYPE	= *(pBuffer + 1);
						vector<LPWEBADDRINFO> infoArray;

						{	////查找符合条件的服务地址
							bool bGoOn = true;
							for (map<DWORD, vector<LPWEBADDRINFO>>::iterator it = RIMConfig::GetInstance()->m_webAddrInfoMap.begin();
								bGoOn && it != RIMConfig::GetInstance()->m_webAddrInfoMap.end(); ++it)
							{
								if (it->first == subSysID)
								{
									if (issueTYPE != -1)
									{	////非-1查找对应的信息
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
									{	////-1查找全部
										for (UINT i = 0; bGoOn && i < it->second.size(); ++i)
										{
											infoArray.push_back(it->second.at(i));
										}
									}

									bGoOn = false;
								}
							}
						}

						{	////构造消息 并发送
							const int	totalLen		= 4 + 4 + (4 + 256 + 256) * infoArray.size();
							DWORD		totalCnt		= infoArray.size();
							LPBYTE		pBuffer			= new BYTE[totalLen];
							memset(pBuffer, 0, totalLen);
							LPBYTE		pChar			= pBuffer;

							memcpy_s(pChar, 4, &subSysID, sizeof(DWORD));		pChar += 4;		////SUBSYSTEMID		子系统ID
							memcpy_s(pChar, 4, &totalCnt, sizeof(DWORD));		pChar += 4;		////子类型的个数

							for (UINT i = 0; i < infoArray.size(); ++i)
							{
								LPWEBADDRINFO lpwebaddrinfo = infoArray.at(i);
								memcpy_s(pChar, 4, &(lpwebaddrinfo->nSubModeID), sizeof(DWORD));
								pChar += 4;	////sName			服务名称
								memcpy_s(pChar, 256, lpwebaddrinfo->sName.c_str(), lpwebaddrinfo->sName.size());
								pChar += 256;	////sName			服务名称
								memcpy_s(pChar, 256, lpwebaddrinfo->sLocation.c_str(), lpwebaddrinfo->sLocation.size());
								pChar += 256;	////sLocation		服务地址
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
		{	////子系统发给子系统的消息 
			pSendMsgTemp = RIM30MSG::MakeMsg(pMsg->GetSendGuid().c_str(), pMsg->GetRecvGuid().c_str(), pMsg->GetMsgID(), pMsg->GetDataType(), pMsg->GetDataBuf(), pMsg->GetDataLength());
			for(UINT i = 0; i < pServer->m_subSockInfoVec.size(); ++i)
			{
				if(strcmp(pMsg->GetRecvGuid().c_str(), pServer->m_subSockInfoVec.at(i)->guid) == 0)
				{	////////查找要发送数据到相应的SOCKET  转发数据
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
			MessageBox(NULL, _T("消息处理处错误异常，指针不应该为空\n"), _T("RIM3.0 错误"), MB_OK);
	}

	if(pSendSockInfo)
	{	////根据发送消息队列进行消息发送
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
		{	////先关闭连接接收线程
			SetEvent(m_hExit);
			// 		::PostThreadMessage(m_pThread->m_nThreadID, WM_QUIT, 0, 0);

			WaitForSingleObject(m_pThread->m_hThread, INFINITE);
			WaitForSingleObject(m_hSendDataThread, INFINITE);		CloseHandle(m_hSendDataThread);
			delete m_pThread;
			m_pThread = NULL;

			CloseHandle(m_hExit);
			m_hExit = NULL;
		}

		{	////等持并关闭所有子系统EXE
			CloseAllSubSystem();
		}

		{	////关闭所有完成端口线程
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

		{	////清空子系统SOCKET信息
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

		{	////清空子系统保留发送信息
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

//	工作线程，循环检测完成端口状态，获取PerIoData中的数据
UINT __stdcall CRIM30MsgTransServer::ServerWorkerThread(LPVOID pVoid)
{
	CRIM30MsgTransServer *pServer				= (CRIM30MsgTransServer*) pVoid;	 
	HANDLE				&CompletionPort			= pServer->m_completionPort;		// 完成端口句柄

	DWORD				BytesTransferred		= 0;								// 数据传输的字节数
	LPSUBSOCKINFO		pSockInfo				= NULL;								// 套接字句柄结构体
	LPIOOVERLAPPED		pIOOverlapped			= NULL;								// I/O操作结构体
	DWORD				RecvBytes				= 0;								// 发送和接收的数量
	DWORD				Flags					= 0;								// WSARecv()函数中的标识位

	const int			unitDatalen				= RIM30TCPBLOCKDATALEN;				//一包真实数据的长度
	const DWORD			unitPackLen				= sizeof(RIM30TCPBLOCK);
	LPSUBSOCKINFO		pSockInfoTemp			= NULL;
	TCHAR				szTemp[MAX_PATH]		= {0};
	bool				bDecrementIO			= false;							//是否需要减少接收计数

	while (TRUE)
	{
		// 检查完成端口的状态
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
						MessageBox(NULL, _T("基它错误异常\n"), _T("RIM3.0 错误"), MB_OK);
					pServer->m_socketInfolock.unlock();
				}
				else
					MessageBox(NULL, _T("基它错误异常\n"), _T("RIM3.0 错误"), MB_OK);
			}
		}
		else if (pSockInfo == NULL)
		{	// 如果数据传送完了，则退出
			if (BytesTransferred == 0xFFFFFFFF)
			{	////线程退出标识
				break;
			}
			else
			{	////作业标识
				switch (BytesTransferred)
				{
				case JOB_OBJECT_MSG_END_OF_JOB_TIME:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：作业到达时间限制n"));
					break;

				case JOB_OBJECT_MSG_END_OF_PROCESS_TIME:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：作业 进程(Id=%d) 到达时间限制\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：作业中包含过多的进程\n"));
					break;

				case JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：作业目前工作进程已经为0个\n"));
					break;

				case JOB_OBJECT_MSG_NEW_PROCESS:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：新进程 (Id=%d) 加入作业\n"), pIOOverlapped);

					break;

				case JOB_OBJECT_MSG_EXIT_PROCESS:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：进程 (Id=%d) 终止\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：进程 (Id=%d) 异常终止\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：进程 (Id=%d) 超出内存限制\n"), pIOOverlapped);
					break;

				case JOB_OBJECT_MSG_JOB_MEMORY_LIMIT:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：进程 (Id=%d) 超出作业内存限制\n"), pIOOverlapped);
					break;

				default:
					_stprintf_s(szTemp, MAX_PATH, TEXT("RIM3.0 JOB：未知的通知: %d\n"), BytesTransferred);
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
				ZeroMemory(&(pIOOverlapped->overlapped), sizeof(OVERLAPPED));								////清0为发送准备

				if (pIOOverlapped->cacheBuf.len > unitPackLen)
				{
					MessageBox(NULL, _T("ServerWorkerThread 固有缓冲区长度异常\n"), _T("RIM3.0 错误"), MB_OK);
					{	////重新开始接收
						pIOOverlapped->bytesRECV	= 0;													////已经接收了多少个字节了
						pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;		////当前起始接收字节的BUF位置
						pIOOverlapped->cacheBuf.len	= unitPackLen;											////剩余还有多少个字节凑够 unitRecvLen
					}
				}

				Flags = 0;

				if (BytesTransferred > unitPackLen)
				{
					MessageBox(NULL, _T("ServerWorkerThread 接收长度异常\n"), _T("RIM3.0 错误"), MB_OK);
				}
				else if (BytesTransferred < pIOOverlapped->cacheBuf.len)
				{	//接收的长度不够一个BUFBLOCK继续接收

					int remainLen = pIOOverlapped->cacheBuf.len - BytesTransferred;							////还有多少个字节凑够 unitRecvLen

					ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
					pIOOverlapped->bytesRECV	+= BytesTransferred;										////已经接收了多少个字节了
					pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;			////当前起始接收字节的BUF位置
					pIOOverlapped->cacheBuf.len	= remainLen;												////剩余还有多少个字节凑够 unitRecvLen

					///////IO请求
					if (pSockInfo->socket != INVALID_SOCKET && WSARecv(pSockInfo->socket, &(pIOOverlapped->cacheBuf), 1, &RecvBytes, &Flags, &(pIOOverlapped->overlapped), NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != ERROR_IO_PENDING)
						{
							pSockInfo->bDelete = true;
							pSockInfo->deleteInfo = "WSARecv";
							OutputDebugString(_T("RIM3.0 WSARecv() 请求剩余包接收错误\n"));
							//MessageBox(NULL, _T("WSARecv() 请求剩余包接收错误\n"), _T("RIM3.0 错误"), MB_OK);
						}
						else
							InterlockedIncrement(&pSockInfo->IOTotalCnt);
					}
					else
						InterlockedIncrement(&pSockInfo->IOTotalCnt);
				}
				else
				{
					pIOOverlapped->bytesRECV	+= BytesTransferred;										////已经接收了多少个字节了
					ASSERT(pIOOverlapped->bytesRECV >= unitPackLen);


					LPRIM30TCPBLOCK pBlock		= LPRIM30TCPBLOCK(pIOOverlapped->buffer + (pIOOverlapped->bytesRECV - unitPackLen));
					BOOL			bEnd		= ((pBlock->_len + pBlock->_offset) == pBlock->_fileSize);	////没有收到文件结尾则继续接收数据

					int blockLen = pBlock->_len;
					memcpy(pBlock, pBlock->_buf, pBlock->_len);												////取真实数据
					pIOOverlapped->bytesRECV	= pIOOverlapped->bytesRECV - unitPackLen + blockLen;		////已经接收了多少个字节了

					if (bEnd)
					{
						LPRIM30MSG pMsg = (LPRIM30MSG)pIOOverlapped->buffer;
						///////////********处理消息 **********/////////////////////
						/*
						这里判断消息是发往哪个socket的然后发送过去。那个socket必须对应一个pIOOverlapped，但它之前可能是Read状态该如何处理？

						*/

						ProcessMsg(pMsg, pSockInfo, pServer);
						///////////********处理完成 **********/////////////////////

						{	////重新开始接收
							ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
							pIOOverlapped->bytesRECV	= 0;													////已经接收了多少个字节了
							pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;		////当前起始接收字节的BUF位置
							pIOOverlapped->cacheBuf.len	= unitPackLen;											////剩余还有多少个字节凑够 unitRecvLen
						}
					}
					else
					{	////未到结尾继续接收
						ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
						pIOOverlapped->cacheBuf.buf	= pIOOverlapped->buffer + pIOOverlapped->bytesRECV;		////当前起始接收字节的BUF位置
						pIOOverlapped->cacheBuf.len	= unitPackLen;											////剩余还有多少个字节凑够 unitRecvLen
					}

					///////IO请求
					if (pSockInfo->socket != INVALID_SOCKET && WSARecv(pSockInfo->socket, &(pIOOverlapped->cacheBuf), 1, &RecvBytes, &Flags, &(pIOOverlapped->overlapped), NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != ERROR_IO_PENDING)
						{
							//MessageBox(NULL, _T("WSARecv() 正常请求接收错误\n"), _T("RIM3.0 错误"), MB_OK);
							pSockInfo->bDelete = true;
							pSockInfo->deleteInfo = "WSARecv";
							OutputDebugString(_T("RIM3.0 WSARecv() 正常请求接收错误n"));
						}
						else
							InterlockedIncrement(&pSockInfo->IOTotalCnt);
					}
					else
						InterlockedIncrement(&pSockInfo->IOTotalCnt);
				}
			}
			else if(pIOOverlapped->bRecvOrSend == FALSE)
			{	////发送
				bDecrementIO = true;

				pSockInfo->sendTotalByte += BytesTransferred;

				ZeroMemory(&(pSockInfo->pSendOverlapped->overlapped), sizeof(OVERLAPPED));			////清0为发送准备

				if (BytesTransferred > unitPackLen)
				{
					MessageBox(NULL, _T("ServerWorkerThread 发送长度异常\n"), _T("RIM3.0 错误"), MB_OK);
					{	////缓冲区数据已经发送完毕了
						pSockInfo->pSendOverlapped->packSEND		= 0;
						pSockInfo->pSendOverlapped->bytesSEND		= 0;
						pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////当前起始接收字节的BUF位置
						pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////剩余还有多少个字节凑够 unitRecvLen
						pSockInfo->pSendOverlapped->sendTolLength	= 0;
					}
				}
				else if (BytesTransferred < pSockInfo->pSendOverlapped->cacheBuf.len)
				{	////一包数据没发完
					pSockInfo->pSendOverlapped->packSEND	+= BytesTransferred;					////整包发送了多少字节数
					if (false == CRIM30MsgTransServer::SendPackByRemainLen(pSockInfo))
					{
						MessageBox(NULL, _T("ServerWorkerThread 发送非完整数据包时异常\n"), _T("RIM3.0 错误"), MB_OK);
						{	////缓冲区数据已经发送完毕了
							pSockInfo->pSendOverlapped->packSEND		= 0;
							pSockInfo->pSendOverlapped->bytesSEND		= 0;
							pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////当前起始接收字节的BUF位置
							pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////剩余还有多少个字节凑够 unitRecvLen
							pSockInfo->pSendOverlapped->sendTolLength	= 0;
						}
					}
				}
				else
				{
					pSockInfo->pSendOverlapped->packSEND			+= BytesTransferred;			////整包发送了多少字节数
					if (pSockInfo->pSendOverlapped->packSEND != unitPackLen)
					{
						MessageBox(NULL, _T("ServerWorkerThread 发送长度异常\n"), _T("RIM3.0 错误"), MB_OK);
						{	////缓冲区数据已经发送完毕了
							pSockInfo->pSendOverlapped->packSEND		= 0;
							pSockInfo->pSendOverlapped->bytesSEND		= 0;
							pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////当前起始接收字节的BUF位置
							pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////剩余还有多少个字节凑够 unitRecvLen
							pSockInfo->pSendOverlapped->sendTolLength	= 0;
						}
					}

					if (pSockInfo->pSendOverlapped->sendTolLength - pIOOverlapped->bytesSEND > unitPackLen)
					{
						pSockInfo->pSendOverlapped->bytesSEND		+= unitDatalen;
						pSockInfo->pSendOverlapped->packSEND		= 0;							////继续发送一个整包
						if (false == CRIM30MsgTransServer::SendPackByRemainLen(pSockInfo))
						{
							OutputDebugString(_T("ServerWorkerThread 发送下一个完整数据包时异常\n"));
							////MessageBox(NULL, _T("ServerWorkerThread 发送下一个完整数据包时异常\n"), _T("RIM3.0 错误"), MB_OK);
							{	////缓冲区数据已经发送完毕了
								pSockInfo->pSendOverlapped->packSEND		= 0;
								pSockInfo->pSendOverlapped->bytesSEND		= 0;
								pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////当前起始接收字节的BUF位置
								pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////剩余还有多少个字节凑够 unitRecvLen
								pSockInfo->pSendOverlapped->sendTolLength	= 0;
							}
						}
					}
					else
					{
						pSockInfo->pSendOverlapped->bytesSEND = pSockInfo->pSendOverlapped->sendTolLength;
						{	////缓冲区数据已经发送完毕了
							pSockInfo->pSendOverlapped->packSEND		= 0;
							pSockInfo->pSendOverlapped->bytesSEND		= 0;
							pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer + pSockInfo->pSendOverlapped->bytesSEND;	////当前起始接收字节的BUF位置
							pSockInfo->pSendOverlapped->cacheBuf.len	= 0;																			////剩余还有多少个字节凑够 unitRecvLen
							pSockInfo->pSendOverlapped->sendTolLength	= 0;
						}

						{	////根据发送消息队列进行消息发送
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

		////删除SOCKET资源
		if (pSockInfo && pSockInfo->bDelete)
		{	////如果有其它IO请求在处理，计数必不为0，
			pServer->m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);

			if(bDecrementIO)		{	InterlockedDecrement(&pSockInfo->IOTotalCnt); bDecrementIO	= false;	}
			
			if (pSockInfo->IOTotalCnt == 0)
			{	////能进来说明不在 lockread之上 也不在lockWrite之上
				for (vector<LPSUBSOCKINFO>::iterator it = pServer->m_subSockInfoVec.begin(); it != pServer->m_subSockInfoVec.end(); ++it)
				{
					pSockInfoTemp = *it;
					if (pSockInfo == pSockInfoTemp)
					{
						TCHAR ch[MAX_PATH] ={ 0 };
						_stprintf_s(ch, MAX_PATH, _T("\nRIM3.0 CRIM30MsgTransServer::关闭一个连接 socket:%d. 接收计数:%d 还有%d个连接, %d, %s\n"),
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
		{	////如果有其它IO请求在处理，只将本次计数减1
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
		{	////先把消息压入到发送列表中
			//while(pServer->m_userSendQueue.GetQueueCount())	////不循环 关闭时快速判断是否退出线程
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
						{////////发送数据到相应的SOCKET

							pSockInfoTemp->sendLock.Lock();
							if(pSockInfoTemp->pSendOverlapped->sendTolLength == 0)
							{
								LPRIM30MSG pMsgTemp = RIM30MSG::MakeMsg(pServer->m_serverGuid.c_str(), pSockInfoTemp->guid, pMsg->GetMsgID(), pMsg->GetDataType(), pMsg->GetDataBuf(), pMsg->GetDataLength());
								SendPack(pMsgTemp, pSockInfoTemp);
								RIM30MSG::FreeMsg(pMsgTemp);

								delete	pUserMsgInfo;	pUserMsgInfo = NULL;
							}
							pSockInfoTemp->sendLock.Unlock();							
							////非正常数据包，第一次处理后即要删除
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

					if(pUserMsgInfo)	/////说明还未发送
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

	SOCKET 			servSocket				= INVALID_SOCKET;	// 监听套接字
	SOCKET 			clientSocket			= INVALID_SOCKET;	// 与客户端进行通信的套接字
	LPSUBSOCKINFO	pSockInfo				= NULL;				// 套接字句柄结构体
	DWORD 			RecvBytes				= 0;				// 接收到的字节数
	DWORD 			Flags					= 0;				// WSARecv()函数中指定的标识位
	UINT 			ThreadID				= 0;				// 工作线程编号
	DWORD 			dwRet					= 0;				// 函数返回值
	SOCKADDR_IN		serverAddr;									// 服务器地址
	SYSTEM_INFO		SystemInfo;									// 获取系统信息（这里主要用于获取CPU数量）
	WSADATA			wsaData;									// Windows Socket初始化信息

	// 初始化Windows Sockets环境
	if ((dwRet = WSAStartup(0x0202, &wsaData)) != 0)
	{
		MessageBox(NULL, _T("RIM3.0 WSAStartup failed with error\n"), _T("RIM3.0 错误"), MB_OK);
		OutputDebugString(_T("RIM3.0 WSAStartup failed with error\n"));
		return -1;
	}
	// 创建新的完成端口
	if ((m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		MessageBox(NULL, _T("RIM3.0 CreateIoCompletionPort failed with error\n"), _T("RIM3.0 错误"), MB_OK);
		OutputDebugString(_T("RIM3.0 CreateIoCompletionPort failed with error\n"));
		return -1;
	}

	{	/////将作业与完成端口相关联
		BOOL bRet = FALSE;
		bRet = m_job.Create(NULL, RIM30MsgServer_JOBNAME);
		bRet = m_job.SetEndOfJobInfo(JOB_OBJECT_POST_AT_END_OF_JOB);
		if(FALSE == m_job.AssociateCompletionPort(m_completionPort, ((UINT_PTR)NULL)))
		{
			MessageBox(NULL, _T("RIM3.0 m_job.AssociateCompletionPort failed with error\n"), _T("RIM3.0 错误"), MB_OK);
			OutputDebugString(_T("RIM3.0 m_job.AssociateCompletionPort failed with error\n"));
			return -1;
		}
	}
	// 获取系统信息
	GetSystemInfo(&SystemInfo);   
	// 根据CPU数量启动线程
	for(UINT i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		HANDLE ThreadHandle;
		// 创建线程，运行ServerWorkerThread()函数            
		if ((ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, ServerWorkerThread, (LPVOID)this, 0, &ThreadID)) == NULL)
		{
			MessageBox(NULL, _T("RIM3.0 CreateThread() failed with error\n"), _T("RIM3.0 错误"), MB_OK);
			OutputDebugString(_T("RIM3.0 CreateThread() failed with error\n"));
			return -1;
		}      
		m_threadHandles[i] = ThreadHandle;
	}
	m_hSendDataThread = (HANDLE)_beginthreadex(NULL, 0, ThreadSendData, (LPVOID)this, 0, NULL);

	// 创建监听套接字
	if ((servSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("RIM3.0 WSASocket() failed with error\n"), _T("RIM3.0 错误"), MB_OK);
		OutputDebugString(_T("RIM3.0 WSASocket() failed with error\n"));
		return -1;
	}

	ULONG reuse = true;
	if(setsockopt( servSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CRIM30MsgTransServer SO_REUSEADDR 设置错误\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(servSocket);
		return -1;
	}

	// 绑定到本地地址的9990端口
	serverAddr.sin_family		= AF_INET;
	serverAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	serverAddr.sin_port			= htons(m_serverPort);
	
	int ret = bind(servSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret)
	{
		MessageBox(NULL, _T("CRIM30MsgTransServer 套接子绑定失败\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(servSocket);
		return -1;
	}

	//int nScktOpt = 5000;
	////设置接收超时
	//if(setsockopt(Listen, SOL_SOCKET, SO_RCVTIMEO, (char *)&nScktOpt, sizeof(nScktOpt)) == SOCKET_ERROR)
	//{
	//	MessageBox(NULL, _T("CRIM30MsgTransServer 设置接收超时\n"), _T("RIM3.0 错误"), MB_OK);
	//	closesocket(Listen);
	//	WSACleanup();
	//	return;
	//}
	////设置发送超时
	//nScktOpt = 5000;
	//if(setsockopt(Listen, SOL_SOCKET, SO_SNDTIMEO,(char *)&nScktOpt, sizeof(nScktOpt)))
	//{
	//	MessageBox(NULL, _T("CRIM30MsgTransServer 设置发送超时\n"), _T("RIM3.0 错误"), MB_OK);
	//	closesocket(Listen);
	//	return;
	//}

	if(SOCKET_ERROR == listen(servSocket, SOMAXCONN))
	{
		MessageBox(NULL, _T("CRIM30MsgTransServer 监听失败\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(servSocket);
		return -1;
	}

	//return -1;
	// 监听端口打开，就开始在这里循环，一有socket连上，WSAAccept就创建一个socket，
	// 这个socket 和完成端口联上
	while(servSocket != INVALID_SOCKET)
	{
		dwRet = WaitForSingleObject(m_hExit, 0);
		if(dwRet == WAIT_OBJECT_0)
			break;

		if(CRIM30MsgTransServer::SelectSocketRead(servSocket, 1) > 0)
		{
			// 等待客户连接
			if ((clientSocket = WSAAccept(servSocket, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
			{
				MessageBox(NULL, _T("CRIM30MsgTransServer 等待客户连接失败\n"), _T("RIM3.0 错误"), MB_OK);
				break;
			}

			{// 分配并设置套接字句柄结构体
				pSockInfo			= new SUBSOCKINFO;  
				pSockInfo->socket	= clientSocket;

				// 为I/O操作结构体分配内存空间
				pSockInfo->pRecvOverlapped->bRecvOrSend 	= TRUE;
				pSockInfo->pSendOverlapped->bRecvOrSend 	= FALSE;

				// 初始化I/O操作结构体
				ZeroMemory(&(pSockInfo->pSendOverlapped->overlapped), sizeof(OVERLAPPED));
				pSockInfo->pSendOverlapped->bytesSEND		= 0;
				pSockInfo->pSendOverlapped->bytesRECV		= 0;
				pSockInfo->pSendOverlapped->cacheBuf.len	= 0;						////一个BLOCK
				pSockInfo->pSendOverlapped->cacheBuf.buf	= pSockInfo->pSendOverlapped->buffer;
				pSockInfo->pSendOverlapped->sendTolLength	= 0;						// 发送数据总长度
				pSockInfo->pSendOverlapped->packSEND		= 0;						// 一个整包发送的字节数（一包数据可能未发完）

				// 初始化I/O操作结构体
				ZeroMemory(&(pSockInfo->pRecvOverlapped->overlapped), sizeof(OVERLAPPED));
				pSockInfo->pRecvOverlapped->bytesSEND		= 0;
				pSockInfo->pRecvOverlapped->bytesRECV		= 0;
				pSockInfo->pRecvOverlapped->cacheBuf.len	= sizeof(RIM30TCPBLOCK);	////要求接收一个BLOCK
				pSockInfo->pRecvOverlapped->cacheBuf.buf	= pSockInfo->pRecvOverlapped->buffer;

				m_socketInfolock.lock(CReadWriteLock::LOCK_LEVEL_WRITE);
				m_subSockInfoVec.push_back(pSockInfo);
				TCHAR ch[MAX_PATH] ={ 0 };
				_stprintf_s(ch, MAX_PATH, _T("RIM3.0 CRIM30MsgTransServer::接收到一个连接 socket:%d. 第%d个连接\n"), DWORD(clientSocket), m_subSockInfoVec.size());
				OutputDebugString(ch);
				m_socketInfolock.unlock();
			}

			// 将与客户端进行通信的套接字Accept与完成端口CompletionPort相关联
			if (CreateIoCompletionPort((HANDLE)pSockInfo->socket, m_completionPort, (DWORD)pSockInfo, 0) == NULL)
			{
				MessageBox(NULL, _T("RIM3.0 CreateIoCompletionPort 失败\n"), _T("RIM3.0 错误"), MB_OK);
				OutputDebugString(_T("RIM3.0 CreateIoCompletionPort failed with error\n"));
				break;
			}


			// 接收数据，放到PerIoData中，而perIoData又通过工作线程中的ServerWorkerThread函数取出,
			if (pSockInfo->socket != INVALID_SOCKET)
			{
				if (WSARecv(pSockInfo->socket, &(pSockInfo->pRecvOverlapped->cacheBuf), 1, &RecvBytes, &Flags, &(pSockInfo->pRecvOverlapped->overlapped), NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != ERROR_IO_PENDING)
					{
						pSockInfo->bDelete = true;
						pSockInfo->deleteInfo = "WSARecv";
						MessageBox(NULL, _T("WSARecv() failed with error\n"), _T("RIM3.0 错误"), MB_OK);
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

	{	////向所有子系统SOCKET发送关闭信息
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
			_stprintf_s(ch, MAX_PATH, _T("RIM3.0 CRIM30MsgTransServer::还有%d个本地进程没有关闭！"), m_subSockInfoVec.size());
			OutputDebugString(ch);
			break;
		}
	}
}

UINT CRIM30MsgTransServer::GetSubSystemCnt()
{
	/*现在所有子系统均在本地启动，个数为m_subProcessIDs.size()*/
	/*如果有子系统在远程启动，个数为m_subSockInfoVec.size()*/
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
