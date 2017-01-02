//********************************************************************
// RIM30MSGDEFINE.H �ļ�ע��
// �ļ��� ��: RIM30MSGDEFINE.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 11:56
// �ļ����� : 
//*********************************************************************

#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
using namespace std;
#include "ReadWriteLock.h"

////��zy 2015-04-27 ����							��ϵͳ����Ψһ��GUIDԤ�ȶ���
#define RIM30GUID_INITPROCESS					"{E98CCD85-88BF-47c2-8C03-EC3C0633CCAC}"	////��ʼ�����̷�����Ϣ��GUID
////��zy 2015-04-27 ����

#define RIM30MSG_HEART_PACK						0xFFFFFFFF					////��������ϢID

/************************************������ά����Ϣ��ID:0~100��:Begin**********************************/
#define RIM30MSG_ONLINE_ASK						1							////��ϵͳ��������
#define RIM30MSG_ONLINE_ANSWER					2							////��ϵͳ����Ӧ��
#define RIM30MSG_OFFLINE_ASK					3							////��ϵͳ��������
#define RIM30MSG_OFFLINE_ANSWER					4							////��ϵͳ����Ӧ��
#define RIM30MSG_SHUTDOWN_ASK					5							////�ر�����
#define RIM30MSG_SHUTDOWN_ANSWER				6							////�ر�Ӧ��
#define RIM30MSG_BROADCAST						7							////ͨ�ù㲥��Ϣ
#define	RIM30MSG_QUERYSERVERADDR_ASK			8							////��ѯ�����ַ��Ϣ����
#define	RIM30MSG_QUERYSERVERADDR_ANSWER			9							////��ѯ�Ƿ����ַ��ϢӦ��

/************************************������ά����Ϣ��ID:0~100��:End**********************************/

/************************************������ά����Ϣ��ID:200~399��:Begin**********************************/
#define RIM30MSG_MEASUREDF_START_ASK			200							////������������
#define RIM30MSG_MEASUREDF_START_ANSWER			201							////��������Ӧ��


/************************************������ά����Ϣ��ID:200~399��:End**********************************/

////��zy 2015-06-05 ����		////����   ������IP��ѯ��ʶ��
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
////��zy 2015-06-05 ����

#pragma pack(push, 1)

const int	RIM30TCPBLOCKDATALEN = 1440;
typedef struct tagRIM30TCPBLOCK
{
	UINT 										_fileSize;					//������buffer���ļ����ܴ�С
	UINT 										_offset;					//������ܵĻ�������ǰ���ݵ�ƫ����
	UINT 										_len;						//��ǰ������Ч���ļ������ݵĳ���
	BYTE 										_buf[RIM30TCPBLOCKDATALEN];	//������ݵ�buf
}RIM30TCPBLOCK, *LPRIM30TCPBLOCK;

#define DATA_BUFSIZE							1024*16						// ���ͺͽ�����Ϣ����󳤶� 16KB
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

	char 										sendGUID[64];				//���ͷ�GUID
	char 										recvGUID[64];				//���շ�GUID
	DWORD										msgID;						//��Ϣ����
	DWORD										dataType;					//��������
	DWORD										dataLength;					//���ݳ���
	BYTE										data[1];					//ָ�����ݻ�����

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
	// Qualifier:								���ڹ��������Ϣ���ݰ�
	// Parameter: const char * pSendGuid		���ͷ���GUID
	// Parameter: const char * pRecvGuid		���շ���GUID
	// Parameter: DWORD msgID					��ϢID
	// Parameter: DWORD dataType				��Ϣ����������������
	// Parameter: LPBYTE pData					���ڹ�����Ϣ������������
	// Parameter: DWORD dataLength				��Ϣ�����������ݳ���
	//************************************
	static tagRIM30MSG*							MakeMsg(const char *pSendGuid, const char *pRecvGuid, DWORD msgID, DWORD dataType, LPBYTE pData, DWORD dataLength);
	//************************************
	// Method:    FreeMsg
	// FullName:  tagRIM30MSG::FreeMsg
	// Access:    public static 
	// Returns:   void
	// Qualifier:								���ڰ�ȫ�ͷ���Ϣ���ݰ�
	// Parameter: tagRIM30MSG * pMsg			��Ϣ���ݰ�ָ��
	//************************************
	static	void								FreeMsg(tagRIM30MSG* pMsg);

	/*��ȡ���ͷ���GUID*/
	string										GetSendGuid()	{	return	sendGUID;	}
	/*��ȡ���շ���GUID*/
	string										GetRecvGuid()	{	return	recvGUID;	}
	/*��ȡ��ϢID*/
	DWORD										GetMsgID()		{	return	msgID;		}
	/*��ȡ��Ϣ����������Ϣ����*/
	DWORD										GetDataType()	{	return	dataType;	}
	/*��ȡ��Ϣ�����������ݳ���*/
	DWORD										GetDataLength()	{	return	dataLength;	}
	/*��ȡ������Ϣ���ݰ����ܳ���*/
	DWORD										GetTotalLength(){	return	(sizeof(char) * 64 * 2 + sizeof(DWORD) * 3 + dataLength);	}
	/*��ȡ��Ϣ������������ָ��*/
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
	/*��ȡ���д����¼����*/
	HANDLE								GetQueueEvent();
	/*ѹ����Ϣ����Ϣ����*/
	void								Push(LPRIM30MSG pMsg, bool bHead = false);
	/*������Ϣ����Ϣ������*/
	int									Pop(LPRIM30MSG *ppMsg);
	/*��ȡ��Ϣ�����е���Ϣ����*/
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
