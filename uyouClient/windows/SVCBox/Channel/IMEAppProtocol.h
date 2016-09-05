#pragma once

#include <singleton.h>

//注意，这个协议里面的所有字符串都是ansi/utf-8格式的，不要用WCHAR
#define JSON_DATA_BEGIN "_BXBF_DATA_BEGIN"
#define JSON_DATA_END "_BXBF_DATA_END"
#define JSON_DATA_BEGIN_SIZE (sizeof(JSON_DATA_BEGIN)-1)
#define JSON_DATA_END_SIZE (sizeof(JSON_DATA_END)-1)

class IMEAppProtocol : public SinglePattern
{
	DECLARE_SINGLETON(IMEAppProtocol)

public:
	enum EM_OPER_TYPE
	{
		EMV_OPER_HOME,							////返回首页
		EMV_OPER_RETURN,						////返回按键
		EMV_OPER_TASKMGR,						////任务管理器
		EMV_OPER_INPUT_CHAR,					////输入文字
		EMV_OPER_INPUT_KEY,						////按键，如回车，TAB
		EMV_OPER_SDCARD_SPACE,					////获取SD卡空间
		EMV_OPER_GETXY,							////获取屏幕尺寸
		EMV_OPER_DOWNLOADAPP,					////无用
		EMV_OPER_INPUTM,						////获取输入法状态  当前输入法打开还是关闭
		EMV_OPER_DOWNLOADAPPPROG,				////无用
		EMV_OPER_DOWNLOADPAUSE,					////无用
		EMV_OPER_DOWNLOADFINISH,				////无用
		EMV_OPER_HEARTBEAT,						////心跳包
		EMV_OPER_APPRUNNING,					////当前正在运行的APP
		EMV_OPER_OPENAPP,						////打开APP
		EMV_OPER_UPGRADE,						////检测升级包
		EMV_ANDROID_TO_PC_UPGRADEINFO,			////通知升级信息
		EMV_PC_TO_ANDROID_PUSH_SUCESS,			//PC通知AndroidPush完成
		EMV_ANDROID_TO_PC_UPGRADEFINISH,		//Android升级完成后通知PC
		EMV_ANDROID_TO_PC_UPGRADE_EXCEPTION,	//Android 升级失败通知
		EMV_OPER_OPENRECENTAPPLICATIONS,		//打开任务管理器
		EMV_OPER_GETMEDIASTORAGEDIRECTORY,		//获取SD卡目录
		EMV_OPER_GETVOLUME,						//获取盒子音量
		EMV_OPER_SETVOLUME,						//设置盒子音量
		EMV_OPER_REBOOT_DEVICE,					//重启例子
		
	};

	typedef std::pair<EM_OPER_TYPE, string> IMEAppMessage;

public:
	string ToString(IMEAppMessage Message);
	boost::optional<IMEAppMessage> FromString(string szBuffer);
	boost::optional<vector<string>> ParseMessage(const EM_OPER_TYPE MessageType, LPCSTR lpszData);
	
protected: 
	virtual void InitialInstance();
	virtual void DisposeInstance() { };

private:
	boost::bimap<EM_OPER_TYPE, string> m_mapCommOperationName;
};