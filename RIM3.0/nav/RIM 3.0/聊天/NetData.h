#pragma once


//TCPЭ��ͨ�����ݽṹ
const int	BUFBLOCKUNITLEN = 1440;
struct BUFBLOCK
{
	UINT 		_fileSize;				//������buffer���ļ����ܴ�С
	UINT 		_offset;				//������ܵĻ�������ǰ���ݵ�ƫ����
	UINT 		_len;					//��ǰ������Ч���ļ������ݵĳ���
	BYTE 		_buf[BUFBLOCKUNITLEN];	//������ݵ�buf
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
		UDP_BEGIN								= 1,				//UDP��ʼ��ϢID
		UDP_OFFLINE								= 2,				//������Ϣ 
		UDP_ONLINE_NOTIFY						= 3,				//������Ϣ֪ͨ
		UDP_ONLINE_ANSWER						= 4,				//������ϢӦ��
		UDP_END									= 50,				//UDP������ϢID

		TCP_BEGIN								= 100,				//TCP��ʼ��ϢID
		TCP_CHATMSG								= 101,				//ͨ����Ϣ
		TCP_FILEASKSEND							= 102,				//�����ļ�������Ϣ
		TCP_FILEASKRECEIVE						= 103,				//�����ļ�������Ϣ
		TCP_FILEASKREFUSE						= 104,				//�����ļ��ܾ���Ϣ
		TCP_END									= 1000,				//TCP������ϢID
	};
protected:
	void										ClearBuffer();
	SYSTEMTIME									m_dataCreateTime;	//�������ɵ�ʱ��

	LPBYTE										m_pBuf;
	UINT										m_bufLen;

	DWORD										m_srcTypeID;		//Դ�û�ID
	DWORD										m_desTypeID;		//��ϢĿ���û�ID
	tstring										m_srcID;			//���ͷ��û�����
	tstring										m_desID;			//���շ��û�����
	DWORD										m_dataType;			//��������
	tstring										m_fromOrToIP;		//��Ϊ������Ϣʱ��Ϊ����Դ��IP��������Ϣʱ��ΪĿ�ķ���IP
	USHORT										m_fromOrToPort;		//��Ϊ������Ϣʱ��Ϊ����Դ��Port��������Ϣʱ��ΪĿ�ķ���Port

public:	
	//************************************
	// Method:    ParseUDPBuffer
	// FullName:  CUDPData::ParseUDPBuffer
	// Access:    public 
	// Returns:   bool
	// Qualifier:								�����������յ�UDP ����
	// Parameter: LPBYTE lpBuffer
	// Parameter: UINT bufLen
	//************************************
	virtual	bool								ParseNETBuffer(LPBYTE lpBuffer, UINT bufLen, struct sockaddr FAR * from);

	//************************************
	// Method:    MakeUDPBuffer
	// FullName:  CUDPData::MakeUDPBuffer
	// Access:    public 
	// Returns:   bool
	// Qualifier:								�������콫Ҫ���͵�UDP����
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
