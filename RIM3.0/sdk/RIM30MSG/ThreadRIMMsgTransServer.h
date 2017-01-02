//********************************************************************
// THREADRIMMSGTRANSSERVER.H 文件注释
// 文件名 　: THREADRIMMSGTRANSSERVER.H
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// 作者 　　: 张永
// 创建时间 : 2015/4/7 14:05
// 文件描述 : 
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
// 定义I/O操作的结构体
typedef struct tagIOOVERLAPPED 
{
	OVERLAPPED						overlapped;					// 重叠结构
	WSABUF							cacheBuf;					// 缓冲区对象
	char							buffer[DATA_BUFSIZE];		// 缓冲区数组

	DWORD							sendTolLength;				// 发送数据总长度
	DWORD							packSEND;					// 一个整包发送的字节数（一包数据可能未发完）
	DWORD							bytesSEND;                  // 发送字节数  已发送整包单位字节数

	DWORD							bytesRECV;                  // 接收的字节数    

	BOOL							bRecvOrSend;				// TRUE  为接收  FALSE为发送 
}IOOVERLAPPED, *LPIOOVERLAPPED;

// 套接字句柄结构体 子系统相关信息
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
		{	//清空消息队列
			tcpSendQueue.Pop(&pMsg);
			RIM30MSG::FreeMsg(pMsg);
		}
	}

	SOCKET							socket;						// 与子系统相关联的套接字
	DWORD							processID;					// 与子系统相关联的进程ID
	char 							guid[64];					// 子系统GUID
	LPIOOVERLAPPED					pRecvOverlapped;			// 识别接收的IO结构
	LPIOOVERLAPPED					pSendOverlapped;			// 识别发送的IO结构

	long volatile					IOTotalCnt;					//当前累积的IO计数请求			important
	bool							bDelete;					//接收引起的删除				important
	string							deleteInfo;					//指示删除的原因

	CMutex							sendLock;					//对每个发送进行加锁处理		important
	CRIM30MsgQueue					tcpSendQueue;				//发送消息队列					important

	__int64							recvTotalByte;				//当前累积的接收计数请求
	__int64							sendTotalByte;				//当前累积的发送计数请求
}SUBSOCKINFO, *LPSUBSOCKINFO;

////用于处理用户以不同的方式及条件 来进行主动发送消息 的信息结构
typedef class tagUSERMSGINFO 
{
public:
	tagUSERMSGINFO(DWORD processIDTemp, LPRIM30MSG pMsgTemp)
	{	////为非正常消息包，需要根据进程ID进行发送
		processID					= processIDTemp;
		clk							= clock();
		pMsg						= pMsgTemp;
	}

	tagUSERMSGINFO(LPRIM30MSG pMsgTemp)
	{	////为下正常消息包，可直接发送
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
	HANDLE							m_completionPort;			// 完成端口句柄
	short							m_serverPort;
#ifdef DEBUG						//这里用于DEBUG时对线程启动和退出时的一些信息输出
	CString							m_strName;
	CString							GetName();
	void							GetName(LPCTSTR lpName);
#endif

protected:
	CJob							m_job;						////用于管理本地子系统EXE的作业对象
	CReadWriteLock					m_socketInfolock;			////读写锁
	vector<LPSUBSOCKINFO>			m_subSockInfoVec;			////管理所有子系统数组

	CRIM30MsgQueue					m_userSendQueue;


	//************************************
	// Method:    GetNewGuid
	// FullName:  CRIM30MsgTransServer::GetNewGuid
	// Access:    protected 
	// Returns:   std::string		返回GUID字符串
	// Qualifier:					创建新的GUID
	//************************************
	string							CreateNewGuid();
	//************************************
	// Method:    ThreadProc
	// FullName:  CRIM30MsgTransServer::ThreadProc
	// Access:    virtual protected 
	// Returns:   UINT				返回值
	// Qualifier:					循环处理连接请求的线程函数 
	//************************************
	virtual UINT					ThreadProc();
	//************************************
	// Method:    SelectSocketRead
	// FullName:  CRIM30MsgTransServer::SelectSocketRead
	// Access:    protected static 
	// Returns:   int				与Winsock中select函数返回值同义，大于0时表示有可操作套接字
	// Qualifier:					等待并判断套接子是否可读
	// Parameter: SOCKET sock		socket
	// Parameter: __in DWORD dwMilliseconds		等待多久，单位为ms
	//************************************
	static int						SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds);
	//************************************
	// Method:    SelectSocketWrite
	// FullName:  CRIM30MsgTransServer::SelectSocketWrite
	// Access:    protected static 
	// Returns:   int				与Winsock中select函数返回值同义，大于0时表示有可操作套接字
	// Qualifier:					等待并判断套接子是否可写
	// Parameter: SOCKET sock		socket
	// Parameter: __in DWORD dwMilliseconds		等待多久，单位为ms
	//************************************
	static int						SelectSocketWrite(SOCKET sock, __in DWORD dwMilliseconds);
	//************************************
	// Method:    SendPackByRemainLen
	// FullName:  CRIM30MsgTransServer::SendPackByRemainLen
	// Access:    protected static 
	// Returns:   bool				发送成功返回TRUE，失败返回FALSE
	// Qualifier:					在bytesSEND偏移后构造一包数据，从该包数据偏移packSEND位置起始发送剩余包数据
	// Parameter: LPSUBSOCKINFO pSockInfo	子系统套接子信息
	//************************************
	static bool						SendPackByRemainLen(LPSUBSOCKINFO pSockInfo);

	static bool						SendPack(LPRIM30MSG pMSG, LPSUBSOCKINFO pSockInfo);
	static bool						ProcessMsg(LPRIM30MSG pMsg, LPSUBSOCKINFO pSockInfo, CRIM30MsgTransServer *pServer);

	//************************************
	// Method:    SetPriority
	// FullName:  CRIM30MsgTransServer::SetPriority
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:					设置服务端线程优先级
	// Parameter: int nPriority		优先级的级别
	//************************************
	BOOL							SetPriority(int nPriority);
	bool							IsProgramExist(LPCTSTR lpszExeName, DWORD processID);
public:
	//************************************
	// Method:    Begin
	// FullName:  CRIM30MsgTransServer::Begin
	// Access:    virtual public 
	// Returns:   HRESULT			成功：返回S_OK,失败：返回S_FALSE;
	// Qualifier:					启动服务
	// Parameter: short serverPort	服务端绑定端口
	//************************************
	virtual HRESULT					Begin(short serverPort = 0);
	//************************************
	// Method:    End
	// FullName:  CRIM30MsgTransServer::End
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					结束服务端收发机制
	//************************************
	virtual void					End();

	virtual BOOL					OpenSubSystem(string exePath = "", LPRIM30MSG pInitMsg = NULL);
	virtual void					CloseAllSubSystem();
	virtual UINT					GetSubSystemCnt();
	virtual string					GetSubSystemGuid(UINT index);
	virtual void					PushSendMsg(LPRIM30MSG pMsg);
};
