#include "stdafx.h"
#include "InputManager.h"
#include "ScreenZoomHandler.h"
#include "../../Common/Common.h"

ScreenZoomHandler::ScreenZoomHandler()
	: m_nScreenWidth(1920)
{
	m_bIsTouchDown = FALSE;
}

BOOL ScreenZoomHandler::MouseActionHandler(int mouseEvent, int x, int y, TOUCHTYPE Type)
{
	boost::mutex::scoped_lock Lock(InputManager::GetInstance().GetTouchScreen().m_Mutex);
	if (mouseEvent != InputHookHelper::RIGHT_MOUSE_BUTTON)
	{
		m_LastPosition.SetPoint(x, y);
		return FALSE;
	}

	int iDelta = x - m_LastPosition.x;

	BOOL bIsProcessed = FALSE;

	switch (Type)
	{
	case TOUCHTYPE_MOVE:
		//�Ҽ��϶�
		if (iDelta && m_bIsTouchDown)
		{
			CalculateZoomPosition(iDelta);
			SendTouchMessage(m_iZoomPosition1, y, m_iZoomPosition2, y, TOUCHTYPE_MOVE, TRUE);
		}
		break;
	case TOUCHTYPE_DOWN:
		CalculateZoomPosition(0);		//��ʼ��
		//SendTouchMessage(m_iZoomPosition1, y, m_iZoomPosition2, y, TOUCH_DOWN, TRUE);
		m_bIsTouchDown = TRUE;
		break;
	case TOUCHTYPE_UP:
		OnMouseLeave(0, 0);
		break;
	default:
		break;
	}

	m_LastPosition.SetPoint(x, y);

	return bIsProcessed;
}

void ScreenZoomHandler::OnMouseLeave(WPARAM, LPARAM)
{
	if (m_bIsTouchDown)
	{
		//����Ƴ�����ָͷ����
		CalculateZoomPosition(0);
		SendTouchMessage(0, 0, 0, 0, TOUCHTYPE_UP);
		m_bIsTouchDown = FALSE;
	}
}

void ScreenZoomHandler::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	auto iDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	auto iKeyState = GET_KEYSTATE_WPARAM(wParam);		
	if (iKeyState & MK_CONTROL)
	{	//���������ctrl��		
		{			
			const UINT nMoveStep = 10;		//������������Ϊ����ֵ�����Ե���
			const UINT nMoveIntelval = 20;

			POINT CenterPoint = m_LastPosition;
			InputManager::GetInstance().ClientToDeviceScreen((int&)CenterPoint.x, (int&)CenterPoint.y);	//ӳ�䵽androidϵͳ�е�����

			POINT Point1Begin = CenterPoint;
			POINT Point2Begin = CenterPoint;
			if (iDelta < 0)
			{
				Point1Begin.x += nMoveStep * nMoveIntelval;
				Point2Begin.x -= nMoveStep * nMoveIntelval;
			}

			//����
			//SendTouchMessage(Point1Begin.x, Point1Begin.y, Point2Begin.x, Point2Begin.y, TOUCH_DOWN);

			for (auto i = 1; i < nMoveStep; i++)
			{
				//�ƶ�
				SendTouchMessage(Point1Begin.x - i * nMoveIntelval, Point1Begin.y,
					Point2Begin.x + i * nMoveIntelval, Point2Begin.y,
					TOUCHTYPE_MOVE);

				//ÿ���ƶ���Ҫ������Ϣ������ģ�ⲻ����
				Common::SystemHelper::DelayMS(10);
			}
			//̧��
			SendTouchMessage(0, 0, 0, 0, TOUCHTYPE_UP);	//̧���ʱ����Ҫָ��x��y����			
		}
	}
}

void ScreenZoomHandler::CalculateZoomPosition(int iDelta)
{
	if(iDelta == 0)
		m_iZoomPosition1 = m_nScreenWidth / 4;
	else
	{
		m_iZoomPosition1 -= iDelta;
		if (m_iZoomPosition1 > (int)(m_nScreenWidth / 2))
			m_iZoomPosition1 = (int)(m_nScreenWidth / 2);
		else if (m_iZoomPosition1 < 0)
			m_iZoomPosition1 = 0;
	}

	m_iZoomPosition2 = m_nScreenWidth - m_iZoomPosition1;
}

void ScreenZoomHandler::SendTouchMessage(int x1, int y1, int x2, int y2, TOUCHTYPE Action, BOOL bIsMapping)
{
	if (bIsMapping)
	{
		int x = 0;
		//����y�������������任
		InputManager::GetInstance().ClientToDeviceScreen(x, y1);
		InputManager::GetInstance().ClientToDeviceScreen(x, y2);
	}

	InputManager::GetInstance().SendTouchMessage(x1, y1, Action, TOUCH_ID_ZOOM_1);
	InputManager::GetInstance().SendTouchMessage(x2, y2, Action, TOUCH_ID_ZOOM_2);
}