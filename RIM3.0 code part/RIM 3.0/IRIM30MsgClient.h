#pragma once


// IRIM30MsgClient

interface IRIM30MsgClient
{
	virtual bool				IsConnected() = 0;
	virtual void				PushMsg(LPRIM30MSG pMsg) = 0;
	virtual void				PopMsg(LPRIM30MSG* pMsg) = 0;
};


