//********************************************************************
// IRIM30MSGSERVER.H 文件注释
// 文件名 　: IRIM30MSGSERVER.H
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 11:54
// 文件描述 : 服务端接口
//*********************************************************************
#pragma once
#include ".\Common\RIM30MsgDefine.h"

// IRIM30MsgClient

interface IRIM30MsgServer
{
	virtual ~IRIM30MsgServer(void){};
	//************************************
	// Method:    Begin
	// FullName:  IRIM30MsgServer::Begin
	// Access:    virtual public 
	// Returns:   HRESULT
	// Qualifier:					启动服务端运行机制，并分配相关资源
	// Parameter: BOOL bInitCom
	//************************************
	virtual HRESULT					Begin(short serverPort = 0)			= 0;
	//************************************
	// Method:    End
	// FullName:  IRIM30MsgServer::End
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					结束服务端运行机制，并清理所有分配资源
	//************************************
	virtual void					End()								= 0;
	//************************************
	// Method:    PushSendMsg
	// FullName:  IRIM30MsgClient::PushSendMsg
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					将消息结构压入发送队列 
	// Parameter: LPRIM30MSG pMsg	消息结构指针【由用户构造的消息数据包在压入队列后，由系统管理释放】
	//************************************
	virtual void					PushSendMsg(LPRIM30MSG pMsg)		= 0;
	//************************************
	// Method:    OpenSubSystem
	// FullName:  IRIM30MsgServer::OpenSubSystem
	// Access:    virtual public 
	// Returns:   BOOL
	// Qualifier:					打开子系统
	// Parameter: string exePath
	//************************************
	virtual BOOL					OpenSubSystem(string exePath = "", LPRIM30MSG pInitMsg = NULL)	= 0;
	//************************************
	// Method:    CloseAllSubSystem
	// FullName:  IRIM30MsgServer::CloseAllSubSystem
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					关闭所有子系统
	//************************************
	virtual void					CloseAllSubSystem()					= 0;
	//************************************
	// Method:    GetSubSystemCnt
	// FullName:  IRIM30MsgServer::GetSubSystemCnt
	// Access:    virtual public 
	// Returns:   UINT
	// Qualifier:					获取当前子系统的个数
	//************************************
	virtual UINT					GetSubSystemCnt()					= 0;

	virtual string					GetSubSystemGuid(UINT index)		= 0;
	
};

IRIM30MsgServer *					GetRIM30Server();

