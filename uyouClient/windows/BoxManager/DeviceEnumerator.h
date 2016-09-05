#pragma once

#include <BlockingQueue.h>
#include <singleton.h>
#include <boost/thread/recursive_mutex.hpp>


class CDeviceEnumerator : public Singleton<CDeviceEnumerator>
{
	DECLARE_SINGLETON(CDeviceEnumerator)

protected:
	virtual void								InitialInstance();
	virtual void								DisposeInstance();

public:
	HRESULT										Refresh();

	String										GetUnallocatedCable(DWORD *nCableId);
	void										WindowAttached(LPCTSTR lpszCableName, HWND hAttachedWnd);
	void										WindowDetached(LPCTSTR lpszCableName, HWND hAttachedWnd);
protected:
	static COLORREF								GetBackgroundWindowPixelColor(HWND hWnd, int x, int y);
	static COLORREF								GetWindowPixelColor(HWND hWnd, int x, int y);
	static UINT									GetWindowColorValue(HWND hWnd);

	void										WorkerThread();

	void										GotDeviceIdNotify(HWND hWnd, String szDeviceId);

private:
	typedef std::pair<HWND, String>				CableInfo;

	CString										m_adbPath;
	mutable boost::recursive_mutex				g_GeneralMutex;
	std::map<String, CableInfo>					m_CableInfoMap;				//��cable name���󶨵�hwnd���Լ�android device id��ӳ��

	BlockingQueue<CableInfo>					m_FindDeviceTaskQueue;		//�Դ��߳�ͬ��hwnd�Լ�cable name
};