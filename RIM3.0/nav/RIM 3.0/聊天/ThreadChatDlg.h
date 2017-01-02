#pragma once

#include "ChatBkDlg.h"

// CChatDlgThread

class CThreadChatDlg : public CWinThread
{
	DECLARE_DYNCREATE(CThreadChatDlg)

protected:
	CThreadChatDlg();           // protected constructor used by dynamic creation
	virtual ~CThreadChatDlg();

public:
	virtual BOOL								InitInstance();
	virtual int									ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

	LPUSERINFO									m_pFriendUser;
public:
	void										ActiveWnd();
	void										SetUser(LPUSERINFO	pUser);
};


