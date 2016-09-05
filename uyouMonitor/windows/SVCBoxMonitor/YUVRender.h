// YUVRender.h

#if !defined(AFX_YUVRENDER_H__4FEDEFF4_5597_405B_904E_F0194726B859__INCLUDED_)
#define AFX_YUVRENDER_H__4FEDEFF4_5597_405B_904E_F0194726B859__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <dshow.h>


class CYUVRender  
{
public:
	CYUVRender();
	virtual ~CYUVRender();

	void YUV420toYUV422(BYTE *lpSrcY, int SrcPitch, BYTE *lpSrcU, BYTE *lpSrcV, int stride_uv, BYTE *lpDst, int SrcWidth, int SrcHeight, unsigned int DstPitch);
	BOOL DDW_Rander(BYTE *lpSrcY,BYTE *lpSrcU,BYTE *lpSrcV);
	BOOL DDW_Init(HWND hWnd, DWORD dwWidth, DWORD dwHeight);
// 	void SetIsShowBmps(BOOL bShowRecord,BOOL bShowClose, BOOL bShowAudioOn);
// 	void SetBmpsRects(CRect rcRecord,CRect rcClose,CRect rcAudio,CRect rcWnd);
private:
	BOOL DDW_Release();
	IDirectDraw7 		*m_pDD;
	IDirectDrawSurface7 *m_pPrimarySurface;
	IDirectDrawSurface7 *m_pOffscreenSurface;
	IDirectDrawSurface7 *m_pOffSurfaceText;
	DWORD 				m_dwWidth;
	DWORD 				m_dwHeight;
	HWND  				m_hWnd;
	BOOL  				m_bInitOK ;
// 	BOOL  				m_bShowRecordBmp;
// 	BOOL  				m_bShowCloseBmp;
// 	BOOL  				m_bShowAudioOnBmp;
// 	RECT				m_rcRecord;
// 	RECT				m_rcClose;
// 	RECT				m_rcAudio;
// 	RECT				m_rcWnd;
};

#endif 
