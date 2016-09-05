#include "stdafx.h"
#include "YUVRender.h"

//#pragma comment(lib,"strmbasd.lib")
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SAFE_DELETE(p)  { if(p != NULL) { delete (p); (p) = NULL; } }		//Delete object by New create 
#define SAFE_DELETEA(p) { if(p != NULL) { delete[] (p); (p) = NULL; } }		//Delete Arrary
#define SAFE_RELEASE(p) { if(p != NULL) { (p)->Release(); (p) = NULL; } }
#define SAFE_FREE(p)	{ if(p != NULL) { free(p); (p) = NULL; } }

CYUVRender::CYUVRender()
{
	m_dwWidth 			= 0;
	m_dwHeight			= 0;
	m_hWnd				= NULL;
	m_pDD				= NULL;
	m_pPrimarySurface	= NULL;
	m_pOffscreenSurface = NULL;
	m_pOffSurfaceText	= NULL;
	m_bInitOK			= FALSE ;
// 	m_bShowRecordBmp	= FALSE;
// 	m_bShowCloseBmp		= FALSE;
// 	m_bShowAudioOnBmp	= FALSE; 
}

CYUVRender::~CYUVRender()
{
	DDW_Release();
}


BOOL CYUVRender::DDW_Init(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
	if(dwHeight == m_dwHeight && dwWidth == m_dwWidth && m_hWnd == hWnd)
		return TRUE ;

 	DDW_Release();

	HRESULT hr;
	DDSURFACEDESC2 ddsd;

	DirectDrawCreateEx(NULL,(VOID**)&m_pDD,IID_IDirectDraw7, NULL);
	hr = m_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
	if(FAILED(hr))
	{
		return false;
	}
	
	ZeroMemory(&ddsd, sizeof( ddsd ));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hr = m_pDD->CreateSurface(&ddsd, &m_pPrimarySurface, NULL);
	if(FAILED(hr))
	{
		return false;
	}

	DDPIXELFORMAT ddpfOverlayFormats = {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','422'), 0, 0, 0, 0, 0};
	
	ZeroMemory(&ddsd, sizeof( ddsd ));
	ddsd.dwSize			= sizeof( ddsd );
	ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT|DDSD_PIXELFORMAT;
	ddsd.dwWidth        = dwWidth;
	ddsd.dwHeight       = dwHeight;
	ddsd.ddpfPixelFormat= ddpfOverlayFormats;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	hr = m_pDD->CreateSurface(&ddsd, &m_pOffscreenSurface, NULL);
	if(FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&ddsd, sizeof( ddsd ));
	ddsd.dwSize			= sizeof( ddsd );
	ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth        = dwWidth;
	ddsd.dwHeight       = dwHeight;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	hr = m_pDD->CreateSurface(&ddsd, &m_pOffSurfaceText, NULL);
	if(FAILED(hr))
	{
		return false;
	}

	IDirectDrawClipper * pcClipper;
	if( FAILED(hr = m_pDD->CreateClipper(0, &pcClipper, NULL)) )
	{
		return false;
	}
	
	if( FAILED(hr = pcClipper->SetHWnd(0, hWnd)) )
	{
		return false;
	}
	
	if( FAILED(hr = m_pPrimarySurface->SetClipper( pcClipper )) )
	{
		return false;
	}
	
	pcClipper->Release();
	
	DDBLTFX ddbltfx;
	ZeroMemory(&ddbltfx, sizeof( ddbltfx ));
	ddbltfx.dwSize      = sizeof( ddbltfx );
	ddbltfx.dwFillColor = RGB(0,0,0);
	m_pOffscreenSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
	
	m_hWnd		= hWnd;
	m_dwWidth	= dwWidth;
	m_dwHeight	= dwHeight;

	m_bInitOK = TRUE ;
	return true;
}

BOOL CYUVRender::DDW_Release()
{
	SAFE_RELEASE(m_pOffscreenSurface);
	SAFE_RELEASE(m_pPrimarySurface);
	SAFE_RELEASE(m_pOffSurfaceText);
	if(m_pDD)
		m_pDD->SetCooperativeLevel(0, DDSCL_NORMAL);
	SAFE_RELEASE(m_pDD);
	m_pDD = NULL ;	
	m_pPrimarySurface   = NULL ;
	m_pOffscreenSurface = NULL ;
	m_pOffSurfaceText = NULL;
	return true;
}

BOOL CYUVRender::DDW_Rander(BYTE *lpSrcY,BYTE *lpSrcU,BYTE *lpSrcV)
{
	if((m_pPrimarySurface == NULL)||(m_pOffscreenSurface == NULL))
	{
		return false;
	}
	
	DDSURFACEDESC2 ddsd;
	HRESULT hr;
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	hr = m_pOffscreenSurface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	
	if(FAILED(hr))
	{
		m_pOffscreenSurface->Restore();
		return false;
	}
	
	YUV420toYUV422(lpSrcY,m_dwWidth,lpSrcU,lpSrcV,m_dwWidth>>1,(BYTE*)ddsd.lpSurface,m_dwWidth,m_dwHeight,ddsd.lPitch);

	m_pOffscreenSurface->Unlock(NULL);
	
	RECT rectF;
	rectF.left = 0;
	rectF.top = 0;
	rectF.right = m_dwWidth;
	rectF.bottom = m_dwHeight;
	if (m_pOffSurfaceText->Blt(&rectF, m_pOffscreenSurface, NULL, DDBLT_WAIT ,NULL) == DDERR_SURFACELOST)
	{
		if (m_pOffSurfaceText)
			m_pOffscreenSurface->Restore();
	}
	
// 	CRect rc2;
// 	rc2.left  = 8;
// 	rc2.top   = 4 ;
// 	rc2.right = 100 ;
// 	rc2.bottom = 28;
// 	HDC hdc;
// 
// 	m_pOffSurfaceText->GetDC(&hdc);
// 	CDC pDC;
// 	pDC.Attach(hdc);
// 	CFont tf ;
// 	if(tf.GetSafeHandle() == NULL)
// 	{
// 		LOGFONT lf ;
// 
// 		CWnd::FromHandle(m_hWnd)->GetFont()->GetLogFont(&lf) ;
// 		wcscpy(lf.lfFaceName,_T("Î¢ÈíÑÅºÚ"));
// 
// 		lf.lfWeight*=1.5/m_rcWnd.Width()*m_dwWidth ;
// 		lf.lfHeight*=1.5/m_rcWnd.Height()*m_dwHeight ;
// 		tf.CreateFontIndirect(&lf);
// 	}
// 
// 	pDC.SelectObject(&tf) ;
// 	CSize SI = pDC.GetOutputTextExtent(m_strDevName);
// 	rc2.left   = 8;
// 	rc2.top    = 6 ;
// 	rc2.right  = 6+SI.cx +2;
// 	rc2.bottom = 8+SI.cy -2;
// 
// 	SetBkMode(hdc, TRANSPARENT);
// 	SetTextColor(hdc, RGB(245,245,245)); 
// 	SetBkColor(hdc,RGB(100,100,100)) ;
// 
// 	CRect rcText;
// 	rcText.left = rc2.left*1.0/m_rcWnd.Width()*m_dwWidth;
// 	rcText.right = rc2.left + rc2.Width()*1.0/m_rcWnd.Width()*m_dwWidth;
// 	rcText.top = rc2.top*1.0/m_rcWnd.Height()*m_dwHeight;
// 	rcText.bottom = rc2.bottom*1.0/m_rcWnd.Height()*m_dwHeight;
// 
// 	DrawText(hdc,m_strDevName,-1,rcText,DT_LEFT);
// 	pDC.ExcludeClipRect(rcText) ;
// 	tf.DeleteObject();
// 
// 
// 	static CBitmap TM ;
// 	static CBitmap btn;
// 	static CBitmap btn1;
// 	static CBitmap bRec1;
// 	static CBitmap bRec2;
// 	static CDC memDC1;
// 	static CDC memDC2;
// 	static CDC memDC3;
// 	static CDC memDCRec1;
// 	static CDC memDCRec2;
// 
// 	if(memDC1.GetSafeHdc() == NULL)
// 	{
// 		TM.Attach(LoadBmpFormFile(_T("VClose.bmp"))) ;
// 		memDC1.CreateCompatibleDC(&pDC) ;
// 		memDC1.SelectObject(&TM);
// 
// 		btn.Attach(LoadBmpFormFile(_T("Volume.bmp")));
// 		memDC2.CreateCompatibleDC(&pDC) ;
// 		memDC2.SelectObject(&btn);
// 
// 		btn1.Attach(LoadBmpFormFile(_T("VolumeNo.bmp")));
// 		memDC3.CreateCompatibleDC(&pDC) ;
// 		memDC3.SelectObject(&btn1);
// 
// 		bRec1.Attach(LoadBmpFormFile(_T("RecFlag1.bmp"))) ;
// 		memDCRec1.CreateCompatibleDC(&pDC) ;
// 		memDCRec1.SelectObject(&bRec1);
// 
// 		bRec2.Attach(LoadBmpFormFile(_T("RecFlag2.bmp"))) ;
// 		memDCRec2.CreateCompatibleDC(&pDC) ;
// 		memDCRec2.SelectObject(&bRec2);
// 	}
// 
// 	int nMargin = 4 ;
// 
// 	CRect rc;
// 	::GetClientRect(m_hWnd,rc) ;
// 	CRect rcRec = rc ;
// 	rcRec.top  = rcRec.bottom-30 ;
// 	rcRec.left+=10 ;
// 	rcRec.right = rcRec.left+30 ;
// 	if(m_bShowRecordBmp)
// 	{
// 		CRect rcRecord;
// 		rcRecord.left = rcRec.left*1.0/m_rcWnd.Width()*m_dwWidth;
// 		rcRecord.right = rcRecord.left + rcRec.Width()*1.0/m_rcWnd.Width()*m_dwWidth;
// 		rcRecord.top = rcRec.top*1.0/m_rcWnd.Height()*m_dwHeight;
// 		rcRecord.bottom = rcRec.bottom*1.0/m_rcWnd.Height()*m_dwHeight;
// 		
// 		BITMAP bpm;
// 		if(g_bRecOddFlag%2)
// 			bRec1.GetBitmap(&bpm);
// 		else
// 			bRec2.GetBitmap(&bpm);
// 
// 		pDC.StretchBlt(rcRecord.left,rcRecord.top,30.0/m_rcWnd.Width()*m_dwWidth,23.0/m_rcWnd.Height()*m_dwHeight,g_bRecOddFlag%2 ? (&memDCRec1):(&memDCRec2),0,0,bpm.bmWidth,bpm.bmHeight,SRCCOPY);
// 		pDC.ExcludeClipRect(rcRecord) ;
// 	}
// 
// 	if(m_bShowCloseBmp)
// 	{
// 		CRect rcClose;
// 		rcClose.left = m_rcClose.left*1.0/m_rcWnd.Width()*m_dwWidth + 4.0/m_rcWnd.Width()*m_dwWidth;
// 		rcClose.right = rcClose.left + m_rcClose.Width()*1.0/m_rcWnd.Width()*m_dwWidth;
// 		rcClose.top = m_rcClose.top*1.0/m_rcWnd.Height()*m_dwHeight - 4.0/m_rcWnd.Height()*m_dwHeight;
// 		rcClose.bottom = m_rcClose.bottom*1.0/m_rcWnd.Height()*m_dwHeight - 4.0/m_rcWnd.Height()*m_dwHeight;
// 		BITMAP bpm;
// 		TM.GetBitmap(&bpm);
// 		pDC.StretchBlt(rcClose.left,rcClose.top,rcClose.Width(),rcClose.Height(),&memDC1,0,0,bpm.bmWidth,bpm.bmHeight,SRCCOPY);
// 
// 		CRect rcAudio;
// 		rcAudio.left = m_rcAudio.left*1.0/m_rcWnd.Width()*m_dwWidth + 4.0/m_rcWnd.Width()*m_dwWidth;
// 		rcAudio.right = rcAudio.left + m_rcAudio.Width()*1.0/m_rcWnd.Width()*m_dwWidth;
// 		rcAudio.top = m_rcAudio.top*1.0/m_rcWnd.Height()*m_dwHeight + 4.0/m_rcWnd.Height()*m_dwHeight;
// 		rcAudio.bottom = m_rcAudio.bottom*1.0/m_rcWnd.Height()*m_dwHeight + 4.0/m_rcWnd.Height()*m_dwHeight;
// 
// 		btn.GetBitmap(&bpm);
// 		if(m_bShowAudioOnBmp)
// 			pDC.StretchBlt(rcAudio.left,rcAudio.top,rcAudio.Width(),rcAudio.Height(),&memDC2,0,0,bpm.bmWidth,bpm.bmHeight,SRCCOPY);
// 		else
// 			pDC.StretchBlt(rcAudio.left,rcAudio.top,rcAudio.Width(),rcAudio.Height(),&memDC3,0,0,bpm.bmWidth,bpm.bmHeight,SRCCOPY);
// 
// 		pDC.ExcludeClipRect(rcClose) ;
// 		pDC.ExcludeClipRect(rcAudio) ;
// 	}
// 
// 	pDC.Detach();
// 	m_pOffSurfaceText->ReleaseDC(hdc);

	RECT rcWindow;
	::GetClientRect(m_hWnd, &rcWindow);
	::ClientToScreen(m_hWnd, (POINT*)&rcWindow.left);
	::ClientToScreen(m_hWnd, (POINT*)&rcWindow.right);

	//InflateRect(&rcWindow,-4,-4);
	//m_pPrimarySurface->GetDC(&hdc);
	//::SetStretchBltMode(hdc,HALFTONE);

	if(m_pPrimarySurface->Blt(&rcWindow,m_pOffSurfaceText,NULL,DDBLT_WAIT,NULL) == DDERR_SURFACELOST)
	{
		if(m_pPrimarySurface)
		{
			m_pPrimarySurface->Restore();
		}

		if(m_pOffSurfaceText)
		{
			m_pOffSurfaceText->Restore();
		}
	}
	//m_pPrimarySurface->ReleaseDC(hdc);

	return TRUE;
}

void CYUVRender::YUV420toYUV422(BYTE *lpSrcY, int SrcPitch, BYTE *lpSrcU, BYTE *lpSrcV, int stride_uv, BYTE *lpDst, int SrcWidth, int SrcHeight, unsigned int DstPitch)
{
	int SrcStride  =  SrcPitch + SrcPitch - SrcWidth;	
	int SrcStrideU = (SrcPitch - SrcWidth)>>1;
	int DstStride  = (DstPitch - SrcWidth)<<1;

	__asm
	{
		push ebx

		mov edi , [lpDst]
		mov esi , [lpSrcY]
		mov eax , [lpSrcU]
		mov ebx , [lpSrcV]
		mov ecx , [SrcHeight]
		mov edx , [SrcWidth]
cyc: 
		movq mm2 , qword ptr [eax] //u
		movq mm3 , qword ptr [ebx] //v

		movq mm0 , qword ptr [esi]   //y1
		movq mm1 , qword ptr [esi+8] //y2

		movq mm4 , mm2
		punpcklbw mm2 , mm3 // uv1
		punpckhbw mm4 , mm3 // uv2

		movq mm6 , mm2
		movq mm7 , mm4

		movq mm3 , mm0
		movq mm5 , mm1

		punpcklbw mm0 , mm2 // uyvy1
		punpckhbw mm3 , mm2 // uyvy2
		punpcklbw mm1 , mm4 // uyvy3
		punpckhbw mm5 , mm4 // uyvy4

		movq qword ptr [edi]    , mm0
		movq qword ptr [edi+8]  , mm3
		movq qword ptr [edi+16] , mm1
		movq qword ptr [edi+24] , mm5

		add esi , [SrcPitch]
		add edi , [DstPitch]

		movq mm0 , qword ptr [esi]   //y1
		movq mm1 , qword ptr [esi+8] //y2

		movq mm2, mm6 
		movq mm4, mm7 

		movq mm3 , mm0
		movq mm5 , mm1

		punpcklbw mm0 , mm2 // uyvy1
		punpckhbw mm3 , mm2 // uyvy2
		punpcklbw mm1 , mm4 // uyvy3
		punpckhbw mm5 , mm4 // uyvy4

		movq qword ptr [edi]    , mm0
		movq qword ptr [edi+8]  , mm3
		movq qword ptr [edi+16] , mm1
		movq qword ptr [edi+24] , mm5

		sub esi , [SrcPitch]
		sub edi , [DstPitch]

		add eax , 8
		add ebx , 8
		add esi , 16
		add edi , 32

		sub edx , 16
		ja cyc

		mov edx , [SrcWidth]

		add esi , [SrcStride]
		add eax , [SrcStrideU]
		add ebx , [SrcStrideU]
		add edi , [DstStride]

		sub ecx,2
		ja cyc
		emms

		pop ebx
	}
}
// 
// void CYUVRender::SetIsShowBmps( BOOL bShowRecord,BOOL bShowClose, BOOL bShowAudioOn )
// {
// 	m_bShowRecordBmp	= bShowRecord;
// 	m_bShowCloseBmp		= bShowClose;
// 	m_bShowAudioOnBmp	= bShowAudioOn; 
// }
// 
// void CYUVRender::SetBmpsRects( CRect rcRecord,CRect rcClose,CRect rcAudio ,CRect rcWnd)
// {
// 	m_rcRecord = rcRecord;
// 	m_rcClose = rcClose;
// 	m_rcAudio = rcAudio;
// 	m_rcWnd = rcWnd;
// }