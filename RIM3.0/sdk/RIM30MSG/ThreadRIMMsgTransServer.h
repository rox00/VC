//********************************************************************
// THREADRIMMSGTRANSSERVER.H �ļ�ע��
// �ļ��� ��: THREADRIMMSGTRANSSERVER.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// ���� ����: ����
// ����ʱ�� : 2015/4/7 14:05
// �ļ����� : 
//*********************************************************************
#pragma once
#include "IRIM30MsgServer.h"	
#include ".\Common\RIM30MsgDefine.h"
#include <vector>
#include <list>
#include <set>
#include <TlHelp32.h>
#include "Common\Job.h"
using namespace std;

#define	RIM30MsgServer_JOBNAME		TEXT("{043B1DF8-16ED-4c1e-AAB4-43B9645EE166}")
// ����I/O�����Ľṹ��
typedef struct tagIOOVERLAPPED 
{
	OVERLAPPED						overlapped;					// �ص��ṹ
	WSABUF							cacheBuf;					// ����������
	char							buffer[DATA_BUFSIZE];		// ����������

	DWORD							sendTolLength;				// ���������ܳ���
	DWORD							packSEND;					// һ���������͵��ֽ�����һ�����ݿ���δ���꣩
	DWORD							bytesSEND;                  // �����ֽ���  �ѷ���������λ�ֽ���

	DWORD							bytesRECV;                  // ���յ��ֽ���    

	BOOL							bRecvOrSend;				// TRUE  Ϊ����  FALSEΪ���� 
}IOOVERLAPPED, *LPIOOVERLAPPED;

// �׽��־���ṹ�� ��ϵͳ�����Ϣ
typedef struct tagSUBSOCKINFO
{
	tagSUBSOCKINFO()
	{
		socket						= INVALID_SOCKET;
		processID					= 0;
		pRecvOverlapped 			= new IOOVERLAPPED;
		pSendOverlapped 			= new IOOVERLAPPED;

		ZeroMemory(guid, 64);
		recvTotalByte				= 0;
		sendTotalByte				= 0;
		IOTotalCnt					= 0;
		bDelete						= false;
	}
	~tagSUBSOCKINFO()
	{
		if(pRecvOverlapped) delete pRecvOverlapped;
		if(pSendOverlapped) delete pSendOverlapped;

		LPRIM30MSG pMsg = NULL;
		while(tcpSendQueue.GetQueueCount())
		{	//�����Ϣ����
			tcpSendQueue.Pop(&pMsg);
			RIM30MSG::FreeMsg(pMsg);
		}
	}

	SOCKET							socket;						// ����ϵͳ��������׽���
	DWORD							processID;					// ����ϵͳ������Ľ���ID
	char 							guid[64];					// ��ϵͳGUID
	LPIOOVERLAPPED					pRecvOverlapped;			// ʶ����յ�IO�ṹ
	LPIOOVERLAPPED					pSendOverlapped;			// ʶ���͵�IO�ṹ

	long volatile					IOTotalCnt;					//��ǰ�ۻ���IO��������			important
	bool							bDelete;					//���������ɾ��				important
	string							deleteInfo;					//ָʾɾ����ԭ��

	CMutex							sendLock;					//��ÿ�����ͽ��м�������		important
	CRIM30MsgQueue					tcpSendQueue;				//������Ϣ����					important

	__int64							recvTotalByte;				//��ǰ�ۻ��Ľ��ռ�������
	__int64							sendTotalByte;				//��ǰ�ۻ��ķ��ͼ�������
}SUBSOCKINFO, *LPSUBSOCKINFO;

////���ڴ����û��Բ�ͬ�ķ�ʽ������ ����������������Ϣ ����Ϣ�ṹ
typedef class tagUSERMSGINFO 
{
public:
	tagUSERMSGINFO(DWORD processIDTemp, LPRIM30MSG pMsgTemp)
	{	////Ϊ��������Ϣ������Ҫ���ݽ���ID���з���
		processID					= processIDTemp;
		clk							= clock();
		pMsg						= pMsgTemp;
	}

	tagUSERMSGINFO(LPRIM30MSG pMsgTemp)
	{	////Ϊ��������Ϣ������ֱ�ӷ���
		processID					= 0xFFFFFFFF;
		clk							= clock();
		pMsg						= pMsgTemp;
	}

	~tagUSERMSGINFO()
	{
		RIM30MSG::FreeMsg(pMsg);
	}

public:
	LPRIM30MSG						pMsg;
	DWORD							processID;
	clock_t							clk;
}USERMSGINFO, *LPUSERMSGINFO;

class CRIM30MsgTransServer : public IRIM30MsgServer
{
private:
	static UINT __stdcall			ServerWorkerThread(LPVOID pVoid);
	static UINT __stdcall			ThreadSendData(void *pDt);
	static UINT						__ThreadProc(LPVOID lpParam);
public:
	CRIM30MsgTransServer(void);
	virtual ~CRIM30MsgTransServer(void);

protected:
	CWinThread						*m_pThread;
	HANDLE							m_hSendDataThread;
	HANDLE							m_hExit;
	BOOL							m_bInitCom;

	HANDLE							m_threadHandles[64];
	string							m_serverGuid;
	HANDLE							m_completionPort;			// ��ɶ˿ھ��
	short							m_serverPort;
#ifdef DEBUG						//��������DEBUGʱ���߳��������˳�ʱ��һЩ��Ϣ���
	CString							m_strName;
	CString							GetName();
	void							GetName(LPCTSTR lpName);
#endif

protected:
	CJob							m_job;						////���ڹ�������ϵͳEXE����ҵ����
	CReadWriteLock					m_socketInfolock;			////��д��
	vector<LPSUBSOCKINFO>			m_subSockInfoVec;			////����������ϵͳ����

	CRIM30MsgQueue					m_userSendQueue;


	//************************************
	// Method:    GetNewGuid
	// FullName:  CRIM30MsgTransServer::GetNewGuid
	// Access:    protected 
	// Returns:   std::string		����GUID�ַ���
	// Qualifier:					�����µ�GUID
	//************************************
	string							CreateNewGuid();
	//************************************
	// Method:    ThreadProc
	// FullName:  CRIM30MsgTransServer::ThreadProc
	// Access:    virtual protected 
	// Returns:   UINT				����ֵ
	// Qualifier:					ѭ����������������̺߳��� 
	//************************************
	virtual UINT					ThreadProc();
	//************************************
	// Method:    SelectSocketRead
	// FullName:  CRIM30MsgTransServer::SelectSocketRead
	// Access:    protected static 
	// Returns:   int				��Winsock��select��������ֵͬ�壬����0ʱ��ʾ�пɲ����׽���
	// Qualifier:					�ȴ����ж��׽����Ƿ�ɶ�
	// Parameter: SOCKET sock		socket
	// Parameter: __in DWORD dwMilliseconds		�ȴ���ã���λΪms
	//************************************
	static int						SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds);
	//************************************
	// Method:    SelectSocketWrite
	// FullName:  CRIM30MsgTransServer::SelectSocketWrite
	// Access:    protected static 
	// Returns:   int				��Winsock��select��������ֵͬ�壬����0ʱ��ʾ�пɲ����׽���
	// Qualifier:					�ȴ����ж��׽����Ƿ��д
	// Parameter: SOCKET sock		socket
	// Parameter: __in DWORD dwMilliseconds		�ȴ���ã���λΪms
	//************************************
	static int						SelectSocketWrite(SOCKET sock, __in DWORD dwMilliseconds);
	//************************************
	// Method:    SendPackByRemainLen
	// FullName:  CRIM30MsgTransServer::SendPackByRemainLen
	// Access:    protected static 
	// Returns:   bool				���ͳɹ�����TRUE��ʧ�ܷ���FALSE
	// Qualifier:					��bytesSENDƫ�ƺ���һ�����ݣ��Ӹð�����ƫ��packSENDλ����ʼ����ʣ�������
	// Parameter: LPSUBSOCKINFO pSockInfo	��ϵͳ�׽�����Ϣ
	//************************************
	static bool						SendPackByRemainLen(LPSUBSOCKINFO pSockInfo);

	static bool						SendPack(LPRIM30MSG pMSG, LPSUBSOCKINFO pSockInfo);
	static bool						ProcessMsg(LPRIM30MSG pMsg, LPSUBSOCKINFO pSockInfo, CRIM30MsgTransServer *pServer);

	//************************************
	// Method:    SetPriority
	// FullName:  CRIM30MsgTransServer::SetPriority
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:					���÷�����߳����ȼ�
	// Parameter: int nPriority		���ȼ��ļ���
	//************************************
	BOOL							SetPriority(int nPriority);
	bool							IsProgramExist(LPCTSTR lpszExeName, DWORD processID);
public:
	//************************************
	// Method:    Begin
	// FullName:  CRIM30MsgTransServer::Begin
	// Access:    virtual public 
	// Returns:   HRESULT			�ɹ�������S_OK,ʧ�ܣ�����S_FALSE;
	// Qualifier:					��������
	// Parameter: short serverPort	����˰󶨶˿�
	//************************************
	virtual HRESULT					Begin(short serverPort = 0);
	//************************************
	// Method:    End
	// FullName:  CRIM30MsgTransServer::End
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					����������շ�����
	//************************************
	virtual void					End();

	virtual BOOL					OpenSubSystem(string exePath = "", LPRIM30MSG pInitMsg = NULL);
	virtual void					CloseAllSubSystem();
	virtual UINT					GetSubSystemCnt();
	virtual string					GetSubSystemGuid(UINT index);
	virtual void					PushSendMsg(LPRIM30MSG pMsg);
};
