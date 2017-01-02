#include "StdAfx.h"
#include "ThreadEmsClient.h"
#include <mstcpip.h>  
///////////////////////////////////////////////////////////////////////////CThreadEmsClient//////////////////////////////////////////////
CThreadEmsClient::CThreadEmsClient()
{

}

CThreadEmsClient::~CThreadEmsClient()
{

}

void CThreadEmsClient::SendMsgToEms(SOCKET sock, std::string strSend,BYTE bCommType /*= 1*/)
{
	const int	bufferLen			= strSend.length() + 100;
	LPBYTE		pBuffer				= new BYTE[bufferLen];
	ZeroMemory(pBuffer, bufferLen);
	rmcp::LPRMCPFRAME lpRmcpFrame	= rmcp::LPRMCPFRAME(pBuffer);
	lpRmcpFrame->nVersion   		= MAKEWORD (0, 7);
	lpRmcpFrame->nDataType  		= 30 ;
	lpRmcpFrame->nFlags     		= 0x0 ;
	SYSTEMTIME st;
	GetLocalTime( &st );
	::SystemTimeToFileTime(&st, &lpRmcpFrame->tmStamp);

	LPBYTE pBufferTemp = pBuffer + RMCP_FRAME_HEAD_LEN;
	memcpy(pBufferTemp, &bCommType, sizeof(BYTE));
	pBufferTemp += sizeof(BYTE);
	memcpy(pBufferTemp, strSend.c_str(), sizeof(char)*strSend.length());

	lpRmcpFrame->dwLength = bufferLen;
	lpRmcpFrame->SetCheckSum();

	send(sock, (char*)pBuffer, bufferLen, 0);

	if(pBuffer)	{	delete [] pBuffer;	pBuffer = NULL;	}
}

void CThreadEmsClient::Parse(LPBYTE	pBufferStart, DWORD bufSize)
{
	try
	{
		//MNS����
		rmcp::LPRMCPFRAME lpFrame = rmcp::LPRMCPFRAME(pBufferStart);
		std::string lstr = (char*)(pBufferStart + RMCP_FRAME_HEAD_LEN + sizeof(BYTE));
		lstr = lstr.substr(0, lpFrame->dwLength - RMCP_FRAME_HEAD_LEN - sizeof(BYTE));

		TiXmlDocument xmlDoc;
		xmlDoc.Parse(lstr.c_str());
		lstr = lstr.substr(0, lpFrame->dwLength - RMCP_FRAME_HEAD_LEN);

		TiXmlElement* pRoot = xmlDoc.RootElement();
		if(pRoot == NULL) return;


		TiXmlElement*	pDevNode	= NULL;
		TiXmlElement*	pTypeNode	= NULL;
		pDevNode	= pRoot->FirstChildElement();
		bool bUpdateStatus = false;

		while(pDevNode != NULL)
		{
			string stationId	= pDevNode->Attribute("statid");
			string strDevType	= pDevNode->Attribute("devtype");
			string strDevId		= pDevNode->Attribute("devid");	

			pTypeNode	= pDevNode->FirstChildElement();
			if(pTypeNode)
			{
				int typeID = atoi(pTypeNode->Attribute("id"));
				TiXmlElement*	pPromptNode	= pTypeNode->FirstChildElement("prompt");
				string strStatus = pPromptNode->Attribute("status");	

				switch(typeID)
				{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					{
						if(strStatus.length() == 0)
							break;

						BOOL bBreak = FALSE;
						for (UINT i = 0; i < RIMConfig::GetInstance()->m_stationArray.size(); ++i)
						{
							LPSTATIONINFO lpstationinfo = RIMConfig::GetInstance()->m_stationArray.at(i);
							for (UINT j = 0; j < lpstationinfo->devicesInfos.size(); ++j)
							{
								LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(j);

								if (lpdeviceinfo->pRimDeviceTemp->sStationNO == stationId && lpdeviceinfo->pRimDeviceTemp->sDeviceNO == strDevId)
								{
									DWORD staRcv = atoi(strStatus.c_str());

									bBreak = TRUE;
									if(lpdeviceinfo->deviceState != staRcv)
									{
										lpdeviceinfo->deviceState = staRcv;
										AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATESTATIONSTATUS);
									}
								}

								if(bBreak)	break;
							}
							if(bBreak)	break;
						}
					}
					break;
				case 5:
					break;
				default:break;
				}
			}
			pDevNode = pDevNode->NextSiblingElement();
		} 
	}
	catch(...)
	{

	}
}

UINT CThreadEmsClient::ThreadProc()
{
	TRACE(_T("%s %d \n"), _T("CThreadEmsClient"), GetCurrentThreadId());

	DWORD		dwRet		= 0;
	int			iRetVal 	= 0;
	clock_t     clkBegin	= 0;

	LPBYTE		pBufferMax	= NULL;
	LPBYTE		pBufferStart		= NULL;
	CMemFile	bufFile(0x1000);

	int addrLen = sizeof(SOCKADDR_IN);

	while(m_hEmsSocket != INVALID_SOCKET)
	{
		////��һ����������������������
		dwRet = WaitForSingleObject(m_hExit, 0);
		if(dwRet == WAIT_OBJECT_0)
			break;

		if(m_hEmsSocket == INVALID_SOCKET || RIMConfig::GetInstance()->m_pLogonUser == NULL)
			break;

		iRetVal = SelectSocketRead(m_hEmsSocket, 100);
		if(iRetVal < 0 )
		{
			OutputDebugString(_T("RIM3.0 CThreadEmsClient SelectSocketRead��������ʧ��"));
			AfxMessageBox(_T("CThreadEmsClient SelectSocketRead��������ʧ��"));
			break;
		}
		else if(iRetVal == 0)
		{
			Sleep(1);

			if(/*clock() - clkBegin > 1000 * 60 || */clkBegin == 0)
			{
				TiXmlDocument	xmldoc;

				xmldoc.LoadFile(TIXML_ENCODING_LEGACY);
				xmldoc.Clear();

				TiXmlElement	*pRootNode, *pNodeDevice, *pNodeType;

				TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );
				xmldoc.LinkEndChild( decl );

				pRootNode = new TiXmlElement("subscribe");
				xmldoc.LinkEndChild(pRootNode);

				for (UINT i = 0; i < RIMConfig::GetInstance()->m_stationArray.size(); ++i)
				{
					LPSTATIONINFO lpstationinfo = RIMConfig::GetInstance()->m_stationArray.at(i);
					for (UINT j = 0; j < lpstationinfo->devicesInfos.size(); ++j)
					{
						LPDEVICEINFO lpdeviceinfo = lpstationinfo->devicesInfos.at(j);

						pNodeDevice = new TiXmlElement("devinfo");
						pRootNode->LinkEndChild(pNodeDevice);
						pNodeDevice->SetAttribute("statid", lpstationinfo->pRimStationTemp->sStationNO);
						pNodeDevice->SetAttribute("devtype", 0);		////����Ϊ0
						pNodeDevice->SetAttribute("devid", lpdeviceinfo->pRimDeviceTemp->sDeviceNO);
						pNodeDevice->SetAttribute("devip", lpdeviceinfo->pRimDeviceTemp->sDriverIP);
						pNodeDevice->SetAttribute("devport", lpdeviceinfo->pRimDeviceTemp->nDriverPort);
						pNodeDevice->SetAttribute("devprotocol", lpdeviceinfo->pRimDeviceTemp->nDeviceProtocol);
						pNodeDevice->SetAttribute("devpassword", lpdeviceinfo->pRimDeviceTemp->nMaxRunNUM);

						pNodeType = new TiXmlElement("type");
						pNodeDevice->LinkEndChild(pNodeType);
						pNodeType->SetAttribute("id", 4);
						pNodeType->SetAttribute("name", "1234");
					}
				}

				TiXmlPrinter printer;
				xmldoc.Accept(&printer);
				string strXMl = printer.CStr();

				SendMsgToEms(m_hEmsSocket, strXMl);
				clkBegin = clock();
			}
		}
		else
		{
			rmcp::LPRMCPFRAME lp = NULL;
			
			if(bufFile.GetPosition() == 0)
			{
				bufFile.SetLength(RMCP_FRAME_HEAD_LEN);
			}
			else if(bufFile.GetPosition() == RMCP_FRAME_HEAD_LEN)
			{	
				int a = 0;
				a = 1;
			}

			int needReadLen = int(bufFile.GetLength() - bufFile.GetPosition());
			bufFile.GetBufferPtr(CFile::bufferWrite, needReadLen, (void**)&pBufferStart, (void**)&pBufferMax);
			int nRcv = recv(m_hEmsSocket, (char*)pBufferStart , needReadLen, 0);

			if(nRcv <= 0)
			{
				AfxMessageBox(_T("CThreadEmsClient �׽��ֽ���ʧ�ܣ�"));
				return -1 ;
			}
			else if(nRcv < needReadLen ) //��������
			{
				bufFile.Seek(nRcv, CFile::current);
			}
			else
			{
				if(bufFile.GetLength() == RMCP_FRAME_HEAD_LEN)
				{
					lp = rmcp::LPRMCPFRAME(pBufferStart);

					if(lp->dwLength > 0x10000)
						AfxMessageBox(_T("CThreadEmsClient ����������̫��"));
					else
					{
						bufFile.SetLength(lp->dwLength);
						bufFile.Seek(nRcv, CFile::current);
					}
				}
				else
				{
					bufFile.Seek(0, CFile::begin);
					bufFile.GetBufferPtr(CFile::bufferWrite, needReadLen, (void**)&pBufferStart, (void**)&pBufferMax);
					Parse(pBufferStart, DWORD(bufFile.GetLength()));
					bufFile.SetLength(0);
				}
			}
		}
	}

	closesocket(m_hEmsSocket);
	return 0;
}

HRESULT CThreadEmsClient::Begin(string serverIp /*= "127.0.0.1"*/, short serverPort /*= 0*/)
{
	{
		// ��ʼ��Windows Sockets����
		DWORD	dwRet;										// ��������ֵ
		WSADATA wsaData;									// Windows Socket��ʼ����Ϣ
		if ((dwRet = WSAStartup(0x0202, &wsaData)) != 0)
		{
			OutputDebugString(_T("RIM3.0 WSAStartup failed with error\n"));
			return S_FALSE;
		}

		serverIp			= serverIp;
		serverPort			= serverPort;
		m_hEmsSocket		= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(m_hEmsSocket == INVALID_SOCKET)
		{
			MessageBox(NULL, _T("RIM3.0 CThreadEmsClient �׽��Ӵ���ʧ��"), _T("����"), MB_OK);
			return S_FALSE;
		}


		// ����KeepAlive
		BOOL bKeepAlive = TRUE;
		int nRet = ::setsockopt(m_hEmsSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));
		if (nRet == SOCKET_ERROR)
		{
			return FALSE;
		}

		// ����KeepAlive����
		tcp_keepalive alive_in					= {0};
		tcp_keepalive alive_out					= {0};
		alive_in.keepalivetime					= 5000;                // ��ʼ�״�KeepAlive̽��ǰ��TCP�ձ�ʱ��
		alive_in.keepaliveinterval				= 10000;                // ����KeepAlive̽����ʱ����
		alive_in.onoff                          = TRUE;
		unsigned long ulBytesReturn				= 0;
		nRet = WSAIoctl(m_hEmsSocket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
			&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
		if (nRet == SOCKET_ERROR)
		{
			return FALSE;
		}

		SOCKADDR_IN desAddr;
		desAddr.sin_family			= AF_INET;
		desAddr.sin_port			= htons(serverPort);
		desAddr.sin_addr.s_addr		= inet_addr(serverIp.c_str());
		int ret = connect(m_hEmsSocket, (PSOCKADDR)&desAddr, sizeof(desAddr));
		if (ret)
		{
			OutputDebugString(_T("RIM3.0 CThreadEmsClient ����ʧ��\n"));
			MessageBox(NULL, _T("RIM3.0 CThreadEmsClient ����ʧ��"), _T("����"), MB_OK);
			closesocket(m_hEmsSocket);	m_hEmsSocket = INVALID_SOCKET;
			return S_FALSE;
		}
	}

	CUserThread::Begin();
	return S_OK;
}

CThreadEmsClient* CThreadEmsClient::m_pThreadEmsClient = NULL;
CThreadEmsClient* CThreadEmsClient::GetInstance()
{
	if(m_pThreadEmsClient == NULL)
	{
		m_pThreadEmsClient = new CThreadEmsClient();
	}

	return m_pThreadEmsClient;
}

void CThreadEmsClient::ReleaseInstance()
{
	if(m_pThreadEmsClient)
	{
		delete m_pThreadEmsClient;
		m_pThreadEmsClient = NULL;
	}
}