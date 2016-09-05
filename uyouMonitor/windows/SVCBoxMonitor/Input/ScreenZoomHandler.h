#pragma once

class InputManager;

class ScreenZoomHandler
{
public:
	ScreenZoomHandler();

public:
	BOOL MouseActionHandler(int mouseEvent, int x, int y, TOUCHTYPE Type);
	void OnMouseLeave(WPARAM, LPARAM);
	void OnMouseWheel(WPARAM, LPARAM);
	void OnSystemEvent(){
		if (m_bIsTouchDown)
		{
			//����Ƴ�����ָͷ����
			CalculateZoomPosition(0);
			SendTouchMessage(0, 0, 0, 0, TOUCHTYPE_UP);
			m_bIsTouchDown = FALSE;
		}
	}
protected:
	void CalculateZoomPosition(int iDelta);
	void SendTouchMessage(int x1, int y1, int x2, int y2, TOUCHTYPE Action, BOOL bIsMapping = FALSE);

private:
	CPoint m_LastPosition;		//�����Client���ڵ�����ϵ
	int m_iZoomPosition1, m_iZoomPosition2;
	BOOL m_bIsTouchDown;
	const UINT m_nScreenWidth;
};