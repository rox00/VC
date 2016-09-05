#pragma once

enum WM_SVC_NOTIFY
{
	WM_NOTIFY_CONNECT_STATE_CHANGED	= WM_USER + 10000	,
	WM_NOTIFY_DEVICE_MESSAGE_RECEIVED					,
	WM_NOTIFY_VIDEO_READY								,
	WM_NOTIFY_RESTART									,
};

enum ConnectStatus
{
	DISCONNECT = 0,
	CONNECTING,
	CONNECTED
};

enum BoxFlags
{
	BoxFlags_IsOpenDeviceControl = 1,
	BoxFlags_IsHorizontalMode = 2,
};
extern BOOL										g_IsOpenDeviceControl;
extern BOOL										g_IsHorizontalMode;
extern long										g_currentDeviceWidth;
extern long										g_currentDeviceHeight;



////����

#pragma pack(push, 1)
//���ݾݰ�����

enum PackTypeEnum
{
	/**********************״̬����UDP��0~99*****************************/
	PackTypeEnum_clientidle          = 0,
	PackTypeEnum_clientbusy          = 1,
	PackTypeEnum_serveridle          = 2,
	PackTypeEnum_serverbusy          = 3,
	PackTypeEnum_ImgData             = 99,

	//PackTypeEnum_rebootask           = 50,          //ȥ��
	//PackTypeEnum_rebootcan           = 51,          //ȥ��
	//PackTypeEnum_rebootok            = 52,          //ȥ��

	//PackTypeEnum_askconnect          = 4,           //ȥ��
	//PackTypeEnum_allowconnect        = 5,           //ȥ��
	//PackTypeEnum_refuseconnect       = 6,           //ȥ��
	//PackTypeEnum_forcereboot         = 1000,        //ȥ��

	/**********************�������������ӣ�100~199*****************************/
	PackTypeEnum_installfile        = 100,         //��װ�ļ�
	PackTypeEnum_installok          = 101,         //��װ�ļ��ɹ�
	PackTypeEnum_installfail        = 102,         //��װ�ļ�ʧ��

	PackTypeEnum_rebootask          = 103,         //���������豸
	PackTypeEnum_rebootok           = 104,         //�����ɹ�
	PackTypeEnum_rebootfail         = 105,         //����ʧ��

	/**********************��Ƶ���ƣ������ӣ�200~255*****************************/
	PackTypeEnum_VEDIO			    = 200,			//��Ƶ 
	PackTypeEnum_MOUSE			    = 201,			//���
	PackTypeEnum_KEYBOARD		    = 202,			//����
};

//��꣨ģ����ָ��״̬
enum TOUCHTYPE
{
	TOUCHTYPE_DOWN			= 1,			//����
	TOUCHTYPE_UP			= 2,			//����
	TOUCHTYPE_MOVE			= 3,			//�ƶ�				
};
//����״̬
enum KEYACTIONTYPE
{
	KEYACTION_DOWN			= 1,			//����
	KEYACTION_UP			= 2,			//����
	KEYACTION_FUNC			= 3,			//���ܼ�
};
//���ܼ�ֵ
enum KEYFUCVALUE
{					
	KEYFUC_HOME				= 10001,		//������ҳ
	KEYFUC_RETURN			= 10002,		//���ذ���
	KEYFUC_TASKMGR			= 10003,		//���������
	KEYACTION_CAPSLOCKON	= 10004,		//��д��
	KEYACTION_CAPSLOCKOFF	= 10005,		//��д�ر�
};
//��ͷ
typedef struct tagPACKAGEHEAD
{
	BYTE					head1;			//0xff
	BYTE					head2;			//0xfe
	BYTE					packType;		//������
	INT						len;			//������
}PACKAGEHEAD;
////������ṹ
//typedef struct tagNETPACKET
//{
//	PACKAGEHEAD				packhead;		//��ͷ
//	CHAR*					bodybuff;		//�洢����
//}NETPACKET;
//�����Ϣ��
typedef struct tagMOUSEPACKET
{
	USHORT					x;				//x����
	USHORT					y;				//y����
	BYTE					touchType;		//��������
	INT						nDelayTime;		//�ӳ�ʱ��  ms��λ
	BYTE					nFingerId;		//��ָID

}MOUSEPACKET;
//������Ϣ��
typedef struct tagKEYPACKET
{
	INT32					key;			//���������ֵ
	BYTE					keyAction;		//��������

}KEYPACKET;

#pragma pack(pop)