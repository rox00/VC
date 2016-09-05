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



////网络

#pragma pack(push, 1)
//数据据包类型

enum PackTypeEnum
{
	/**********************状态区（UDP）0~99*****************************/
	PackTypeEnum_clientidle          = 0,
	PackTypeEnum_clientbusy          = 1,
	PackTypeEnum_serveridle          = 2,
	PackTypeEnum_serverbusy          = 3,
	PackTypeEnum_ImgData             = 99,

	//PackTypeEnum_rebootask           = 50,          //去掉
	//PackTypeEnum_rebootcan           = 51,          //去掉
	//PackTypeEnum_rebootok            = 52,          //去掉

	//PackTypeEnum_askconnect          = 4,           //去掉
	//PackTypeEnum_allowconnect        = 5,           //去掉
	//PackTypeEnum_refuseconnect       = 6,           //去掉
	//PackTypeEnum_forcereboot         = 1000,        //去掉

	/**********************功能区（短连接）100~199*****************************/
	PackTypeEnum_installfile        = 100,         //安装文件
	PackTypeEnum_installok          = 101,         //安装文件成功
	PackTypeEnum_installfail        = 102,         //安装文件失败

	PackTypeEnum_rebootask          = 103,         //请求重启设备
	PackTypeEnum_rebootok           = 104,         //重启成功
	PackTypeEnum_rebootfail         = 105,         //重启失败

	/**********************视频控制（长连接）200~255*****************************/
	PackTypeEnum_VEDIO			    = 200,			//视频 
	PackTypeEnum_MOUSE			    = 201,			//鼠标
	PackTypeEnum_KEYBOARD		    = 202,			//键盘
};

//鼠标（模拟手指）状态
enum TOUCHTYPE
{
	TOUCHTYPE_DOWN			= 1,			//按下
	TOUCHTYPE_UP			= 2,			//弹起
	TOUCHTYPE_MOVE			= 3,			//移动				
};
//按键状态
enum KEYACTIONTYPE
{
	KEYACTION_DOWN			= 1,			//按下
	KEYACTION_UP			= 2,			//弹起
	KEYACTION_FUNC			= 3,			//功能键
};
//功能键值
enum KEYFUCVALUE
{					
	KEYFUC_HOME				= 10001,		//返回首页
	KEYFUC_RETURN			= 10002,		//返回按键
	KEYFUC_TASKMGR			= 10003,		//任务管理器
	KEYACTION_CAPSLOCKON	= 10004,		//大写打开
	KEYACTION_CAPSLOCKOFF	= 10005,		//大写关闭
};
//包头
typedef struct tagPACKAGEHEAD
{
	BYTE					head1;			//0xff
	BYTE					head2;			//0xfe
	BYTE					packType;		//包类型
	INT						len;			//包长度
}PACKAGEHEAD;
////网络包结构
//typedef struct tagNETPACKET
//{
//	PACKAGEHEAD				packhead;		//包头
//	CHAR*					bodybuff;		//存储数据
//}NETPACKET;
//鼠标消息包
typedef struct tagMOUSEPACKET
{
	USHORT					x;				//x坐标
	USHORT					y;				//y坐标
	BYTE					touchType;		//触摸类型
	INT						nDelayTime;		//延迟时间  ms单位
	BYTE					nFingerId;		//手指ID

}MOUSEPACKET;
//按键消息包
typedef struct tagKEYPACKET
{
	INT32					key;			//按键虚拟键值
	BYTE					keyAction;		//按键动作

}KEYPACKET;

#pragma pack(pop)