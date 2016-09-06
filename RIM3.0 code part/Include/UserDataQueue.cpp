#include "StdAfx.h"
#include "UserDataQueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CQueueData::CQueueData(void)
{
}

CQueueData::~CQueueData(void)
{

}

CUserDataQueue::CUserDataQueue(void)
{
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CUserDataQueue::~CUserDataQueue(void)
{
	if(m_hEvent != NULL)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}

void CUserDataQueue::Push(CQueueData *pNetData)
{
	ASSERT(pNetData);

	CSingleLock sLock(&m_cs, TRUE);

// 	if(pSendData->GetDataType() == sdt_control)
// 		m_dataList.AddHead(pSendData);
// 	else
		m_dataList.AddTail(pNetData);

	SetEvent(m_hEvent);
}

int CUserDataQueue::Pop(CQueueData **ppNetData)
{
	*ppNetData = NULL;
	int count = m_dataList.GetCount();
	if(count <= 0)
		return count;

	CSingleLock sLock(&m_cs, TRUE);

	*ppNetData = m_dataList.RemoveHead();
	count--;

	if(count == 0)
		ResetEvent(m_hEvent);

	return count;
}

HANDLE CUserDataQueue::GetQueueEvent()
{
	return m_hEvent;
}

int	CUserDataQueue::GetQueueCount()
{
	CSingleLock sLock(&m_cs, TRUE);

	return m_dataList.GetCount();
}

