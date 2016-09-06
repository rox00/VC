#pragma once

class CThreadEmsClient : public CUserThread
{
public:
	virtual ~CThreadEmsClient();

protected:
	CThreadEmsClient();

	virtual UINT								ThreadProc();


	SOCKET										m_hEmsSocket;
	static CThreadEmsClient*					m_pThreadEmsClient;	//内部实例指针
	void										SendMsgToEms(SOCKET sock, std::string strSend,BYTE bCommType = 1);  //1 查询外设指令 2 共享业务指令
	void										Parse(LPBYTE pBufferStart, DWORD bufSize);
public:
	static CThreadEmsClient* 					GetInstance();
	static void 								ReleaseInstance();
	virtual HRESULT								Begin(string serverIp = "127.0.0.1", short serverPort = 0);
};
