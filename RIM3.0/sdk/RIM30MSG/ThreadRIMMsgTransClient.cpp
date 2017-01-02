//********************************************************************
//	USERTHREAD.CPP 文件注释
//	文件名		: USERTHREAD.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\INCLUDE/
//	作者		: 张永
//	创建时间	: 2015/1/13 14:41
//	文件描述	: 
//*********************************************************************	


#include "stdafx.h"
#include "ThreadRIMMsgTransClient.h"

// CThreadRIMMsgTransClient message handlers


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRIM30MsgTransClient g_RIM30MsgTransClient;
IRIM30MsgClient * GetRIM30Client()
{
	return &g_RIM30MsgTransClient;
}

///////////////////////////////////////////////////////////////////////////CThreadRIM30MsgTransRecv//////////////////////////////////////////////
UINT CThreadRIM30MsgTransRecv::__ThreadProc(LPVOID lpParam)
{
	ASSERT(lpParam);
	CThreadRIM30MsgTransRecv *pThread = (CThreadRIM30MsgTransRecv*)lpParam;

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

CThreadRIM30MsgTransRecv::CThreadRIM30MsgTransRecv(void)
{
	m_pInfo			= NULL;
	m_pThread		= NULL;
	m_hExit			= NULL;
	m_bInitCom		= FALSE;
}

CThreadRIM30MsgTransRecv::~CThreadRIM30MsgTransRecv(void)
{
	End();
}

HRESULT CThreadRIM30MsgTransRecv::Begin(LPRIM30MSGTRANSCLIENTINFO pInfo, BOOL bInitCom)
{
	ASSERT(m_hExit == NULL && pInfo != NULL);

	m_pInfo = pInfo;

	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(m_hExit == NULL)
		return E_POINTER;

	m_bInitCom	= bInitCom;
	m_pThread	= AfxBeginThread(CThreadRIM30MsgTransRecv::__ThreadProc, LPVOID(this), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

	if(m_pThread == NULL)
		return E_POINTER;

	return NOERROR;
}

void CThreadRIM30MsgTransRecv::End()
{
	if(m_pThread && m_hExit)
	{
		SetEvent(m_hExit);
		// 		::PostThreadMessage(m_pThread->m_nThreadID, WM_QUIT, 0, 0);

		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		delete m_pThread;
		m_pThread = NULL;

		CloseHandle(m_hExit);
		m_hExit = NULL;
	}
}

BOOL CThreadRIM30MsgTransRecv::SetPriority(int nPriority)
{
	if(m_pThread)
		return m_pThread->SetThreadPriority(nPriority);

	return FALSE;
}

UINT CThreadRIM30MsgTransRecv::ThreadProc()
{
	CPing		ping;
	DWORD		dwRet		= 0;
	int			iRetVal 	= 0;
	int			idleCnt 	= 0;
	string  	strError	= "";


	const DWORD bufSize 	= DATA_BUFSIZE; //16K
	LPBYTE		pBuf		= new BYTE[bufSize];
	CMemFile	memFile(pBuf, bufSize, bufSize);
	RIM30TCPBLOCK bufBlock;
	memFile.SeekToBegin();

	while(TRUE)	 
	{
		dwRet = WaitForSingleObject(m_hExit, 0);
		if(dwRet == WAIT_OBJECT_0)
			break;

		if(m_pInfo->socket == INVALID_SOCKET)
			break;

		iRetVal = CRIM30MsgTransClient::SelectSocketRead(m_pInfo->socket, 100);
		if(iRetVal < 0 )
		{
			OutputDebugString(_T("RIM3.0 SelectSocketRead接收数据失败"));
			AfxMessageBox(_T("SelectSocketRead接收数据失败"));
			break;
		}
		else if(iRetVal == 0)
		{
			++idleCnt;

			if (idleCnt % 600 == 0)
			{	//60秒没收到数据
				OutputDebugString(_T("RIM3.0  1分钟内未探测到服务端, 连接断开"));
				AfxMessageBox(_T("RIM3.0  1分钟内未探测到服务端, 连接断开"));
				break;
			}
			else if(idleCnt%100 == 0)
			{	//空闲10S心跳包
				LPRIM30MSG pMsg = RIM30MSG::MakeMsg("", "", RIM30MSG_HEART_PACK, 0, NULL, 0);
				bool bRet = CRIM30MsgTransClient::SendTCPBufferByBlock(m_pInfo->socket, (LPBYTE)pMsg, pMsg->GetTotalLength());
				RIM30MSG::FreeMsg(pMsg);

				m_pInfo->m_cntHeartSend++;

				if(bRet == false) break;
			}
			else
				continue;
		}
		else
		{
			idleCnt = 0;  //非空闲了

			if(m_pInfo->tcpRecvQueue.GetQueueCount() < 5000)
			{
				if (TRUE == CRIM30MsgTransClient::ReciveTCPBufferBlock(m_pInfo->socket, bufBlock, 10000))
				{
					if (bufBlock._len)
					{
						memFile.Write(bufBlock._buf, bufBlock._len);

						if (bufBlock._len + bufBlock._offset != bufBlock._fileSize)	//没有收到文件结尾则继续接收数据
							continue;
					}
				}
				else
				{
					AfxMessageBox(_T("CThreadRIM30MsgTransRecv 套接字接收失败！"));
					memFile.SeekToBegin();
					break;
				}

				////处理接收来的数据
				DWORD receiveLen = UINT(memFile.Seek(0, CFile::current));
				if (receiveLen && ((bufBlock._len + bufBlock._offset) == bufBlock._fileSize))
				{
					LPRIM30MSG pDataTemp	= (LPRIM30MSG)pBuf;
					if (pDataTemp->GetMsgID() == RIM30MSG_HEART_PACK)
					{
						m_pInfo->m_cntHeartRecv++;
						////心跳包回应
					}
					else if(pDataTemp->GetTotalLength() <= receiveLen)	//防止非法数据包导致分配内存错误
					{
						LPRIM30MSG pData	= (LPRIM30MSG)malloc(pDataTemp->GetTotalLength());
						memcpy(pData, pBuf, pDataTemp->GetTotalLength());
						m_pInfo->tcpRecvQueue.Push(pData);	m_pInfo->m_cntMsgRecv++;
					}
				}
				else if (bufBlock._fileSize > 8192)
				{
					TCHAR ch[MAX_PATH] = { 0 };
					_stprintf_s(ch, MAX_PATH, "CThreadRIM30MsgTransRecv  过长的数据长度%d.", bufBlock._fileSize);
					AfxMessageBox(ch);
				}

				memFile.SeekToBegin();
			}
			else
			{
				OutputDebugString(_T("RIM3.0 警告！接收队列超过5000个请速度处理！"));
			}
		}
	}

	closesocket(m_pInfo->socket); m_pInfo->socket = INVALID_SOCKET;
	memFile.Close();
	if (pBuf)	{ delete[] pBuf;	pBuf = NULL; }

	return 0;
}

#ifdef DEBUG

CString	CThreadRIM30MsgTransRecv::GetName()
{
	return m_strName;
}

void CThreadRIM30MsgTransRecv::GetName(LPCTSTR lpName)
{
	m_strName = lpName;
}

#endif


///////////////////////////////////////////////////////////////////////////CThreadRIM30MsgTransSend//////////////////////////////////////////////
UINT CThreadRIM30MsgTransSend::__ThreadProc(LPVOID lpParam)
{
	ASSERT(lpParam);
	CThreadRIM30MsgTransSend *pThread = (CThreadRIM30MsgTransSend*)lpParam;

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

CThreadRIM30MsgTransSend::CThreadRIM30MsgTransSend(void)
{
	m_pInfo			= NULL;
	m_pThread		= NULL;
	m_hExit			= NULL;
	m_bInitCom		= FALSE;
}

CThreadRIM30MsgTransSend::~CThreadRIM30MsgTransSend(void)
{
	End();
}

HRESULT CThreadRIM30MsgTransSend::Begin(LPRIM30MSGTRANSCLIENTINFO pInfo, BOOL bInitCom /*= FALSE*/)
{
	ASSERT(m_hExit == NULL && pInfo != NULL);

	m_pInfo = pInfo;

	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(m_hExit == NULL)
		return E_POINTER;

	m_bInitCom	= bInitCom;
	m_pThread	= AfxBeginThread(CThreadRIM30MsgTransSend::__ThreadProc, LPVOID(this), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

	if(m_pThread == NULL)
		return E_POINTER;

	return NOERROR;
}

void CThreadRIM30MsgTransSend::End()
{
	if(m_pThread && m_hExit)
	{
		SetEvent(m_hExit);
		// 		::PostThreadMessage(m_pThread->m_nThreadID, WM_QUIT, 0, 0);

		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		delete m_pThread;
		m_pThread = NULL;

		CloseHandle(m_hExit);
		m_hExit = NULL;
	}
}

BOOL CThreadRIM30MsgTransSend::SetPriority(int nPriority)
{
	if(m_pThread)
		return m_pThread->SetThreadPriority(nPriority);

	return FALSE;
}

UINT CThreadRIM30MsgTransSend::ThreadProc()
{
	
	DWORD		dwRet		= 0;
	int			iRetVal 	= 0;

	while(TRUE)	 
	{
		dwRet = WaitForSingleObject(m_hExit, 0);
		if(dwRet == WAIT_OBJECT_0)
			break;

		if(m_pInfo->socket == INVALID_SOCKET)
			break;

		iRetVal = CRIM30MsgTransClient::SelectSocketWrite(m_pInfo->socket, 100);
		if(iRetVal < 0 )
		{
			OutputDebugString(_T("RIM3.0 CThreadRIM30MsgTransSend 当前SOCKET不可写！"));
			break;
		}
		else if(m_pInfo->tcpSendQueue.GetQueueCount())
		{
			LPRIM30MSG pData = NULL;
			m_pInfo->tcpSendQueue.Pop(&pData);
			if(pData)
				CRIM30MsgTransClient::SendTCPBufferByBlock(m_pInfo->socket, (LPBYTE)pData, pData->GetTotalLength());

			RIM30MSG::FreeMsg(pData);	m_pInfo->m_cntMsgSend++;
		}
		else
			Sleep(1);
	}

	closesocket(m_pInfo->socket); m_pInfo->socket = INVALID_SOCKET;

	return 0;
}

#ifdef DEBUG

CString	CThreadRIM30MsgTransSend::GetName()
{
	return m_strName;
}

void CThreadRIM30MsgTransSend::GetName(LPCTSTR lpName)
{
	m_strName = lpName;
}

#endif


///////////////////////////////////////////////////////////////////////////CRIM30MsgTransClient//////////////////////////////////////////////
CRIM30MsgTransClient::CRIM30MsgTransClient(void)
{

}

CRIM30MsgTransClient::~CRIM30MsgTransClient(void)
{
	End();
}

int CRIM30MsgTransClient::SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds)
{
	assert(sock != INVALID_SOCKET);
	TIMEVAL timeOut;
	timeOut.tv_sec	= 0;
	timeOut.tv_usec = dwMilliseconds * 1000;
	fd_set readFD;
	FD_ZERO(&readFD);
	FD_SET(sock, &readFD);
	int iRetVal = select(int(sock), &readFD, NULL, NULL, &timeOut);

	return iRetVal;
}

int CRIM30MsgTransClient::SelectSocketWrite(SOCKET sock, __in DWORD dwMilliseconds)
{
	assert(sock != INVALID_SOCKET);
	TIMEVAL timeOut;
	timeOut.tv_sec	= 0;
	timeOut.tv_usec = dwMilliseconds * 1000;
	fd_set writeFD;
	FD_ZERO(&writeFD);
	FD_SET(sock, &writeFD);
	int iRetVal = select(int(sock), NULL, &writeFD, NULL, &timeOut);

	return iRetVal;
}

bool CRIM30MsgTransClient::ReciveTCPBufferBlock(SOCKET sock, RIM30TCPBLOCK& bufBlock, __in DWORD dwMilliseconds)
{
	long nRcv = 0;
	memset(&bufBlock, 0, sizeof(RIM30TCPBLOCK));

	//nRcv = recv(sock, (char*)(&bufBlock), sizeof(RIM30TCPBLOCK), 0);

	char*	pbuf		= (char*)(&bufBlock);
	int		needReadLen = sizeof(RIM30TCPBLOCK);
	while (true)
	{
		if (CRIM30MsgTransClient::SelectSocketRead(sock, dwMilliseconds) > 0)
		{
			nRcv = recv(sock, pbuf, needReadLen, 0);
			if (nRcv > 0)
			{
				needReadLen -= nRcv;
				if (needReadLen > 0)
				{
					pbuf += nRcv;
					continue;
				}
				else
					return true;
			}
			else
				return false;
		}
		else
			return false;
	}
}

bool CRIM30MsgTransClient::SendTCPBufferByBlock(SOCKET sock, LPBYTE pBuffer, int bufLen)
{
	RIM30TCPBLOCK bufBlock;
	if (sock != INVALID_SOCKET)
	{
		LPBYTE		buffer			= pBuffer;
		int			len				= bufLen;
		const int	dataUnitlen		= RIM30TCPBLOCKDATALEN;
		if (len == 0)
			return false;

		int i = 0;
		while (true)
		{
			memset(&bufBlock, 0, sizeof(RIM30TCPBLOCK));
			bufBlock._fileSize	= bufLen;
			bufBlock._offset	= i * dataUnitlen;
			if (len > 0)
			{
				if (len < dataUnitlen)
				{
					bufBlock._len	= len;
					memcpy(bufBlock._buf, buffer + i * dataUnitlen, len);
					send(sock, (char*)(&bufBlock), sizeof(RIM30TCPBLOCK), 0);
					break;
				}
				else
				{
					bufBlock._len	= dataUnitlen;
					memcpy(bufBlock._buf, buffer + i * dataUnitlen, dataUnitlen);
					len -= dataUnitlen;
					i++;
					send(sock, (char*)(&bufBlock), sizeof(RIM30TCPBLOCK), 0);
				}
			}
			else
				break;
		}

		return true;
	}
	else
	{
		return false;
	}
}

HRESULT CRIM30MsgTransClient::Begin(string serverIp /*= "127.0.0.1"*/, short serverPort /*= 0*/)
{
	{
		// 初始化Windows Sockets环境
		DWORD	dwRet;										// 函数返回值
		WSADATA wsaData;									// Windows Socket初始化信息
		if ((dwRet = WSAStartup(0x0202, &wsaData)) != 0)
		{
			OutputDebugString(_T("RIM3.0 WSAStartup failed with error\n"));
			return S_FALSE;
		}

		m_info.serverIp		= serverIp;
		m_info.serverPort	= serverPort;
		m_info.socket		= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(m_info.socket == INVALID_SOCKET)
		{
			MessageBox(NULL, _T("RIM3.0 CRIM30MsgTransClient 套接子创建失败"), _T("错误"), MB_OK);
			return S_FALSE;
		}

		SOCKADDR_IN desAddr;
		desAddr.sin_family			= AF_INET;
		desAddr.sin_port			= htons(serverPort);
		desAddr.sin_addr.s_addr		= inet_addr(serverIp.c_str());
		int ret = connect(m_info.socket, (PSOCKADDR)&desAddr, sizeof(desAddr));
		if (ret)
		{
			OutputDebugString(_T("RIM3.0 CRIM30MsgTransClient 连接失败\n"));
			MessageBox(NULL, _T("RIM3.0 CRIM30MsgTransClient 连接失败"), _T("错误"), MB_OK);
			closesocket(m_info.socket);	m_info.socket = INVALID_SOCKET;
			return S_FALSE;
		}
	}

	m_threadRecv.Begin(&m_info);
	m_threadSend.Begin(&m_info);
	return S_OK;
}

void CRIM30MsgTransClient::End()
{
	m_threadRecv.End();
	m_threadSend.End();
}

bool CRIM30MsgTransClient::IsConnected()
{
	return m_info.socket == INVALID_SOCKET;
}

void CRIM30MsgTransClient::PushSendMsg(LPRIM30MSG pMsg)
{
	if(pMsg == NULL)
		return;

	m_info.tcpSendQueue.Push(pMsg);
}

void CRIM30MsgTransClient::PopRecvMsg(LPRIM30MSG* pMsg)
{
	if(m_info.tcpRecvQueue.GetQueueCount())
		m_info.tcpRecvQueue.Pop(pMsg);
	else
		*pMsg = NULL;
}
