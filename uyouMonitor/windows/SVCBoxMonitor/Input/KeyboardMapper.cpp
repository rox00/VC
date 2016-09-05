#include "stdafx.h"
#include "KeyboardMapper.h"
#include <json/json.h>
#include <atlconv.h>
#include <fstream>
#include "../../Common/Common.h"


#define DEVICE_WIDTH 1280
#define KEYBOARD_MAPPING_CONFIG_FILE_RELATIVE_PATH		_T("\\User\\mapping\\")
#define KEYBOARD_MAPPING_CONFIG_FILE_EXT_NAME			_T(".cfg")
#define SWIPE_MOVE_STEP		10
//�����¼�������ʱ
#define SWIPE_DELAY_TIME	8

KeyboardMapper::KeyboardMapper(LPCTSTR lpszExePath)
{
	m_szMappingConfigPath = lpszExePath;
	m_szMappingConfigPath += KEYBOARD_MAPPING_CONFIG_FILE_RELATIVE_PATH;
	m_bInputOpened = FALSE;
}

BOOL KeyboardMapper::Save(LPCSTR fileName, LPCSTR dataString)
{
	
	boost::mutex::scoped_lock Lock(m_Mutex);
	int count = 0;
	BOOL delOk = FALSE;
	do 
	{ 
		count++;
		if (DeleteFileA(fileName) == TRUE)
		{
			delOk = TRUE;
			break;
		}
		else{
			DWORD errorCode = GetLastError();
			if (errorCode == 2)
			{//�ļ�������
				delOk = TRUE;
				break;
			}
		}
		Common::SystemHelper::DelayMS(100);
	} while (count < 5);
	if (delOk == FALSE){
		return FALSE;
	}
	std::ofstream configFile(fileName, std::ios::out | std::ios::trunc, _SH_SECURE);
	if (configFile.bad())
	{
		TRACE("�����ļ���ʧ��");
		Common::LogHelper::log(_T("KeyboardMapper"), L4CP_INFO_LOG_LEVEL, _T("�����ļ���ʧ��"));
		return FALSE;
	}
	TRACE("KeyboardMapper save %s \n", dataString);
	if (strcmp(dataString, "") == 0)
	{
		configFile.close();			
		Common::SystemHelper::DelayMS(10);		
		Common::SystemHelper::DelayMS(10);
		Common::LogHelper::log(_T("KeyboardMapper"), L4CP_INFO_LOG_LEVEL, _T("���������Ϣ�ɹ�\n"));
	}
	else{
		configFile << dataString;
		configFile.close();
		std::string msg = ("���������ļ��ɹ� ");
		msg += dataString;
		TRACE("%s\n", msg.c_str());
		Common::LogHelper::log(_T("KeyboardMapper"), L4CP_INFO_LOG_LEVEL, CA2T(msg.c_str()));
	}
		
	return TRUE;
}

BOOL KeyboardMapper::Load(LPCSTR lpszPackageName, CRect vedioRc, BOOL reload /*= FALSE*/)
{
	boost::mutex::scoped_lock Lock(m_Mutex);	
	m_KeyDownList.clear();
	if (reload == FALSE)
	{
		auto FindConfig = m_CachedMappingConfig.find(lpszPackageName);
		if (FindConfig != m_CachedMappingConfig.end())
		{	//֮ǰ�������ֱ����cache������
			m_CurrentMappingConfig = FindConfig->second;
			return TRUE;
		}
	}
	
	//�����������������
	m_CurrentMappingConfig.m_Keys.clear();
	m_CurrentMappingConfig.m_Direction.m_bEnabled = FALSE;
	
#ifdef UNICODE
	USES_CONVERSION;

	auto PackageName = A2W(lpszPackageName);
#else
	auto& PackageName = lpszPackageName;
#endif

	String ConfigFilePathName = m_szMappingConfigPath + PackageName + KEYBOARD_MAPPING_CONFIG_FILE_EXT_NAME; 
	
	std::ifstream ConfigFile(ConfigFilePathName.c_str(), std::ios::in, _SH_SECURE);
	if(ConfigFile.bad())
		return FALSE;

	Json::Reader ConfigReader;
	Json::Value RootNode;
	m_CachedMappingConfig.erase(lpszPackageName);
	if (!ConfigReader.parse(ConfigFile, RootNode))
	{
		ConfigFile.close();
		return FALSE;
	}
	ConfigFile.close();
	/*��Ϊ�����ļ��б�������ص㶼��  1280*720 ��ߵ����ص� ����ת������Ƶ���ڵĵ���������*/
	auto &rDirection = RootNode["Direction"];
	if(!rDirection.empty())
	{
		m_CurrentMappingConfig.m_Direction.m_bEnabled = TRUE;
		m_CurrentMappingConfig.m_Direction.m_Center.x = rDirection["Center"][(UINT)0].asDouble();
		m_CurrentMappingConfig.m_Direction.m_Center.y = rDirection["Center"][(UINT)1].asDouble();

		auto &rArrows = rDirection["Arrows"];
		for(UINT i = 0; i < rArrows.size(); i++)
		{
			MappingConfigItem ConfigItem;
			auto &rItem = rArrows[i];
			ConfigItem.m_Position.x = rItem["Position"][(UINT)0].asDouble();
			ConfigItem.m_Position.y = rItem["Position"][(UINT)1].asDouble();
			//0��ָͷ����꣬1��ָͷ�Ƿ����
			ConfigItem.m_nFingerId = TOUCH_ID_ARRAW_KEY;
			ConfigItem.m_Action = MappingConfigItem::ACTION_SWIPE;

			m_CurrentMappingConfig.m_Keys[rItem["Key"].asUInt()] = ConfigItem;
		}
	}
	else
	{
		m_CurrentMappingConfig.m_Direction.m_bEnabled = FALSE;
	}

	auto &rMapping = RootNode["Mapping"];
	UINT nKeyMappingFingerId = TOUCH_ID_KEY_MAPPING_BEGIN;
	for(UINT i = 0; i < rMapping.size(); i++)
	{
		MappingConfigItem ConfigItem;
		auto &rItem = rMapping[i];
		if (!rItem.empty())
		{
			ConfigItem.m_Position.x = rItem["Position"][(UINT)0].asDouble();
			ConfigItem.m_Position.y = rItem["Position"][(UINT)1].asDouble();
			ConfigItem.m_nFingerId = nKeyMappingFingerId++;

			if (ConfigItem.m_nFingerId > TOUCH_ID_KEY_MAPPING_MAX)
			{
				if (ConfigItem.m_Position.x < DEVICE_WIDTH / 2)
				{
					ConfigItem.m_nFingerId = TOUCH_ID_LEFT_KEY_MAPPING;
				}
				else
				{
					ConfigItem.m_nFingerId = TOUCH_ID_RIGHT_KEY_MAPPING;
				}
			}

			ConfigItem.m_Action = MappingConfigItem::ACTION_TAP;
			m_CurrentMappingConfig.m_Keys[rItem["Key"].asUInt()] = ConfigItem;
		}				
	}

	m_CachedMappingConfig[lpszPackageName] = m_CurrentMappingConfig;

	TRACE("KeyboardMapper::Load(%s) finished\n", lpszPackageName);	

	return TRUE;
}


InputManager::TouchMessageVector KeyboardMapper::MouseMappingFunction(UINT nVirtualKey, UINT nAction)
{
	boost::mutex::scoped_lock Lock(m_Mutex);
	InputManager::TouchMessageVector TouchMessages;	
	auto FindConfigItem = m_CurrentMappingConfig.m_Keys.find(nVirtualKey);
	if (FindConfigItem == m_CurrentMappingConfig.m_Keys.end())
	{
		return TouchMessages;
	}
	auto &ConfigItem = FindConfigItem->second;
	
	//Tap�������Ƚϼ�
	if (nVirtualKey == InputHookHelper::LEFT_MOUSE_BUTTON || nVirtualKey == InputHookHelper::RIGHT_MOUSE_BUTTON)//������Ҽ�
	{
		GenereateTouchMessage(TouchMessages, ConfigItem, (TOUCHTYPE)nAction);
	}
	
	return TouchMessages;
}

//TODO ��Ҫ�ع�����
InputManager::TouchMessageVector KeyboardMapper::KeyMappingFunction(UINT nVirtualKey, UINT nAction)
{
	boost::mutex::scoped_lock Lock(m_Mutex);
	InputManager::TouchMessageVector TouchMessages;

	auto FindConfigItem = m_CurrentMappingConfig.m_Keys.find(nVirtualKey);
	if(FindConfigItem == m_CurrentMappingConfig.m_Keys.end())
		return TouchMessages;

	auto &ConfigItem = FindConfigItem->second;
	
	if(ConfigItem.m_Action == MappingConfigItem::ACTION_SWIPE)
	{
		ASSERT(ConfigItem.m_nFingerId == TOUCH_ID_ARRAW_KEY);	//Ŀǰֻ֧�ַ�����Ļ���

		if(m_CurrentMappingConfig.m_Direction.m_bEnabled == FALSE)
			return TouchMessages;

		if (nAction == KEYACTION_DOWN)
		{
			if(!m_KeyDownList.empty())
			{	//ǰ���м������ˣ���Ҫ�����һ�ΰ��µļ����ﻬ������
				try
				{	//��Ȼ������û���⣬��m_Keys[*m_KeyDownList.rbegin()]��������Σ�գ�ͨ��try...catch������֮
					auto LastKeyPosition = m_CurrentMappingConfig.m_Keys[*m_KeyDownList.rbegin()].m_Position;

					GenereateSwipeMessage(TouchMessages, ConfigItem.m_nFingerId, 
						LastKeyPosition,
						GetSyntheticPosition(LastKeyPosition, ConfigItem.m_Position, m_CurrentMappingConfig.m_Direction.m_Center));
				}
				catch(...)
				{
				}

			}
			else
			{	//ǰ��û�м����£���ָͷ���£��ӷ�������м�㻬������
				GenereateSwipeMessage(TouchMessages, ConfigItem.m_nFingerId, 
					m_CurrentMappingConfig.m_Direction.m_Center, 
					ConfigItem.m_Position, TRUE);
			}
			m_KeyDownList.push_back(nVirtualKey);
		}
		else
		{	//WM_KEYUP
			auto iterKeyDown = std::find(m_KeyDownList.begin(), m_KeyDownList.end(), nVirtualKey);
			if(iterKeyDown != m_KeyDownList.end())
			{	//�������֮ǰ���µļ�
				
				m_KeyDownList.erase(iterKeyDown);	//���˼���Ϊû�а���
				if(m_KeyDownList.empty())
				{	//��ʣ���������ˣ���ָͷ����
					GenereateTouchMessage(TouchMessages, ConfigItem, TOUCHTYPE_UP);
				}
				else
				{	//�������ϴΰ��µļ�
					auto nLastDownKey = *m_KeyDownList.rbegin();	//list��Ϊ�գ����������ǰ�ȫ��
					auto LastKeyPosition = m_CurrentMappingConfig.m_Keys[nLastDownKey].m_Position;
					
					GenereateSwipeMessage(TouchMessages, ConfigItem.m_nFingerId, 
						GetSyntheticPosition(ConfigItem.m_Position, LastKeyPosition, m_CurrentMappingConfig.m_Direction.m_Center),
						LastKeyPosition);
				}
			}
			else 
			{	//�������֮ǰû�а��µļ��������⣬��ָͷ����				
				GenereateTouchMessage(TouchMessages, ConfigItem, TOUCHTYPE_UP);
			}
		}
	}	//if action==swipe���������
	else
	{	//Tap�������Ƚϼ�
		GenereateTouchMessage(TouchMessages, ConfigItem, (nAction == KEYACTION_DOWN) ?
			TOUCHTYPE_DOWN : TOUCHTYPE_UP);
	}

	return TouchMessages;
}

void KeyboardMapper::GenereateTouchMessage(InputManager::TouchMessageVector& rMessages, 
	MappingConfigItem &rConfigItem, TOUCHTYPE nAction)
{
	int x = (rConfigItem.m_Position.x) /1280.0 * g_currentDeviceWidth;
	int y = (rConfigItem.m_Position.y) /720.0 * g_currentDeviceHeight;
	tagMOUSEPACKET TouchMsg;

	TouchMsg.x			= x;
	TouchMsg.y			= y;
	TouchMsg.touchType	= (nAction);
	TouchMsg.nDelayTime	= (0);
	TouchMsg.nFingerId	= (rConfigItem.m_nFingerId);

	rMessages.push_back(TouchMsg);
}

void KeyboardMapper::GenereateSwipeMessage(InputManager::TouchMessageVector& rMessages,
	UINT nFingerId, CPoint ptStart, CPoint ptStop, BOOL bNeedTouchDown)
{
	ptStart.x	= (ptStart.x) /1280.0 * g_currentDeviceWidth;
	ptStart.y	= (ptStart.y) /720.0 * g_currentDeviceHeight;

	ptStop.x	= (ptStop.x) /1280.0 * g_currentDeviceWidth;
	ptStop.y	= (ptStop.y) /720.0 * g_currentDeviceHeight;

	tagMOUSEPACKET TouchMsg;

	if(bNeedTouchDown)
	{
		TouchMsg.x			= (ptStart.x);
		TouchMsg.y			= (ptStart.y);
		TouchMsg.touchType	= (TOUCHTYPE_DOWN);
		TouchMsg.nDelayTime	= (SWIPE_DELAY_TIME);
		TouchMsg.nFingerId	= (nFingerId);

		rMessages.push_back(TouchMsg);
	}

	//bresenham�㷨
	int dx = ptStop.x - ptStart.x;
	int dy = ptStop.y - ptStart.y;
	int abs_dx = abs(dx);
	int abs_dy = abs(dy);
	int unitx = 1;
	if(abs_dx)
		unitx = dx / abs_dx;
	int unity = 1;
	if(abs_dy)
		unity = dy / abs_dy;
	int x = ptStart.x;
	int y = ptStart.y;
	int iError; //���

	auto pGenerateMoveEventFunction = [&](int iJudgeComponent)
	{
		if(iJudgeComponent % SWIPE_MOVE_STEP)
			return;

		TouchMsg.x			= (x);
		TouchMsg.y			= (y);
		TouchMsg.touchType	= (TOUCHTYPE_MOVE);
		TouchMsg.nDelayTime	= (SWIPE_DELAY_TIME);	//�����������������ӳ��ǱȽϺ����
		TouchMsg.nFingerId	= (nFingerId);

		rMessages.push_back(TouchMsg);
	};

	if(abs_dx > abs_dy )
	{
		iError = abs_dx;
		for(int i = 0; i < abs_dx ; i++)
		{
			x += unitx;
			iError = iError - 2 * abs_dy;
			if(iError <= 0)
			{
				y += unity;
				iError = iError + 2 * abs_dx;
			}
			pGenerateMoveEventFunction(x);
		} // for ends
		if(x % SWIPE_MOVE_STEP)
			pGenerateMoveEventFunction(0);	//�յ���뻭����
	}
	else
	{
		iError = abs_dy;
		for(int i = 0; i < abs_dy ; i++)
		{
			y += unity;
			iError = iError - 2 * abs_dx;
			if(iError <= 0)
			{
				x += unitx;
				iError = iError + 2 * abs_dy;
			}
			pGenerateMoveEventFunction(y);
		} // for end
		if(y % SWIPE_MOVE_STEP)
			pGenerateMoveEventFunction(0);	//�յ���뻭����
	}	// if end
}

//************************************
// Method:    GetSyntheticPosition
// FullName:  KeyboardMapper::GetSyntheticPosition
// Access:    protected static 
// Returns:   CPoint
// Qualifier:
// Parameter: CPoint Point1
// Parameter: CPoint Point2
// Parameter: CPoint CenterPoint
//************************************
CPoint KeyboardMapper::GetSyntheticPosition(CPoint Point1, CPoint Point2, CPoint CenterPoint)
{	//ͬʱ�����������������Ҫ������ϳɷ���

	Point1.x	= (Point1.x) /1280.0 * g_currentDeviceWidth;
	Point1.y	= (Point1.y) /720.0 * g_currentDeviceHeight;

	Point2.x	= (Point2.x) /1280.0 * g_currentDeviceWidth;
	Point2.y	= (Point2.y) /720.0 * g_currentDeviceHeight;

	CenterPoint.x	= (CenterPoint.x) /1280.0 * g_currentDeviceWidth;
	CenterPoint.y	= (CenterPoint.y) /720.0 * g_currentDeviceHeight;
	
	//����ѡ�����棬�������ıȽ�Զ���Ǹ�
	CPoint Choice1(Point1.x, Point2.y);
	CPoint Choice2(Point2.x, Point1.y);

	CRect Choice1Rect(Choice1, CenterPoint);
	CRect Choice2Rect(Choice2, CenterPoint);

	if ((Choice1Rect.Width() == 0 && Choice2Rect.Width() == 0)
		|| (Choice1Rect.Height() == 0 && Choice2Rect.Height() == 0))
	{	//�����һ���ˣ�ֱ�ӷ����м��
		return CenterPoint;
	}
	
	//��ʡʱ�䣬ֱ����x��y����֮�ͽ��бȽ�
	if(abs(Choice1Rect.Width()) + abs(Choice1Rect.Height()) >= abs(Choice2Rect.Width()) + abs(Choice2Rect.Height()))
		return Choice1;
	else
		return Choice2;
}

void KeyboardMapper::SetInputOpened(BOOL opened)
{
	m_bInputOpened = opened;
}

BOOL KeyboardMapper::GetInputOpened()
{
	return m_bInputOpened;
}

bool KeyboardMapper::IsDirectKey(UINT nVirtualKey)
{
	boost::mutex::scoped_lock Lock(m_Mutex);

	if (m_CurrentMappingConfig.m_Direction.m_bEnabled)
	{
		auto iterKeyDown = m_CurrentMappingConfig.m_Keys.find(nVirtualKey);
		if (iterKeyDown != m_CurrentMappingConfig.m_Keys.end())
		{	
			if (iterKeyDown->second.m_nFingerId == TOUCH_ID_ARRAW_KEY)
				return true;
		}
	}

	return false;
}

void KeyboardMapper::ClearKeyDownList()
{
	m_KeyDownList.clear();
}
