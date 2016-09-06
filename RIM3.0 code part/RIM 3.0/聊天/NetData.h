#pragma once


//TCP协议通信数据结构
const int	BUFBLOCKUNITLEN = 1440;
struct BUFBLOCK
{
	UINT 		_fileSize;				//缓冲区buffer或文件的总大小
	UINT 		_offset;				//相对于总的缓冲区当前数据的偏移量
	UINT 		_len;					//当前传送有效（文件）数据的长度
	BYTE 		_buf[BUFBLOCKUNITLEN];	//存放数据的buf
};
bool			ReciveTCPBufferBlock(SOCKET sock, BUFBLOCK& bufBlock, __in DWORD dwMilliseconds);
bool			SendTCPBufferByBlock(SOCKET sock, LPBYTE pBuffer, int bufLen);

class CNetData : public CQueueData
{
public:
	CNetData();
	virtual ~CNetData();
	enum NETMSG
	{
		UDP_BEGIN								= 1,				//UDP起始信息ID
		UDP_OFFLINE								= 2,				//下线消息 
		UDP_ONLINE_NOTIFY						= 3,				//上线消息通知
		UDP_ONLINE_ANSWER						= 4,				//上线消息应答
		UDP_END									= 50,				//UDP结束信息ID

		TCP_BEGIN								= 100,				//TCP起始信息ID
		TCP_CHATMSG								= 101,				//通信消息
		TCP_FILEASKSEND							= 102,				//传送文件请求消息
		TCP_FILEASKRECEIVE						= 103,				//传送文件接收消息
		TCP_FILEASKREFUSE						= 104,				//传送文件拒绝消息
		TCP_END									= 1000,				//TCP结束信息ID
	};
protected:
	void										ClearBuffer();
	SYSTEMTIME									m_dataCreateTime;	//数据生成的时间

	LPBYTE										m_pBuf;
	UINT										m_bufLen;

	DWORD										m_srcTypeID;		//源用户ID
	DWORD										m_desTypeID;		//消息目的用户ID
	tstring										m_srcID;			//发送方用户名称
	tstring										m_desID;			//接收放用户名称
	DWORD										m_dataType;			//数据类型
	tstring										m_fromOrToIP;		//当为接收消息时，为发送源的IP，发送消息时，为目的方的IP
	USHORT										m_fromOrToPort;		//当为接收消息时，为发送源的Port，发送消息时，为目的方的Port

public:	
	//************************************
	// Method:    ParseUDPBuffer
	// FullName:  CUDPData::ParseUDPBuffer
	// Access:    public 
	// Returns:   bool
	// Qualifier:								用来解析接收的UDP 数据
	// Parameter: LPBYTE lpBuffer
	// Parameter: UINT bufLen
	//************************************
	virtual	bool								ParseNETBuffer(LPBYTE lpBuffer, UINT bufLen, struct sockaddr FAR * from);

	//************************************
	// Method:    MakeUDPBuffer
	// FullName:  CUDPData::MakeUDPBuffer
	// Access:    public 
	// Returns:   bool
	// Qualifier:								用来构造将要发送的UDP数据
	// Parameter: UDPMSG dataType
	// Parameter: LPUSERINFO pCurUser
	// Parameter: LPUSERINFO pDesUser
	// Parameter: LPBYTE lpDataBuffer
	// Parameter: UINT bufLen
	//************************************
	virtual bool								MakeNETBuffer(CNetData::NETMSG dataType, LPUSERINFO pCurUser, LPUSERINFO pDesUser, tstring toIP, LPBYTE lpDataBuffer, UINT bufLen);
	
	virtual LPBYTE								GetBuffer();
	virtual UINT								GetBufferLen();
	virtual DWORD								GetSourceTypeID();
	virtual DWORD								GetDesTypeID();
	virtual USHORT								GetFromOrToPort();
	virtual tstring								GetFromOrToIP();
	virtual DWORD								GetDataType();
	virtual tstring								GetSourceID();
	virtual tstring								GetDesID();
	virtual SYSTEMTIME							GetDataCreateTime();
};


class CFileNetData : public CNetData
{
public:
	CFileNetData(LPTRANSFILEINFO pTransFileInfo);
	virtual ~CFileNetData();

	LPTRANSFILEINFO								m_pTransfileInfoTemp;
};
