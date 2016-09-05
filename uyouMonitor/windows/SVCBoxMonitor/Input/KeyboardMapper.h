#pragma once

#include <types.h>
#include "InputManager.h"

class KeyboardMapper
{
private:
	struct MappingConfigItem
	{
		UINT m_nFingerId;
		CPoint m_Position;
		
		enum 
		{
			ACTION_TAP,
			ACTION_SWIPE,
		} m_Action;
	};
	
	struct MappingConfig
	{
		std::map<UINT, MappingConfigItem> m_Keys;
		struct 
		{
			BOOL m_bEnabled;
			CPoint m_Center;
		} m_Direction;
	};
	
public:
	KeyboardMapper(LPCTSTR lpszExePath);

public:
	BOOL													Load(LPCSTR lpszPackageName, CRect vedioRc, BOOL reload = FALSE);
	BOOL													Save(LPCSTR fileName, LPCSTR dataString);
	InputManager::TouchMessageVector						KeyMappingFunction(UINT, UINT);
	InputManager::TouchMessageVector						MouseMappingFunction(UINT nVirtualKey, UINT nAction);
	void													ClearKeyDownList();

protected:
	void													GenereateTouchMessage(InputManager::TouchMessageVector& rMessages,
															MappingConfigItem &rConfigItem, TOUCHTYPE nAction);

	void													GenereateSwipeMessage(InputManager::TouchMessageVector& rMessages,
															UINT nFingerId, CPoint ptStart, CPoint ptStop, BOOL bNeedTouchDown = FALSE);

	static CPoint											GetSyntheticPosition(CPoint Point1, CPoint Point2, CPoint CenterPoint);
	
private:
	std::list<UINT>											m_KeyDownList;
	String													m_szMappingConfigPath;
	MappingConfig											m_CurrentMappingConfig;	//���ĳ���ض�Ӧ�õİ���ӳ������
	std::map<std::string, MappingConfig>					m_CachedMappingConfig;	//����һ�ε�mapping�ļ��������ڴ���Ա�����ʹ��
	boost::mutex											m_Mutex;

private:
	BOOL													m_bInputOpened;
public:
	bool													IsDirectKey(UINT nVirtualKey);
	void													SetInputOpened(BOOL opened);
	BOOL													GetInputOpened();
};