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
//滑动事件滑动延时
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
			{//文件不存在
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
		TRACE("配置文件打开失败");
		Common::LogHelper::log(_T("KeyboardMapper"), L4CP_INFO_LOG_LEVEL, _T("配置文件打开失败"));
		return FALSE;
	}
	TRACE("KeyboardMapper save %s \n", dataString);
	if (strcmp(dataString, "") == 0)
	{
		configFile.close();			
		Common::SystemHelper::DelayMS(10);		
		Common::SystemHelper::DelayMS(10);
		Common::LogHelper::log(_T("KeyboardMapper"), L4CP_INFO_LOG_LEVEL, _T("清除配置信息成功\n"));
	}
	else{
		configFile << dataString;
		configFile.close();
		std::string msg = ("保存配置文件成功 ");
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
		{	//之前读入过，直接用cache的内容
			m_CurrentMappingConfig = FindConfig->second;
			return TRUE;
		}
	}
	
	//先清除，待重新填入
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
	/*因为这里文件中保存的像素点都是  1280*720 宽高的像素点 这里转换成视频窗口的的像素坐标*/
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
			//0号指头是鼠标，1号指头是方向键
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
	
	//Tap操作，比较简单
	if (nVirtualKey == InputHookHelper::LEFT_MOUSE_BUTTON || nVirtualKey == InputHookHelper::RIGHT_MOUSE_BUTTON)//左键或右键
	{
		GenereateTouchMessage(TouchMessages, ConfigItem, (TOUCHTYPE)nAction);
	}
	
	return TouchMessages;
}

//TODO 需要重构，改
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
		ASSERT(ConfigItem.m_nFingerId == TOUCH_ID_ARRAW_KEY);	//目前只支持方向键的滑动

		if(m_CurrentMappingConfig.m_Direction.m_bEnabled == FALSE)
			return TouchMessages;

		if (nAction == KEYACTION_DOWN)
		{
			if(!m_KeyDownList.empty())
			{	//前面有键按下了，需要从最后一次按下的键那里滑动过来
				try
				{	//虽然理论上没问题，但m_Keys[*m_KeyDownList.rbegin()]看起来有危险，通过try...catch来忽略之
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
			{	//前面没有键按下，手指头按下，从方向键的中间点滑动过来
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
			{	//弹起的是之前按下的键
				
				m_KeyDownList.erase(iterKeyDown);	//将此键标为没有按下
				if(m_KeyDownList.empty())
				{	//不剩下其他键了，手指头弹起
					GenereateTouchMessage(TouchMessages, ConfigItem, TOUCHTYPE_UP);
				}
				else
				{	//滑回上上次按下的键
					auto nLastDownKey = *m_KeyDownList.rbegin();	//list不为空，所以这里是安全的
					auto LastKeyPosition = m_CurrentMappingConfig.m_Keys[nLastDownKey].m_Position;
					
					GenereateSwipeMessage(TouchMessages, ConfigItem.m_nFingerId, 
						GetSyntheticPosition(ConfigItem.m_Position, LastKeyPosition, m_CurrentMappingConfig.m_Direction.m_Center),
						LastKeyPosition);
				}
			}
			else 
			{	//弹起的是之前没有按下的键，有问题，手指头弹起				
				GenereateTouchMessage(TouchMessages, ConfigItem, TOUCHTYPE_UP);
			}
		}
	}	//if action==swipe（方向键）
	else
	{	//Tap操作，比较简单
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

	//bresenham算法
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
	int iError; //误差

	auto pGenerateMoveEventFunction = [&](int iJudgeComponent)
	{
		if(iJudgeComponent % SWIPE_MOVE_STEP)
			return;

		TouchMsg.x			= (x);
		TouchMsg.y			= (y);
		TouchMsg.touchType	= (TOUCHTYPE_MOVE);
		TouchMsg.nDelayTime	= (SWIPE_DELAY_TIME);	//滑动过程中有少量延迟是比较合理的
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
			pGenerateMoveEventFunction(0);	//终点必须画出来
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
			pGenerateMoveEventFunction(0);	//终点必须画出来
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
{	//同时按下两个方向键，需要计算其合成方向

	Point1.x	= (Point1.x) /1280.0 * g_currentDeviceWidth;
	Point1.y	= (Point1.y) /720.0 * g_currentDeviceHeight;

	Point2.x	= (Point2.x) /1280.0 * g_currentDeviceWidth;
	Point2.y	= (Point2.y) /720.0 * g_currentDeviceHeight;

	CenterPoint.x	= (CenterPoint.x) /1280.0 * g_currentDeviceWidth;
	CenterPoint.y	= (CenterPoint.y) /720.0 * g_currentDeviceHeight;
	
	//两个选择里面，靠近中心比较远的那个
	CPoint Choice1(Point1.x, Point2.y);
	CPoint Choice2(Point2.x, Point1.y);

	CRect Choice1Rect(Choice1, CenterPoint);
	CRect Choice2Rect(Choice2, CenterPoint);

	if ((Choice1Rect.Width() == 0 && Choice2Rect.Width() == 0)
		|| (Choice1Rect.Height() == 0 && Choice2Rect.Height() == 0))
	{	//三点成一线了，直接返回中间点
		return CenterPoint;
	}
	
	//节省时间，直接用x和y距离之和进行比较
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
