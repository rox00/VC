//********************************************************************
// IRIM30MSGCLIENT.H 文件注释
// 文件名 　: IRIM30MSGCLIENT.H
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 11:54
// 文件描述 : 客户端接口
//*********************************************************************

#pragma once
#include ".\Common\RIM30MsgDefine.h"

// IRIM30MsgClient

interface IRIM30MsgClient
{
	virtual ~IRIM30MsgClient(){};
	//************************************
	// Method:    Begin
	// FullName:  IRIM30MsgClient::Begin
	// Access:    virtual public 
	// Returns:   HRESULT				S_OK：执行成功，S_FALSE：执行失败
	// Qualifier:						连接到服务端并启动内部收发线程进行接收或发送消息
	// Parameter: string serverIp		服务端IP
	// Parameter: short serverPort		服务端端口
	//************************************
	virtual HRESULT						Begin(string serverIp = "127.0.0.1", short serverPort = 0) = 0;
	//************************************
	// Method:    End
	// FullName:  IRIM30MsgClient::End
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:						结束客户端的收发机制，释放所占用资源
	//************************************
	virtual void						End() = 0;
	//************************************
	// Method:    IsConnected
	// FullName:  IRIM30MsgClient::IsConnected
	// Access:    virtual public 
	// Returns:   bool					true：正常  false：连接已断开
	// Qualifier:						判断与服务端的连接是否正常
	//************************************
	virtual bool						IsConnected() = 0;
	//************************************
	// Method:    PushSendMsg
	// FullName:  IRIM30MsgClient::PushSendMsg
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:						将消息结构压入发送队列 
	// Parameter: LPRIM30MSG pMsg		消息结构指针【由用户构造的消息数据包在压入队列后，由系统管理释放】
	//************************************
	virtual void						PushSendMsg(LPRIM30MSG pMsg) = 0;
	//************************************
	// Method:    PopRecvMsg
	// FullName:  IRIM30MsgClient::PopRecvMsg
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:						从接收消息队列中取出最先到达的消息包 
	// Parameter: LPRIM30MSG * pMsg		消息结构指针【用户取出消息数据包后需用户手动释放。】
	//************************************
	virtual void						PopRecvMsg(LPRIM30MSG* pMsg) = 0;
};

//************************************
// Method:    GetRIM30Client
// FullName:  GetRIM30Client
// Access:    public 
// Returns:   IRIM30MsgClient *
// Qualifier:							获取客户端对象指针
//************************************
IRIM30MsgClient *						GetRIM30Client();

