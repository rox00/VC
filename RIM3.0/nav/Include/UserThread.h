//********************************************************************
//	USERTHREAD.H �ļ�ע��
//	�ļ���		: USERTHREAD.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\INCLUDE/
//	����		: ����
//	����ʱ��	: 2015/1/13 14:40
//	�ļ�����	: 
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

#ifdef DEBUG					//��������DEBUGʱ���߳��������˳�ʱ��һЩ��Ϣ���
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