//********************************************************************
// READWRITELOCK.H 文件注释
// 文件名 　: READWRITELOCK.H
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 11:56
// 文件描述 : 
//*********************************************************************
#pragma once
//////读写锁
class CReadWriteLock
{
public:
	enum{
		LOCK_LEVEL_NONE		= 0,
		LOCK_LEVEL_READ		= 1,
		LOCK_LEVEL_WRITE	= 2,
	};

	int    				m_currentLevel;
	int    				m_readCount;   
	HANDLE 				m_unlockEvent; 
	HANDLE 				m_accessMutex;
	CRITICAL_SECTION	m_csStateChange;
public:
	CReadWriteLock() 
	{
		m_currentLevel = LOCK_LEVEL_NONE;
		m_readCount    = 0;
		m_unlockEvent  = ::CreateEvent( NULL, TRUE, FALSE, NULL );
		m_accessMutex  = ::CreateMutex( NULL, FALSE, NULL );
		::InitializeCriticalSection( &m_csStateChange );
	}
	~CReadWriteLock() 
	{
		::DeleteCriticalSection( &m_csStateChange );
		if ( m_accessMutex ) ::CloseHandle( m_accessMutex );
		if ( m_unlockEvent ) ::CloseHandle( m_unlockEvent );
	}

	bool lock( int level, int timeout = INFINITE) 
	{
		bool  bresult    = true;
		DWORD waitResult = 0;

		waitResult = ::WaitForSingleObject( m_accessMutex, timeout );
		if ( waitResult != WAIT_OBJECT_0 )  return false;
		if ( level == LOCK_LEVEL_READ && m_currentLevel != LOCK_LEVEL_WRITE )
		{
			::EnterCriticalSection( &m_csStateChange );
			m_currentLevel = level;
			m_readCount += 1;
			::ResetEvent( m_unlockEvent );
			::LeaveCriticalSection( &m_csStateChange );
		}
		else if ( level == LOCK_LEVEL_READ && 
			m_currentLevel == LOCK_LEVEL_WRITE )
		{
			waitResult = ::WaitForSingleObject( m_unlockEvent, timeout );
			if ( waitResult == WAIT_OBJECT_0 )
			{
				::EnterCriticalSection( &m_csStateChange );
				m_currentLevel = level;
				m_readCount += 1;
				::ResetEvent( m_unlockEvent );
				::LeaveCriticalSection( &m_csStateChange );
			}
			else bresult = false;
		}
		else if ( level == LOCK_LEVEL_WRITE && 
			m_currentLevel == LOCK_LEVEL_NONE )
		{
			::EnterCriticalSection( &m_csStateChange );
			m_currentLevel = level;
			::ResetEvent( m_unlockEvent );
			::LeaveCriticalSection( &m_csStateChange );
		}
		else if ( level == LOCK_LEVEL_WRITE && 
			m_currentLevel != LOCK_LEVEL_NONE )
		{
			waitResult = ::WaitForSingleObject( m_unlockEvent, timeout );
			if ( waitResult == WAIT_OBJECT_0 )
			{
				::EnterCriticalSection( &m_csStateChange );
				m_currentLevel = level;
				::ResetEvent( m_unlockEvent );
				::LeaveCriticalSection( &m_csStateChange );
			}
			else bresult = false;
		}
		::ReleaseMutex( m_accessMutex );
		return bresult;
	} // lock()

	bool unlock() 
	{ 
		::EnterCriticalSection( &m_csStateChange );
		if ( m_currentLevel == LOCK_LEVEL_READ )
		{
			m_readCount --;
			if ( m_readCount == 0 ) 
			{
				m_currentLevel = LOCK_LEVEL_NONE;
				::SetEvent (m_unlockEvent);
			}
		}
		else if ( m_currentLevel == LOCK_LEVEL_WRITE )
		{
			m_currentLevel = LOCK_LEVEL_NONE;
			::SetEvent ( m_unlockEvent );
		}
		::LeaveCriticalSection( &m_csStateChange );
		return true;
	}
};  
