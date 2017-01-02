//********************************************************************
// IRIM30MSGSERVER.H �ļ�ע��
// �ļ��� ��: IRIM30MSGSERVER.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 11:54
// �ļ����� : ����˽ӿ�
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
	// Qualifier:					������������л��ƣ������������Դ
	// Parameter: BOOL bInitCom
	//************************************
	virtual HRESULT					Begin(short serverPort = 0)			= 0;
	//************************************
	// Method:    End
	// FullName:  IRIM30MsgServer::End
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					������������л��ƣ����������з�����Դ
	//************************************
	virtual void					End()								= 0;
	//************************************
	// Method:    PushSendMsg
	// FullName:  IRIM30MsgClient::PushSendMsg
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					����Ϣ�ṹѹ�뷢�Ͷ��� 
	// Parameter: LPRIM30MSG pMsg	��Ϣ�ṹָ�롾���û��������Ϣ���ݰ���ѹ����к���ϵͳ�����ͷš�
	//************************************
	virtual void					PushSendMsg(LPRIM30MSG pMsg)		= 0;
	//************************************
	// Method:    OpenSubSystem
	// FullName:  IRIM30MsgServer::OpenSubSystem
	// Access:    virtual public 
	// Returns:   BOOL
	// Qualifier:					����ϵͳ
	// Parameter: string exePath
	//************************************
	virtual BOOL					OpenSubSystem(string exePath = "", LPRIM30MSG pInitMsg = NULL)	= 0;
	//************************************
	// Method:    CloseAllSubSystem
	// FullName:  IRIM30MsgServer::CloseAllSubSystem
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:					�ر�������ϵͳ
	//************************************
	virtual void					CloseAllSubSystem()					= 0;
	//************************************
	// Method:    GetSubSystemCnt
	// FullName:  IRIM30MsgServer::GetSubSystemCnt
	// Access:    virtual public 
	// Returns:   UINT
	// Qualifier:					��ȡ��ǰ��ϵͳ�ĸ���
	//************************************
	virtual UINT					GetSubSystemCnt()					= 0;

	virtual string					GetSubSystemGuid(UINT index)		= 0;
	
};

IRIM30MsgServer *					GetRIM30Server();

