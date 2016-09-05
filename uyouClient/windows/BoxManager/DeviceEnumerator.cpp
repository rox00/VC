#include "stdafx.h"
#include <strmif.h>
#include <uuids.h>
#include <atlbase.h>
#include <atlstr.h>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "DeviceEnumerator.h"
#include "BoxFinder.h"
#include "../Common/AdbUtils.h"

#pragma comment(lib,"Strmiids.lib") 
#ifdef DEBUG
#pragma comment(lib,"CommonD.lib") 
#else
#pragma comment(lib,"Common.lib") 
#endif

#define VIDEO_DEVICE_PID_INFO					L"vid_0c45&pid_6366"
#define VIDEO_PCI_PID_INFO						L"pci#ven_1131&dev_7160"
#define FIND_ADB_DEVICE_TIME_INTERVAL			500
#define FIND_ADB_DEVICE_FAIL_INTERVAL			3000
#define FIND_ADB_DEVICE_WAIT_COLOR_SHOW_TIME	3000


void CDeviceEnumerator::InitialInstance()
{
	m_adbPath		= g_szEXEPath + ADB_EXE_RELATIVE_PATH;
	boost::thread(std::bind(&CDeviceEnumerator::WorkerThread, this)).detach();
}

void CDeviceEnumerator::DisposeInstance()
{
	AdbUtils AdbUtilsHelper(m_adbPath);

	AdbUtilsHelper.KillServer();
}

void CDeviceEnumerator::WorkerThread()
{
	CoInitialize(NULL);

	AdbUtils AdbUtilsHelper(m_adbPath);

	AdbUtilsHelper.RestartServer();

	while (TRUE)
	{
		std::list<CableInfo> TaskList;

		//for test only
		//ATLASSERT(FALSE);

		do 
		{	//��������������û��������������У�һ��ȡ����  û�������ʱ������� 
			auto FirstFindDeviceTask = m_FindDeviceTaskQueue.take();
			
			////�п��ܴ����Ѳ��ڣ������б��ڣ���ʱӦ���������������ȥ �������ټӽ������б���
			if (::IsWindow(FirstFindDeviceTask.first))					
				TaskList.push_back(FirstFindDeviceTask);
		} while (m_FindDeviceTaskQueue.size() != 0);

		//��adb���������Ѿ����ӵ��豸
		AdbUtils::StringArray Serials;
		while (TRUE)
		{
			AdbUtilsHelper.GetSerialNo(Serials);
			if (!Serials.empty())
				break;
			else
				AdbUtilsHelper.RestartServer();		//���һ���豸Ҳ�Ҳ�������ͼ����adb

			Sleep(10);
		}

		//��arrayתΪset���Ա��������
		std::set<String> SerialSet;
		for each(auto SerialNo in Serials)
		{
			SerialSet.insert(SerialNo);
		}

		//ȥ���Ѿ���Ե�adb�豸
		do
		{	//do...while(0)��Ϊ�˸�lockһ����Χ
			boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

			for each(auto iter in m_CableInfoMap)
			{	//���Ѿ���Ե�ɾ����ֻ����û����Ե�SerialSet
				if (!iter.second.second.empty() && iter.second.first)
					SerialSet.erase(iter.second.second);
			}
		} while (0);

		//û��δƥ���adb�豸��
		if (SerialSet.size() == 0)
		{
			for each(auto FindDeviceTask in TaskList)
			{
				m_FindDeviceTaskQueue.put(FindDeviceTask);	//������Żض��У��ȴ�Ƭ�̺�����
			}
			
			Common::SystemHelper::DelayMS(FIND_ADB_DEVICE_FAIL_INTERVAL);
			continue;
		}
		
		//ֻ��һ��ƥ������ֻ��һ��adb�豸�������ж���
		if (SerialSet.size() == 1 && TaskList.size() == 1) 
		{
			GotDeviceIdNotify(TaskList.front().first, *SerialSet.begin());
			continue;
		}

		//��һ����һ�����ϵ�adb�豸��׼������ɫ������
		//��ɫ����ȫ��ͬɫ��ÿ����ɫ��RGBȫ��ȣ�10�����һ�����ӣ�20����ڶ������ӣ��Դ����ơ��ܹ��ܹ�֧�����26������
		std::vector<String> SerialArray(SerialSet.size());
		UINT nIndex = 0;
		for each(auto AndroidDeviceSerial in SerialSet)
		{
			SerialArray[nIndex++] = AndroidDeviceSerial;
			COLORREF ShowColor = RGB(nIndex * 10, nIndex * 10, nIndex * 10);

			//��ɱ����
			AdbUtilsHelper.RunShellCommand(_T("am force-stop com.uyou.determinewindow"), AndroidDeviceSerial.c_str(), 3000);
			//����ʾ����ʾ����ɫ
			String szShellCommand = _T("am start -n com.uyou.determinewindow/com.uyou.determinewindow.MainActivity --el ColorVal ") + boost::lexical_cast<String>(ShowColor | 0xff000000);
			AdbUtilsHelper.RunShellCommand(szShellCommand.c_str(), AndroidDeviceSerial.c_str(), 3000);
		}

		Common::SystemHelper::DelayMS(FIND_ADB_DEVICE_WAIT_COLOR_SHOW_TIME);	//�ȴ�Ӧ����������


		for each(auto FindDeviceTask in TaskList)
		{
			HWND hWnd = FindDeviceTask.first;
			//UINT nColorValue = GetWindowColorValue(hWnd);
			//nIndex = nColorValue / 10 - 1;		//����10�Ժ�������index

			//if (nColorValue == -1 || nIndex >= SerialArray.size())
			//{
			//	//ʧ���ˣ�׼������
			//	m_FindDeviceTaskQueue.put(FindDeviceTask);	//������Żض��У���һ���������
			//	continue;
			//}

			for (nIndex = 0; nIndex < SerialArray.size(); ++nIndex)
			{
				CString copyStr = _T("");//���豸��ʶ����ȥ���ڱ���Ƿ��ϴ��豸
				copyStr.Format(_T("testindex=%d"), nIndex);

				COPYDATASTRUCT copyData ={ 0 };
				copyData.lpData = (PVOID)copyStr.GetBuffer();
				copyData.cbData = (copyStr.GetLength() + 1) * sizeof(TCHAR);
				if (TRUE == SendMessage(hWnd, WM_COPYDATA, 1, (LPARAM)&copyData))
				{
					break;
				}
			}

			if (nIndex >= SerialArray.size())
			{
				//ʧ���ˣ�׼������
				m_FindDeviceTaskQueue.put(FindDeviceTask);	//������Żض��У���һ���������
				continue;
			}
	
			GotDeviceIdNotify(hWnd, SerialArray[nIndex]);
		}

		//�Ѹղ������Ľ��̶�ɱ��
		for each(auto AndroidDeviceSerial in SerialSet)
		{
			AdbUtilsHelper.RunShellCommand(_T("am force-stop com.uyou.determinewindow"), AndroidDeviceSerial.c_str(), 3000);
		}
	}

	return;
}

HRESULT CDeviceEnumerator::Refresh()
{
	boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

	ATL::CComPtr<ICreateDevEnum> pSysDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr))
		return hr;

	ATL::CComPtr<IEnumMoniker> pEnumCat;
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);
	if (FAILED(hr) || pEnumCat == NULL)
		return hr;

	std::map<String, CableInfo>	 tempMap(m_CableInfoMap);  //���ݵ�ǰ��������ͷ�豸·����Ϣ
	while (TRUE)
	{
		ATL::CComPtr<IMoniker> pMoniker;
		ULONG cFetched;
		hr = pEnumCat->Next(1, &pMoniker, &cFetched);
		if (hr != S_OK)		//��Ӧ����FAILED()���ж�
			break;

		ATL::CComPtr<IPropertyBag> pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
		if (FAILED(hr))
			continue;

		ATL::CComVariant varDevicePath;
		hr = pPropBag->Read(L"DevicePath", &varDevicePath, 0);
		if (FAILED(hr))
			continue;

		//varDevicePath�����Ӵ������£�
		//varDevicePath	= L"\\\\?\\usb#vid_0c45&pid_6366&mi_00#8&183af011&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global"
		BOOL bFound		= FALSE;
		//BOX
		if (StrStrIW(varDevicePath.bstrVal, VIDEO_DEVICE_PID_INFO))
			bFound		= TRUE;
		//PCI
		//if (StrStrIW(varDevicePath.bstrVal, VIDEO_PCI_PID_INFO))
		//	bFound		= TRUE;

		if (bFound)
		{
			auto iter = m_CableInfoMap.find(varDevicePath.bstrVal);
			if (iter == m_CableInfoMap.end())
			{	//û���ҵ����cable�����֣��ӽ�ȥ
				m_CableInfoMap[varDevicePath.bstrVal].first			= 0;		//����
			}
			else
			{	//�ҵ���
				tempMap.erase(iter->first);										////ɾ����ǰ���ҵ�������ͷ��Ϣ

				if (!::IsWindow(iter->second.first))
				{	//�ж��Ƿ����ڴ��ڱ��������Ĳ�������Ϊδ���
					m_CableInfoMap[varDevicePath.bstrVal].first		= 0;		//���ҵ���δ���
					m_CableInfoMap[varDevicePath.bstrVal].second	= _T("");	//���ҵ���δ���
				}
			}
		}
	}

	for each (auto it in tempMap)
	{	//�����Ѳ������豸�˴�Ӧ�����֮
		m_CableInfoMap.erase(it.first);
	}

	return S_OK;
}

String CDeviceEnumerator::GetUnallocatedCable(DWORD *pnCableId)
{
	boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

	UINT nCableId = 0;
	for each(auto iter in m_CableInfoMap)
	{
		if (iter.second.first == 0)	//�����û�з����ȥ��û�а󶨴��ڣ�����������ȥ
		{
			*pnCableId = nCableId;
			return iter.first;
		}
		nCableId++;
	}
	return _T("");
}

void CDeviceEnumerator::WindowAttached(LPCTSTR lpszCableName, HWND hAttachedWnd)
{
	boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

	if (m_CableInfoMap.find(lpszCableName) != m_CableInfoMap.end())
	{	//�ҵ����cable������ ����������Ϣ֮���
		m_CableInfoMap[lpszCableName].first = hAttachedWnd;

		//if (m_CableInfoMap[lpszCableName].second.empty())
		{	//֮ǰû�м�¼���󶨹�ϵ��׼�����Ұ󶨹�ϵ
			m_FindDeviceTaskQueue.put(CableInfo(hAttachedWnd, lpszCableName));
		}
		//else
		//{	//֮ǰ���а󶨹�ϵ�ˣ�ֱ��֪ͨ�󶨳ɹ�
		//	GotDeviceIdNotify(hAttachedWnd, m_CableInfoMap[lpszCableName].second);
		//}
	}
}

void CDeviceEnumerator::WindowDetached(LPCTSTR lpszCableName, HWND hAttachedWnd)
{
	boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

	if (m_CableInfoMap.find(lpszCableName) != m_CableInfoMap.end())
	{	//�ҵ����cable������
		if (m_CableInfoMap[lpszCableName].first == hAttachedWnd)
		{
			m_CableInfoMap[lpszCableName].first				= 0;
			m_CableInfoMap[lpszCableName].second			= _T("");
		}
	}
}

void CDeviceEnumerator::GotDeviceIdNotify(HWND hWnd, String szDeviceId)
{
	boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

	//֪ͨ�ͻ����Ѿ��ҵ���ƥ��SVC Cable��Android�豸
	//������������COM��event�������ģ���̫�鷳�ˣ���ΪWindows��Ϣ

	//��¼�󶨹�ϵ���´ΰ��������һЩ
	for (auto iter = m_CableInfoMap.begin(); iter != m_CableInfoMap.end(); iter++)
	{
		if (iter->second.first == hWnd)
		{
			CString copyStr;
			copyStr.Format(_T("cablename=%s::deviceid=%s"), iter->first.c_str(), szDeviceId.c_str());//���豸��ʶ����ȥ���ڱ���Ƿ��ϴ��豸

			COPYDATASTRUCT copyData ={ 0 };
			copyData.lpData = (PVOID)copyStr.GetBuffer();
			copyData.cbData = (copyStr.GetLength() + 1) * sizeof(TCHAR);

			//ע������֪ͨ���Ǹ����ڣ�Ҳ����OCX Ctrl
			if (TRUE == SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)&copyData))
			{
				OutputDebugString(copyStr);
				iter->second.second = szDeviceId;
			}
		}
	}

}

UINT CDeviceEnumerator::GetWindowColorValue(HWND hWnd)
{
	UINT ColorValue = -1;

	POINT PixelList[] =
	{	//��ʱȡ���²�����
		{ 100, 100 },
		{ 200, 200 },
		{ 200, 100 },
		{ 100, 200 },
		{ 300, 300 },
		{ 100, 300 },
		{ 300, 100 },
		{ 200, 300 },
		{ 300, 200 },
	}; 

	for each(auto Pixel in PixelList)
	{
		auto GotColor = GetWindowPixelColor(hWnd, Pixel.x, Pixel.y);
		if (GotColor == -1)		//��һ�����ȡ��ɫʧ�ܣ�����Ϊʧ��
			return -1;

		//ע�⣺Android����ɫ��Ⱦ���ƱȽ���֡�����Ҫ��(x, x, x)��������ɫ��ʱ��������ÿ��x�ϼ�һ�����ȵ��Ŷ���һ���Ŷ�Ϊ������ż������������ֵ������10
		//����������ж�Ҳ����΢����һ��
		if (abs(GetRValue(GotColor) - GetGValue(GotColor)) >= 10 || abs(GetBValue(GotColor) - GetGValue(GotColor)) >= 10
			|| abs(GetBValue(GotColor) - GetRValue(GotColor)) >= 10)	//����������ȫ���
			return -1;

		UINT GotColorValue = (GetRValue(GotColor) + GetGValue(GotColor) + GetBValue(GotColor)) / 3;

		//��һ�γ���
		if (ColorValue == -1)
			ColorValue = GotColorValue;
		else if (GotColorValue != ColorValue)		//���Ե����ɫ�����һ������Ϊʧ��
			return - 1;
	}

	return ColorValue;
}

COLORREF CDeviceEnumerator::GetWindowPixelColor(HWND hWnd, int x, int y)
{
	auto hDC = GetDC(hWnd);
	if (hDC == NULL)
		return -1;

	auto iColor = GetPixel(hDC, x, y);

	ReleaseDC(hWnd, hDC);

	return iColor;
}

//�Ӱ������������ҵ��ĺ�̨ȡɫ���룬���Ǻܺ��ã���ʱ����
COLORREF CDeviceEnumerator::GetBackgroundWindowPixelColor(HWND hWnd, int x, int y)
{
	//CopyScreenToBitmap(hWnd)
	HDC hScrDC = 0, hMemDC;
	HBITMAP hBitmap;
	RECT rect;
	
	if (GetWindowRect(hWnd, &rect) == NULL)
		return -1;

	hScrDC = GetWindowDC(hWnd);
	if (hScrDC == NULL)
		return -1;

	hMemDC = CreateCompatibleDC(hScrDC);
	int nWidth = rect.right - rect.left + 1;
	int nHeight = rect.bottom - rect.top + 1;
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	SelectObject(hMemDC, hBitmap);
	PrintWindow(hWnd, hMemDC, 0);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hScrDC);

	if (!hBitmap)
		return -1;

	POINT sPoint = { 0 };
	::ClientToScreen(hWnd, &sPoint);
	x += (sPoint.x - rect.left),
	y += (sPoint.y - rect.top);

	//int nRet = GetC(hBitmap, x + xd, y + yd, &Cout);

	//����ʧ��ʱ��Ĭ��ֵ
	COLORREF Cout = -1;

	//��ʼ����
	BITMAP Bitmap;
	//���￪ʼ������Ӧ�ľ�������ݽ��г�ʼ��
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap))
	{
		//��ʼ��ʧ�ܣ�����
		return -1;
	}

	if (x >= Bitmap.bmWidth || x < 0 || y >= Bitmap.bmHeight || y < 0)//��ȡɫ�ĵ�����곬�����ڷ�Χ�����غ�ɫ
	{
		return -1;
	}
	//��ʼ��Bitmapλͼ�ṹ
	BITMAPINFOHEADER bi;
	WORD wBitCount = 24;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	//������Ҫ����Ŀռ��С��
	DWORD dwBmBitsSize = ((Bitmap.bmWidth*wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	LPBITMAPINFOHEADER lpbi;
	HANDLE hDib, hPal, hOldPal = NULL;
	hDib = GlobalAlloc(GHND, dwBmBitsSize);//����ռ䡣
	if (hDib)
	{
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);//�����ڴ�ռ�
		hPal = GetStockObject(DEFAULT_PALETTE);//��ȡĬ�ϵĵ�ɫ��
		if (hPal)
		{
			HDC hDC = ::GetDC(NULL);//��ȡ��ĻDC
			hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);//�����µĵ�ɫ�壬������ԭ���ĵ�ɫ��
			RealizePalette(hDC);//������ɫ��
			//��λͼ��Ϣ��ָ���ĸ�ʽ�洢��������
			if (GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi, (LPBITMAPINFO)&bi, DIB_RGB_COLORS))
			{
				LPSTR dq = (LPSTR)lpbi;
				int jsy = Bitmap.bmWidth * 3 + Bitmap.bmWidth % 4;//����һ��λͼ��Ϣռ�õ��ڴ��С
				long wz = (Bitmap.bmHeight - 1)*jsy - jsy*y + x * 3;//������Ҫȡɫ��λ��
				//��ȡ��Ӧ������
				BYTE r = BYTE(dq[wz + 2]);
				BYTE g = BYTE(dq[wz + 1]);
				BYTE b = BYTE(dq[wz]);
				Cout = RGB(r, g, b);
			}

			if (hOldPal)//�ָ���ɫ��
			{
				SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
				RealizePalette(hDC);
				ReleaseDC(NULL, hDC);
			}
		}

		//�ͷ��ڴ�ռ�
		GlobalUnlock(hDib);
		GlobalFree(hDib);
	}

	::DeleteObject(hBitmap);
	
	return Cout;
}