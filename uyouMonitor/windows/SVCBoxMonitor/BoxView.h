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
	//��Ƶ
	IGraphBuilder						*m_pGraph;
	IMediaEventEx						*m_pME;
	IMediaControl						*m_pControl;
	HRESULT								ConnectFilters(IGraphBuilder *pGraph,IBaseFilter *pSrc,IBaseFilter *pDest);
	HRESULT								ConnectFilters(IGraphBuilder *pGraph, IPin *pOut,IBaseFilter *pDest); 
	HRESULT								GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir,IPin **ppPin);
	BOOL								APlaying();

	//��Ƶ
	ICaptureGraphBuilder2				*m_pCGB;
	IVideoWindow						*m_pVW;

	void								SetVideoProcAmp( IBaseFilter *pFilter );
	BOOL								VPlaying();

	//ƥ��cable name���ҵõ�filter
	HRESULT								FindInputFilters(void** gottaFilter, const GUID DeviceCategory);
	void								DeleteMediaType(AM_MEDIA_TYPE *pmt);

	//��Ƶ��ʾ����CWND
	CWnd*								m_pShowWnd;		////������ʾ��Ƶ�Ĵ���
	CWnd*								m_pCtrlWnd;		////���ڽ����쳣��Ϣ�Ĵ���
	String								m_szCableName;
	CRect								m_vedioRect;
	int									m_DesiredWidth;	////��ƵԴ�Ŀ�
	int									m_DesiredHeight;////��ƵԴ�ĸ�
public:
	//��ʼ������Ƶ������
	BOOL Play(CWnd* pShowWnd, CWnd* pCtrlWnd, LPCTSTR lpszCableName, DWORD nCableId);
	//�ͷ���Դ
	void								Release();
	//��С�仯
	void								ReSize(CRect vedioRect);
	//����ͼ��֪ͨ
	BOOL								ProcessFgNotify(WPARAM wParam, LPARAM lParam);
};

