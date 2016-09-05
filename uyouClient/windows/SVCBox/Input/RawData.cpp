#include "StdAfx.h"
#include "RawData.h"

using namespace TouchEvent;

RawData::RawData(Message &msg)
{
	m_pMsg = &msg;
	m_uMsgLength = m_pMsg->ByteSize();
	m_lpBuffer = NULL;
}

RawData::~RawData(void)
{
	if(m_lpBuffer && m_lpBuffer != m_InternalBuffer)
		delete [] m_lpBuffer;
}

/************************************************************************/
/* 获得所需要缓冲区的长度                                                                     */
/************************************************************************/
UINT RawData::GetRawLength()
{
	return m_uMsgLength + RAW_HEADER_LENGTH;
}

/***********************************************************
功能：生成字节数组流
***********************************************************/
void RawData::ToByteArray(char* buf)
{
	char lenBytes[RAW_HEADER_LENGTH] = {0};
	IntToByte(m_uMsgLength, lenBytes);

	memcpy(buf, lenBytes, RAW_HEADER_LENGTH);
	buf += RAW_HEADER_LENGTH;

	m_pMsg->SerializeToArray(buf, m_uMsgLength);
}

boost::asio::const_buffer RawData::ToAsioBuffer()
{
	UINT nLength = GetRawLength();
	m_lpBuffer = m_InternalBuffer;
	if(nLength > m_InternalBufferSize)
		m_lpBuffer = new char[nLength];

	ToByteArray(m_lpBuffer);

	return boost::asio::const_buffer(m_lpBuffer, nLength);
}

void RawData::IntToByte(int iNumber, char *byte)
{
	*(int *)byte= htonl(iNumber);
}

int RawData::ByteToInt(char *buf)
{
	return ntohl(*(int*)buf);
}





