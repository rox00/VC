#pragma once

// CChatMonitorThread

class CThreadUDPMonitorRecive : public CUserThread
{
public:
	virtual ~CThreadUDPMonitorRecive();

	enum	{RECIVEBUFLEN = 4096};
protected:
	CThreadUDPMonitorRecive();

	virtual UINT								ThreadProc();

	LPBYTE										m_pBuf;

	SOCKET										m_hChatMonitorSocket;
	static CThreadUDPMonitorRecive*				m_pChatMonitorReciveThread;	//�ڲ�ʵ��ָ��
public:
	static CThreadUDPMonitorRecive* 			GetInstance();
	static void 								ReleaseInstance();
};


class CThreadUDPMonitorSend : public CUserThread
{
public:
	CThreadUDPMonitorSend();
	virtual ~CThreadUDPMonitorSend();

	enum	{SENDBUFLEN = 4096};
protected:

	virtual UINT								ThreadProc();

	SOCKET										m_chatMonitorSocket;

	static CThreadUDPMonitorSend*				m_pChatMonitorSendThread;	//�ڲ�ʵ��ָ��
public:
	static CThreadUDPMonitorSend* 				GetInstance();
	static void 								ReleaseInstance();
};


