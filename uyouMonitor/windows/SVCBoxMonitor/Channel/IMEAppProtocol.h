#pragma once

#include <singleton.h>

//ע�⣬���Э������������ַ�������ansi/utf-8��ʽ�ģ���Ҫ��WCHAR
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
		EMV_OPER_HOME,							////������ҳ
		EMV_OPER_RETURN,						////���ذ���
		EMV_OPER_TASKMGR,						////���������
		EMV_OPER_INPUT_CHAR,					////��������
		EMV_OPER_INPUT_KEY,						////��������س���TAB
		EMV_OPER_SDCARD_SPACE,					////��ȡSD���ռ�
		EMV_OPER_GETXY,							////��ȡ��Ļ�ߴ�
		EMV_OPER_DOWNLOADAPP,					////����
		EMV_OPER_INPUTM,						////��ȡ���뷨״̬  ��ǰ���뷨�򿪻��ǹر�
		EMV_OPER_DOWNLOADAPPPROG,				////����
		EMV_OPER_DOWNLOADPAUSE,					////����
		EMV_OPER_DOWNLOADFINISH,				////����
		EMV_OPER_HEARTBEAT,						////������
		EMV_OPER_APPRUNNING,					////��ǰ�������е�APP
		EMV_OPER_OPENAPP,						////��APP
		EMV_OPER_UPGRADE,						////���������
		EMV_ANDROID_TO_PC_UPGRADEINFO,			////֪ͨ������Ϣ
		EMV_PC_TO_ANDROID_PUSH_SUCESS,			//PC֪ͨAndroidPush���
		EMV_ANDROID_TO_PC_UPGRADEFINISH,		//Android������ɺ�֪ͨPC
		EMV_ANDROID_TO_PC_UPGRADE_EXCEPTION,	//Android ����ʧ��֪ͨ
		EMV_OPER_OPENRECENTAPPLICATIONS,		//�����������
		EMV_OPER_GETMEDIASTORAGEDIRECTORY,		//��ȡSD��Ŀ¼
		EMV_OPER_GETVOLUME,						//��ȡ��������
		EMV_OPER_SETVOLUME,						//���ú�������
		EMV_OPER_REBOOT_DEVICE,					//��������
		
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