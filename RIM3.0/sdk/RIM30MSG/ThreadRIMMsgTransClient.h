//********************************************************************
//	USERTHREAD.H 文件注释
//	文件名		: USERTHREAD.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\INCLUDE/
//	作者		: 张永
//	创建时间	: 2015/1/13 14:40
//	文件描述	: 
//*********************************************************************	
#pragma once
#include ".\Common\Ping.h"
#include ".\Common\RIM30MsgDefine.h"
#include "IRIM30MsgClient.h"

typedef struct tagRIM30MSGTRANSCLIENTINFO
{
	tagRIM30MSGTRANSCLIENTINFO()
	{
		m_cntHeartSend	= 0;
		m_cntHeartRecv	= 0;
		m_cntMsgSend	= 0;
		m_cntMsgRecv	= 0;

		socket			= INVALID_SOCKET;
		serverIp		= "127.0.0.1";
		serverPort		= 0;
	}

	~tagRIM30MSGTRANSCLIENTINFO()
	{
		socket		= INVALID_SOCKET;
		serverIp	= "";
		serverPort	= 0;

		while(tcpRecvQueue.GetQueueCount())
		{	//清空消息队列
			LPRIM30MSG pData = NULL;
			tcpRecvQueue.Pop(&pData);
			RIM30MSG::FreeMsg(pData);
		}

		while(tcpSendQueue.GetQueueCount())
		{	//清空消息队列
			LPRIM30MSG pData = NULL;
			tcpSendQueue.Pop(&pData);
			RIM30MSG::FreeMsg(pData);
		}
	}
	__int64							m_cntHeartSend;
	__int64							m_cntHeartRecv;
	__int64							m_cntMsgSend;
	__int64							m_cntMsgRecv;

	string							serverIp;
	short							serverPort;
	SOCKET							socket;
	CRIM30MsgQueue					tcpSendQueue;
	CRIM30MsgQueue					tcpRecvQueue;

}RIM30MSGTRANSCLIENTINFO, *LPRIM30MSGTRANSCLIENTINFO;

class CThreadRIM30MsgTransRecv
{
private:
	static UINT						__ThreadProc(LPVOID lpParam);

public:
	CThreadRIM30MsgTransRecv(void);
	virtual ~CThreadRIM30MsgTransRecv(void);

protected:
	CWinThread						*m_pThread;
	HANDLE							m_hExit;
	BOOL							m_bInitCom;
	LPRIM30MSGTRANSCLIENTINFO		m_pInfo;

#ifdef DEBUG						//这里用于DEBUG时对线程启动和退出时的一些信息输出
	CString							m_strName;
	CString							GetName();
	void							GetName(LPCTSTR lpName);
#endif

protected:
	virtual UINT					ThreadProc();

public:
	virtual HRESULT					Begin(LPRIM30MSGTRANSCLIENTINFO pInfo, BOOL bInitCom = FALSE);
	virtual void					End();
	BOOL							SetPriority(int nPriority);

};


class CThreadRIM30MsgTransSend
{
private:
	static UINT						__ThreadProc(LPVOID lpParam);

public:
	CThreadRIM30MsgTransSend(void);
	virtual ~CThreadRIM30MsgTransSend(void);

protected:
	CWinThread						*m_pThread;
	HANDLE							m_hExit;
	BOOL							m_bInitCom;
	LPRIM30MSGTRANSCLIENTINFO		m_pInfo;

#ifdef DEBUG						//这里用于DEBUG时对线程启动和退出时的一些信息输出
	CString							m_strName;
	CString							GetName();
	void							GetName(LPCTSTR lpName);
#endif

protected:
	virtual UINT					ThreadProc();

public:
	virtual HRESULT					Begin(LPRIM30MSGTRANSCLIENTINFO pInfo, BOOL bInitCom = FALSE);
	virtual void					End();
	BOOL							SetPriority(int nPriority);
};

class CRIM30MsgTransClient : public IRIM30MsgClient
{
public:
	RIM30MSGTRANSCLIENTINFO			m_info;
	CRIM30MsgTransClient(void);
	virtual ~CRIM30MsgTransClient(void);

public:
	static int						SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds);
	static int						SelectSocketWrite(SOCKET sock, __in DWORD dwMilliseconds);
	static bool 					ReciveTCPBufferBlock(SOCKET sock, RIM30TCPBLOCK& bufBlock, __in DWORD dwMilliseconds);
	static bool 					SendTCPBufferByBlock(SOCKET sock, LPBYTE pBuffer, int bufLen);

protected:
	CThreadRIM30MsgTransRecv		m_threadRecv;
	CThreadRIM30MsgTransSend		m_threadSend;

public:
	virtual HRESULT					Begin(string serverIp = "127.0.0.1", short serverPort = 0);
	virtual void					End();
	virtual bool					IsConnected();
	virtual void					PushSendMsg(LPRIM30MSG pMsg);
	virtual void					PopRecvMsg(LPRIM30MSG* pMsg);

};
