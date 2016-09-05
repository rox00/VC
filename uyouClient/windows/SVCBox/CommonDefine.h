#pragma once

enum WM_SVC_NOTIFY
{
	WM_NOTIFY_CONNECT_STATE_CHANGED	= WM_USER + 10000	,
	WM_NOTIFY_DEVICE_MESSAGE_RECEIVED					,
	WM_NOTIFY_VIDEO_READY								,
	WM_NOTIFY_RESTART									,
	WM_NOTIFY_COPYDATA
};

enum ConnectStatus
{
	DISCONNECT = 0,
	CONNECTING,
	CONNECTED
};

enum BoxFlags
{
	BoxFlags_IsOpenDeviceControl = 1,
};
extern BOOL	g_IsOpenDeviceControl;
extern long	g_currentDeviceWidth;
extern long	g_currentDeviceHeight;