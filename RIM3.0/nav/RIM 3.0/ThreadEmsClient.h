#pragma once

class CThreadEmsClient : public CUserThread
{
public:
	virtual ~CThreadEmsClient();

protected:
	CThreadEmsClient();

	virtual UINT								ThreadProc();


	SOCKET										m_hEmsSocket;
	static CThreadEmsClient*					m_pThreadEmsClient;	//�ڲ�ʵ��ָ��
	void										SendMsgToEms(SOCKET sock, std::string strSend,BYTE bCommType = 1);  //1 ��ѯ����ָ�� 2 ����ҵ��ָ��
	void										Parse(LPBYTE pBufferStart, DWORD bufSize);
public:
	static CThreadEmsClient* 					GetInstance();
	static void 								ReleaseInstance();
	virtual HRESULT								Begin(string serverIp = "127.0.0.1", short serverPort = 0);
};
