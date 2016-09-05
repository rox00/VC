#pragma once

class MouseOperationHandler
{
	friend class InputManager;
public:
	MouseOperationHandler();
	void Initialize(HWND hAttachWnd){
		m_bIsAimingOpened = FALSE;
		m_hAttachWnd = hAttachWnd;
	}

	BOOL								GetAimPoint(POINT &pt);
	BOOL								GetFirePoint(POINT &pt);

protected:
	BOOL								MouseActionHandler(int mouseEvent, int x, int y, TOUCHTYPE Type);
	void								OnMouseLeave(WPARAM, LPARAM);	

private:
	CPoint								m_deviceAimPosition;
	BOOL								m_bIsAimingOpened;
	HWND								m_hAttachWnd;
};