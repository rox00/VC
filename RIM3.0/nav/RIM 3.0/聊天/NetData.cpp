// E:\My Project\visual studio 2008\RIM 3.0\聊天\ChatSendData.cpp : implementation file
//

#include "stdafx.h"
#include "NetData.h"


// CChatSendData


bool ReciveTCPBufferBlock(SOCKET sock, BUFBLOCK& bufBlock, __in DWORD dwMilliseconds)
{
	long nRcv = 0;
	memset(&bufBlock, 0, sizeof(BUFBLOCK));

	//nRcv = recv(sock, (char*)(&bufBlock), sizeof(BUFBLOCK), 0);

	char*	pbuf		= (char*)(&bufBlock);
	int		needReadLen = sizeof(BUFBLOCK);
	while (true)
	{
		if (SelectSocketRead(sock, dwMilliseconds) > 0)
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

bool SendTCPBufferByBlock(SOCKET sock, LPBYTE pBuffer, int bufLen)
{
	BUFBLOCK bufBlock;
	if (sock != INVALID_SOCKET)
	{
		LPBYTE		buffer			= pBuffer;
		int			len				= bufLen;
		const int	dataUnitlen		= BUFBLOCKUNITLEN;
		if (len == 0)
			return false;

		int i = 0;
		while (true)
		{
			memset(&bufBlock, 0, sizeof(BUFBLOCK));
			bufBlock._fileSize	= bufLen;
			bufBlock._offset	= i * dataUnitlen;
			if (len > 0)
			{
				if (len < dataUnitlen)
				{
					bufBlock._len	= len;
					memcpy(bufBlock._buf, buffer + i * dataUnitlen, len);
					send(sock, (char*)(&bufBlock), sizeof(BUFBLOCK), 0);
					break;
				}
				else
				{
					bufBlock._len	= dataUnitlen;
					memcpy(bufBlock._buf, buffer + i * dataUnitlen, dataUnitlen);
					len -= dataUnitlen;
					i++;
					send(sock, (char*)(&bufBlock), sizeof(BUFBLOCK), 0);
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

CNetData::CNetData()
{
	GetLocalTime(&m_dataCreateTime);

	m_srcTypeID				= 0;			//源用户ID
	m_desTypeID				= 0;			//消息目的用户ID
	m_srcID			= _T("");;
	m_desID			= _T("");;

	m_pBuf				= NULL;
	m_bufLen			= 0;;

	m_fromOrToIP		= _T("");				//
	m_fromOrToPort		= 0;
}

CNetData::~CNetData()
{
	ClearBuffer();
}

void CNetData::ClearBuffer()
{
	if(m_pBuf)
	{
		delete [] m_pBuf;
		m_pBuf = NULL;
	}
	m_bufLen = 0;
}
//
//bool CChatNetData::ParseBuffer(LPBYTE lpBuffer, UINT bufLen)
//{
//	if(bufLen > 8 + 4)
//	{
//		int i = 0;
//		if(lpBuffer[0] == 0xff && lpBuffer[1] == 0xfe && lpBuffer[bufLen-2] == 0xfe && lpBuffer[bufLen-1] == 0xff)
//		{
//			i++;
//			i++;
//
//			m_srcID = *((DWORD*)(lpBuffer + i));	i += 4;
//			m_desID = *((DWORD*)(lpBuffer + i));	i += 4;
//
//			string message((char*)lpBuffer + i, bufLen - i - 2);
//			m_message = CA2T(message.c_str());
//		}
//		else
//			return false;
//	}
//	else
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool CChatNetData::MakeBuffer(LPBYTE lpBuffer, UINT &bufLen)
//{
//	ZeroMemory(lpBuffer, bufLen);
//
//	if(bufLen > 8 + 4)
//	{
//		int i = 0;
//		lpBuffer[i] = 0xff; 	i++;
//		lpBuffer[i] = 0xfe; 	i++;
//		*((DWORD*)(lpBuffer + i)) = m_srcID;		i += 4;
//		*((DWORD*)(lpBuffer + i)) = m_desID;		i += 4;
//
//
//		//构造消息字符串
//		SYSTEMTIME systemtime;
//		GetLocalTime(&systemtime);
//		char ch[MAX_PATH] = {0};
//		sprintf_s(ch, MAX_PATH, "%s (%d-%02d-%02d %02d:%02d:%02d)\n\t", CT2A(m_srcUserName.c_str()), systemtime.wYear, systemtime.wMonth, systemtime.wDay, systemtime.wHour, systemtime.wMinute, systemtime.wSecond);
//
//		string message = string(ch) + string(CT2A(m_message.c_str()));
//
//		UINT remainMessageLen = bufLen - 8 - 4;
//		if(remainMessageLen < message.size())
//		{	//超过了buffer的长度，就只拷贝剩余空间长度
//			memcpy(lpBuffer + i, message.c_str(), remainMessageLen);
//			i += remainMessageLen;
//		}
//		else
//		{	//剩余空间多于要发送信息时，拷贝全部字符串长度
//			memcpy(lpBuffer + i, message.c_str(), message.size());
//			i += message.size();
//		}
//
//		lpBuffer[i] = 0xfe; 	i++;
//		lpBuffer[i] = 0xff; 	i++;
//
//		bufLen = i;
//	}
//	else
//	{
//		return false;
//	}
//
//	return true;
//}






bool CNetData::ParseNETBuffer(LPBYTE lpBuffer, UINT bufLen, struct sockaddr FAR * from)
{
	ClearBuffer();
	if (bufLen > 8 + 4 + 250 + 250)
	{
		int i = 0;
		if(lpBuffer[0] == 0xff && lpBuffer[1] == 0xfe && lpBuffer[bufLen-2] == 0xfe && lpBuffer[bufLen-1] == 0xff)
		{
			i++;
			i++;

			char ch[MAX_PATH] = { 0 };
			memcpy(ch, lpBuffer + i, 250);	i += 250; m_srcID = CA2T(ch);
			memcpy(ch, lpBuffer + i, 250);	i += 250; m_desID= CA2T(ch);

			m_srcTypeID 	= *((DWORD*)(lpBuffer + i));	i += sizeof(m_srcTypeID);
			m_desTypeID 	= *((DWORD*)(lpBuffer + i));	i += sizeof(m_desTypeID);
			m_dataType		= *((DWORD*)(lpBuffer + i));	i += sizeof(m_dataType);

			m_bufLen		= bufLen - i - 2;
			m_pBuf			= new BYTE[m_bufLen];

			memcpy(m_pBuf, (char*)lpBuffer + i,  bufLen - i - 2);

			if(from)
			{
				m_fromOrToIP	= CA2T(inet_ntoa(((LPSOCKADDR_IN)from)->sin_addr));
				m_fromOrToPort	= ntohs(((LPSOCKADDR_IN)from)->sin_port);
			}
		}
		else
			return false;
	}
	else
	{
		return false;
	}

	return true;
}

bool CNetData::MakeNETBuffer(CNetData::NETMSG dataType, LPUSERINFO pCurUser, LPUSERINFO pDesUser, tstring toIP, LPBYTE lpDataBuffer, UINT bufLen)
{
	///////////////////数据包格式
	////0xff, 0xfe, 发送方名称(250byte),目的方名称(250byte),m_srcID(4byte),m_desID(4byte), data(bufLen byte), 0xfe, 0xff。  结束
	ClearBuffer();
	if(pCurUser && pDesUser && lpDataBuffer && bufLen > 0)
	{
		////////用户的唯一标识
		m_srcTypeID 		= pCurUser->userTypeID;
		m_desTypeID 		= pDesUser->userTypeID;
		m_srcID				= pCurUser->UserID;
		m_desID				= pDesUser->UserID;

		m_dataType 			= dataType;
		m_fromOrToIP		= toIP;

		if(m_dataType > UDP_BEGIN && m_dataType < UDP_END)	//UDP消息
			m_fromOrToPort	= pDesUser->userUdpPortTemp;
		else if(m_dataType > TCP_BEGIN && m_dataType < TCP_END)	//TCP消息
			m_fromOrToPort	= pDesUser->userTcpPortTemp;
		else
			return false;


		//分配空间
		m_bufLen	= 2 + 250 + 250 + sizeof(m_srcTypeID) + sizeof(m_desTypeID) + sizeof(m_dataType) + bufLen + 2;
		m_pBuf		= new BYTE[m_bufLen];
		ZeroMemory(m_pBuf, m_bufLen);

		int i = 0;

		//////////////构造数据包
		m_pBuf[i] = 0xff; 	i++;
		m_pBuf[i] = 0xfe; 	i++;
		////拷贝名称至缓冲区
		string str = CT2A(m_srcID.c_str());
		memcpy(m_pBuf + i, str.c_str(), str.size());	i += 250;
		str = CT2A(m_desID.c_str());
		memcpy(m_pBuf + i, str.c_str(), str.size());	i += 250;

		*((DWORD*)(m_pBuf + i)) = m_srcTypeID;				i += sizeof(m_srcTypeID);
		*((DWORD*)(m_pBuf + i)) = m_desTypeID;				i += sizeof(m_desTypeID);
		*((DWORD*)(m_pBuf + i)) = m_dataType;			i += sizeof(m_dataType);

		memcpy(m_pBuf + i, lpDataBuffer, bufLen);
		i += bufLen;

		m_pBuf[i] = 0xfe; 	i++;
		m_pBuf[i] = 0xff; 	i++;

		bufLen = i;
	}
	else
	{
		return false;
	}

	return true;
}

LPBYTE CNetData::GetBuffer()
{
	return m_pBuf;
}

UINT CNetData::GetBufferLen()
{
	return m_bufLen;
}

DWORD CNetData::GetSourceTypeID()
{
	return m_srcTypeID;
}

DWORD CNetData::GetDesTypeID()
{
	return m_desTypeID;
}

USHORT CNetData::GetFromOrToPort()
{
	return m_fromOrToPort;
}

tstring CNetData::GetFromOrToIP()
{
	return m_fromOrToIP;
}

DWORD CNetData::GetDataType()
{
	return m_dataType;
}

tstring CNetData::GetSourceID()
{
	return m_srcID;
}

tstring CNetData::GetDesID()
{
	return m_desID;
}

SYSTEMTIME CNetData::GetDataCreateTime()
{
	return m_dataCreateTime;
}



//CFileNetData::CFileNetData()
//{
//
//}
//
//CFileNetData::~CFileNetData()
//{
//
//}
// 
// LPTRANSFILEINFO CFileNetData::GetTransfileInfo()
// {
// 	return m_pTransfileInfo;
// }
// 
// bool CFileNetData::ParseNETBuffer(LPBYTE lpBuffer, UINT bufLen, struct sockaddr FAR * from)
// {
// 
// }
// 
// bool CFileNetData::MakeNETBuffer(CNetData::NETMSG dataType, LPUSERINFO pCurUser, LPUSERINFO pDesUser, LPBYTE lpDataBuffer, UINT bufLen)
// {
// 
// }

CFileNetData::CFileNetData(LPTRANSFILEINFO pTransFileInfo)
{
	m_pTransfileInfoTemp = pTransFileInfo;
}

CFileNetData::~CFileNetData()
{

}
