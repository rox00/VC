//********************************************************************
// IRIM30MSGCLIENT.H �ļ�ע��
// �ļ��� ��: IRIM30MSGCLIENT.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 11:54
// �ļ����� : �ͻ��˽ӿ�
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
	// Returns:   HRESULT				S_OK��ִ�гɹ���S_FALSE��ִ��ʧ��
	// Qualifier:						���ӵ�����˲������ڲ��շ��߳̽��н��ջ�����Ϣ
	// Parameter: string serverIp		�����IP
	// Parameter: short serverPort		����˶˿�
	//************************************
	virtual HRESULT						Begin(string serverIp = "127.0.0.1", short serverPort = 0) = 0;
	//************************************
	// Method:    End
	// FullName:  IRIM30MsgClient::End
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:						�����ͻ��˵��շ����ƣ��ͷ���ռ����Դ
	//************************************
	virtual void						End() = 0;
	//************************************
	// Method:    IsConnected
	// FullName:  IRIM30MsgClient::IsConnected
	// Access:    virtual public 
	// Returns:   bool					true������  false�������ѶϿ�
	// Qualifier:						�ж������˵������Ƿ�����
	//************************************
	virtual bool						IsConnected() = 0;
	//************************************
	// Method:    PushSendMsg
	// FullName:  IRIM30MsgClient::PushSendMsg
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:						����Ϣ�ṹѹ�뷢�Ͷ��� 
	// Parameter: LPRIM30MSG pMsg		��Ϣ�ṹָ�롾���û��������Ϣ���ݰ���ѹ����к���ϵͳ�����ͷš�
	//************************************
	virtual void						PushSendMsg(LPRIM30MSG pMsg) = 0;
	//************************************
	// Method:    PopRecvMsg
	// FullName:  IRIM30MsgClient::PopRecvMsg
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:						�ӽ�����Ϣ������ȡ�����ȵ������Ϣ�� 
	// Parameter: LPRIM30MSG * pMsg		��Ϣ�ṹָ�롾�û�ȡ����Ϣ���ݰ������û��ֶ��ͷš���
	//************************************
	virtual void						PopRecvMsg(LPRIM30MSG* pMsg) = 0;
};

//************************************
// Method:    GetRIM30Client
// FullName:  GetRIM30Client
// Access:    public 
// Returns:   IRIM30MsgClient *
// Qualifier:							��ȡ�ͻ��˶���ָ��
//************************************
IRIM30MsgClient *						GetRIM30Client();

