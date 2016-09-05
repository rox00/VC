#include "stdafx.h"
#include "../MainFrame.h"
#include "InputManager.h"
#include "MouseOperationHandler.h"

MouseOperationHandler::MouseOperationHandler()
{
	m_bIsAimingOpened = FALSE;
}

BOOL MouseOperationHandler::GetAimPoint(POINT &pt)
{	
	auto &&AimTouchEvents = InputManager::GetInstance().GetInputMethodOperationDevice().m_MouseMappingFunction(
		InputHookHelper::RIGHT_MOUSE_BUTTON, TouchEvent::TOUCH_DOWN);

	if (AimTouchEvents.size() > 0)
	{
		auto iter = AimTouchEvents.begin();
		TouchEvent::Message msg = (*iter);
		int x = msg.x();
		int y = msg.y();

		pt.x = x;
		pt.y = y;
		return TRUE;
	}
	return FALSE;
	
}

BOOL MouseOperationHandler::GetFirePoint(POINT &pt)
{
	auto &&FireTouchEvents = InputManager::GetInstance().GetInputMethodOperationDevice().m_MouseMappingFunction(
		InputHookHelper::LEFT_MOUSE_BUTTON,	TouchEvent::TOUCH_DOWN);

	if (FireTouchEvents.size() > 0)
	{
		auto iter = FireTouchEvents.begin();
		//TRACE(_T("fire -------"));
		TouchEvent::Message msg = (*iter);
		int x = msg.x();
		int y = msg.y();

		//全屏状态下的坐标换算
		//InputManager::GetInstance().DoTouchScreenMapping(x, y);
		
		pt.x = x;
		pt.y = y;
		return TRUE;
	}
	return FALSE;
}

BOOL MouseOperationHandler::MouseActionHandler(int mouseEvent, int x, int y, TouchEvent::ActionType Type)
{
	////【zy 2015-12-11 ↓】
	//if (m_bIsAimingOpened)
	//{
	//	static int i= 0;
	//	char ch[MAX_PATH] ={ 0 };
	//	sprintf_s(ch, MAX_PATH, "\n(zyo): x:%d y:%d name3:%d Index(i):%d \n", x, y, 0, i++);
	//	OutputDebugStringA(ch);//***zy***
	//}
	////【zy 2015-12-11 ↑】
	boost::mutex::scoped_lock Lock(InputManager::GetInstance().GetTouchScreen().m_Mutex);
	InputManager::GetInstance().ClientToDeviceScreen(x, y);	//从窗口坐标转换为android系统坐标
	
	if (!InputManager::GetInstance().GetInputMethodOperationDevice().m_MouseMappingFunction)
		return FALSE;

	//TODO：每次鼠标消息都会重新GetFirePoint和GetAimPoint，可能降低效率
	POINT	ptFireDevice;
	BOOL	bHasFirePoint		= GetFirePoint(ptFireDevice);

	POINT	ptAimDevice, ptAimScreen;
	BOOL	bHasAimPoint		= GetAimPoint(ptAimDevice);
	CRect	deviceScreenRect	= CRect(1, 1, g_currentDeviceWidth - 1, g_currentDeviceHeight - 1);

	int		xTemp				= ptAimDevice.x;
	int		yTemp				= ptAimDevice.y;
	InputManager::GetInstance().DeviceScreenToScreen(xTemp, yTemp);
	ptAimScreen.x = xTemp, ptAimScreen.y = yTemp;

	int		xScreen				= x;
	int		yScreen				= y;
	InputManager::GetInstance().DeviceScreenToScreen(xScreen, yScreen);

	if (m_bIsAimingOpened && GetCapture() != InputHookHelper::GetInstance().GetAttachWnd())
	{	//如果已经进入瞄准映射状态， 但是捕获鼠标焦点的窗口不是视频窗口的话，则重新设置
		SetCapture(InputHookHelper::GetInstance().GetAttachWnd());
	}

	if (mouseEvent == InputHookHelper::RIGHT_MOUSE_BUTTON && Type == TouchEvent::TOUCH_DOWN)
	{	//右键按下
		if(bHasAimPoint)		//如果有瞄准点
		{
			m_bIsAimingOpened = !m_bIsAimingOpened;
			if(m_bIsAimingOpened)
			{	//鼠标移动到瞄准点
				SetCursorPos(ptAimScreen.x, ptAimScreen.y);
				SetCapture(InputHookHelper::GetInstance().GetAttachWnd());
				ShowCursor(FALSE);
				
				m_deviceAimPosition.SetPoint(ptAimDevice.x, ptAimDevice.y);
				InputManager::GetInstance().SendTouchMessage(ptAimDevice.x, ptAimDevice.y, TouchEvent::TOUCH_DOWN, TOUCH_ID_AIM_MAPPING);	//在瞄准镜的地方按下
			}
			else
			{	//关闭瞄准功能，瞄准用的手指弹起
				InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);
				ReleaseCapture();
				ShowCursor(TRUE);
			}
			return TRUE;
		}		
		
		return FALSE;			//return TRUE以后，其他handler就不能响应右键按下了，右键拖动缩放的功能会失效
	}
	
	//处理同时按左右键，可能出现长按点的问题
	if(Type != TouchEvent::TOUCH_MOVE)
	{
		if (bHasFirePoint)
		{
			InputManager::GetInstance().SendTouchMessage(ptFireDevice.x, ptFireDevice.y,
				TouchEvent::TOUCH_UP, TOUCH_ID_FIRE_MAPPING);
		}
	}
		
	//鼠标移动，并且瞄准镜是打开的
	if (mouseEvent == InputHookHelper::NULL_MOUSE_BUTTON  && Type == TouchEvent::TOUCH_MOVE && m_bIsAimingOpened)
	{	//进入瞄准状态后的鼠标移动，响应拖到准星操作
		if (ptAimDevice.x != x || ptAimDevice.y != y)
		{	//如果不是回到瞄准点的操作
			//m_deviceAimPosition.Offset(x - ptAimDevice.x, y - ptAimDevice.y);
			m_deviceAimPosition.SetPoint(x, y);

			if (!deviceScreenRect.PtInRect(m_deviceAimPosition))
			{	//如果已经移出屏幕范围了
				InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);					//先抬起手指
				InputManager::GetInstance().SendTouchMessage(ptAimDevice.x, ptAimDevice.y, TouchEvent::TOUCH_DOWN, TOUCH_ID_AIM_MAPPING);	//重新到瞄准镜的地方按下
				m_deviceAimPosition.SetPoint(ptAimDevice.x, ptAimDevice.y);

				//把屏幕光标移动回瞄准点，这会再次发出一个MOVE消息，但不会满足(ptAim.x != x || ptAim.y != y)的条件
				SetCursorPos(ptAimScreen.x, ptAimScreen.y);
			}
			InputManager::GetInstance().SendTouchMessage(m_deviceAimPosition.x, m_deviceAimPosition.y, TouchEvent::TOUCH_MOVE, TOUCH_ID_AIM_MAPPING);
		}
		
		return TRUE;
	}
	
	if (mouseEvent == InputHookHelper::LEFT_MOUSE_BUTTON)
	{
		if (Type == TouchEvent::TOUCH_DOWN || Type == TouchEvent::TOUCH_UP)
		{
			if (m_bIsAimingOpened && bHasFirePoint == TRUE)
			{
				//进入瞄准状态后的点击事件
				InputManager::GetInstance().SendTouchMessage(ptFireDevice.x, ptFireDevice.y, Type, TOUCH_ID_FIRE_MAPPING);
				return TRUE;
			}
			else
			{
				//未进入瞄准状态,或者没有开火点时，响应点击当前位置
				InputManager::GetInstance().SendTouchMessage(x, y, Type, TOUCH_ID_MOUSE_CLICK);
				return TRUE;
			}

		}
		else
		{	//Type == TouchEvent::TOUCH_MOVE
			//左键按住的移动事件
			if (m_bIsAimingOpened)
			{
				if (ptAimDevice.x != x || ptAimDevice.y != y)
				{	//如果不是回到瞄准点的操作
					//m_deviceAimPosition.Offset(x - ptAimDevice.x, y - ptAimDevice.y);
					m_deviceAimPosition.SetPoint(x, y);

					if (!deviceScreenRect.PtInRect(m_deviceAimPosition))
					{	//如果已经移出屏幕范围了
						InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);					//先抬起手指
						InputManager::GetInstance().SendTouchMessage(ptAimDevice.x, ptAimDevice.y, TouchEvent::TOUCH_DOWN, TOUCH_ID_AIM_MAPPING);	//重新到瞄准镜的地方按下
						m_deviceAimPosition.SetPoint(ptAimDevice.x, ptAimDevice.y);

						//把屏幕光标移动回瞄准点，这会再次发出一个MOVE消息，但不会满足(ptAim.x != x || ptAim.y != y)的条件
						SetCursorPos(ptAimScreen.x, ptAimScreen.y);
					}
					InputManager::GetInstance().SendTouchMessage(m_deviceAimPosition.x, m_deviceAimPosition.y, TouchEvent::TOUCH_MOVE, TOUCH_ID_AIM_MAPPING);
				}
			}
			else
			{
				InputManager::GetInstance().SendTouchMessage(x, y, Type, TOUCH_ID_MOUSE_CLICK);
			}
			return TRUE;
		}
	}
	
	return FALSE;
}


void MouseOperationHandler::OnMouseLeave(WPARAM, LPARAM)
{
	if (m_bIsAimingOpened)
	{
		//鼠标移出，手指头弹起
		InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);
	}
	else
	{
		InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_MOUSE_CLICK);
	}
}
