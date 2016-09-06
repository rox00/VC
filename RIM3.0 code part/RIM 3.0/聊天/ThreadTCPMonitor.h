#pragma once

// CChatMonitorThread

class CThreadTCPMonitorRecive : public CUserThread
{
public:
	virtual ~CThreadTCPMonitorRecive();

protected:
	CThreadTCPMonitorRecive();

	virtual UINT								ThreadProc();

	SOCKET										m_hChatMonitorSocket;
	static CThreadTCPMonitorRecive*				m_pChatMonitorReciveThread;	//内部实例指针
public:
	static CThreadTCPMonitorRecive* 			GetInstance();
	static void 								ReleaseInstance();
	static UINT __stdcall						ThreadSrvCnnct(void *pDt);
	static void									ReceiveFile(SOCKET &sockCnn, CQueueData *pQueueData);
};


class CThreadTCPMonitorSend : public CUserThread
{
public:
	CThreadTCPMonitorSend();
	virtual ~CThreadTCPMonitorSend();

protected:

	static const int							m_constBufLen	= 0x1000000;
	static char									*m_pSendFileBuffer;

	virtual UINT								ThreadProc();

	SOCKET										m_chatMonitorSocket;

	static CThreadTCPMonitorSend*				m_pChatMonitorSendThread;	//内部实例指针
public:
	static CThreadTCPMonitorSend* 				GetInstance();
	static void 								ReleaseInstance();
	static UINT __stdcall						ThreadSendData(void *pDt);
	static void									SendFile(tstring dircOrPath, SOCKET &chatMonitorSocket, LPVOID pData, clock_t beginClr/* = 0*/);
};

