//********************************************************************
// RIM30MSGDEFINE.H 文件注释
// 文件名 　: RIM30MSGDEFINE.H
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 11:56
// 文件描述 : 
//*********************************************************************

#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
using namespace std;
#include "ReadWriteLock.h"

////【zy 2015-04-27 ↓】							子系统保持唯一则GUID预先定义
#define RIM30GUID_INITPROCESS					"{E98CCD85-88BF-47c2-8C03-EC3C0633CCAC}"	////初始化进程发送消息的GUID
////【zy 2015-04-27 ↑】

#define RIM30MSG_HEART_PACK						0xFFFFFFFF					////心跳包消息ID

/************************************导航器维护消息（ID:0~100）:Begin**********************************/
#define RIM30MSG_ONLINE_ASK						1							////子系统上线请求
#define RIM30MSG_ONLINE_ANSWER					2							////子系统上线应答
#define RIM30MSG_OFFLINE_ASK					3							////子系统下线请求
#define RIM30MSG_OFFLINE_ANSWER					4							////子系统下线应答
#define RIM30MSG_SHUTDOWN_ASK					5							////关闭请求
#define RIM30MSG_SHUTDOWN_ANSWER				6							////关闭应答
#define RIM30MSG_BROADCAST						7							////通用广播消息
#define	RIM30MSG_QUERYSERVERADDR_ASK			8							////查询服务地址信息请求
#define	RIM30MSG_QUERYSERVERADDR_ANSWER			9							////查询登服务地址信息应答

/************************************导航器维护消息（ID:0~100）:End**********************************/

/************************************监测测向维护消息（ID:200~399）:Begin**********************************/
#define RIM30MSG_MEASUREDF_START_ASK			200							////启动测量请求
#define RIM30MSG_MEASUREDF_START_ANSWER			201							////启动测量应答


/************************************监测测向维护消息（ID:200~399）:End**********************************/

////【zy 2015-06-05 ↓】		////定义   各服务IP查询标识号
enum RIM30MSG__SubSystemID {
	RIM30MSG__SubSystemID__xDMS 				= 0,
	RIM30MSG__SubSystemID__xRMS 				= 1, 
	RIM30MSG__SubSystemID__xMSA 				= 2, 
	RIM30MSG__SubSystemID__xECS 				= 3, 
	RIM30MSG__SubSystemID__xBSD 				= 4, 
	RIM30MSG__SubSystemID__xFRS 				= 5, 
	RIM30MSG__SubSystemID__xSMS 				= 6, 
	RIM30MSG__SubSystemID__xLES 				= 7, 
	RIM30MSG__SubSystemID__xOAS 				= 8, 
	RIM30MSG__SubSystemID__xGIS 				= 9, 
	RIM30MSG__SubSystemID__xNULL				= 10,
	RIM30MSG__SubSystemID__99					= 99,
};
////【zy 2015-06-05 ↑】

#pragma pack(push, 1)

const int	RIM30TCPBLOCKDATALEN = 1440;
typedef struct tagRIM30TCPBLOCK
{
	UINT 										_fileSize;					//缓冲区buffer或文件的总大小
	UINT 										_offset;					//相对于总的缓冲区当前数据的偏移量
	UINT 										_len;						//当前传送有效（文件）数据的长度
	BYTE 										_buf[RIM30TCPBLOCKDATALEN];	//存放数据的buf
}RIM30TCPBLOCK, *LPRIM30TCPBLOCK;

#define DATA_BUFSIZE							1024*16						// 发送和接收消息的最大长度 16KB
typedef struct tagRIM30MSG
{
	~tagRIM30MSG()
	{
	}

protected:
	tagRIM30MSG()
	{
		ZeroMemory(sendGUID, 64);
		ZeroMemory(recvGUID, 64);
		msgID									= 0;
		dataType								= 0;
		dataLength								= 0;
	}

	char 										sendGUID[64];				//发送方GUID
	char 										recvGUID[64];				//接收方GUID
	DWORD										msgID;						//消息定义
	DWORD										dataType;					//数据类型
	DWORD										dataLength;					//数据长度
	BYTE										data[1];					//指向数据缓冲区

public:
	enum{
		DATATYPE_BINARY							= 1,
		DATATYPE_XML							= 2,
	};

	//************************************
	// Method:    MakeMsg
	// FullName:  tagRIM30MSG::MakeMsg
	// Access:    public static 
	// Returns:   tagRIM30MSG*
	// Qualifier:								用于构造基本消息数据包
	// Parameter: const char * pSendGuid		发送方的GUID
	// Parameter: const char * pRecvGuid		接收方的GUID
	// Parameter: DWORD msgID					消息ID
	// Parameter: DWORD dataType				消息数据区的数据类型
	// Parameter: LPBYTE pData					用于构造消息数据区的数据
	// Parameter: DWORD dataLength				消息数据区的数据长度
	//************************************
	static tagRIM30MSG*							MakeMsg(const char *pSendGuid, const char *pRecvGuid, DWORD msgID, DWORD dataType, LPBYTE pData, DWORD dataLength);
	//************************************
	// Method:    FreeMsg
	// FullName:  tagRIM30MSG::FreeMsg
	// Access:    public static 
	// Returns:   void
	// Qualifier:								用于安全释放消息数据包
	// Parameter: tagRIM30MSG * pMsg			消息数据包指针
	//************************************
	static	void								FreeMsg(tagRIM30MSG* pMsg);

	/*获取发送方的GUID*/
	string										GetSendGuid()	{	return	sendGUID;	}
	/*获取接收方的GUID*/
	string										GetRecvGuid()	{	return	recvGUID;	}
	/*获取消息ID*/
	DWORD										GetMsgID()		{	return	msgID;		}
	/*获取消息数据区的消息类型*/
	DWORD										GetDataType()	{	return	dataType;	}
	/*获取消息数据区的数据长度*/
	DWORD										GetDataLength()	{	return	dataLength;	}
	/*获取整个消息数据包的总长度*/
	DWORD										GetTotalLength(){	return	(sizeof(char) * 64 * 2 + sizeof(DWORD) * 3 + dataLength);	}
	/*获取消息数据区的数据指针*/
	LPBYTE										GetDataBuf()	{	return	data;		}

}RIM30MSG, *LPRIM30MSG;


inline tagRIM30MSG* tagRIM30MSG::MakeMsg(const char *pSendGuid, const char *pRecvGuid, DWORD msgID, DWORD dataType, LPBYTE pData, DWORD dataLength)
{
	if(pSendGuid == NULL)				return NULL;
	if(pData == NULL && dataLength > 0)	return NULL;

	if(pSendGuid)
	{
		tagRIM30MSG *pMsg = (tagRIM30MSG*)malloc(sizeof(char) * 64 * 2 + sizeof(DWORD) * 3 + dataLength);

		memcpy_s(pMsg->sendGUID, 64, pSendGuid, 64);
		if(pRecvGuid)	memcpy_s(pMsg->recvGUID, 64, pRecvGuid, 64);

		pMsg->msgID			= msgID;
		pMsg->dataType		= dataType;
		pMsg->dataLength	= dataLength;

		if(pData && dataLength > 0)
			memcpy_s(pMsg->data, dataLength, pData, dataLength);

		return pMsg;
	}
	else
		return NULL;

}

inline void tagRIM30MSG::FreeMsg(tagRIM30MSG* pMsg)
{
	if(pMsg)
		free(pMsg);
}


#pragma pack(pop)


///////////////////////////////////////////////////////////////////////////CRIM30MsgQueue//////////////////////////////////////////////
class CRIM30MsgQueue
{
public:
	CRIM30MsgQueue(void);
	~CRIM30MsgQueue(void);

protected:

	CList<LPRIM30MSG, LPRIM30MSG>		m_dataList;
	HANDLE								m_hEvent;
	CCriticalSection					m_cs;

public:
	/*获取队列触发事件句柄*/
	HANDLE								GetQueueEvent();
	/*压入消息到消息队列*/
	void								Push(LPRIM30MSG pMsg, bool bHead = false);
	/*弹出消息从消息队列中*/
	int									Pop(LPRIM30MSG *ppMsg);
	/*获取消息队列中的消息个数*/
	int									GetQueueCount();
};

inline CRIM30MsgQueue::CRIM30MsgQueue(void)
{
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

inline CRIM30MsgQueue::~CRIM30MsgQueue(void)
{
	if(m_hEvent != NULL)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}

inline void CRIM30MsgQueue::Push(LPRIM30MSG pMsg, bool bHead /*= false*/)
{
	ASSERT(pMsg);

	CSingleLock sLock(&m_cs, TRUE);

	if(bHead)
		m_dataList.AddHead(pMsg);
	else
		m_dataList.AddTail(pMsg);

	SetEvent(m_hEvent);
}

inline int CRIM30MsgQueue::Pop(LPRIM30MSG *ppMsg)
{
	*ppMsg = NULL;
	int count = m_dataList.GetCount();
	if(count <= 0)
		return count;

	CSingleLock sLock(&m_cs, TRUE);

	*ppMsg = m_dataList.RemoveHead();
	count--;

	if(count == 0)
		ResetEvent(m_hEvent);

	return count;
}

inline HANDLE CRIM30MsgQueue::GetQueueEvent()
{
	return m_hEvent;
}

inline int	CRIM30MsgQueue::GetQueueCount()
{
	CSingleLock sLock(&m_cs, TRUE);

	return m_dataList.GetCount();
}
