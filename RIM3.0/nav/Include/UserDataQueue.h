#pragma once

class CQueueData
{
public:
	CQueueData(void);
	virtual ~CQueueData(void);
};

class CUserDataQueue
{
public:
	CUserDataQueue(void);
	~CUserDataQueue(void);

protected:

	CList<CQueueData*,CQueueData*>			m_dataList;
	HANDLE									m_hEvent;
	CCriticalSection						m_cs;

public:
	HANDLE									GetQueueEvent();

	void									Push(CQueueData *pSendData);
	int										Pop(CQueueData **ppSendData);
	int										GetQueueCount();

};

