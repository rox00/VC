#pragma once

// CChatMonitorThread

class CThreadUDPDataProcess : public CUserThread
{
public:
	virtual ~CThreadUDPDataProcess();

protected:
	CThreadUDPDataProcess();

	virtual UINT								ThreadProc();

	static CThreadUDPDataProcess*				m_pDataProcessReciveThread;	//内部实例指针
public:
	static CThreadUDPDataProcess* 				GetInstance();
	static void 								ReleaseInstance();
};

class CThreadTCPDataProcess : public CUserThread
{
public:
	virtual ~CThreadTCPDataProcess();

protected:
	CThreadTCPDataProcess();

	virtual UINT								ThreadProc();

	static CThreadTCPDataProcess*				m_pDataProcessReciveThread;	//内部实例指针
public:
	static CThreadTCPDataProcess* 				GetInstance();
	static void 								ReleaseInstance();
};
