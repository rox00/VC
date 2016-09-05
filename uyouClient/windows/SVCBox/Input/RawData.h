#pragma once

#include "touch_event.pb.h"

#ifndef RAW_HEADER_LENGTH
#define RAW_HEADER_LENGTH 4
#endif

class RawData
{
public:
	RawData(TouchEvent::Message &msg);
	~RawData(void);

	void ToByteArray(char* buf);
	boost::asio::const_buffer ToAsioBuffer();
	UINT GetRawLength();

private:
	void IntToByte(int iNumber, char *byte);
	int ByteToInt(char *buf);
	
	static const UINT m_InternalBufferSize = 1024;
	char m_InternalBuffer[m_InternalBufferSize];
	char *m_lpBuffer;
	
	TouchEvent::Message *m_pMsg;
	UINT m_uMsgLength;
};
