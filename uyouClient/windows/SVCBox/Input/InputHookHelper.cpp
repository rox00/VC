#include "stdafx.h"
#include "InputHookHelper.h"
#include "InputManager.h"
#include <WindowsX.h>
#include <Dbt.h>

WNDPROC	InputHookHelper::m_pWndProc = 0;
void InputHookHelper::DisposeInstance()
{
	if(m_MsgHook)
	{
		UnhookWindowsHookEx(m_MsgHook);
		m_MsgHook = NULL;
	}
}

void InputHookHelper::InitialInstance()
{
	m_hAttachWnd = NULL;
}

LRESULT CALLBACK InputHookHelper::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_NCHITTEST:
		{
			RECT    rc;
			GetClientRect(hWnd, &rc);
			int     agWidth				= 3; //�սǿ��
			POINT    pt;
			pt.x	= lParam & 0xFFFF;
			pt.y	= lParam >> 16;

			ScreenToClient(hWnd, &pt);

			if (pt.x < agWidth				||
				pt.x > rc.right - agWidth	||
				pt.y < agWidth				||
				pt.y > rc.bottom - agWidth
				)
				return HTTRANSPARENT;
		}
		break;
		case WM_KEYDOWN:
			RECT    rc;
			GetClientRect(hWnd, &rc);
			break;
	}

	LRESULT ret = CallWindowProc(m_pWndProc, hWnd, message, wParam, lParam);

	return ret;
}

void InputHookHelper::Start(HWND hAttachWnd)
{
	//�ҵ���ǰ����Ӵ���
	HWND hChild = GetWindow(hAttachWnd, GW_CHILD);
	TCHAR  text[MAX_PATH] ={ 0 };
	while (hChild!=NULL){
		::GetWindowText(hChild, text, MAX_PATH);
		if (StrStr(text, _T("Movie")))
		{
			m_hAttachWnd = hChild;
			break;
		}

		if (StrStr(text, _T("VideoRenderer")))
		{
			m_hAttachWnd = hChild;
			break;
		}
		hChild=::GetNextWindow(hChild, GW_HWNDNEXT);
	}
	////�滻��Ϣ�������̣���һЩ�����Ĳ���
	m_pWndProc = (WNDPROC)GetWindowLong(m_hAttachWnd, GWL_WNDPROC);
	SetWindowLong(m_hAttachWnd, GWL_WNDPROC, (long)WndProc);
	//���ý�����hook���ػ��û��������Ϣ
	m_MsgHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetCurrentThreadId());// GetWindowThreadProcessId(hAttachWnd, NULL));
}

LRESULT CALLBACK InputHookHelper::GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	static BOOL			bIsMouseLeaveTracked	= FALSE;
	static BOOL			bStartWheel				= FALSE;
	static short		xPos					= 0;
	static short		yPos					= 0;

	if(wParam == PM_REMOVE)
	{
		LPMSG lpMsg = (LPMSG)lParam;
		
		TouchEvent::ActionType	nTouchEvent;
		MOUSE_BUTTONS			nButton			= NULL_MOUSE_BUTTON;

		CHAR title[256];
		GetWindowTextA(lpMsg->hwnd, title, 256);
		if (strstr(title, "��") == NULL && g_IsOpenDeviceControl == TRUE){
			switch (lpMsg->message)
			{
			case WM_CHAR:
				if (Common::SystemHelper::ScreenHelper::_bScale)
				{
					//InputManager::GetInstance().CharInputHandler(lpMsg->wParam);
				}
				break;
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				InputManager::GetInstance().KeyActionHandler(lpMsg->wParam, lpMsg->message, lpMsg->lParam & 0x40000000);
				break;
			}
			/*
			case WM_SYSCOMMAND:
			//if (lpMsg->hwnd == InputHookHelper::GetInstance().m_hAttachWnd)
			{
			switch (lpMsg->wParam)
			{
			case SC_CLOSE:
			case SC_MAXIMIZE:
			case SC_MINIMIZE:
			case SC_RESTORE:
			goto system_message_listener;
			}
			}
			break;
			system_message_listener:
			{
			auto iter = InputHookHelper::GetInstance().m_SystemEventListeners.find(lpMsg->message);
			if (iter != InputHookHelper::GetInstance().m_SystemEventListeners.end()){
			for (auto iterListener = iter->second.begin(); iterListener != iter->second.end(); iterListener++)
			{	//observerģʽ�����ε��ö��������
			(*iterListener)();
			}
			}
			break;
			}
			*/
			case WM_MOUSEWHEEL:
				if (bStartWheel == FALSE)
				{
					bStartWheel = TRUE;
					goto mouse_message_listener;
				}
			case WM_MOUSELEAVE:
				//if (lpMsg->hwnd == InputHookHelper::GetInstance().m_hAttachWnd)
			{
				bIsMouseLeaveTracked = FALSE;		//�Զ�ֹͣtrack
				goto mouse_message_listener;
			}
			break;

		mouse_message_listener:
			{
				auto iter = InputHookHelper::GetInstance().m_MouseEventListeners.find(lpMsg->message);
				if (iter != InputHookHelper::GetInstance().m_MouseEventListeners.end())
				{
					for (auto iterListener = iter->second.begin(); iterListener != iter->second.end(); iterListener++)
					{	//observerģʽ�����ε��ö��������
						(*iterListener)(lpMsg->wParam, lpMsg->lParam);
					}
				}
				bStartWheel = FALSE;
			}
			break;

			case WM_LBUTTONDBLCLK:		//���˫�������������ͬ�ȶԴ�
			case WM_LBUTTONDOWN:
				nTouchEvent = TouchEvent::TOUCH_DOWN;
				nButton = LEFT_MOUSE_BUTTON;

				goto mouse_message_handler;
			case WM_LBUTTONUP:
				nTouchEvent = TouchEvent::TOUCH_UP;
				nButton = LEFT_MOUSE_BUTTON;
				goto mouse_message_handler;
			case WM_RBUTTONDOWN:
				nTouchEvent = TouchEvent::TOUCH_DOWN;
				nButton = RIGHT_MOUSE_BUTTON;
				goto mouse_message_handler;
			case WM_RBUTTONUP:
				nTouchEvent = TouchEvent::TOUCH_UP;
				nButton = RIGHT_MOUSE_BUTTON;

				goto mouse_message_handler;
			case WM_MBUTTONDOWN:
				nTouchEvent = TouchEvent::TOUCH_DOWN;
				nButton = MIDDLE_MOUSE_BUTTON;
				goto mouse_message_handler;
			case WM_MBUTTONUP:
				nTouchEvent = TouchEvent::TOUCH_UP;
				nButton = MIDDLE_MOUSE_BUTTON;
				goto mouse_message_handler;

			case WM_MOUSEMOVE:
				if (lpMsg->hwnd != InputHookHelper::GetInstance().m_hAttachWnd)
					break;

				{	//�������Ƿ��ƶ�
					POINT ptCursor;
					GetCursorPos(&ptCursor);
					if (ptCursor.x == xPos && ptCursor.y == yPos)
						break;	//���λ��û�ƶ��Ͳ������ƶ�����
					else
					{
						xPos	= ptCursor.x;
						yPos	= ptCursor.y;
					}
				}

				nTouchEvent = TouchEvent::TOUCH_MOVE;
				if (lpMsg->wParam & MK_LBUTTON)
					nButton = LEFT_MOUSE_BUTTON;
				else if (lpMsg->wParam & MK_RBUTTON)
					nButton = RIGHT_MOUSE_BUTTON;
				else if (lpMsg->wParam & MK_MBUTTON)
					nButton = MIDDLE_MOUSE_BUTTON;
				else
					nButton = NULL_MOUSE_BUTTON;

				if (bIsMouseLeaveTracked == FALSE)
				{
					TRACKMOUSEEVENT TrackEvent = { 0 };
					TrackEvent.cbSize = sizeof(TrackEvent);
					TrackEvent.dwFlags = TME_LEAVE;
					TrackEvent.hwndTrack = InputHookHelper::GetInstance().m_hAttachWnd;
					_TrackMouseEvent(&TrackEvent);
					bIsMouseLeaveTracked = TRUE;
				}

				goto mouse_message_handler;

			mouse_message_handler:
				{
					if (lpMsg->hwnd == InputHookHelper::GetInstance().m_hAttachWnd)
					{
						auto xPos = GET_X_LPARAM(lpMsg->lParam);
						auto yPos = GET_Y_LPARAM(lpMsg->lParam);

						if (nTouchEvent != TouchEvent::TOUCH_MOVE)
						{
							TRACE("mouse event : %x button %x coordinate (%d %d)\n", nTouchEvent, nButton, xPos, yPos);
						}

						for (auto iter = InputHookHelper::GetInstance().m_MouseEventHandlers.begin(); iter != InputHookHelper::GetInstance().m_MouseEventHandlers.end(); iter++)
						{	//ְ����ģʽ�����ε��ö�������������������һ������TRUE����ʾ���Ѿ�������ˣ�����Ҫ����������
							if ((*iter)(nButton, xPos, yPos, nTouchEvent) == TRUE)
								break;
						}
					}
					break;
				}
			default:
				break;
			}
		}												
	}

	return CallNextHookEx(InputHookHelper::GetInstance().m_MsgHook, code, wParam, lParam);
}

HWND InputHookHelper::GetAttachWnd()
{
	if (IsWindow(m_hAttachWnd))
		return m_hAttachWnd;

	return m_hAttachWnd = NULL;
}
