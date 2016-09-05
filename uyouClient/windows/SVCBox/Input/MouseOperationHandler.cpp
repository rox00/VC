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

		//ȫ��״̬�µ����껻��
		//InputManager::GetInstance().DoTouchScreenMapping(x, y);
		
		pt.x = x;
		pt.y = y;
		return TRUE;
	}
	return FALSE;
}

BOOL MouseOperationHandler::MouseActionHandler(int mouseEvent, int x, int y, TouchEvent::ActionType Type)
{
	////��zy 2015-12-11 ����
	//if (m_bIsAimingOpened)
	//{
	//	static int i= 0;
	//	char ch[MAX_PATH] ={ 0 };
	//	sprintf_s(ch, MAX_PATH, "\n(zyo): x:%d y:%d name3:%d Index(i):%d \n", x, y, 0, i++);
	//	OutputDebugStringA(ch);//***zy***
	//}
	////��zy 2015-12-11 ����
	boost::mutex::scoped_lock Lock(InputManager::GetInstance().GetTouchScreen().m_Mutex);
	InputManager::GetInstance().ClientToDeviceScreen(x, y);	//�Ӵ�������ת��Ϊandroidϵͳ����
	
	if (!InputManager::GetInstance().GetInputMethodOperationDevice().m_MouseMappingFunction)
		return FALSE;

	//TODO��ÿ�������Ϣ��������GetFirePoint��GetAimPoint�����ܽ���Ч��
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
	{	//����Ѿ�������׼ӳ��״̬�� ���ǲ�����꽹��Ĵ��ڲ�����Ƶ���ڵĻ�������������
		SetCapture(InputHookHelper::GetInstance().GetAttachWnd());
	}

	if (mouseEvent == InputHookHelper::RIGHT_MOUSE_BUTTON && Type == TouchEvent::TOUCH_DOWN)
	{	//�Ҽ�����
		if(bHasAimPoint)		//�������׼��
		{
			m_bIsAimingOpened = !m_bIsAimingOpened;
			if(m_bIsAimingOpened)
			{	//����ƶ�����׼��
				SetCursorPos(ptAimScreen.x, ptAimScreen.y);
				SetCapture(InputHookHelper::GetInstance().GetAttachWnd());
				ShowCursor(FALSE);
				
				m_deviceAimPosition.SetPoint(ptAimDevice.x, ptAimDevice.y);
				InputManager::GetInstance().SendTouchMessage(ptAimDevice.x, ptAimDevice.y, TouchEvent::TOUCH_DOWN, TOUCH_ID_AIM_MAPPING);	//����׼���ĵط�����
			}
			else
			{	//�ر���׼���ܣ���׼�õ���ָ����
				InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);
				ReleaseCapture();
				ShowCursor(TRUE);
			}
			return TRUE;
		}		
		
		return FALSE;			//return TRUE�Ժ�����handler�Ͳ�����Ӧ�Ҽ������ˣ��Ҽ��϶����ŵĹ��ܻ�ʧЧ
	}
	
	//����ͬʱ�����Ҽ������ܳ��ֳ����������
	if(Type != TouchEvent::TOUCH_MOVE)
	{
		if (bHasFirePoint)
		{
			InputManager::GetInstance().SendTouchMessage(ptFireDevice.x, ptFireDevice.y,
				TouchEvent::TOUCH_UP, TOUCH_ID_FIRE_MAPPING);
		}
	}
		
	//����ƶ���������׼���Ǵ򿪵�
	if (mouseEvent == InputHookHelper::NULL_MOUSE_BUTTON  && Type == TouchEvent::TOUCH_MOVE && m_bIsAimingOpened)
	{	//������׼״̬�������ƶ�����Ӧ�ϵ�׼�ǲ���
		if (ptAimDevice.x != x || ptAimDevice.y != y)
		{	//������ǻص���׼��Ĳ���
			//m_deviceAimPosition.Offset(x - ptAimDevice.x, y - ptAimDevice.y);
			m_deviceAimPosition.SetPoint(x, y);

			if (!deviceScreenRect.PtInRect(m_deviceAimPosition))
			{	//����Ѿ��Ƴ���Ļ��Χ��
				InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);					//��̧����ָ
				InputManager::GetInstance().SendTouchMessage(ptAimDevice.x, ptAimDevice.y, TouchEvent::TOUCH_DOWN, TOUCH_ID_AIM_MAPPING);	//���µ���׼���ĵط�����
				m_deviceAimPosition.SetPoint(ptAimDevice.x, ptAimDevice.y);

				//����Ļ����ƶ�����׼�㣬����ٴη���һ��MOVE��Ϣ������������(ptAim.x != x || ptAim.y != y)������
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
				//������׼״̬��ĵ���¼�
				InputManager::GetInstance().SendTouchMessage(ptFireDevice.x, ptFireDevice.y, Type, TOUCH_ID_FIRE_MAPPING);
				return TRUE;
			}
			else
			{
				//δ������׼״̬,����û�п����ʱ����Ӧ�����ǰλ��
				InputManager::GetInstance().SendTouchMessage(x, y, Type, TOUCH_ID_MOUSE_CLICK);
				return TRUE;
			}

		}
		else
		{	//Type == TouchEvent::TOUCH_MOVE
			//�����ס���ƶ��¼�
			if (m_bIsAimingOpened)
			{
				if (ptAimDevice.x != x || ptAimDevice.y != y)
				{	//������ǻص���׼��Ĳ���
					//m_deviceAimPosition.Offset(x - ptAimDevice.x, y - ptAimDevice.y);
					m_deviceAimPosition.SetPoint(x, y);

					if (!deviceScreenRect.PtInRect(m_deviceAimPosition))
					{	//����Ѿ��Ƴ���Ļ��Χ��
						InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);					//��̧����ָ
						InputManager::GetInstance().SendTouchMessage(ptAimDevice.x, ptAimDevice.y, TouchEvent::TOUCH_DOWN, TOUCH_ID_AIM_MAPPING);	//���µ���׼���ĵط�����
						m_deviceAimPosition.SetPoint(ptAimDevice.x, ptAimDevice.y);

						//����Ļ����ƶ�����׼�㣬����ٴη���һ��MOVE��Ϣ������������(ptAim.x != x || ptAim.y != y)������
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
		//����Ƴ�����ָͷ����
		InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_AIM_MAPPING);
	}
	else
	{
		InputManager::GetInstance().SendTouchMessage(0, 0, TouchEvent::TOUCH_UP, TOUCH_ID_MOUSE_CLICK);
	}
}
