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
		MessageBox(NULL, _T("CThreadTCPMonitorRecive �׽��Ӵ���ʧ��"), _T("RIM3.0 ����"), MB_OK);
		return;
	}

	ULONG reuse = true;
	if(setsockopt( m_hChatMonitorSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive SO_REUSEADDR ���ô���\n"), _T("RIM3.0 ����"), MB_OK);
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
		MessageBox(NULL, _T("CThreadTCPMonitorRecive �׽��Ӱ�ʧ��\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	int nScktOpt = 5000;
	//���ý��ճ�ʱ
	if(setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nScktOpt, sizeof(nScktOpt)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive ���ý��ճ�ʱ\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		WSACleanup();
		return;
	}
	//���÷��ͳ�ʱ
	nScktOpt = 5000;
	if(setsockopt(m_hChatMonitorSocket, SOL_SOCKET, SO_SNDTIMEO,(char *)&nScktOpt, sizeof(nScktOpt)))
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive ���÷��ͳ�ʱ\n"), _T("RIM3.0 ����"), MB_OK);
		closesocket(m_hChatMonitorSocket);
		return;
	}

	if(SOCKET_ERROR == listen(m_hChatMonitorSocket, SOMAXCONN))
	{
		MessageBox(NULL, _T("CThreadTCPMonitorRecive ����ʧ��\n"), _T("RIM3.0 ����"), MB_OK);
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
	{	//��������ȫһ�η��͵�TCP���ݰ�

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

				if (bufBlock._len + bufBlock._offset != bufBlock._fileSize)	//û���յ��ļ���β�������������
					continue;
			}
		}
		else
		{
			AfxMessageBox(_T("CThreadTCPMonitorRecive �׽��ֽ���ʧ�ܣ�"));
			memFile.SeekToBegin();
			break;
		}

		//������������
		//DWORD receiveLen = UINT(memFile.Seek(0, CFile::current));
		//if(receiveLen)
		//{
		//	{/////��ѹ������
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
	case CNetData::TCP_FILEASKSEND:	//����
		{
			LPUSERINFO lpSourceUserInfo = NULL;
			for (UINT j = 0; j < RIMConfig::GetInstance()->m_userArray.size(); ++j)
			{
				LPUSERINFO lpUserInfo = RIMConfig::GetInstance()->m_userArray[j];
				if(	//pNetData->GetSourceID() == lpUserInfo->userID									&&	////�����û���ID��0XFFFFFFFF
					pNetData->GetDesTypeID() == RIMConfig::GetInstance()->m_pLogonUser->userTypeID			&&
					_tcscmp(pNetData->GetSourceID().c_str(), lpUserInfo->UserID.c_str()) == 0
					)
				{	//���ж��ĸ��û��������� ���ҽ��Ƿ����ҵ�
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

			//////����ͼ��֪ͨ��Ϣ  2����������Ϣ  3�����ļ���Ϣ
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
						{////�������������Է�ȡ��
							pTransFileInfo->transError = _T("�Է�ȡ�����ļ�����,�ļ�����ʧ�ܡ�");
							pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							break;
						}
						Sleep(1);
					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSING:
					{
						beginRateClr = clock();

						/////****************************************���Ϳ�ʼ����

						if (SendTCPBufferByBlock(sockCnn, (LPBYTE)(pTransFileInfo->GetBeginCmd().c_str()), pTransFileInfo->GetBeginCmd().size()) == false)
						{
							pTransFileInfo->transError = _T("�ļ�����ʧ�ܡ�");
							pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//����״̬
							break;
						}

						///////��һ���ļ����������, ��ʼ��ѭ��������ļ��б�
						FILEINFO	fileInfo;
						char		*pChar			= NULL;
						DWORD		writeLen		= 0;
						DWORD		bufLen			= 0;
						DWORD		buferOff		= 0;
						while (TRUE)
						{
							if (pTransFileInfo->GetTransFlag() == tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL)
								break;

							//�����ļ�
							///////***********************************��ʼ�����ļ�
							int retRead = SelectSocketRead(sockCnn, 10);
							if (retRead < 0)
							{
								pTransFileInfo->transError = _T("�ļ�����ʧ�ܡ�");
								pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//����״̬
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
								{////�������������Է�ȡ��
									pTransFileInfo->transError = _T("�Է�ȡ�����ļ�����,�ļ�����ʧ�ܡ�");
									pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
									break;
								}
								else if (bufBlock._offset == 0 && (bufBlock._fileSize == bufBlock._len) && NULL != strstr((char*)(bufBlock._buf), pTransFileInfo->GetEndCmd().c_str()))
								{//�ļ�����ɹ����
									pTransFileInfo->transError = _T("�ļ�����ɹ���");
									pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
									break;
								}
								else if (bufBlock._offset == 0 && (bufBlock._fileSize == bufBlock._len) && NULL != (pChar = strstr((char*)(bufBlock._buf), pTransFileInfo->GetNameBaseCmd().c_str())))
								{	///Ҫ�������ļ�
									if (SendTCPBufferByBlock(sockCnn, bufBlock._buf, bufBlock._len) == false)
									{	//////��ԭָ����ΪӦ��
										pTransFileInfo->transError = _T("�ļ�����ʧ�ܡ�");
										pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//����״̬
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
									TCHAR szPath1[MAX_PATH];																			//���ѡ���Ŀ¼·�� 
									_tfullpath(szPath1, filePath0.c_str(), 256);
									MakeSureDirectoryPathExists(CT2A(szPath1));
									hUpdateFile = ::CreateFile(szPath1, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
									memset(pFileBuffer, 0, constBufLen + BUFBLOCKUNITLEN);

									clr0 = clock();
									continue;
								}

								if (bufBlock._len == 0)
									ASSERT(0);

								////��zy 2015-01-29 �����������
								pTransFileInfo->transBytesTemp += bufBlock._len;
								if (pTransFileInfo->transBytesTemp > 0)
								{
									clock_t span = clock() - beginRateClr;
									lldiv_tRIM dt	= lldiv_RIM(pTransFileInfo->transBytesTemp * 1000, LONGLONG(span?span:1));
									pTransFileInfo->rateTemp = dt.quot;
								}

								memcpy(pFileBuffer + buferOff, bufBlock._buf, bufBlock._len);
								buferOff += bufBlock._len;

								if (buferOff >= constBufLen) //bufBlock��������ݳ���Ϊ1024
								{	//������������д���ļ���
									WriteFile(hUpdateFile, pFileBuffer, buferOff, &writeLen, NULL);
									if (buferOff != writeLen) ASSERT(0); buferOff = 0;
								}

								if ((bufBlock._len + bufBlock._offset) != bufBlock._fileSize)							//û���յ��ļ���β�������������
									continue;
								else
								{	//�ļ��ر�ʱ��дһ���ļ�
									if (buferOff)
									{
										WriteFile(hUpdateFile, pFileBuffer, buferOff, &writeLen, NULL);
										if (buferOff != writeLen) ASSERT(0); buferOff = 0;
									}

									//if (fileInfo.size == 1426432)
										{
											TRACE(_T("���գ�д�����%s:%d�ֽڣ� ��ʱ��%d ms ʱ�䣺%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());
										}
									if (hUpdateFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hUpdateFile);	hUpdateFile = INVALID_HANDLE_VALUE; }
									//if (fileInfo.size == 1426432)
										{
											TRACE(_T("���գ��رչ���%s:%d�ֽڣ� ��ʱ��%d ms ʱ�䣺%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());
										}
									continue;
								}
							}
							else
							{
								pTransFileInfo->transError = _T("�ļ�����ʧ�ܣ��ļ�����ʧ�ܡ�");
								pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);		//����״̬
								break;
							}
						}
						TRACE(_T("���գ�%s:%d�ֽڣ� ��ʱ��%d ms ʱ�䣺%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());

					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL:
					{
						TRACE(_T("����ָ��ʱ�䣺%d ms\n"), clock());
						////��zy 2015-01-29 ���� �����û��ֶ�ȡ������
						SendTCPBufferByBlock(sockCnn, (LPBYTE)pTransFileInfo->GetCancelCmd().c_str(), pTransFileInfo->GetCancelCmd().size());
						if (pTransFileInfo->transError.size() == 0)
							pTransFileInfo->transError = _T("�û�ȡ�����ļ�����,�ļ�����ʧ�ܡ�");

						pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND:
					{
						TRACE(_T("����ָ��ʱ�䣺%d ms\n"), clock());
						if (pTransFileInfo->transError.size() == 0)
							pTransFileInfo->transError = _T("�ļ�����ʧ�ܡ�");

						pTransFileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSEND);
						bCircle = FALSE;	///�ж�ѭ��
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
		///��һ����������������������
		dwRet = WaitForSingleObject(m_hExit, 0);
		if(dwRet == WAIT_OBJECT_0)
			break;

		SOCKET sockCnn = INVALID_SOCKET;

		if(SelectSocketRead(m_hChatMonitorSocket, 0) > 0 && RIMConfig::GetInstance()->m_pLogonUser)
		{
			/////��SelectSocket�ɹ�������accept������
			if((sockCnn = accept(m_hChatMonitorSocket, (sockaddr FAR*)&fromAddr, &addrLen)) == INVALID_SOCKET)
			{
				AfxMessageBox(_T("CThreadTCPMonitorRecive �׽���acceptʧ�ܣ�"));
				closesocket(m_hChatMonitorSocket);
				WSACleanup();
				break;
			}
			else{
				//���յ�һ���µ�����, ���������߳�//�˴���������
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
		MessageBox(NULL, _T("CThreadTCPMonitorSend �׽��Ӵ���ʧ��"), _T("RIM3.0 ����"), MB_OK);
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

	// ��ʼ����
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
				pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ʧ�ܡ�");
				pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);					//����״̬
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
					pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ʧ�ܡ�");
					closesocket(chatMonitorSocket);
					break;
				}
				else if (retRead == 0)
				{
					::Sleep(10);
					continue;
				}

				////��zy 2015-01-29 ���� //�׽��ֿɶ�˵���������ļ����û�ȡ���˲���
				if (ReciveTCPBufferBlock(chatMonitorSocket, bufBlock, 3000) == false)
				{
					pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ʧ��.��");
					closesocket(chatMonitorSocket);
					break;
				}

				pchar	= NULL;
				////����Է�ȡ��
				if (pchar = strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().c_str()))
				{//�û�ȡ�����ļ�����
					pFileNetData->m_pTransfileInfoTemp->transError = _T("�Է�ȡ�����ļ�����,�ļ�����ʧ�ܡ�");
					pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
					closesocket(chatMonitorSocket);
					break;
				}
				else if (pchar = strstr((char*)(bufBlock._buf), sendStr))
				{
					clock_t clr0 = clock();

					////��zy 2015-01-29 ���� �ҵ��ļ���ʼ�����ļ�
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
							////��zy 2015-01-29 ���� ���ݷ��ʹ��ļ�
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

										////��zy 2015-01-29 �����������
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
										////��zy 2015-01-29 �����������
										pFileNetData->m_pTransfileInfoTemp->transBytesTemp += BUFBLOCKUNITLEN;
									}

									{
										////��zy 2015-01-29 ���� �����û��ֶ�ȡ������
										if (pFileNetData->m_pTransfileInfoTemp->GetTransFlag() == tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL)
										{
											bOutBreak = TRUE; break;
										}
										else if (ReciveTCPBufferBlock(chatMonitorSocket, bufBlock, 0) && (bufBlock._offset == 0) && (bufBlock._fileSize == bufBlock._len) && NULL != strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().c_str()))
										{////�������������Է�ȡ��
											pFileNetData->m_pTransfileInfoTemp->transError = _T("�Է�ȡ�����ļ�����,�ļ�����ʧ�ܡ�");
											pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
											bOutBreak = TRUE; break;
										}


										////��zy 2015-01-29 �����������
										clock_t span = clock() - beginRateClr;
										lldiv_tRIM dt	= lldiv_RIM(pFileNetData->m_pTransfileInfoTemp->transBytesTemp * 1000, LONGLONG(span?span:1));
										pFileNetData->m_pTransfileInfoTemp->rateTemp = dt.quot;
										//pFileNetData->m_pTransfileInfoTemp->rateTemp		= MulDiv(pFileNetData->m_pTransfileInfoTemp->transBytesTemp, 1000, clock() - beginClr);
									}

								}
								else
									break;
							}
							////��zy 2015-01-29 ����

							//�����ļ���ǰ��ƫ��
							fileOff = ::SetFilePointer(hFile, liOff.LowPart, &liOff.HighPart, (DWORD)CFile::current);
							if (fileOff == fileInfo.size)
								break;
						}
						TRACE(_T("���ͣ�%s:%d�ֽڣ� ��ʱ��%d ms ʱ�䣺%d\n"), fileInfo.name.c_str(), fileInfo.size, clock() - clr0, clock());
						if (hFile != INVALID_HANDLE_VALUE)	{ CloseHandle(hFile);	hFile = INVALID_HANDLE_VALUE; }
						break;	//// һ��ֻ����һ���ļ����ļ�������Ͼ�����ѭ��
					}
					else if (GetLastError() == ERROR_FILE_NOT_FOUND)
					{
						pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�������,�ļ�����ʧ�ܡ�");
						pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
						break;
					}
				}
				else
				{
					pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ʧ�ܡ�");
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

	//////�����ļ�������
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
				MessageBox(NULL, _T("CThreadTCPMonitorSend �׽��Ӵ���ʧ��"), _T("RIM3.0 ����"), MB_OK);
				break;
			}

			SOCKADDR_IN desAddr;
			desAddr.sin_family			= AF_INET;
			desAddr.sin_port			= htons(pNetData->GetFromOrToPort());
			desAddr.sin_addr.s_addr		= inet_addr(CT2A(pNetData->GetFromOrToIP().c_str()));
			int ret = connect(chatMonitorSocket, (PSOCKADDR)&desAddr, sizeof(desAddr));
			if (ret)
			{
				OutputDebugString(_T("RIM3.0 CThreadTCPMonitorSend ����ʧ��\n"));
				closesocket(chatMonitorSocket);
				break;
			}
			SendTCPBufferByBlock(chatMonitorSocket, pNetData->GetBuffer(), pNetData->GetBufferLen());

			//{///ѹ������

			//	int memFileLen = pNetData->GetBufferLen();
			//	LPBYTE pBufferTemp = new BYTE[pNetData->GetBufferLen() + 5];

			//	////////////////////////////�����ݽ���ѹ��
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

		case CNetData::TCP_FILEASKSEND:	//����
		{
			CFileNetData *pFileNetData = (CFileNetData*)pNetData;
			if (pFileNetData->GetBufferLen() == 0 || NULL == pFileNetData->m_pTransfileInfoTemp)
				break;

			SOCKET chatMonitorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (chatMonitorSocket == INVALID_SOCKET)
			{
				MessageBox(NULL, _T("CThreadTCPMonitorSend �׽��Ӵ���ʧ��"), _T("RIM3.0 ����"), MB_OK);
				break;
			}

			SOCKADDR_IN desAddr;
			desAddr.sin_family			= AF_INET;
			desAddr.sin_port			= htons(pFileNetData->GetFromOrToPort());
			desAddr.sin_addr.s_addr		= inet_addr(CT2A(pFileNetData->GetFromOrToIP().c_str()));
			int ret = connect(chatMonitorSocket, (PSOCKADDR)&desAddr, sizeof(desAddr));
			if (ret)
			{
				OutputDebugString(_T("RIM3.0 CThreadTCPMonitorSend ����ʧ��\n"));
				pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSEND);			//����״̬
				pFileNetData->m_pTransfileInfoTemp->transError = _T("�û����ܲ����ߣ��ļ�����ʧ�ܡ�");
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

						//////��zy 2015-01-29 ���� //���ȷ����ļ��б�XML��
						if (false == SendTCPBufferByBlock(chatMonitorSocket, pFileNetData->GetBuffer(), pFileNetData->GetBufferLen()))
						{
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ��б���ʧ�ܣ��ļ�����ʧ�ܡ�");
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
							pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ʧ�ܡ�");
							closesocket(chatMonitorSocket);
							break;
						}
						else if (retRead == 0)
						{
							::Sleep(10);
							break;
						}

						////��zy 2015-01-29 ���� //�׽��ֿɶ�˵���������ļ����û�ȡ���˲���
						if (ReciveTCPBufferBlock(chatMonitorSocket, bufBlock, 3000) == false)
						{
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ʧ��.��");
							closesocket(chatMonitorSocket);
							break;
						}

						pchar	= NULL;
						if (pchar = strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetEndCmd().c_str()))
						{//�ļ�����ɹ����
							pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ɹ���");
							pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);
							closesocket(chatMonitorSocket);
							break;
						}
						////����Է�ȡ��
						else if (pchar = strstr((char*)(bufBlock._buf), pFileNetData->m_pTransfileInfoTemp->GetCancelCmd().c_str()))
						{//�û�ȡ�����ļ�����
							pFileNetData->m_pTransfileInfoTemp->transError = _T("�Է�ȡ�����ļ�����,�ļ�����ʧ�ܡ�");
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
							{	///�����ǰ��������ڴ����״̬
								SendTCPBufferByBlock(chatMonitorSocket, (LPBYTE)pFileNetData->m_pTransfileInfoTemp->GetEndCmd().c_str(), pFileNetData->m_pTransfileInfoTemp->GetEndCmd().size());
								pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�������ɡ�");
								pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);						//����״̬
								break;
							}
							else if(pFileNetData->m_pTransfileInfoTemp->GetTransFlag() != tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL)
							{
								///////���ڴ����У��п�����ȡ���п����ǽ���  ������ʵ���Բ�����
								pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);						//����״̬
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
							pFileNetData->m_pTransfileInfoTemp->transError = _T("�û�ȡ�����ļ�����,�ļ�����ʧ�ܡ�");

						pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND);

					}
						break;
					case tagTRANSFILEINFO::TRANS_FLAG_TRANSPREEND:
					{
						if (pFileNetData->m_pTransfileInfoTemp->transError.size() == 0)
							pFileNetData->m_pTransfileInfoTemp->transError = _T("�ļ�����ʧ�ܡ�  ");

						pFileNetData->m_pTransfileInfoTemp->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSEND);
						bCircle = FALSE;	///�ж�ѭ��
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
		///��һ����������������������
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


