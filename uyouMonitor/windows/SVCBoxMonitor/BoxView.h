#pragma once

#define WIN32_LEAN_AND_MEAN

#define __IDxtCompositor_INTERFACE_DEFINED__   
#define __IDxtAlphaSetter_INTERFACE_DEFINED__   
#define __IDxtJpeg_INTERFACE_DEFINED__   
#define __IDxtKey_INTERFACE_DEFINED__ 

#include "include/dshow/dshow.h"
#include "include/dshow/qedit.h"
#include <d3d9.h>
#include <vmr9.h>

extern const GUID CLSID_FlipFilter;
extern const GUID CLSID_FFDSHOW;
class CBoxView
{
public:
	CBoxView(void);
	~CBoxView(void);

private:	
	//音频
	IGraphBuilder						*m_pGraph;
	IMediaEventEx						*m_pME;
	IMediaControl						*m_pControl;
	HRESULT								ConnectFilters(IGraphBuilder *pGraph,IBaseFilter *pSrc,IBaseFilter *pDest);
	HRESULT								ConnectFilters(IGraphBuilder *pGraph, IPin *pOut,IBaseFilter *pDest); 
	HRESULT								GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir,IPin **ppPin);
	BOOL								APlaying();

	//视频
	ICaptureGraphBuilder2				*m_pCGB;
	IVideoWindow						*m_pVW;

	void								SetVideoProcAmp( IBaseFilter *pFilter );
	BOOL								VPlaying();

	//匹配cable name并且得到filter
	HRESULT								FindInputFilters(void** gottaFilter, const GUID DeviceCategory);
	void								DeleteMediaType(AM_MEDIA_TYPE *pmt);

	//视频显示所在CWND
	CWnd*								m_pShowWnd;		////用于显示视频的窗口
	CWnd*								m_pCtrlWnd;		////用于接收异常消息的窗口
	String								m_szCableName;
	CRect								m_vedioRect;
	int									m_DesiredWidth;	////视频源的宽
	int									m_DesiredHeight;////视频源的高
public:
	//初始化音视频并播放
	BOOL Play(CWnd* pShowWnd, CWnd* pCtrlWnd, LPCTSTR lpszCableName, DWORD nCableId);
	//释放资源
	void								Release();
	//大小变化
	void								ReSize(CRect vedioRect);
	//处理图表通知
	BOOL								ProcessFgNotify(WPARAM wParam, LPARAM lParam);
};

