// Common.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Common.h"


// ���ǵ���������һ��ʾ��
COMMON_API int nCommon=0;

// ���ǵ���������һ��ʾ����
COMMON_API int fnCommon(void)
{
	return 42;
}

const IID IID_IDeviceTopology = __uuidof(IDeviceTopology);
const IID IID_IPart = __uuidof(IPart);
const IID IID_IConnector = __uuidof(IConnector);
const IID IID_IAudioInputSelector = __uuidof(IAudioInputSelector);

bool Common::LogHelper::bInit = false;
bool Common::SystemHelper::ScreenHelper::_bScale = false;
DEVMODE Common::SystemHelper::ScreenHelper::_oldDM = Common::SystemHelper::ScreenHelper::GetCurrentDisplayMode();
/*
SharedObjectPtr<Appender> Common::LogHelper::_consoleAppender;
SharedObjectPtr<Appender> Common::LogHelper::_fileAppender;
Logger Common::LogHelper::_logger;
*/
Common::SystemHelper::VolumeHelper Common::SystemHelper::VolumeHelper::_volumeHelper;