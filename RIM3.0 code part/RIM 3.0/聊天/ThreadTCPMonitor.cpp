// ChatMonitorThread.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadTCPMonitor.h"
#include "NetData.h"
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

CThreadTCPMonitorRecive::CThreadTCPMonitorRecive()
{
	m_hChatMonitorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_hChatMonitorSocket == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive 套接子创建失败"), _T("RIM3.0 错误"), MB_OK);
		return;
	}

	ULONG reuse = true;
	if(setsockopt( m_hChatMonitorSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive SO_REUSEADDR 设置错误\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(TCP_CHATSERVER_PORT);
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	int ret = bind(m_hChatMonitorSocket, (sockaddr*)&addr, sizeof(addr));
	if(ret)
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive 套接子绑定失败\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	int nScktOpt = 5000;
	//设置接收超时
	if(setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nScktOpt, sizeof(nScktOpt)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive 设置接收超时\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		WSACleanup();
		return;
	}
	//设置发送超时
	nScktOpt = 5000;
	if(setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_SNDTIMEO,(char *)&nScktOpt, sizeof(nScktOpt)))
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive 设置发送超时\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	if(SOCKET_ERROR == listen(m_hChatMonitorSocket, SOMAXCONN))
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive 监听失败\n"), _T("RIM3.0 错误"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

}

CThreadTCPMonitorRecive::~CThreadTCPMonitorRecive()
{

}


UINT __stdcall CThreadTCPMonitorRecive::ThreadSrvCnnct(void *pDt)
{
	if(pDt == NULL)
		return 0;

	LPBYTE		pBuffer		= (LPBYTE)pDt;
	SOCKADDR_IN fromAddr;
	SOCKET		sockCnn		= INVALID_SOCKET;

	memcpy(&fromAddr,	pBuffer, sizeof(SOCKADDR_IN));
	memcpy(&sockCnn,	pBuffer + sizeof(SOCKADDR_IN), sizeof(SOCKET));


	const DWORD bufSize 	= 0x2000000; //32M
	LPBYTE		pBuf		= new BYTE[bufSize];
	CMemFile	memFile(pBuf, bufSize, bufSize);
	memFile.SeekToBegin();

	while (sockCnn != INVALID_SOCKET)
	{	//仅用于收全一次发送的TCP数据包

		int retRead = 0;

		retRead = SelectSocketRead(sockCnn, 10);
		if (retRead < 0)
		{
			break;
		}
		else if (retRead == 0)	//	zero if the time limit expired
		{
			::Sleep(20);
			continue;
		}

		BUFBLOCK bufBlock;
		if (TRUE == ReciveTCPBufferBlock(sockCnn, bufBlock, 10000))
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
			AfxMessageBox(_T("CThreadTCPMonitorRecive 套接字接收失败！"));
			memFile.SeekToBegin();
			break;
		}

		//正常运行下来
		//DWORD receiveLen = UINT(memFile.Seek(0, CFile::current));
		//if(receiveLen)
		//{
		//	{/////解压缩数据
		//		if (HZIP hz = OpenZip(pBuf, receiveLen, ""))
		//		{
		//			ZIPENTRY ze ={ 0 };
		//			if (ZR_OK == GetZipItem(hz, -1, &ze))
		//			{
		//				int numitems = ze.index;
		//				for (int i = 0; i < numitems; i++)
		//				{
		//					if (ZR_OK == GetZipItem(hz, i, &ze))
		//					{
		//						if (ZR_OK == UnzipItem(hz, i, ze.name))
		//						{
		//							TCHAR ch[MAX_PATH] ={ 0 };
		//							GetCurrentDirectory(MAX_PATH, ch);
		//							tstring filePath = tstring(ch) + _T("\\") + ze.name;

		//							if (IsFileExist(filePath.c_str()))
		//							{
		//								CFile cFile(filePath.c_str(), CFile::modeRead);
		//								cFile.SeekToBegin();
		//								const DWORD	fileLength	= DWORD(cFile.GetLength()); 
		//								LPBYTE		lpBuffer	= new BYTE[fileLength];
		//								cFile.Read(lpBuffer, fileLength);
		//								{
		//									SOCKADDR_IN sockaddr;
		//									int len = sizeof(SOCKADDR_IN);
		//									getsockname(sockCnn, (LPSOCKADDR)&sockaddr, &len);
		//									CNetData *pNetData = new CNetData();
		//									pNetData->ParseNETBuffer(lpBuffer, fileLength, (LPSOCKADDR)&sockaddr);

		//									bool bNeedDelete = true;
		//									if (pNetData->GetBufferLen())
		//									{
		//										switch (pNetData->GetDataType())
		//										{
		//											case CNetData::TCP_FILEASKSEND:
		//												ReceiveFile(sockCnn, pNetData);
		//												bNeedDelete = true;
		//												break;
		//											default:
		//												RIMConfig::GetInstance()->m_TCPReceiveQueue.Push(pNetData);
		//												bNeedDelete = false;
		//												break;
		//										}
		//									}

		//									if (bNeedDelete)	{ delete pNetData;	pNetData = NULL; }
		//								}

		//								if (lpBuffer)	{ delete[] lpBuffer;	lpBuffer = NULL; }
		//								cFile.Close();
		//								::DeleteFile(filePath.c_str());
		//							}
		//						}
		//					}
		//				}
		//			}

		//			if (hz != NULL)	{ CloseZip(hz);	hz = NULL; }
		//		}
		//}
		DWORD receiveLen = UINT(memFile.Seek(0, CFile::current));
		if (receiveLen)
		{
			CNetData *pNetData = new CNetData();
			pNetData->ParseNETBuffer(pBuf, receiveLen, (LPSOCKADDR)&fromAddr);

			bool bNeedDelete = true;
			if (pNetData->GetBufferLen())
			{
				switch (pNetData->GetDataType())
				{
					case CNetData::TCP_FILEASKSEND:
						ReceiveFile(sockCnn, pNetData);
						bNeedDelete = true;
						break;
					default:
						RIMConfig::GetInstance()->m_TCPReceiveQueue.Push(pNetData);
						bNeedDelete = false;
						break;
				}
			}

			if (bNeedDelete)
				delete pNetData;
		}

		break;
	}

	closesocket(sockCnn);
	memFile.Close();
	if (pBuf)	{ delete[] pBuf;	pBuf = NULL; }
	if (pBuffer){ delete[] pBuffer;	pBuffer = NULL; }

	return true;
}

void CThreadTCPMonitorRecive::ReceiveFile(SOCKET &sockCnn, CQueueData *pQueueData)
{
	CNetData *pNetData = (CNetData*)pQueueData;
	switch(pNetData->GetDataType())
	{
	case CNetData::TCP_FILEASKSEND:	//接收
		{
			LPUSERINFO lpSourceUserInfo = NULL;
			for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
			{
				LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];
				if(	//pNetData->GetSourceID() == lpUserInfo->userID									&&	////匿名用户的ID是0XFFFFFFFF
					pNetData->GetDesTypeID() == RIMConfig::GetInstance()->m_pLogonUser->userTypeID			&&
					_tcscmp(pNetData->GetSourceID().c_str(), lpUserInfo->UserID.c_str()) == 0
					)
				{	//先判定哪个用户发过来的 并且仅是发给我的
					lpSourceUserInfo = lpUserInfo;
					break;
				}
			}

			if(lpSourceUserInfo == NULL)
				break;

			LPTRANSFILEINFO pTransFileInfo = new TRANSFILEINFO();
			string str((char*)pNetData->GetBuffer(), pNetData->GetBufferLen());
			if(false == TRANSFILEINFO::LoadFromXmlStr(str, pTransFileInfo))
				break;

			PlaySound(RelativeToFullPath(VEDIO_RELATIVEPATH_MSG).c_str(), NULL, SND_FILENAME | SND_ASYNC);
			lpSourceUserInfo->receiveFileQueue.Push(pTransFileInfo);

			//////托盘图标通知消息  2代表聊天消息  3代表文件消息
			AfxGetApp()->m_pMainWnd->PostMessage(WM_FLICKERNOTIFYICON, 3, (LPARAM)lpSourceUserInfo);

			BUFBLOCK	bufBlock;
			clock_t		clr0					= 0;
			const int	constBufLen				= 0x1000000;
			char		*pFileBuffer			= new char[constBufLen + BUFBLOCKUNITLEN];
			HANDLE		hUpdateFile				= INVALID_HANDLE_VALUE;
			clock_t 	beginRateClr			= 0;
			BOOL		bCircle = TRUE;
			while(bCircle)
			{
				switch (pTransFileInfo->GetTransFlag())
				{
					case tagTRANSFILEINFO::TRANS_FLAG_PRETRANS:
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSBEGIN:
					{
						if (ReciveTCPBufferBlock(sockCnn, bufBlock, 0) && NULL != strstr((char*)(bufBlock._buf), pTransFileInfo->GetCancelCmd().c_str()))
						{////传输过程中如果对方取消
							pTransFileInfo->transError = _T("对方取消了文件传输,文件传输失败。");
							pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							break;
						}
						Sleep(1);
					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSING:
					{
						beginRateClr = clock();

						/////****************************************发送开始命令

						if (SendTCPBufferByBlock(sockCnn, (LPBYTE)(pTransFileInfo->GetBeginCmd().c_str()), pTransFileInfo->GetBeginCmd().size()) == false)
						{
							pTransFileInfo->transError = _T("文件传输失败。");
							pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//结束状态
							break;
						}

						///////第一个文件请求发送完毕, 开始轮循请求接收文件列表
						FILEINFO	fileInfo;
						char		*pChar			= NULL;
						DWORD		writeLen		= 0;
						DWORD		bufLen			= 0;
						DWORD		buferOff		= 0;
						while (TRUE)
						{
							if (pTransFileInfo->GetTransFlag() == tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL)
								break;

							//更新文件
							///////***********************************开始更新文件
							int retRead = SelectSocketRead(sockCnn, 10);
							if (retRead < 0)
							{
								pTransFileInfo->transError = _T("文件传输失败。");
								pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//结束状态
								break;
							}
							else if (retRead == 0)	//	zero if the time limit expired
							{
								::Sleep(20);
								continue;
							}

							if (TRUE == ReciveTCPBufferBlock(sockCnn, bufBlock, 100))
							{
								if (bufBlock._offset == 0 && (bufBlock._fileSize == bufBlock._len) && NULL != strstr((char*)(bufBlock._buf), pTransFileInfo->GetCancelCmd().c_str()))
								{////传输过程中如果对方取消
									pTransFileInfo->transError = _T("对方取消了文件传输,文件传输失败。");
									pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
									break;
								}
								else if (bufBlock._offset == 0 && (bufBlock._fileSize == bufBlock._len) && NULL != strstr((char*)(bufBlock._buf), pTransFileInfo->GetEndCmd().c_str()))
								{//文件传输成功完成
									pTransFileInfo->transError = _T("文件传输成功。");
									pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
									break;
								}
								else if (bufBlock._offset == 0 && (bufBlock._fileSize == bufBlock._len) && NULL != (pChar = strstr((char*)(bufBlock._buf), pTransFileInfo->GetNameBaseCmd().c_str())))
								{	///要发送新文件
									if (SendTCPBufferByBlock(sockCnn, bufBlock._buf, bufBlock._len) == false)
									{	//////以原指令作为应答
										pTransFileInfo->transError = _T("文件传输失败。");
										pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//结束状态
										break;
									}

									char *pCharPos		= NULL;
									char *pCharTemp     = strtok_s(pChar, (","), &pCharPos);
									while (pCharTemp)
									{
										char *pChar = NULL;
										string::size_type copySize    = 0;
										int subSize                    = 0;
										string subStr(pCharTemp);
										const char findStr[][20] ={ "NAME=", "FILESIZE="};
										if (pChar = strstr(pCharTemp, findStr[0]))
										{
											subStr        = pChar;
											subSize        = strlen(findStr[0]);
											subStr        = subStr.substr(subSize, subStr.size() - subSize);

											fileInfo.name = CA2T(subStr.c_str());
										}
										else if (pChar = strstr(pCharTemp, findStr[1]))
										{
											subStr        = pChar;
											subSize        = strlen(findStr[1]);
											subStr        = subStr.substr(subSize, subStr.size() - subSize);

											fileInfo.size = atoi(subStr.c_str());
										}
				
										pCharTemp = strtok_s(NULL, ",", &pCharPos);
									}

									pChar += strlen(pTransFileInfo->GetNameBaseCmd().c_str());

									tstring filePath0 = _T("");
									if (pTransFileInfo->IsDic())
										filePath0 = pTransFileInfo->GetApplicationName() + _T("\\") + fileInfo.name;
									else
										filePath0 = pTransFileInfo->GetApplicationName();


									if (hUpdateFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hUpdateFile);	hUpdateFile = INVALID_HANDLE_VALUE; }
									TCHAR szPath1[MAX_PATH];																			//存放选择的目录路径 
									_tfullpath(szPath1, filePath0.c_str(), 256);
									MakeSureDirectoryPathExists(CT2A(szPath1));
									hUpdateFile = ::CreateFile(szPath1, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
									memset(pFileBuffer, 0, constBufLen + BUFBLOCKUNITLEN);

									clr0 = clock();
									continue;
								}

								if (bufBlock._len == 0)
									ASSERT(0);

								////【zy 2015-01-29 ↓】计算进度
								pTransFileInfo->transBytesTemp += bufBlock._len;
								if (pTransFileInfo->transBytesTemp > 0)
								{
									clock_t span = clock() - beginRateClr;
									lldiv_tRIM dt	= lldiv_RIM(pTransFileInfo->transBytesTemp * 1000, LONGLONG(span?span:1));
									pTransFileInfo->rateTemp = dt.quot;
								}

								memcpy(pFileBuffer + buferOff, bufBlock._buf, bufBlock._len);
								buferOff += bufBlock._len;

								if (buferOff >= constBufLen) //bufBlock最大发送数据长度为1024
								{	//将所接收数据写入文件中
									WriteFile(hUpdateFile, pFileBuffer, buferOff, &writeLen, NULL);
									if (buferOff != writeLen) ASSERT(0); buferOff = 0;
								}

								if ((bufBlock._len + bufBlock._offset) != bufBlock._fileSize)							//没有收到文件结尾则继续接收数据
									continue;
								else
								{	//文件关闭时再写一次文件
									if (buferOff)
									{
										WriteFile(hUpdateFile, pFileBuffer, buferOff, &writeLen, NULL);
										if (buferOff != writeLen) ASSERT(0); buferOff = 0;
									}

									//if (fileInfo.size == 1426432)
										{
											TRACE(_T("接收：写入完毕%s:%d字节， 用时：%d ms 时间：%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());
										}
									if (hUpdateFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hUpdateFile);	hUpdateFile = INVALID_HANDLE_VALUE; }
									//if (fileInfo.size == 1426432)
										{
											TRACE(_T("接收：关闭够柄%s:%d字节， 用时：%d ms 时间：%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());
										}
									continue;
								}
							}
							else
							{
								pTransFileInfo->transError = _T("文件接收失败，文件传输失败。");
								pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);		//结束状态
								break;
							}
						}
						TRACE(_T("接收：%s:%d字节， 用时：%d ms 时间：%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());

					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL:
					{
						TRACE(_T("撤消指令时间：%d ms\n"), clock());
						////【zy 2015-01-29 ↓】 本地用户手动取消传输
						SendTCPBufferByBlock(sockCnn, (LPBYTE)pTransFileInfo->GetCancelCmd().c_str(), pTransFileInfo->GetCancelCmd().size());
						if (pTransFileInfo->transError.size() == 0)
							pTransFileInfo->transError = _T("用户取消了文件传输,文件传输失败。");

						pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND:
					{
						TRACE(_T("结束指令时间：%d ms\n"), clock());
						if (pTransFileInfo->transError.size() == 0)
							pTransFileInfo->transError = _T("文件传输失败。");

						pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSEND);
						bCircle = FALSE;	///中断循环
					}
						break;
				}
			}

			Sleep(50);
			if (hUpdateFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hUpdateFile);	hUpdateFile = INVALID_HANDLE_VALUE; }
			if (pFileBuffer)	{ delete[] pFileBuffer; pFileBuffer = NULL; }

		}
	}
}

UINT CThreadTCPMonitorRecive::ThreadProc()
{
	TRACE(_T("%s %d \n"), _T("CThreadTCPMonitorRecive"), GetCurrentThreadId());

	SOCKADDR_IN fromAddr;
	int addrLen = sizeof(SOCKADDR_IN);

	DWORD dwRet = 0;
	while(m_hChatMonitorSocket != INVALID_SOCKET)
	{
		///第一个句柄代表队列里面有数据
		dwRet = WaitForSingleObject(m_hExit, 0);
		if(dwRet == WAIT_OBJECT_0)
			break;

		SOCKET sockCnn = INVALID_SOCKET;

		if(SelectSocketRead(m_hChatMonitorSocket, 0) > 0 && RIMConfig::GetInstance()->m_pLogonUser)
		{
			/////用SelectSocket成功避免了accept的阻塞
			if((sockCnn = accept(m_hChatMonitorSocket, (sockaddr FAR*)&fromAddr, &addrLen)) == INVALID_SOCKET)
			{
				AfxMessageBox(_T("CThreadTCPMonitorRecive 套接字accept失败！"));
				closesocket(m_hChatMonitorSocket);
				WSACleanup();
				break;
			}
			else{
				//接收到一个新的连接, 进入连接线程//此处开启连接
				LPBYTE pBuffer = new BYTE[sizeof(SOCKADDR_IN) + sizeof(SOCKET)];
				memcpy(pBuffer, &fromAddr, sizeof(SOCKADDR_IN));
				memcpy(pBuffer + sizeof(SOCKADDR_IN), &sockCnn, sizeof(SOCKET));

				HANDLE hThrdCnn = (HANDLE)_beginthreadex(NULL, 0, ThreadSrvCnnct, (LPVOID)pBuffer, 0, NULL);
				CloseHandle(hThrdCnn);
				Sleep(10);
			}
		}
		else
		{
			Sleep(1);
		}
	}

	closesocket(m_hChatMonitorSocket);

	return 0;
}

CThreadTCPMonitorRecive* CThreadTCPMonitorRecive::m_pChatMonitorReciveThread = NULL;
CThreadTCPMonitorRecive* CThreadTCPMonitorRecive::GetInstance()
{
	if(m_pChatMonitorReciveThread == NULL)
	{
		m_pChatMonitorReciveThread = new CThreadTCPMonitorRecive();
	}

	return m_pChatMonitorReciveThread;
}

void CThreadTCPMonitorRecive::ReleaseInstance()
{
	if(m_pChatMonitorReciveThread)
	{
		delete m_pChatMonitorReciveThread;
		m_pChatMonitorReciveThread = NULL;
	}
}



char * CThreadTCPMonitorSend::m_pSendFileBuffer = NULL;
CThreadTCPMonitorSend::CThreadTCPMonitorSend()
{
	m_chatMonitorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_chatMonitorSocket == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("CThreadTCPMonitorSend 套接子创建失败"), _T("RIM3.0 错误"), MB_OK);
		return;
	}
}

CThreadTCPMonitorSend::~CThreadTCPMonitorSend()
{
}


void CThreadTCPMonitorSend::SendFile(tstring dircOrPath, SOCKET &chatMonitorSocket, LPVOID pData, clock_t beginRateClr/* = 0*/)
{
	if (chatMonitorSocket == INVALID_SOCKET || pData == NULL)
		return;

	CFileNetData *pFileNetData = (CFileNetData*)pData;

	// 开始查找
	TCHAR curPath[MAX_PATH] ={ 0 };
	TCHAR filePath[MAX_PATH] ={ 0 };
	if (dircOrPath.empty())
		return;

	if (IsFileExist(dircOrPath.c_str()))
	{
		FILEINFO fileInfo;
		HANDLE hFile = CreateFile(dircOrPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			DWORD dwFileSize = GetFileSize(hFile, NULL);
			CloseHandle(hFile);
			if (dwFileSize != 0xFFFFFFFF && dwFileSize != 0)
			{
				if (pFileNetData->m_pTransfileInfoTemp->IsDic())
				{
					TCHAR	relativePath[MAX_PATH]	={ 0 };
					PathRelativePathTo(relativePath, pFileNetData->m_pTransfileInfoTemp->GetApplicationName().c_str(), FILE_ATTRIBUTE_DIRECTORY, dircOrPath.c_str(), FILE_ATTRIBUTE_NORMAL);

					fileInfo.name 	= relativePath;
				}
				else
					fileInfo.name 	= PathFindFileName(pFileNetData->m_pTransfileInfoTemp->GetApplicationName().c_str());
				fileInfo.size 	= dwFileSize;
			}
		}

		if (fileInfo.name.size())
		{
			char sendStr[1024] ={ 0 };

			string formatStr = pFileNetData->m_pTransfileInfoTemp->GetNameBaseCmd() + string(CT2A(fileInfo.name.c_str())) + ",FILESIZE=%d,";
			sprintf_s(sendStr, 1024, formatStr.c_str(), fileInfo.size);

			if (SendTCPBufferByBlock(chatMonitorSocket, (LPBYTE)sendStr, strlen(sendStr)) == false)
			{
				pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输失败。");
				pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//结束状态
				return;
			}

			//TCHAR		cmdStr[] ={ 0 };
			BUFBLOCK	bufBlock;
			int			retRead			= 0;
			char		*pchar			= NULL;
			HANDLE		hFile			= INVALID_HANDLE_VALUE;
			ZeroMemory(m_pSendFileBuffer, m_constBufLen + BUFBLOCKUNITLEN);

			while (true)
			{

				retRead = SelectSocketRead(chatMonitorSocket, 100);
				if (retRead < 0)
				{
					pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输失败。");
					closesocket(chatMonitorSocket);
					break;
				}
				else if (retRead == 0)
				{
					::Sleep(10);
					continue;
				}

				////【zy 2015-01-29 ↓】 //套接字可读说明有请求文件或用户取消了操作
				if (ReciveTCPBufferBlock(chatMonitorSocket, bufBlock, 3000) == false)
				{
					pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输失败.。");
					closesocket(chatMonitorSocket);
					break;
				}

				pchar	= NULL;
				////如果对方取消
				if (pchar = strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().c_str()))
				{//用户取消了文件传输
					pFileNetData->m_pTransfileInfoTemp->transError = _T("对方取消了文件传输,文件传输失败。");
					pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					closesocket(chatMonitorSocket);
					break;
				}
				else if (pchar = strstr((char*)(bufBlock._buf), sendStr))
				{
					clock_t clr0 = clock();

					////【zy 2015-01-29 ↓】 找到文件后开始传输文件
					hFile = ::CreateFile(dircOrPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile != INVALID_HANDLE_VALUE)
					{

						LARGE_INTEGER	liOff;
						liOff.QuadPart			= 0;

						DWORD	fileOff			= 0;
						BOOL	bOutBreak		= FALSE;
						DWORD	readLen 		= 0;
						memset(m_pSendFileBuffer, 0, m_constBufLen + BUFBLOCKUNITLEN);
						while (bOutBreak == FALSE && ::ReadFile(hFile, m_pSendFileBuffer, m_constBufLen, &readLen, NULL) && readLen)
						{
							////【zy 2015-01-29 ↓】 兼容发送大文件
							LPBYTE	buffer	= (LPBYTE)m_pSendFileBuffer;
							int		len		= readLen;
							int		i		= 0; 
							while (true)
							{
								memset(&bufBlock, 0, sizeof(BUFBLOCK));
								bufBlock._fileSize	= fileInfo.size;
								bufBlock._offset	= fileOff + i * BUFBLOCKUNITLEN;
								if (len > 0)
								{
									if (len < BUFBLOCKUNITLEN)
									{
										bufBlock._len	= len;
										memcpy(bufBlock._buf, buffer + i * BUFBLOCKUNITLEN, len);
										send(chatMonitorSocket, (char*)(&bufBlock), sizeof(BUFBLOCK), 0);

										////【zy 2015-01-29 ↓】计算进度
										pFileNetData->m_pTransfileInfoTemp->transBytesTemp += len;
										break;
									}
									else
									{
										bufBlock._len	= BUFBLOCKUNITLEN;
										memcpy(bufBlock._buf, buffer + i * BUFBLOCKUNITLEN, BUFBLOCKUNITLEN);
										len -= BUFBLOCKUNITLEN;
										i++;
										send(chatMonitorSocket, (char*)(&bufBlock), sizeof(BUFBLOCK), 0);
										////【zy 2015-01-29 ↓】计算进度
										pFileNetData->m_pTransfileInfoTemp->transBytesTemp += BUFBLOCKUNITLEN;
									}

									{
										////【zy 2015-01-29 ↓】 本地用户手动取消传输
										if (pFileNetData->m_pTransfileInfoTemp->GetTransFlag() == tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL)
										{
											bOutBreak = TRUE; break;
										}
										else if (ReciveTCPBufferBlock(chatMonitorSocket, bufBlock, 0) && (bufBlock._offset == 0) && (bufBlock._fileSize == bufBlock._len) && NULL != strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().c_str()))
										{////传输过程中如果对方取消
											pFileNetData->m_pTransfileInfoTemp->transError = _T("对方取消了文件传输,文件传输失败。");
											pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
											bOutBreak = TRUE; break;
										}


										////【zy 2015-01-29 ↓】计算进度
										clock_t span = clock() - beginRateClr;
										lldiv_tRIM dt	= lldiv_RIM(pFileNetData->m_pTransfileInfoTemp->transBytesTemp * 1000, LONGLONG(span?span:1));
										pFileNetData->m_pTransfileInfoTemp->rateTemp = dt.quot;
										//pFileNetData->m_pTransfileInfoTemp->rateTemp		= MulDiv(pFileNetData->m_pTransfileInfoTemp->transBytesTemp, 1000, clock() - beginClr);
									}

								}
								else
									break;
							}
							////【zy 2015-01-29 ↑】

							//计算文件当前的偏移
							fileOff = ::SetFilePointer(hFile, liOff.LowPart, &liOff.HighPart, (DWORD)CFile::current);
							if (fileOff == fileInfo.size)
								break;
						}
						TRACE(_T("发送：%s:%d字节， 用时：%d ms 时间：%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());
						if (hFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hFile);	hFile = INVALID_HANDLE_VALUE; }
						break;	//// 一次只发送一个文件，文件发送完毕就跳出循环
					}
					else if (GetLastError() == ERROR_FILE_NOT_FOUND)
					{
						pFileNetData->m_pTransfileInfoTemp->transError = _T("文件不存在,文件传输失败。");
						pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
						break;
					}
				}
				else
				{
					pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输失败。");
					pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					break;
				}
			}
			if (hFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hFile);	hFile = INVALID_HANDLE_VALUE; }
		}

		return;
	}
	else if (IsDicExist(dircOrPath.c_str()))
	{
		dircOrPath += _T("\\");
		_tcscpy_s(curPath, MAX_PATH, dircOrPath.c_str());

		_tcscpy_s(filePath, MAX_PATH, curPath);
		_tcscat_s(filePath, MAX_PATH, _T("*.*"));
	}
	else
		return;

	//////查找文件并发送
	WIN32_FIND_DATA findFileData;
	HANDLE hFind=::FindFirstFile(filePath, &findFileData);
	while (hFind != INVALID_HANDLE_VALUE && pFileNetData->m_pTransfileInfoTemp->GetTransFlag() == tagTRANSFILEINFO::TRANS_FLAG_TRANSING)
	{
		if (findFileData.dwFileAttributes == INVALID_FILE_ATTRIBUTES)
			continue;
		else if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_tcscmp(findFileData.cFileName, _T(".")) != 0 && _tcscmp(findFileData.cFileName, _T("..")) != 0)
			{
				tstring dicPath = tstring(curPath) + findFileData.cFileName;
				SendFile(dicPath, chatMonitorSocket, pFileNetData, beginRateClr);
			}
		}
		else
		{

			tstring fileName = findFileData.cFileName;
			tstring desPath	=  tstring(curPath) + findFileData.cFileName;
			SendFile(desPath, chatMonitorSocket, pFileNetData, beginRateClr);
		}

		if (!FindNextFile(hFind, &findFileData))
			break;
	}

	::FindClose(hFind);
}

UINT __stdcall CThreadTCPMonitorSend::ThreadSendData(void *pDt)
{
	CNetData *pNetData = (CNetData *)pDt;

	switch (pNetData->GetDataType())
	{
		case CNetData::TCP_CHATMSG:
		{
			SOCKET chatMonitorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (chatMonitorSocket == INVALID_SOCKET)
			{
				MessageBox(NULL, _T("CThreadTCPMonitorSend 套接子创建失败"), _T("RIM3.0 错误"), MB_OK);
				break;
			}

			SOCKADDR_IN desAddr;
			desAddr.sin_family			= AF_INET;
			desAddr.sin_port			= htons(pNetData->GetFromOrToPort());
			desAddr.sin_addr.s_addr		= inet_addr(CT2A(pNetData->GetFromOrToIP().c_str()));
			int ret = connect(chatMonitorSocket, (PSOCKADDR)&desAddr, sizeof(desAddr));
			if (ret)
			{
				OutputDebugString(_T("RIM3.0 CThreadTCPMonitorSend 连接失败\n"));
				closesocket(chatMonitorSocket);
				break;
			}
			SendTCPBufferByBlock(chatMonitorSocket, pNetData->GetBuffer(), pNetData->GetBufferLen());

			//{///压缩数据

			//	int memFileLen = pNetData->GetBufferLen();
			//	LPBYTE pBufferTemp = new BYTE[pNetData->GetBufferLen() + 5];

			//	////////////////////////////对数据进行压缩
			//	if (HZIP hz = CreateZip(pBufferTemp, memFileLen + 5, ""))
			//	{
			//		tstring newGuidStr = GetNewGuid() + _T(".txt");
			//		if (ZR_OK == ZipAdd(hz, newGuidStr.c_str(), pNetData->GetBuffer(), pNetData->GetBufferLen()))
			//		{
			//			LPBYTE	pZipBuffer	= NULL;
			//			DWORD	destlen		= 0;
			//			ZipGetMemory(hz, (void**)&pZipBuffer, &destlen);
			//			if (pZipBuffer && destlen)
			//			{
			//				SendTCPBufferByBlock(chatMonitorSocket, pZipBuffer, destlen);
			//			}
			//		}

			//		if (hz != NULL)	{ CloseZip(hz);	hz = NULL; }
			//	}
			//	if (pBufferTemp)
			//		delete[] pBufferTemp;
			//}

			closesocket(chatMonitorSocket);
		}
			break;

		case CNetData::TCP_FILEASKSEND:	//发送
		{
			CFileNetData *pFileNetData = (CFileNetData*)pNetData;
			if (pFileNetData->GetBufferLen() == 0 || NULL == pFileNetData->m_pTransfileInfoTemp)
				break;

			SOCKET chatMonitorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (chatMonitorSocket == INVALID_SOCKET)
			{
				MessageBox(NULL, _T("CThreadTCPMonitorSend 套接子创建失败"), _T("RIM3.0 错误"), MB_OK);
				break;
			}

			SOCKADDR_IN desAddr;
			desAddr.sin_family			= AF_INET;
			desAddr.sin_port			= htons(pFileNetData->GetFromOrToPort());
			desAddr.sin_addr.s_addr		= inet_addr(CT2A(pFileNetData->GetFromOrToIP().c_str()));
			int ret = connect(chatMonitorSocket, (PSOCKADDR)&desAddr, sizeof(desAddr));
			if (ret)
			{
				OutputDebugString(_T("RIM3.0 CThreadTCPMonitorSend 连接失败\n"));
				pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSEND);			//结束状态
				pFileNetData->m_pTransfileInfoTemp->transError = _T("用户可能不在线，文件传输失败。");
				closesocket(chatMonitorSocket);
				break;
			}

			BUFBLOCK	bufBlock;
			int			retRead			= 0;
			clock_t		beginClr		= 0;
			char		*pchar			= NULL;
			const int	constBufLen		= 0x1000000;
			char		*pFileBuffer	= new char[constBufLen + BUFBLOCKUNITLEN];
			HANDLE		hFile			= INVALID_HANDLE_VALUE;


			BOOL bCircle = TRUE;
			while (bCircle)
			{
				switch (pFileNetData->m_pTransfileInfoTemp->GetTransFlag())
				{
					case tagTRANSFILEINFO::TRANS_FLAG_PRETRANS:
						Sleep(1);
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSBEGIN:

						//////【zy 2015-01-29 ↓】 //首先发关文件列表XML，
						if (false == SendTCPBufferByBlock(chatMonitorSocket, pFileNetData->GetBuffer(), pFileNetData->GetBufferLen()))
						{
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							pFileNetData->m_pTransfileInfoTemp->transError = _T("文件列表发送失败，文件传输失败。");
							closesocket(chatMonitorSocket);
							break;
						}

						pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSING);
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSING:
					{
						retRead = SelectSocketRead(chatMonitorSocket, 100);
						if (retRead < 0)
						{
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输失败。");
							closesocket(chatMonitorSocket);
							break;
						}
						else if (retRead == 0)
						{
							::Sleep(10);
							break;
						}

						////【zy 2015-01-29 ↓】 //套接字可读说明有请求文件或用户取消了操作
						if (ReciveTCPBufferBlock(chatMonitorSocket, bufBlock, 3000) == false)
						{
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输失败.。");
							closesocket(chatMonitorSocket);
							break;
						}

						pchar	= NULL;
						if (pchar = strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetEndCmd().c_str()))
						{//文件传输成功完成
							pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输成功。");
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							closesocket(chatMonitorSocket);
							break;
						}
						////如果对方取消
						else if (pchar = strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().c_str()))
						{//用户取消了文件传输
							pFileNetData->m_pTransfileInfoTemp->transError = _T("对方取消了文件传输,文件传输失败。");
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							closesocket(chatMonitorSocket);
							break;
						}
						else if (pchar = strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetBeginCmd().c_str()))
						{
							beginClr = clock();
							SendFile(pFileNetData->m_pTransfileInfoTemp->GetApplicationName(), chatMonitorSocket, pFileNetData, beginClr);
							if (pFileNetData->m_pTransfileInfoTemp->GetTransFlag() == tagTRANSFILEINFO::TRANS_FLAG_TRANSING && 
								pFileNetData->m_pTransfileInfoTemp->transBytesTemp == pFileNetData->m_pTransfileInfoTemp->GetTotalSize())
							{	///如果当前还处理存在传输的状态
								SendTCPBufferByBlock(chatMonitorSocket, (LPBYTE)pFileNetData->m_pTransfileInfoTemp->GetEndCmd().c_str(), pFileNetData->m_pTransfileInfoTemp->GetEndCmd().size());
								pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输完成。");
								pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);						//结束状态
								break;
							}
							else if(pFileNetData->m_pTransfileInfoTemp->GetTransFlag() != tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL)
							{
								///////正在传输中，有可能是取消有可能是结束  这里其实可以不处理
								pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);						//结束状态
								break;
							}
						}
					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL:
					{
						SendTCPBufferByBlock(chatMonitorSocket, (LPBYTE)pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().c_str(), pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().size());
						Sleep(100);

						if (pFileNetData->m_pTransfileInfoTemp->transError.size() == 0)
							pFileNetData->m_pTransfileInfoTemp->transError = _T("用户取消了文件传输,文件传输失败。");

						pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);

					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND:
					{
						if (pFileNetData->m_pTransfileInfoTemp->transError.size() == 0)
							pFileNetData->m_pTransfileInfoTemp->transError = _T("文件传输失败。  ");

						pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSEND);
						bCircle = FALSE;	///中断循环
					}
						break;
				}
			}

			if (hFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hFile);	hFile = INVALID_HANDLE_VALUE; }
			if (pFileBuffer)	{ delete[] pFileBuffer; pFileBuffer = NULL; }
			if (chatMonitorSocket)	{ closesocket(chatMonitorSocket); chatMonitorSocket = INVALID_SOCKET; }
		}
			break;
	}

	if(pNetData)
		delete pNetData;

	return true;
}

UINT CThreadTCPMonitorSend::ThreadProc()
{
	TRACE(_T("%s %d \n"), _T("CThreadTCPMonitorSend"), GetCurrentThreadId());

	DWORD dwRet = 0;
	HANDLE handle[2];
	handle[0] = RIMConfig::GetInstance()->m_TCPSendQueue.GetQueueEvent();
	handle[1] = m_hExit;
	while(TRUE)	 
	{
		///第一个句柄代表队列里面有数据
		dwRet = WaitForMultipleObjects(2, handle, FALSE, INFINITE);
		if(dwRet != WAIT_OBJECT_0)
			break;


		if(RIMConfig::GetInstance()->m_TCPSendQueue.GetQueueCount())
		{
			CNetData *pNetData = NULL;
			RIMConfig::GetInstance()->m_TCPSendQueue.Pop((CQueueData **)&pNetData);
			HANDLE hThrdCnn = (HANDLE)_beginthreadex(NULL, 0, ThreadSendData, (LPVOID)pNetData, 0, NULL);
		}
		else
		{
		}
	}

	return 0;
}


CThreadTCPMonitorSend* CThreadTCPMonitorSend::m_pChatMonitorSendThread = NULL;
CThreadTCPMonitorSend* CThreadTCPMonitorSend::GetInstance()
{
	if(m_pChatMonitorSendThread == NULL)
	{
		m_pChatMonitorSendThread = new CThreadTCPMonitorSend();
		m_pSendFileBuffer = new char[m_constBufLen + BUFBLOCKUNITLEN];
	}

	return m_pChatMonitorSendThread;
}

void CThreadTCPMonitorSend::ReleaseInstance()
{
	if(m_pChatMonitorSendThread)
	{
		if(m_pSendFileBuffer)
			delete [] m_pSendFileBuffer;

		delete m_pChatMonitorSendThread;
		m_pChatMonitorSendThread = NULL;
	}
}


