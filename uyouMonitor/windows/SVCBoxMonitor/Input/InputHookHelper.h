#pragma once

#include <singleton.h>

typedef std::function<BOOL(int, int, int, TOUCHTYPE)> MouseEventHandler;
typedef std::function<void(WPARAM, LPARAM)> MouseEventListener;
typedef std::function<void()> SystemEventListener;

class InputHookHelper : public SinglePattern
{
	DECLARE_SINGLETON(InputHookHelper)

public:
	enum MOUSE_BUTTONS
	{
		LEFT_MOUSE_BUTTON,
		MIDDLE_MOUSE_BUTTON,
		RIGHT_MOUSE_BUTTON,
		NULL_MOUSE_BUTTON,
		MOUSE_BUTTON_NUMBER
	};

public:
	virtual void										InitialInstance();
	virtual void										DisposeInstance();

public:
	void												Start(HWND hAttachWnd);
	void												AddMouseEventHandler(MouseEventHandler handler) { m_MouseEventHandlers.push_back(handler);  }
	void												AddMouseEventListener(UINT nMessage, MouseEventListener listener) { m_MouseEventListeners[nMessage].push_back(listener); }
	void												AddSystemEventListener(UINT nMessage, SystemEventListener listener) { m_SystemEventListeners[nMessage].push_back(listener); }	
	HWND												GetAttachWnd();
protected:
	static LRESULT CALLBACK								GetMsgProc(int code, WPARAM wParam,	LPARAM lParam);
	static LRESULT CALLBACK								WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	static WNDPROC										m_pWndProc;
	HHOOK												m_MsgHook;
	HWND												m_hAttachWnd;
	std::list<MouseEventHandler>						m_MouseEventHandlers;
	std::map<UINT, std::list<MouseEventListener>>		m_MouseEventListeners;
	std::map<UINT, std::list<SystemEventListener>>		m_SystemEventListeners;
};