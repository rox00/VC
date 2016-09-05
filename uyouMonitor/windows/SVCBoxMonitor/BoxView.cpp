#include "StdAfx.h"
#include "BoxView.h"
#include <atlcomcli.h>
#include <WinIoCtl.h>
#include <SetupAPI.h>
#include <initguid.h>

#pragma comment(lib,"Setupapi.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Strmiids.lib") 

#define VIDEO_DEVICE_PID_INFO	L"vid_0c45&pid_6366"

// {04FE9017-F873-410e-871E-AB91661A4EF7}
DEFINE_GUID(CLSID_FFDSHOW, 0x04fe9017, 0xf873, 0x410e, 0x87, 0x1e, 0xab, 0x91, 0x66, 0x1a, 0x4e, 0xf7);
DEFINE_GUID(CLSID_FlipFilter,
	0x80337959, 0xd465, 0x45e7, 0xa8, 0xa7, 0xdb, 0x38, 0x8c, 0x41, 0xc1, 0x9a);
CBoxView::CBoxView(void)
	: m_DesiredWidth(1280)
	, m_DesiredHeight(720)
{	 
	m_pShowWnd	= NULL;
	m_pCtrlWnd	= NULL;
	m_pGraph	= NULL; // Filter graph builder object
	m_pControl	= NULL; // Media control object
	m_pCGB		= NULL;
	m_pVW		= NULL;
	m_pME		= NULL;
}

CBoxView::~CBoxView(void)
{

}

void CBoxView::Release()
{
	if (m_pControl != NULL)
	{
		m_pControl->Stop();
		m_pControl->Release();
		m_pControl = NULL;
	}
	if (m_pME != NULL)
	{
		m_pME->Release();
		m_pME = NULL;
	}
	if (m_pVW != NULL)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Owner(NULL);
		m_pVW->put_MessageDrain(NULL);		
		m_pVW->Release();
		m_pVW = NULL;
	}

	if (m_pCGB != NULL)
	{
		m_pCGB->Release();
		m_pCGB = NULL;
	}
	
	if (m_pGraph != NULL)
	{
		m_pGraph->Release();
		m_pGraph = NULL;
	}	
}

HRESULT CBoxView::FindInputFilters(void** gottaFilter, const GUID DeviceCategory)
{
	CComPtr<ICreateDevEnum> pSysDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr))
		return hr;

	CComPtr<IEnumMoniker> pEnumCat;
	hr = pSysDevEnum->CreateClassEnumerator(DeviceCategory, &pEnumCat, 0);
	if (FAILED(hr) || pEnumCat == NULL)
		return hr;

	while (TRUE)
	{
		CComPtr<IMoniker> pMoniker;
		ULONG cFetched;
		hr = pEnumCat->Next(1, &pMoniker, &cFetched);
		if (hr != S_OK)		//不应该用FAILED()来判断
			break;

		LPOLESTR strName = NULL;
		hr = pMoniker->GetDisplayName(NULL, NULL, &strName);

		CComPtr<IPropertyBag> pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
		if (FAILED(hr))
			continue;

		CComVariant varProp;
		if (DeviceCategory == CLSID_VideoInputDeviceCategory)
		{
			hr = pPropBag->Read(L"DevicePath", &varProp, 0);		//视频设备有这个属性，能看到vid和pid，比较容易认

			if (FAILED(hr) || StrCmpIW(varProp.bstrVal, m_szCableName.c_str()) != 0)
				continue;
		}
		else
		{
			hr = pPropBag->Read(L"FriendlyName", &varProp, 0);		//音频设备只有类似于这样的名字：“麦克风 (6- USB 2.0 Camera)”。不太容易辨认和区分
			if (FAILED(hr) || StrStrW(varProp.bstrVal, L"USB 2.0 Camera") == NULL)
				continue;
		}

		hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, gottaFilter);
		if (SUCCEEDED(hr))
			return TRUE;
	}

	return S_FALSE;
}

BOOL CBoxView::Play(CWnd* pShowWnd, CWnd* pCtrlWnd, LPCTSTR lpszCableName, DWORD nCableId)
{
	m_pShowWnd		= pShowWnd;
	m_pCtrlWnd		= pCtrlWnd;
	m_szCableName	= lpszCableName;

	BOOL bRet = FALSE;

	Release();

	//video和audio的公共初始化

	// Create the Filter Graph Manager
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraph);
	if (SUCCEEDED(hr))
	{
		hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&m_pCGB);
			hr = m_pCGB->SetFiltergraph(m_pGraph);

			//由于声音输入设备无法区分，为了避免多个实例去连接同一个声音输入设备导致问题，规定只有第一个设备才会出现声音
			if (nCableId == 0)
				APlaying();

			if (SUCCEEDED(hr))
			{
				bRet = VPlaying();
			}
		}
	}

	if (bRet == FALSE)	////说明图像渲染失败，这时即使音频处理成功，也要释放。
	{
		Release();
	}

	return bRet;
}

void CBoxView::SetVideoProcAmp(IBaseFilter *pFilter)
{
	CComPtr <IAMVideoProcAmp> pProcAmp;
	int nBrightness = 0;
	int nContrast = 40;
	int nHue = 0;
	int nSaturation = 100;
	int nSharpness = 3;
	int nGamma = 100;
	int nWhiteBalance = 4600;
	int nBacklightCompensation = 1;
	int nGain = 4;

	HRESULT hr = pFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&pProcAmp);
	if (SUCCEEDED(hr))
	{
		long Min, Max, Step, Default, Flags, Val;

		// 亮度.Brightness
		hr = pProcAmp->GetRange(VideoProcAmp_Brightness, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_Brightness, &Val, &Flags);
			Val = nBrightness;//Min+(Max-Min)*g_Brightness/100;
			pProcAmp->Set(VideoProcAmp_Brightness, Val, Flags);
		}

		//对比度.Contrast
		hr = pProcAmp->GetRange(VideoProcAmp_Contrast, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_Contrast, &Val, &Flags);
			Val = nContrast;
			pProcAmp->Set(VideoProcAmp_Contrast, Val, Flags);
		}

		//色调.Hue
		hr = pProcAmp->GetRange(VideoProcAmp_Hue, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_Hue, &Val, &Flags);
			Val = nHue;
			pProcAmp->Set(VideoProcAmp_Hue, Val, Flags);
		}

		//饱和度.Saturation
		hr = pProcAmp->GetRange(VideoProcAmp_Saturation, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_Saturation, &Val, &Flags);
			Val = nSaturation;
			pProcAmp->Set(VideoProcAmp_Saturation, Val, Flags);
		}

		//清晰度.Sharpness
		hr = pProcAmp->GetRange(VideoProcAmp_Sharpness, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_Sharpness, &Val, &Flags);
			Val = nSharpness;
			pProcAmp->Set(VideoProcAmp_Sharpness, Val, Flags);
		}

		//伽玛.Gamma
		hr = pProcAmp->GetRange(VideoProcAmp_Gamma, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_Gamma, &Val, &Flags);
			Val = nGamma;
			pProcAmp->Set(VideoProcAmp_Gamma, Val, Flags);
		}

		//白平衡.WhiteBalance
		hr = pProcAmp->GetRange(VideoProcAmp_WhiteBalance, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_WhiteBalance, &Val, &Flags);
			Val = nWhiteBalance;
			pProcAmp->Set(VideoProcAmp_WhiteBalance, Val, Flags);
		}

		//白平衡.BacklightCompensation
		hr = pProcAmp->GetRange(VideoProcAmp_BacklightCompensation, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_BacklightCompensation, &Val, &Flags);
			Val = nBacklightCompensation;
			pProcAmp->Set(VideoProcAmp_BacklightCompensation, Val, Flags);
		}

		//增益.Gain
		hr = pProcAmp->GetRange(VideoProcAmp_Gain, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pProcAmp->Get(VideoProcAmp_Gain, &Val, &Flags);
			Val = nGain;
			pProcAmp->Set(VideoProcAmp_Gain, Val, Flags);
		}
	}
}

BOOL CBoxView::VPlaying()
{
	if (m_pGraph == NULL)
	{
		return FALSE;
	}

	CComPtr<IBaseFilter> pVideoInputFilter;
	HRESULT hr = FindInputFilters((void**)&pVideoInputFilter, CLSID_VideoInputDeviceCategory);
	if (pVideoInputFilter != NULL)
	{
		hr = m_pGraph->AddFilter(pVideoInputFilter, L"VCapture");
		//设置
		SetVideoProcAmp(pVideoInputFilter);

		CComPtr<IPin>			pAudio			= NULL;

		CComPtr<IPin>			pPinTemp		= NULL;
		CComPtr<IEnumPins>		pienumpins		= NULL;
		pVideoInputFilter->EnumPins(&pienumpins);
		while (pienumpins != NULL && pienumpins->Next(1, &pPinTemp, NULL) == S_OK)
		{
			PIN_DIRECTION ThisPinDir;
			pPinTemp->QueryDirection(&ThisPinDir);
			if (ThisPinDir == PINDIR_OUTPUT)
			{
				PIN_INFO pininfo;
				pPinTemp[0].QueryPinInfo(&pininfo);
				if (StrStr(pininfo.achName,_T("Audio")))
				{   //播放音频
					hr = m_pGraph->Render(pPinTemp);
					pAudio = pPinTemp;
					break;
				}
				else if (StrStr(pininfo.achName, _T("捕获"))     ||
						StrStr(pininfo.achName, _T("Capture"))
					)
				{	////设置捕获Filter的格式为输出PIN支持的某种宽高的格式
					CComPtr<IEnumMediaTypes> 	ptypes			= NULL;
					AM_MEDIA_TYPE				*ptype			= NULL;
					VIDEOINFOHEADER				*pvideoInfo		= NULL;

					pPinTemp->EnumMediaTypes(&ptypes);
					while (ptypes != NULL && ptypes->Next(1, &ptype, NULL) == 0)
					{
						if (ptype->majortype== MEDIATYPE_Video)
						{
							pvideoInfo = (VIDEOINFOHEADER *)ptype->pbFormat;
							if (m_DesiredHeight != 0									&&
								m_DesiredWidth  != 0									&&
								abs(pvideoInfo->rcSource.right)  == m_DesiredWidth		&&
								abs(pvideoInfo->rcSource.bottom) == m_DesiredHeight
								)
							{   //如果格式与所要求的格式一致的话，优先使用输出Pin的这种格式

								CComPtr<IAMStreamConfig>		pVSC = NULL;;      // for video cap
								hr = m_pCGB->FindInterface(&PIN_CATEGORY_CAPTURE,
									&MEDIATYPE_Video, pVideoInputFilter,
									IID_IAMStreamConfig, (void **)&pVSC);

								if (pVSC)
								{
									int nFrame = pvideoInfo->AvgTimePerFrame; // 得到采集的帧率
									CString str;
									str.Format(_T("FRAME: %d\n"), nFrame);
									OutputDebugString(str);

									//pvideoInfo->AvgTimePerFrame = (LONGLONG)(10000000/50);

									pVSC->SetFormat(ptype);
								}
								// Delete the media type when you are done.
								DeleteMediaType(ptype);
							}
						}
					}
				}
			}
			pPinTemp.Release();
		}


		if (pAudio)
		{	//是采集卡
			hr = m_pCGB->RenderStream(&PIN_CATEGORY_PREVIEW,
				&MEDIATYPE_Video,
				pVideoInputFilter,
				NULL,
				NULL);
			if (FAILED(hr))
				return FALSE;
		}
		else
		{	////目前PCI采集卡上有视频接口，而盒子的Filter上没有
			CComPtr<IPin>			pInPin			= NULL;
			CComPtr<IPin>			pOutPin			= NULL;
			CComPtr<IBaseFilter>	smarttee		= NULL;
			CComPtr<IBaseFilter>	decompression	= NULL;
			CComPtr<IBaseFilter>	pPreview		= NULL;

			hr = CoCreateInstance(CLSID_VideoRenderer, 0, CLSCTX_ALL, IID_IBaseFilter, (void**)&pPreview);
			//hr = CoCreateInstance(CLSID_FFDSHOW, 0, CLSCTX_ALL, IID_IBaseFilter, (void**)&decompression);
			if (decompression == NULL)
			{
				hr = CoCreateInstance(CLSID_SmartTee, 0, CLSCTX_ALL, IID_IBaseFilter, (void**)&smarttee);
				hr = CoCreateInstance(CLSID_MjpegDec, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&decompression);
				if (hr != S_OK)
					return FALSE;

				{
					CComPtr<IEnumPins>		pEnum = 0;
					CComPtr<IPin>			pPin = 0;
					HRESULT hr = smarttee->EnumPins(&pEnum);
					if (FAILED(hr))
					{
						return hr;
					}
					while (pEnum->Next(1, &pPin, NULL) == S_OK)
					{
						PIN_DIRECTION ThisPinDir;
						pPin->QueryDirection(&ThisPinDir);
						if (ThisPinDir == PINDIR_OUTPUT)
						{
							CComPtr<IPin> pTmp = 0;
							hr = pPin->ConnectedTo(&pTmp);
							if (SUCCEEDED(hr)) // Already connected--not the pin we want
							{
								pTmp.Release();
							}
							else // Unconnected--this is the pin we want
							{
								PIN_INFO info;
								pPin->QueryPinInfo(&info);
								if (_tcsstr(info.achName, _T("Preview")))
								{
									//pEnum->Release();
									pOutPin = pPin;
									break;
								}
							}
						}
						pPin.Release();
					}
					pEnum.Release();
				}

				hr = m_pGraph->AddFilter(smarttee, L"CLSID_SmartTee");
				hr = m_pGraph->AddFilter(decompression, L"CLSID_MJPEG");
				hr = m_pGraph->AddFilter(pPreview, L"CLSID_VideoRenderer");

				ConnectFilters(m_pGraph, pVideoInputFilter, smarttee);
				ConnectFilters(m_pGraph, pOutPin, decompression);
				ConnectFilters(m_pGraph, decompression, pPreview);
			}
			else
			{	//创建FFDShow解码器
				hr = m_pGraph->AddFilter(decompression, L"CLSID_FFDSHOW");
				hr = m_pGraph->AddFilter(pPreview, L"CLSID_VideoRenderer");

				ConnectFilters(m_pGraph, pVideoInputFilter, decompression);
				ConnectFilters(m_pGraph, decompression, pPreview);
			}
		}

		//设置设备丢失时要处理消息的窗口
		hr = m_pGraph->QueryInterface(IID_IMediaEventEx, (void **)&m_pME);
		if (hr == NOERROR)
		{
			m_pME->SetNotifyWindow((OAHWND)m_pCtrlWnd->GetSafeHwnd(), WM_FGNOTIFY, 0);
		}

		hr = m_pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVW);
		hr = m_pVW->put_AutoShow(OAFALSE);

		if (FAILED(hr))
			return FALSE;

		RECT rc;
		GetClientRect(m_pShowWnd->GetSafeHwnd(), &rc);
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		m_pVW->put_WindowStyleEx(WS_EX_ACCEPTFILES);
		m_pVW->put_Owner((OAHWND)m_pShowWnd->GetSafeHwnd());
		//m_pVW->put_MessageDrain((OAHWND)m_pShowWnd->GetSafeHwnd());
		m_pVW->put_Visible(OATRUE);
		//m_pVW->put_Width(rc.right);
		//m_pVW->put_Height(rc.bottom);
		//m_pVW->SetWindowForeground(1);

		//m_pVW->SetWindowPosition(0, 0, rc.right, rc.bottom); // be this big
		ReSize(m_vedioRect);

		hr = m_pControl->Run();

		return SUCCEEDED(hr);
	}

	return FALSE;
}

BOOL CBoxView::APlaying()
{
	if (m_pGraph == NULL)
	{
		return FALSE;
	}

	CComPtr<IBaseFilter> pAudioInputFilter;

	HRESULT hr = S_OK;

	//查找audio filter并加入graph
	hr = FindInputFilters((void**)&pAudioInputFilter, CLSID_AudioInputDeviceCategory);
	if (NULL == pAudioInputFilter)
	{
		TRACE(L"[SVC]  CBoxView:: Could not create the Filter AudioInputFilter");
		return FALSE;
	}

	CComPtr<IPin> pinIn;
	CComPtr<IAMAudioInputMixer> pPinMixer;
	GetUnconnectedPin(pAudioInputFilter, PINDIR_INPUT, &pinIn);
	hr = pinIn->QueryInterface(IID_IAMAudioInputMixer, (void **)&pPinMixer);
	if (SUCCEEDED(hr))
	{
		hr = pPinMixer->put_Enable(TRUE);
	}

	hr = m_pGraph->AddFilter(pAudioInputFilter, L"ACapture");
	//创建render filter并加入graph
	CComPtr <IBaseFilter> pAudioRenderer = NULL;
	hr = CoCreateInstance(CLSID_AudioRender, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pAudioRenderer);
	hr = m_pGraph->AddFilter(pAudioRenderer, L"Audio Renderer");
	if (pAudioRenderer == NULL)
	{
		TRACE(L"[SVC]  CBoxView:: Could not create the Filter AudioRenderer");
		return FALSE;
	}

	//获取麦克风输出脚
	CComPtr<IPin> pAudioOutput;
	CComPtr<IEnumPins> pEnum;
	pAudioInputFilter->EnumPins(&pEnum);
	hr = pEnum->Reset();
	hr = pEnum->Next(1, &pAudioOutput, NULL);

	//设置麦克风输出脚
	IAMStreamConfig *pCfg = NULL;
	hr = pAudioOutput->QueryInterface(IID_IAMStreamConfig, (void **)&pCfg);
	// Read current media type/format   
	AM_MEDIA_TYPE *pmt = { 0 };
	hr = pCfg->GetFormat(&pmt);
	WAVEFORMATEX *pWF = (WAVEFORMATEX *)pmt->pbFormat;
	// Release interfaces   
	pCfg->Release();

	//设置麦克风输出脚缓冲
	IAMBufferNegotiation *pNeg;
	pAudioOutput->QueryInterface(IID_IAMBufferNegotiation, (void **)&pNeg);
	ALLOCATOR_PROPERTIES prop = { 0 };
	prop.cbBuffer = pWF->nAvgBytesPerSec * 50 / 1000;
	prop.cBuffers = -1;
	prop.cbAlign = -1;
	prop.cbPrefix = -1;
	hr = pNeg->SuggestAllocatorProperties(&prop);
	pNeg->Release();

#if 0
	//method 1:Render RUN
	hr = m_pCGB->RenderStream(&PIN_CATEGORY_PREVIEW,
		&MEDIATYPE_Audio,
		pAudioInputFilter,
		NULL,
		NULL);
	
#else

	//method 2:Connect RUN
	hr = ConnectFilters(m_pGraph, pAudioOutput, pAudioRenderer);

#endif

	return FAILED(hr);
}

HRESULT CBoxView::GetUnconnectedPin(IBaseFilter *pFilter, // Pointer to the filter.
	PIN_DIRECTION PinDir, // Direction of the pin to find.
	IPin **ppPin) // Receives a pointer to the pin.
{
	*ppPin = 0;
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return hr;
	}
	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr)) // Already connected--not the pin we want
			{
				pTmp->Release();
			}
			else // Unconnected--this is the pin we want
			{
				pEnum->Release();
				*ppPin = pPin;
				return S_OK;
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	// Did not find a matching pin.
	return E_FAIL;
}


// Connect two filters together with the Filter Graph Manager,
// Stolen from the DX9 SDK.
// This is the base version.
HRESULT CBoxView::ConnectFilters(IGraphBuilder *pGraph, // Filter Graph Manager.
	IPin *pOut, // Output pin on the upstream filter.
	IBaseFilter *pDest) // Downstream filter.
{
	if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}
	// Find an input pin on the downstream filter.
	IPin *pIn = 0;
	HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
	if (FAILED(hr))
	{
		return hr;
	}
	// Try to connect them.
	hr = pGraph->Connect(pOut, pIn);
	pIn->Release();
	return hr;
}


// Connect two filters together with the Filter Graph Manager.
// Again, stolen from the DX9 SDK.
// This is an overloaded version.
HRESULT CBoxView::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest)
{
	if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}
	// Find an output pin on the first filter.
	IPin *pOut = 0;
	HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = ConnectFilters(pGraph, pOut, pDest);
	pOut->Release();
	return hr;
}

void CBoxView::ReSize(CRect vedioRect)
{
	if ((m_pShowWnd != NULL) && (m_pVW != NULL))
	{
		m_pVW->SetWindowPosition(vedioRect.left, vedioRect.top, vedioRect.Width(), vedioRect.Height());
	}

	m_vedioRect = vedioRect;
}

BOOL CBoxView::ProcessFgNotify(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = TRUE;
	if (m_pME)
	{
		LONG		event;
		LONG_PTR	l1, l2;

		while (m_pME->GetEvent(&event, &l1, &l2, 0) == S_OK)
		{
			m_pME->FreeEventParams(event, l1, l2);
			switch (event)
			{
			case EC_ERRORABORT:
			case EC_DEVICE_LOST:
				bRet = FALSE;
				break;

			default:
				break;
			}
		} 
	}
	return bRet;
}
void CBoxView::DeleteMediaType(AM_MEDIA_TYPE *pmt)
{
	// allow NULL pointers for coding simplicity

	if (pmt == NULL) {
		return;
	}

	if (pmt->cbFormat != 0) {
		CoTaskMemFree((PVOID)pmt->pbFormat);

		// Strictly unnecessary but tidier
		pmt->cbFormat = 0;
		pmt->pbFormat = NULL;
	}
	if (pmt->pUnk != NULL) {
		pmt->pUnk->Release();
		pmt->pUnk = NULL;
	}

	CoTaskMemFree((PVOID)pmt);
}
