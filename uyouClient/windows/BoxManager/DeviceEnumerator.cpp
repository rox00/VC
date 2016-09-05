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
		{	//看看队列里面有没有其他任务，如果有，一起取出来  没有任务的时候会阻塞 
			auto FirstFindDeviceTask = m_FindDeviceTaskQueue.take();
			
			////有可能窗口已不在，任务列表还在，此时应当从任务中清除出去 即不发再加进缓存列表中
			if (::IsWindow(FirstFindDeviceTask.first))					
				TaskList.push_back(FirstFindDeviceTask);
		} while (m_FindDeviceTaskQueue.size() != 0);

		//用adb查找所有已经连接的设备
		AdbUtils::StringArray Serials;
		while (TRUE)
		{
			AdbUtilsHelper.GetSerialNo(Serials);
			if (!Serials.empty())
				break;
			else
				AdbUtilsHelper.RestartServer();		//如果一个设备也找不到，试图重启adb

			Sleep(10);
		}

		//从array转为set，以便后续操作
		std::set<String> SerialSet;
		for each(auto SerialNo in Serials)
		{
			SerialSet.insert(SerialNo);
		}

		//去掉已经配对的adb设备
		do
		{	//do...while(0)是为了给lock一个范围
			boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

			for each(auto iter in m_CableInfoMap)
			{	//把已经配对的删掉，只留下没有配对的SerialSet
				if (!iter.second.second.empty() && iter.second.first)
					SerialSet.erase(iter.second.second);
			}
		} while (0);

		//没有未匹配的adb设备了
		if (SerialSet.size() == 0)
		{
			for each(auto FindDeviceTask in TaskList)
			{
				m_FindDeviceTaskQueue.put(FindDeviceTask);	//把任务放回队列，等待片刻后重来
			}
			
			Common::SystemHelper::DelayMS(FIND_ADB_DEVICE_FAIL_INTERVAL);
			continue;
		}
		
		//只有一个匹配任务，只有一个adb设备，不用判断了
		if (SerialSet.size() == 1 && TaskList.size() == 1) 
		{
			GotDeviceIdNotify(TaskList.front().first, *SerialSet.begin());
			continue;
		}

		//有一个或一个以上的adb设备，准备用颜色来查找
		//颜色规则：全屏同色，每个颜色的RGB全相等，10代表第一个盒子，20代表第二个盒子，以此类推。总共能够支持最多26个盒子
		std::vector<String> SerialArray(SerialSet.size());
		UINT nIndex = 0;
		for each(auto AndroidDeviceSerial in SerialSet)
		{
			SerialArray[nIndex++] = AndroidDeviceSerial;
			COLORREF ShowColor = RGB(nIndex * 10, nIndex * 10, nIndex * 10);

			//先杀进程
			AdbUtilsHelper.RunShellCommand(_T("am force-stop com.uyou.determinewindow"), AndroidDeviceSerial.c_str(), 3000);
			//再显示该显示的颜色
			String szShellCommand = _T("am start -n com.uyou.determinewindow/com.uyou.determinewindow.MainActivity --el ColorVal ") + boost::lexical_cast<String>(ShowColor | 0xff000000);
			AdbUtilsHelper.RunShellCommand(szShellCommand.c_str(), AndroidDeviceSerial.c_str(), 3000);
		}

		Common::SystemHelper::DelayMS(FIND_ADB_DEVICE_WAIT_COLOR_SHOW_TIME);	//等待应用启动起来


		for each(auto FindDeviceTask in TaskList)
		{
			HWND hWnd = FindDeviceTask.first;
			//UINT nColorValue = GetWindowColorValue(hWnd);
			//nIndex = nColorValue / 10 - 1;		//除以10以后反向计算出index

			//if (nColorValue == -1 || nIndex >= SerialArray.size())
			//{
			//	//失败了，准备重试
			//	m_FindDeviceTaskQueue.put(FindDeviceTask);	//把任务放回队列，等一会儿再来试
			//	continue;
			//}

			for (nIndex = 0; nIndex < SerialArray.size(); ++nIndex)
			{
				CString copyStr = _T("");//把设备标识传出去用于辨别是否认错设备
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
				//失败了，准备重试
				m_FindDeviceTaskQueue.put(FindDeviceTask);	//把任务放回队列，等一会儿再来试
				continue;
			}
	
			GotDeviceIdNotify(hWnd, SerialArray[nIndex]);
		}

		//把刚才启动的进程都杀掉
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

	std::map<String, CableInfo>	 tempMap(m_CableInfoMap);  //备份当前所有摄像头设备路径信息
	while (TRUE)
	{
		ATL::CComPtr<IMoniker> pMoniker;
		ULONG cFetched;
		hr = pEnumCat->Next(1, &pMoniker, &cFetched);
		if (hr != S_OK)		//不应该用FAILED()来判断
			break;

		ATL::CComPtr<IPropertyBag> pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
		if (FAILED(hr))
			continue;

		ATL::CComVariant varDevicePath;
		hr = pPropBag->Read(L"DevicePath", &varDevicePath, 0);
		if (FAILED(hr))
			continue;

		//varDevicePath的样子大致如下：
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
			{	//没有找到这个cable的名字，加进去
				m_CableInfoMap[varDevicePath.bstrVal].first			= 0;		//新增
			}
			else
			{	//找到了
				tempMap.erase(iter->first);										////删除当前已找到的摄像头信息

				if (!::IsWindow(iter->second.first))
				{	//判断是否由于窗口崩溃产生的残留，设为未配对
					m_CableInfoMap[varDevicePath.bstrVal].first		= 0;		//已找到，未配对
					m_CableInfoMap[varDevicePath.bstrVal].second	= _T("");	//已找到，未配对
				}
			}
		}
	}

	for each (auto it in tempMap)
	{	//对于已拨除的设备此处应当清除之
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
		if (iter.second.first == 0)	//如果还没有分配出去（没有绑定窗口），则将其分配出去
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
	{	//找到这个cable的名字 将窗口与信息之间绑定
		m_CableInfoMap[lpszCableName].first = hAttachedWnd;

		//if (m_CableInfoMap[lpszCableName].second.empty())
		{	//之前没有记录过绑定关系，准备查找绑定关系
			m_FindDeviceTaskQueue.put(CableInfo(hAttachedWnd, lpszCableName));
		}
		//else
		//{	//之前就有绑定关系了，直接通知绑定成功
		//	GotDeviceIdNotify(hAttachedWnd, m_CableInfoMap[lpszCableName].second);
		//}
	}
}

void CDeviceEnumerator::WindowDetached(LPCTSTR lpszCableName, HWND hAttachedWnd)
{
	boost::recursive_mutex::scoped_lock lock(g_GeneralMutex);

	if (m_CableInfoMap.find(lpszCableName) != m_CableInfoMap.end())
	{	//找到这个cable的名字
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

	//通知客户，已经找到了匹配SVC Cable的Android设备
	//本来这里是用COM的event机制做的，但太麻烦了，改为Windows消息

	//记录绑定关系，下次绑定起来会快一些
	for (auto iter = m_CableInfoMap.begin(); iter != m_CableInfoMap.end(); iter++)
	{
		if (iter->second.first == hWnd)
		{
			CString copyStr;
			copyStr.Format(_T("cablename=%s::deviceid=%s"), iter->first.c_str(), szDeviceId.c_str());//把设备标识传出去用于辨别是否认错设备

			COPYDATASTRUCT copyData ={ 0 };
			copyData.lpData = (PVOID)copyStr.GetBuffer();
			copyData.cbData = (copyStr.GetLength() + 1) * sizeof(TCHAR);

			//注意这里通知的是父窗口，也就是OCX Ctrl
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
	{	//暂时取以下采样点
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
		if (GotColor == -1)		//有一个点获取颜色失败，都认为失败
			return -1;

		//注意：Android的颜色渲染机制比较奇怪。当需要画(x, x, x)这样的颜色的时候，它会在每个x上加一个不等的扰动，一般扰动为整数，偶见负数，绝对值不超过10
		//所以这里的判断也会稍微复杂一点
		if (abs(GetRValue(GotColor) - GetGValue(GotColor)) >= 10 || abs(GetBValue(GotColor) - GetGValue(GotColor)) >= 10
			|| abs(GetBValue(GotColor) - GetRValue(GotColor)) >= 10)	//三分量必须全相等
			return -1;

		UINT GotColorValue = (GetRValue(GotColor) + GetGValue(GotColor) + GetBValue(GotColor)) / 3;

		//第一次成立
		if (ColorValue == -1)
			ColorValue = GotColorValue;
		else if (GotColorValue != ColorValue)		//测试点的颜色如果不一样，认为失败
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

//从按键精灵里面找到的后台取色代码，不是很好用，暂时不用
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

	//设置失败时的默认值
	COLORREF Cout = -1;

	//开始处理
	BITMAP Bitmap;
	//这里开始根据相应的句柄对数据进行初始化
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap))
	{
		//初始化失败，返回
		return -1;
	}

	if (x >= Bitmap.bmWidth || x < 0 || y >= Bitmap.bmHeight || y < 0)//若取色的点的坐标超出窗口范围，返回黑色
	{
		return -1;
	}
	//初始化Bitmap位图结构
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

	//计算需要申请的空间大小。
	DWORD dwBmBitsSize = ((Bitmap.bmWidth*wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	LPBITMAPINFOHEADER lpbi;
	HANDLE hDib, hPal, hOldPal = NULL;
	hDib = GlobalAlloc(GHND, dwBmBitsSize);//申请空间。
	if (hDib)
	{
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);//锁定内存空间
		hPal = GetStockObject(DEFAULT_PALETTE);//获取默认的调色板
		if (hPal)
		{
			HDC hDC = ::GetDC(NULL);//获取屏幕DC
			hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);//设置新的调色板，并保存原来的调色板
			RealizePalette(hDC);//关联调色板
			//将位图信息按指定的格式存储到缓冲区
			if (GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi, (LPBITMAPINFO)&bi, DIB_RGB_COLORS))
			{
				LPSTR dq = (LPSTR)lpbi;
				int jsy = Bitmap.bmWidth * 3 + Bitmap.bmWidth % 4;//计算一行位图信息占用的内存大小
				long wz = (Bitmap.bmHeight - 1)*jsy - jsy*y + x * 3;//计算需要取色的位置
				//读取相应的数据
				BYTE r = BYTE(dq[wz + 2]);
				BYTE g = BYTE(dq[wz + 1]);
				BYTE b = BYTE(dq[wz]);
				Cout = RGB(r, g, b);
			}

			if (hOldPal)//恢复调色板
			{
				SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
				RealizePalette(hDC);
				ReleaseDC(NULL, hDC);
			}
		}

		//释放内存空间
		GlobalUnlock(hDib);
		GlobalFree(hDib);
	}

	::DeleteObject(hBitmap);
	
	return Cout;
}