//********************************************************************
//	USERTHREAD.H 文件注释
//	文件名		: USERTHREAD.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\INCLUDE/
//	作者		: 张永
//	创建时间	: 2015/1/13 14:40
//	文件描述	: 
//*********************************************************************	
#pragma once

class CUserThread
{
	friend	 UINT				__ThreadProc(LPVOID lpParam);

public:
	CUserThread(void);
	virtual ~CUserThread(void);

protected:
	CWinThread					*m_pThread;
	HANDLE						m_hExit;
	BOOL						m_bInitCom;

#ifdef DEBUG					//这里用于DEBUG时对线程启动和退出时的一些信息输出
	CString						m_strName;
	CString						GetName();
	void						GetName(LPCTSTR lpName);
#endif

protected:
	virtual UINT				ThreadProc() = 0;

public:
	virtual HRESULT				Begin(BOOL bInitCom = FALSE);
	virtual void				End();

	BOOL						SetPriority(int nPriority);

};