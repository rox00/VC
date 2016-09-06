//********************************************************************
//	USERTHREAD.CPP 文件注释
//	文件名		: USERTHREAD.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\INCLUDE/
//	作者		: 张永
//	创建时间	: 2015/1/13 14:41
//	文件描述	: 
//*********************************************************************	

#include "StdAfx.h"
#include "UserThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT __ThreadProc(LPVOID lpParam)
{
	ASSERT(lpParam);
	CUserThread *pThread = (CUserThread*)lpParam;

	if(pThread->m_bInitCom)
		CoInitialize(NULL);

	UINT uiRet = pThread->ThreadProc();

	if(pThread->m_bInitCom)
		CoUninitialize();

#ifdef DEBUG
	if(!pThread->m_strName.IsEmpty())
	{
		CString strMsg = _T("Thread \"");
		strMsg +=	pThread->m_strName;
		strMsg +=	_T("\" Is Exited");

		TRACE(strMsg);
	}
#endif

	return uiRet;
}

CUserThread::CUserThread(void)
{
	m_pThread		= NULL;
	m_hExit			= NULL;
	m_bInitCom		= FALSE;
}

CUserThread::~CUserThread(void)
{
	End();
}

HRESULT CUserThread::Begin(BOOL bInitCom)
{
	ASSERT(m_hExit == NULL);

	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(m_hExit == NULL)
		return E_POINTER;

	m_bInitCom	= bInitCom;
	m_pThread	= AfxBeginThread(::__ThreadProc, LPVOID(this), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

	if(m_pThread == NULL)
		return E_POINTER;

	return NOERROR;
}

void CUserThread::End()
{
	if(m_pThread && m_hExit)
	{
		SetEvent(m_hExit);
// 		::PostThreadMessage(m_pThread->m_nThreadID, WM_QUIT, 0, 0);

		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		delete m_pThread;
		m_pThread = NULL;

		CloseHandle(m_hExit);
		m_hExit = NULL;
	}
}

BOOL CUserThread::SetPriority(int nPriority)
{
	if(m_pThread)
		return m_pThread->SetThreadPriority(nPriority);

	return FALSE;
}

#ifdef DEBUG

CString	CUserThread::GetName()
{
	return m_strName;
}

void CUserThread::GetName(LPCTSTR lpName)
{
	m_strName = lpName;
}

#endif
