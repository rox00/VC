#include "stdafx.h"
#include "GVAPPlayer.h"
#include "YUVRender.h"

#define FRAME_SIZE	(1920*1080*4)

#include "convert.h"

BOOL  g_bRecOddFlag = FALSE;
DWORD	g_dwOldWidth[MAX_PICCOUNT] = {0};
DWORD	g_dwOldHeight[MAX_PICCOUNT] = {0};

// extern SysParam g_Param ;
// / 服务端与客户端协议-查询命令
// #define COMMAND_C_S_QUERY_LOGIN_REQ		0x1001
// #define COMMAND_S_C_QUERY_LOGIN_ACK		0x1002
// #define COMMAND_C_S_QUERY_INFO_REQ		0x1003
// #define COMMAND_S_C_QUERY_INFO_ACK		0x1004
// #define COMMAND_C_S_QUERY_NULL	    	0x1005
// 
// #define COMMAND_C_S_TRANSMIT_LOGIN_REQ		0x2001
// #define COMMAND_S_C_TRANSMIT_LOGIN_ACK		0x2002
// #define COMMAND_C_S_TRANSMIT_NULL	   	    0x2003
// #define COMMAND_C_S_TRANSMIT_DATAINFO_REQ	0x6
// #define COMMAND_S_C_TRANSMIT_DATAINFO_ACK	0x7
// #define COMMAND_C_S_TRANSMIT_DATA_REQ		0x8
// #define COMMAND_S_C_TRANSMIT_DATA_ACK		0x9
// #define COMMAND_C_S_TRANSMIT_REQ			0x10
// #define COMMAND_S_C_TRANSMIT_ACK			0x11

int g_samplingFrequency[] = {
	96000,		//96000 Hz
	88200,		//88200 Hz
	64000,		//64000 Hz
	48000,		//48000 Hz
	44100,
	32000,
	24000,
	22050,
	16000,
	12000,
	11025,
	8000,
	7350
	//Reserved
	//Reserved
	//frequency is written explictly
};

#define QUERY_SERVER_PORT     5551
#define TRANS_SERVER_PORT     5552

#define PTZ_SCALE_REQ 14

static CGOSCAMDecoder *s_pDecoders[MAX_PICCOUNT] = {NULL} ;

static void InitYUVInfo(DWORD dwDecoder,int nWidth=0,int nHeight=0,HWND hShowWnd=NULL,DWORD dwMode=0);

static YUVDataInfo *GetYUVInfo(DWORD dwIndex)
{
	if(s_pDecoders[dwIndex]==NULL || dwIndex>MAX_PICCOUNT)
		return NULL ;

	return ((YUVDataInfo *)s_pDecoders[dwIndex]->m_pYUVInfo);
}

static DWORD GetAvailableDecoder()
{
	DWORD  i = 0;

	for(i=0; i<MAX_PICCOUNT; i++)
	{
		if(s_pDecoders[i] == NULL)
		{
			return i ;
		}

		YUVDataInfo *pInfo = GetYUVInfo(i) ;

		if(!pInfo->bInUsing)
		{
			pInfo->bInUsing = TRUE ;
			return i;
		}
	}
	return -1;
}

void InitYUVInfo(DWORD dwDecIndex,int nWidth,int nHeight,HWND hShowWnd,DWORD dwMode)
{
	if(dwDecIndex>=MAX_PICCOUNT)
		return ;

	YUVDataInfo *pInfo = GetYUVInfo(dwDecIndex) ; 
	if(pInfo == NULL)
		return;

	HWND hOldWnd = pInfo->hShowWnd;
	s_pDecoders[dwDecIndex]->m_dwWidth  = nWidth  ;
	s_pDecoders[dwDecIndex]->m_dwHeight = nHeight ;

	if(hShowWnd != NULL)
	{
		if(pInfo->hShowWnd != NULL)
		{
			ShowWindow(pInfo->hShowWnd,SW_HIDE);
			ShowWindow(pInfo->hShowWnd,SW_SHOW);
		}
		pInfo->hShowWnd = hShowWnd ;
	}

	pInfo->bInUsing = TRUE ;

	if(nWidth == 0 || nHeight == 0)
		return ;

	if((pInfo->nWidth == 0 || pInfo->nHeight == 0))
	{
		pInfo->nWidth  = nWidth;
		pInfo->nHeight = nHeight;

		DWORD dwSize  = nWidth*nHeight;

		pInfo->pY      = (LPBYTE)malloc(dwSize);
		pInfo->pU      = (LPBYTE)malloc(dwSize/4);
		pInfo->pV      = (LPBYTE)malloc(dwSize/4);
// 		pInfo->pRGBbuf = (LPBYTE)malloc(dwSize*3);

// 		LPBITMAPINFO pTmpInfo = (LPBITMAPINFO)calloc(1,sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)));
// 		pInfo->pBmpInfo = pTmpInfo;
// 		pTmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
// 		pTmpInfo->bmiHeader.biPlanes   = 1;
// 		pTmpInfo->bmiHeader.biBitCount = 24;
// 		pTmpInfo->bmiHeader.biWidth  = nWidth;
// 		pTmpInfo->bmiHeader.biHeight = nHeight;
// 
// 		if(pInfo->hDrawDIB != NULL)
// 			DrawDibClose(pInfo->hDrawDIB);
// 
// 		pInfo->hDrawDIB = DrawDibOpen() ;

// 		if(pInfo->srcHdc != NULL)
// 		{
// 			ReleaseDC(hOldWnd,pInfo->srcHdc) ;
// 			DeleteDC(pInfo->bufHdc);
// 			pInfo->pBufBM->DeleteObject() ;
// 			delete pInfo->pBufBM ;
// 		}
// 		HDC hDC = GetWindowDC(pInfo->hShowWnd);
// 		CDC *pDC = CDC::FromHandle(::GetDC(pInfo->hShowWnd)) ;
// 		CDC *pBDC= new CDC() ;
// 		pBDC->CreateCompatibleDC(pDC) ;
// 
// 		HBITMAP hBBM = ;
// 		BITMAP ;
// 		CBitmap *pBBM = new CBitmap();
// 		pBBM->CreateCompatibleBitmap(pBDC,nWidth,nHeight);
// 		SelectObject()
// 
// 		pBDC->SelectObject(pBBM) ;

// 		pInfo->pSrcDC = pDC  ;
// 		pInfo->pBufDC = pBDC ;
//		pInfo->pBufBM = pBBM ;
	}
}

DWORD g_dwFlickerTime = 0;
// static void AlarmFlicker( HDC hDC=NULL ,RECT* pRect=NULL)
// {
// 	if (!hDC || !pRect) return;
// 	
// 	g_dwFlickerTime++;
// 	if (g_dwFlickerTime > 2048u) g_dwFlickerTime = 0;
// 	DWORD dwRemainder = g_dwFlickerTime%4;
// 	
// 	HBRUSH	hBrush;
// 	HPEN	hPen;
// 	
// 	hBrush	= (HBRUSH)GetStockObject(NULL_BRUSH);
// 	if (dwRemainder<2)
// 	{
// 		hPen = ::CreatePen(PS_SOLID,4,RGB(255,200,14));
// 	}
// 	else
// 	{
// 		hPen = ::CreatePen(PS_SOLID,4,RGB(200,0,0));
// 	}
// 	
// 	if (hPen && hBrush)
// 	{
// 		pRect->DeflateRect(1,1,1,1);
// 		::SelectObject(hDC,hBrush);
// 		::SelectObject(hDC,hPen);
// 		::Rectangle(hDC,pRect->left,pRect->top,pRect->right,pRect->bottom);
// 		
// 		::DeleteObject(hBrush);
// 		::DeleteObject(hPen);
// 	}
// }

static void ShowBmpData(YUVDataInfo *pInfo)
{
	if(pInfo == NULL)
		return ;

	int nX = pInfo->nWidth ;
	int nY = pInfo->nHeight;

  	CYUVRender *pRender = (CYUVRender *)pInfo->pRender ;
  	if(pRender == NULL)
  	{
  		pRender = new CYUVRender() ;
  		pInfo->pRender = pRender;
  	}
  	pRender->DDW_Init(pInfo->hShowWnd,nX,nY);
  	pRender->DDW_Rander(pInfo->pY,pInfo->pU,pInfo->pV);
  	return ;
}

// void CGVAPPlayer::ReSizeVideo()
// {
// 	CRect rc ;
// 	::GetClientRect(m_hReport,rc) ;
// 	rc.InflateRect(-6,-6,-6,-6) ;
// 	m_rcVedio  = rc ;
// 	m_rcClsBtn = rc;
// 	m_rcVolBtn = rc;
// 
// 	m_rcClsBtn.top+=1 ;
// 	m_rcClsBtn.left  = m_rcClsBtn.right-18 ;
// 	m_rcClsBtn.bottom= m_rcClsBtn.top+16 ;
// 
// 	m_rcVolBtn.left = m_rcVolBtn.right  - 33 ;
// 	m_rcVolBtn.top  = m_rcVolBtn.bottom - 25 ;
// 
// 	m_rcMotion = m_rcVolBtn;
// 	m_rcMotion.left = m_rcVolBtn.left - m_rcMotion.Width();
// 	m_rcMotion.right = m_rcMotion.left + m_rcMotion.Width();
// }

// CString MakeRecordFileName(LPCSTR lpszCamID,LPCSTR lpszCamName,BOOL bCapFile)
// {
// 	CString strFile ;
// 	CTime tm = CTime::GetCurrentTime() ;
// 
// 	char szPath[2048] = {0} ;
// 	char szName[1024] = {0} ;
// 	sprintf(szPath,"%s\\%s(%s)\\%d-%02d-%02d",g_Param.szRecordPath,lpszCamName,lpszCamID,tm.GetYear(),tm.GetMonth(),tm.GetDay()) ;
// 	sprintf(szName,"%s(%s)_%d-%02d-%02d-%02d-%02d-%02d",lpszCamName,lpszCamID,tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond()) ;
// 	
// 	USES_CONVERSION ;
// 	strFile.Format(_T("%s\\%s.264"),A2W(szPath),A2W(szName)) ;
// 
// 	if(bCapFile)
// 		strFile.Format(_T("%s\\%s.jpg"),A2W(szPath),A2W(szName)) ;
// 	CreateMultiLayerDir(W2A(strFile)) ;
// 
// 	return strFile ;
// }


CGOSCAMDecoder::CGOSCAMDecoder()
{
	m_dwWidth  = 490;
	m_dwHeight = 385;
	m_pYUVInfo = NULL ;

	m_dwDecIndex = MAX_PICCOUNT;
	for (DWORD i=0; i<MAX_PICCOUNT; i++)
	{
		if (s_pDecoders[i] == NULL)
		{
			m_dwDecIndex = i;
			break;
		}
	}

	if (m_dwDecIndex >= MAX_PICCOUNT)
		return;


	s_pDecoders[m_dwDecIndex] = this;

	if (m_pYUVInfo == NULL)
	{
		m_pYUVInfo = (YUVDataInfo*)calloc(1,sizeof(YUVDataInfo));
	}
}

CGOSCAMDecoder::~CGOSCAMDecoder()
{
	YUVDataInfo *pInfo = (YUVDataInfo*)m_pYUVInfo;
	if (pInfo)
	{
		if (pInfo->hDrawDIB)
			DrawDibClose(pInfo->hDrawDIB);
		if (pInfo->pY)
			free(pInfo->pY);
		if (pInfo->pU)
			free(pInfo->pU);
		if (pInfo->pV)
			free(pInfo->pV);
		if (pInfo->pRGBbuf)
			free(pInfo->pRGBbuf);
		if (pInfo->pBmpInfo)
			free(pInfo->pBmpInfo);

// 		if (pInfo->pBufBM)
// 			delete pInfo->pBufBM;
	
		if (pInfo->pCVenter!=NULL)
			delete(pInfo->pCVenter);
		
// 		if (pInfo->pBufDC)
// 			delete(pInfo->pBufDC);

		free(pInfo);
		pInfo = NULL;
		m_pYUVInfo = NULL;
	}

	s_pDecoders[m_dwDecIndex] = NULL;
}

YUVDataInfo *CGOSCAMDecoder::SetDecoderInfo(void *pPlayer,HWND hShowWnd,LPCSTR lpszCamName)
{
	if(m_pYUVInfo!=NULL)
	{
		YUVDataInfo *pInfo = m_pYUVInfo;
		pInfo->hShowWnd    = hShowWnd ;
		pInfo->bFristFrame = TRUE ;
		if(lpszCamName)
			strcpy_s(pInfo->szCamName,lpszCamName);
		pInfo->pPlayer = pPlayer ;
	}
	return m_pYUVInfo ;
}


void CGOSCAMDecoder::CB_VideoPlay( char* pY,char* pCb,char* pCr,int iIndex,int nLineSize,int nLineSize1,int nX,int nY )
{
	if ( iIndex>=128 || s_pDecoders[iIndex]==NULL || nX==0 || nY==0 )
		return ;

	if(		(g_dwOldWidth[iIndex] != 0 && g_dwOldHeight[iIndex] != 0) 
		&&	((g_dwOldWidth[iIndex] != nX) || (g_dwOldHeight[iIndex] != nY)))
	{
		YUVDataInfo *pInfo = GetYUVInfo(iIndex) ; 
		if(pInfo)
		{
 			if (pInfo->pY)
 				free(pInfo->pY);
 			if (pInfo->pU)
 				free(pInfo->pU);
 			if (pInfo->pV)
 				free(pInfo->pV);
 			if (pInfo->pRGBbuf)
 				free(pInfo->pRGBbuf);
 			if (pInfo->pBmpInfo)
 				free(pInfo->pBmpInfo);
 
 			pInfo->nWidth = 0;
			pInfo->nHeight = 0;
		}
	}

	InitYUVInfo(iIndex,nX,nY);
	YUVDataInfo *pInfo = GetYUVInfo(iIndex); 
	if (!pInfo)
		return;

	int i=0; 
	int nYUVLen = 0;
	for(i=0; i<nY; i++)
	{
		memcpy(pInfo->pY+nYUVLen,pY+i*nLineSize,nX);
		nYUVLen += nX ;
	}

	nYUVLen = 0;
	for(i=0; i<nY/2; i++)
	{
		memcpy(pInfo->pU+nYUVLen,pCb+i*nLineSize1,nX/2);
		nYUVLen += nX/2 ;
	}

	nYUVLen = 0;
	for(i=0; i<nY/2; i++)
	{
		memcpy(pInfo->pV+nYUVLen,pCr+i*nLineSize1,nX/2);
		nYUVLen += nX/2 ;
	}

	ShowBmpData(pInfo);
	g_dwOldWidth[iIndex] = nX;
	g_dwOldHeight[iIndex] = nY;
}


// CGOSCAMRecorder::CGOSCAMRecorder()
// {
// 	m_bSavedIFrame = FALSE ;
// 	m_bSplit = FALSE ;
// }
// 
// CGOSCAMRecorder::~CGOSCAMRecorder()
// {	
// 	StopRecord();
// }

//////////
// void CGOSCAMRecorder::WriteRecordData(BYTE *pData,DWORD dwSize,DWORD dwFrameType,DWORD dwIFrame/*0*/,DWORD dwFrameRate/*0*/,DWORD dwTimeStamp/*0*/)
// {	
// 	USES_CONVERSION;
// 	if (!m_bSplit)
// 	{
// 		m_bSplit = TRUE;
// 		m_start = time(NULL);
// 	}
// 	
// 	if (time(NULL) - m_start >= g_Param.dwRecLen* 60u)
// 	{	
// 		CString lpNewPath = MakeRecordFileName(W2A(m_strDevID), W2A(m_strDevName));
// 		StopRecord();
// 		StartRecord(lpNewPath, m_strDevID, m_strDevName);
// 		m_bSplit = FALSE;
// 		return;
// 	}
// 
// 	if(!IsRecording())
// 		return ;
// 
// 	FrameHead RecHEAD = {0};
// 	if (dwFrameRate == 0)
// 		dwFrameRate = 25;
// 
// 	if(m_bSavedIFrame || dwIFrame)
// 	{
// 		m_bSavedIFrame    = TRUE ;
// 		RecHEAD.type      = dwFrameType;
// 		RecHEAD.nIFrame	  = dwIFrame;
// 		RecHEAD.length    = dwSize;
// 		RecHEAD.framerate = dwFrameRate;
// 		RecHEAD.dwTimeStamp = dwTimeStamp;
// 
// 		m_FileRec.Write(&RecHEAD,sizeof(FrameHead));
// 		m_FileRec.Write(pData,dwSize);
// 	}
// }

// BOOL CGOSCAMRecorder::IsRecording()
// {
// 	return m_FileRec != CFile::hFileNull ;
// }

// void CGOSCAMRecorder::StartRecord(LPCTSTR lpRecordFile,CString strID, CString strCamName)
// {
// 	StopRecord() ;
// 	m_strDevID   = strID;
// 	m_strDevName = strCamName;
// 
// 	m_FileRec.Open(lpRecordFile,CFile::modeCreate|CFile::modeWrite);
// 
// 	
// }

// void CGOSCAMRecorder::StopRecord()
// {
// 	if(IsRecording())
// 	{
// 		m_bSavedIFrame = FALSE ;
// 		m_FileRec.Close();
// 	}
// }

// 
// BOOL TCL(DWORD dwDiskIndex, DWORD nLimitSize)
// {
// 	TCHAR szDrive[20]={L"C:"} ;
// 	szDrive[0] = 'A' + dwDiskIndex ;
// 	ULARGE_INTEGER	ullFreeBytes={0};
// 	::GetDiskFreeSpaceEx(szDrive,&ullFreeBytes,NULL,NULL);
// 	return (ullFreeBytes.QuadPart/(1024*1024)<nLimitSize);
// }


#define  MAXFRM_SIZE  1920*1080*4 

// CGVAPPlayer::CGVAPPlayer()
// {
// 	m_bToLogin  = FALSE;
// 	m_bAudio	= FALSE;
// 	m_bRecing	= FALSE;
// 	m_bFirstTime= true;
// 	m_bDiscardFrame= true;
// 	m_nSockTrans   = INVALID_SOCKET ;
// 	m_bEndWork     = FALSE ;
// 	m_bGetIRFrame  = FALSE ;
// 	m_iVideoType   = PUBVIDTYPE;
// 	m_bOnlyKeFrame = FALSE ;
// 
// // 	m_pRecorder    = new CGOSCAMRecorder();
// 	m_pVidDecoder  = new CGOSCAMDecoder();
// // 	m_pAudDecoder  = new CWaveOut();
// // 	m_pDevInfo     = NULL ;
// 
// 	m_bIsDDNS = TRUE;
// 	m_bShowCloseBtn = FALSE ;
// 	m_nSokConType = 0;
// 	
// 	m_hHeartBEvent  = CreateEvent(NULL,FALSE,FALSE,NULL) ;
// 	m_hDataRecv	    = CreateEvent(NULL,FALSE,FALSE,NULL) ;
// 	m_hPlayVidEvent = CreateEvent(NULL,FALSE,FALSE,NULL) ;
// 	m_hPlayAudEvent = CreateEvent(NULL,FALSE,FALSE,NULL) ;
// 	
// 	m_pszProtoBuf = new CHAR[MAXFRM_SIZE];
// 
// // 	m_pBufVideoPlay = new AVFrameData();
// // 	m_pBufVideoPlay->pszBuf = new BYTE[MAXFRM_SIZE];
// // 	
// // 	m_pBufAudioPlay = new AVFrameData();
// // 	m_pBufAudioPlay->pszBuf = new BYTE[0x10000];
// 
// 	m_pThreadHeartBeat = NULL;
// 	m_pThreadDataRecv  = NULL;
// 	m_pThreadVidPlay   = NULL;
// 	m_pThreadAudPlay   = NULL;
// 	bReCnntFlag = FALSE;
// 
// 	m_bSendStreamType = TRUE;
// 	m_nVideoFrameRate = 0;
// 	m_bPlayAlarmSound = TRUE;
// }
// 
// CGVAPPlayer::~CGVAPPlayer()
// {
// 	if(!m_bEndWork)
// 		StopConnect();
// 
// 	if(m_pVidDecoder)
// 	{
// 		delete m_pVidDecoder;
// // 		delete m_pAudDecoder;
// // 		delete m_pRecorder ;
// 
// 		m_pVidDecoder = NULL ;
// // 		m_pAudDecoder = NULL ;
// // 		m_pRecorder = NULL ;
// 	}
// 
// // 	if(m_pDevInfo != NULL)
// // 	{
// // 		free(m_pDevInfo);
// // 		m_pDevInfo = NULL ;
// // 	}
// 
// 	CloseHandle(m_hHeartBEvent);
// 	CloseHandle(m_hDataRecv);
// 	CloseHandle(m_hPlayAudEvent);
// 	CloseHandle(m_hPlayVidEvent);
// 
// 	if(m_pszProtoBuf)
// 	{
// 		delete []m_pszProtoBuf;
// 		m_pszProtoBuf = NULL;
// 	}
// 
// // 	if(m_pBufVideoPlay)
// // 	{
// // 		if(m_pBufVideoPlay->pszBuf)
// // 		{
// // 			delete []m_pBufVideoPlay->pszBuf;
// // 			m_pBufVideoPlay->pszBuf = NULL;
// // 		}
// // 		delete m_pBufVideoPlay;
// // 		m_pBufVideoPlay = NULL;
// // 	}
// // 
// // 	if(m_pBufAudioPlay)
// // 	{
// // 		if(m_pBufAudioPlay->pszBuf)
// // 		{
// // 			delete []m_pBufAudioPlay->pszBuf ;
// // 			m_pBufAudioPlay->pszBuf = NULL;
// // 		}
// // 		delete m_pBufAudioPlay ;
// // 		m_pBufAudioPlay = NULL;
// // 	}
// }

// void CGVAPPlayer::DoDecKeyFrameOnly(BOOL bKFrameOnly)
// {
// 	m_bOnlyKeFrame = bKFrameOnly ;
// }
// 
// void CGVAPPlayer::StopConnect() 
// {
// 	g_dwOldWidth[m_pVidDecoder->m_dwDecIndex] = 0;
// 	g_dwOldHeight[m_pVidDecoder->m_dwDecIndex] = 0;
// 	m_bEndWork = TRUE ;
// 	bReCnntFlag = FALSE;
// 
// 	SetEvent(m_hHeartBEvent);
// 	SetEvent(m_hDataRecv);
// 	SetEvent(m_hPlayVidEvent);
// 	SetEvent(m_hPlayAudEvent);
// 	
// 
// 	if (m_pThreadDataRecv)
// 	{
// 		if( WAIT_OBJECT_0 != WaitForSingleObject(m_pThreadDataRecv->m_hThread,1400))
// 		{
// 			TerminateThread(m_pThreadDataRecv->m_hThread, 0);
// 		}
// 		delete m_pThreadDataRecv;
// 		m_pThreadDataRecv = NULL;
// 	}
// 
// 	if (m_pThreadHeartBeat)
// 	{
// 		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pThreadHeartBeat->m_hThread, 1400))
// 		{
// 			TerminateThread(m_pThreadHeartBeat->m_hThread,0);
// 		}
// 		delete m_pThreadHeartBeat;
// 		m_pThreadHeartBeat = NULL;
// 	}
// 
// 	if (m_pThreadVidPlay)
// 	{
// 		if( WAIT_OBJECT_0 != WaitForSingleObject(m_pThreadVidPlay->m_hThread,1400))
// 		{
// 			TerminateThread(m_pThreadVidPlay->m_hThread, 0);
// 		}
// 		delete m_pThreadVidPlay;
// 		m_pThreadVidPlay = NULL;
// 	}
// 
// 	if (m_pThreadAudPlay)
// 	{
// 		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pThreadAudPlay->m_hThread,1400))
// 		{
// 			TerminateThread(m_pThreadAudPlay->m_hThread,0);
// 		}		
// 		delete m_pThreadAudPlay;
// 		m_pThreadAudPlay = NULL;
// 	}
// 
// 	if(m_nSockTrans != INVALID_SOCKET)
// 	{
// 		sendDataREQ(0,FALSE);
// 		closesocket(m_nSockTrans);
// 		m_nSockTrans = INVALID_SOCKET ;
// 	}
// 
// 	Sleep(100) ;
// 
// 
// 	m_bufVidQueue.Empty();
// 	m_bufAudQueue.Empty();
// 	m_bGetIRFrame  = FALSE ;
// 	if(m_pDevInfo != NULL)
// 	{
// 		free(m_pDevInfo) ;
// 		m_pDevInfo = NULL ;
// 	}
// }
// 
// /////////////////
// UINT CGVAPPlayer::ThreadDataRecv(LPVOID lpParam)
// {
// 	CGVAPPlayer *pConnTor = (CGVAPPlayer *)lpParam ;
// 	pConnTor->m_bDiscardFrame = true;
// 
// 	if (pConnTor == NULL)
// 		return 0 ;
// 
// 
// 	pConnTor->m_dwRecvErrTime = 0;
// 
// 	while(WaitForSingleObject(pConnTor->m_hDataRecv,1) != WAIT_OBJECT_0)
// 	{
// 		if(pConnTor->m_bEndWork)
// 			break;
// 
// 		if(pConnTor->RecvDataAndProcess(1) == 0)
// 		{
// 			if(pConnTor->m_bEndWork)
// 				break;
// 
// 			if(pConnTor->m_dwRecvErrTime > 0)
// 			{
// 				/*if(!pConnTor->m_bEndWork)
// 				{*/
// 				Sleep(5000);
// 				DeviceInfo *pTmp = pConnTor->m_pDevInfo;
// 				if (pTmp&&!pTmp->bIsPBDevice&& !pTmp->bIsLocal && !pTmp->bIsGroup)
// 				{
// 					if (pTmp->dwStatus != 2)
// 					{	
// 						Sleep(5000);
// 						continue;
// 					}
// 				}
// 
// 				if (pConnTor->ConnectToServer() == CON_SUCCESS)
// 				{
// 					pConnTor->bReCnntFlag = TRUE;
// 				}else	
// 				{
// 					pConnTor->bReCnntFlag = FALSE;
// 				}
// 				::PostMessage(pConnTor->m_hReport,WM_COMMAND,0x4000,(DWORD)pConnTor->bReCnntFlag) ;
// 				/*}*/
// 			}
// 		}
// // 		TRACE("\nTHREAD RECV DATA DOING");
// 	}
// 	/*if(!bReCnntFlag)
// 	{
// 		::PostMessage(pConnTor->m_hReport,WM_COMMAND,0x3000,0) ;
// 	}*/
// 
// 	return 0;
// }
// 
// UINT CGVAPPlayer::ThreadHeartBeat(LPVOID lpParam)
// {
// 	CGVAPPlayer *pConnTor = (CGVAPPlayer *)lpParam ;
// 	if(pConnTor == NULL) return 0 ;
// 
// 	while(WaitForSingleObject(pConnTor->m_hHeartBEvent,10000) != WAIT_OBJECT_0)
// 	{
// 		if(pConnTor->m_bEndWork)
// 			break;
// 		CGOSCAMProtocolAX HB ;
// 		HB.SetCommand(COMMAND_C_S_TRANSMIT_NULL) ;
// 		pConnTor->sendCmd(&HB,1);
// 	}
// 	return 0;
// }
// /////////////////
// UINT CGVAPPlayer::ThreadVidPlay(LPVOID lpParam)
// {
// 	CGVAPPlayer *pConnTor = (CGVAPPlayer *)lpParam ;
// 	if(pConnTor == NULL)
// 		return 0 ;
// 	
// 	while(WaitForSingleObject(pConnTor->m_hPlayVidEvent,1) != WAIT_OBJECT_0)
// 	{
// 		if(pConnTor->m_bEndWork)
// 			break;
// 		pConnTor->PlayAVideoFrame();
// 	}
// 	
// 	return 0;
// }
// 
// UINT CGVAPPlayer::ThreadAudPlay(LPVOID lpParam)
// {
// 	CGVAPPlayer *pConnTor = (CGVAPPlayer *)lpParam ;
// 	if(pConnTor == NULL)
// 		return 0 ;
// 	
// 	while(WaitForSingleObject(pConnTor->m_hPlayAudEvent,1) != WAIT_OBJECT_0)
// 	{
// 		if(pConnTor->m_bEndWork)
// 			break;
// 		pConnTor->PlayAAudioFrame();
// 	}
// 	
// 	return 0;
// }
// 
// ////////////
// 
// void CGVAPPlayer::AddVideoFrame(BYTE *pszBuf,stFrameHeader *pFrameHeader)
// {
// 	if (pFrameHeader->dwSize==0)
// 		return ;
// 	
// 	AVFrameData *pFrame = (AVFrameData *)calloc(1,sizeof(AVFrameData));
// 	pFrame->pszBuf  = (BYTE*)calloc(1,pFrameHeader->dwSize);
// 	memcpy(&pFrame->fheader,pFrameHeader,sizeof(stFrameHeader));
// 	memcpy(pFrame->pszBuf,pszBuf,pFrameHeader->dwSize);
// 	
// 	m_nVideoFrameRate =  pFrameHeader->dwFrameRate;
// 
// 	if(!m_bDiscardFrame)
// 	{
// 		if(!m_bufVidQueue.AddBuf(pFrame))
// 		{
// 			TRACE("AddVideoFrame() FAIL \n");
// 			free(pFrame->pszBuf);
// 			free(pFrame);
// 			m_bDiscardFrame = true;
// 		}
// 	}
// 	else	// AddBuf is not OK
// 	{
// 		if(pFrame->fheader.nIFrame&0xff)
// 		{
// 			if(m_bufVidQueue.AddBuf(pFrame))
// 			{
// 				m_bDiscardFrame = false;
// 				return;
// 			}
// 		}
// 		free(pFrame->pszBuf);
// 		free(pFrame);
// 	}
// }
// 
// void CGVAPPlayer::AddAudioFrame(BYTE *pszBuf,stFrameHeader *pFrameHeader)
// {
// 	if (pFrameHeader->dwSize==0) return ;
// 	
// 	AVFrameData *pFrame = (AVFrameData *)calloc(1,sizeof(AVFrameData)) ;
// 	pFrame->pszBuf  = (BYTE*)calloc(1,pFrameHeader->dwSize);
// 	memcpy(&pFrame->fheader,pFrameHeader,sizeof(stFrameHeader));
// 	memcpy(pFrame->pszBuf,pszBuf,pFrameHeader->dwSize);
// 	
// 	if(!m_bufAudQueue.AddBuf(pFrame))
// 	{
// 		free(pFrame->pszBuf);
// 		free(pFrame);
// 	}
// }
// 
// void CGVAPPlayer::PlayAVideoFrame()
// {
// 	if(m_bEndWork)
// 		return ;
// 	if (m_bufVidQueue.GetBuf(m_pBufVideoPlay))
// 	{
// 		if(m_bEndWork)
// 			return ;
// 
// 		BOOL bIsKeyFrame = m_pBufVideoPlay->fheader.nIFrame&0xff ;
// 		if(m_bGetIRFrame || bIsKeyFrame)
// 		{
// 			if(!bIsKeyFrame && m_bOnlyKeFrame)
// 				return ;
// 
// 			m_bGetIRFrame = TRUE ;
// 			if (m_pVidDecoder != NULL)
// 			{
// 				if(m_pVidDecoder->m_pYUVInfo != NULL)//reconnect crash!, by marshal
// 					m_pVidDecoder->m_pYUVInfo->bAlarm = ((m_pBufVideoPlay->fheader.nIFrame&0xff00)==0x100) ? TRUE : FALSE;
// 
// 				if(m_bFirstTime)
// 				{
// 					m_dwLastTimeStamp = m_pBufVideoPlay->fheader.dwTimeStamp;
// 					m_bFirstTime = false;
// 				}
// 				
// 				m_dwDecodeStartTime = GetTickCount();
// 		
// // 				if (IsWindowVisible(m_hReport))
// 				{
// 					m_pVidDecoder->DoVideoDecode(m_pBufVideoPlay->pszBuf,m_pBufVideoPlay->fheader.dwSize);
// 				}
// 				m_dwDecodeEndTime = GetTickCount();
// 				if(m_bEndWork)
// 					return ;
// 
// 				Rest(m_pBufVideoPlay->fheader.dwTimeStamp-m_dwLastTimeStamp,1000/m_pBufVideoPlay->fheader.dwFrameRate,m_dwDecodeEndTime-m_dwDecodeStartTime);
// 				m_dwLastTimeStamp = m_pBufVideoPlay->fheader.dwTimeStamp;
// 			}
// 		}
// 	}
// }
// 
// void CGVAPPlayer::PlayAAudioFrame()
// {
// 	if(m_bEndWork)
// 		return ;
// 	if (m_bufAudQueue.GetBuf(m_pBufAudioPlay))
// 	{
// 		if (m_pAudDecoder != NULL)
// 		{
// 			if(m_pAudDecoder->m_bFirstTime)//set sample rate
// 			{
// 				DWORD dwSampleRateTemp=8000;
// 				if(m_pBufAudioPlay->fheader.dwFrameRate<8000)
// 					dwSampleRateTemp = 8000*2;
// 				else
// 					dwSampleRateTemp=m_pBufAudioPlay->fheader.dwFrameRate*2;
// 
// 				m_pAudDecoder->SetSamplesPerSecond(dwSampleRateTemp);
// // 				SetAudioProperty((BYTE*)m_pBufAudioPlay->pszBuf);
// 				m_pAudDecoder->Start();
// 				m_pAudDecoder->m_bFirstTime=false;
// 			}
// 			
// //			if (IsWindowVisible(m_hReport))
// 				m_pAudDecoder->PlayAFrame(m_pBufAudioPlay->pszBuf,m_pBufAudioPlay->fheader.dwSize);
// 		}
// 	}
// }
// 
// DWORD CGVAPPlayer::sendCmd(void *pReq,DWORD dwCmdDest)
// {
// 	SOCKET sWorkSock = m_nSockTrans;
// 
// 	if(sWorkSock == INVALID_SOCKET || pReq == NULL)
// 		return 0;
// 
// 	CGOSCAMProtocolAX *pCmd = (CGOSCAMProtocolAX *)pReq ;
// 	pCmd->DoBuildString() ;
// 
// 	return ForceSend(sWorkSock,pCmd->m_szpCmdBuf,pCmd->m_dwBufLen,1000) ;
// }
// 
// 
// DWORD CGVAPPlayer::RecvDataAndProcess(DWORD dwDataType)
// {
// 	SOCKET sWorkSock = m_nSockTrans;
// 
// 	if(sWorkSock == INVALID_SOCKET)
// 	{
// 		++m_dwRecvErrTime;
// 		return 0;
// 	}
// 
// 	if(14 == ForceRecv(sWorkSock,m_pszProtoBuf,14,6000) )
// 	{
// 		int  nDataLen = 0 ;
// 		sscanf(m_pszProtoBuf+8,"%06X",&nDataLen);
// 		if(nDataLen>=0x4ffff || nDataLen<0)
// 			return 0 ;
// 		
// 		if( nDataLen == ForceRecv(sWorkSock,m_pszProtoBuf+14,nDataLen,10000,FALSE) )
// 		{
// 			m_pszProtoBuf[14+nDataLen] = 0 ;
// 			return DoDealWithACK(m_pszProtoBuf) ;
// 			return 1;		
// 		}
// 	}
// 
// 	m_dwRecvErrTime++;
// 	return 0 ;
// }
// 
// 
// int CGVAPPlayer::DoDealWithACK(LPCSTR lpAckString)
// {
// 	if(m_bEndWork)
// 		return FALSE;
// 
// 	CGOSCAMProtocolAX ACK ;
// 	ACK.DoDecodeString((CHAR *)lpAckString);
// 
// 	Field *pLg=NULL;
// 	if (!m_bIsDDNS)
// 	{
// 		BOOL bOk = FALSE;
// 		pLg= ACK.GetFieldInfo((LPCSTR)("Ret")) ;
// 		if ( pLg != NULL)
// 		{
// 			bOk = stricmp((CHAR*)pLg->pszFieldContent,"1") == 0;
// 			if ( !bOk )
// 			{
// 				pLg = ACK.GetFieldInfo("ErrorInfo") ;
// 				if (pLg != NULL)
// 					TRACE("@@@@@ ErrorInfo:%s\n",pLg->pszFieldContent) ;
// 				pLg = NULL ;
// 			}
// 		}
// 
// 		if (!bOk)
// 			return FALSE;
// 	}
// 
// 	switch(ACK.m_dwCommand)
// 	{
// 	case COMMAND_S_C_TRANSMIT_LOGIN_ACK:
// 		{
// 			sendDataREQ(1);
// 		}
// 		break;
// 
// 	case COMMAND_S_C_TRANSMIT_DATA_ACK:
// 		{
// 			pLg = ACK.GetFieldInfo("Data");
// 			if(pLg != NULL && pLg->pszFieldContent && pLg->dwFieldContentLen>0)
// 			{
// 				INT   nReadLen  = 0 ;
// 				INT   nFrameSize = 0 ;
// 				INT   nHeadSize = sizeof(StDataInfo) ;
// 				INT   nTotalLen = pLg->dwFieldContentLen-nHeadSize ;
// 				BYTE *pData     = pLg->pszFieldContent;
// 
// 				//TRACE("COMMAND_S_C_TRANSMIT_DATA_ACK: %d,%d\n",nHeadSize,nTotalLen) ;
// 				while(nReadLen<nTotalLen)
// 				{
// 					StDataInfo *pInfo = (StDataInfo *)pData;
// 					pInfo->nAVType = ntohl(pInfo->nAVType);
// 					pInfo->nIFrame = ntohl(pInfo->nIFrame);
// 					pInfo->dwSize  = ntohl(pInfo->dwSize);
// 					nFrameSize     = pInfo->dwSize;
// 					pInfo->dwFrameRate = ntohl(pInfo->dwFrameRate);
// 					pInfo->dwTimeStamp = ntohl(pInfo->dwTimeStamp);
// 					if (pInfo->dwFrameRate == 0)
// 						pInfo->dwFrameRate = 5;
// 
// 					if((nFrameSize<0 || nFrameSize>nTotalLen)||
// 						(pInfo->nAVType!=1&&pInfo->nAVType!=2)||
// 						((pInfo->nIFrame&0xff)!=1&&(pInfo->nIFrame&0xff)!=0))
// 					{
// 						++m_dwRecvErrTime;
// 						break;
// 					}
// 
// 					m_pRecorder->WriteRecordData((LPBYTE)(pData+nHeadSize),nFrameSize,pInfo->nAVType,((pInfo->nIFrame&0xff)==1),pInfo->dwFrameRate,pInfo->dwTimeStamp);
// 
// 					if (pInfo->nAVType == 1)
// 					{
// 						if (pInfo->dwFrameRate == 0)
// 							pInfo->dwFrameRate = 5;
// 
// 						AddVideoFrame(pData+nHeadSize,pInfo);
// 					}
// 					else if (pInfo->nAVType == 2)
// 					{
// 						if(pInfo->dwFrameRate < 8000)// 8K sampleRate
// 							pInfo->dwFrameRate=8000;
// 						
// 						AddAudioFrame(pData+nHeadSize,pInfo);
// 					}
// 
// 					pData    += (nFrameSize+nHeadSize) ;
// 					nReadLen += (nFrameSize+nHeadSize) ;
// 				}
// 			}
// 		}
// 		break;
// 	}
// 	return TRUE;
// }
// 
// int CGVAPPlayer::ConnectToServer() 
// {
// 	m_bEndWork = FALSE ;
// 	if(m_pDevInfo == NULL)
// 		return CON_CAM_MISS;
// 
// 	DWORD dwConError = CON_SERVER_ERROR ;
// 
// 	if(m_nSockTrans != INVALID_SOCKET)
// 	{
// 		closesocket(m_nSockTrans) ;
// 		m_nSockTrans = INVALID_SOCKET ;
// 	}
// 
// 	USES_CONVERSION ;
// 	
// 	if(!m_strDDNSIP.IsEmpty())
// 	{
// 		m_nSockTrans = QuickConnectToTCP(m_sDDNSPort,W2A(m_strDDNSIP));
// 		m_bIsDDNS = TRUE;
// 	}
// 
// 	if(m_nSockTrans == INVALID_SOCKET)
// 	{
// 		m_nSockTrans = QuickConnectToTCP(m_sPort,W2A(m_strIP));
// 		m_bIsDDNS = FALSE;
// 	}
// 	
// 	if(m_nSockTrans != INVALID_SOCKET)
// 	{
// 		CGOSCAMProtocolAX Cnn ;
// 		Cnn.SetCommand(COMMAND_C_S_TRANSMIT_LOGIN_REQ);
// 		Cnn.AddNewFiled("UserName",m_pDevInfo->szUserName);
// 		Cnn.AddNewFiled("Password",m_pDevInfo->szDevPwd);		
// 		Cnn.AddNewFiled("DeviceSerial",m_pDevInfo->szDevID);
// 		sendCmd(&Cnn,1);
// 		TRACE("\nReq data from server,username:%s,pwd:%s,devID:%s",m_pDevInfo->szUserName,m_pDevInfo->szDevPwd,m_pDevInfo->szDevID);
// 
// 		if(!RecvDataAndProcess(1))
// 		{
// 	/*		m_bEndWork = TRUE ;*/
// 			dwConError = CON_PWD_ERROR ;
// 			closesocket(m_nSockTrans);
// 			m_nSockTrans = INVALID_SOCKET ;
// 			TRACE("\nFailed to this Req data from server,username:%s,pwd:%s,devID:%s",m_pDevInfo->szUserName,m_pDevInfo->szDevPwd,m_pDevInfo->szDevID);
// 		}
// 		else
// 		{
// 			dwConError = CON_SUCCESS ;
// 			m_bToLogin = TRUE;
// 		}
// 	}
// /*
// 	else
// 	{
// 		m_bEndWork = TRUE ;
// 	}*/
// 
// 	return dwConError;
// }
// 
// BOOL CGVAPPlayer::DoPlayDevice(DeviceInfo *pDevInfo,HWND hShowWnd)
// {	
// 	BOOL bRet = FALSE;
// 	
// 	m_pDevInfo = pDevInfo ;
// 	if (pDevInfo == NULL || m_pVidDecoder == NULL)
// 		return bRet;
// 
// 	m_pDevInfo = (DeviceInfo *)calloc(1,sizeof(DeviceInfo))  ;
// 	memcpy(m_pDevInfo,pDevInfo,sizeof(DeviceInfo)) ;
// 
// 	CHAR *pszName = pDevInfo->szDevName ;
// 	if(pDevInfo->dwDevType == 10)
// 	{
// 		pszName = DecodeFromUTF8_S(pszName) ;
// 		strcpy(m_pDevInfo->szDevName,pszName);
// 	}
// 	
// 	if (pDevInfo->bIsLocal)
// 		m_iVideoType = LOCALVIDETYPE;
// 
// 	m_hReport = hShowWnd;
// 	m_pVidDecoder->SetDecoderInfo(this,hShowWnd,pszName) ;
// 
// 	CString strURL(pDevInfo->szDataURL);
// 
// 	CString strDirURL, strDDNSURL;
// 	int nFind = strURL.Find(';');
// 
// 	if (nFind != -1)
// 	{
// 		strDirURL  = strURL.Left(nFind);
// 		strDDNSURL = strURL.Mid(nFind+1);
// 	}
// 	else
// 	{
// 		strDirURL = strURL;
// 	}
// 	
// 	if (!strDirURL.IsEmpty())
// 	{
// 		nFind = strDirURL.Find(_T("ssp://"));
// 		if (nFind  != -1)
// 		{
// 			strDirURL = strDirURL.Mid(nFind+6);
// 		}
// 
// 		nFind = strDirURL.Find(':');
// 		if (nFind  != -1)
// 		{
// 			m_strIP = strDirURL.Left(nFind);
// 			m_sPort = _ttoi(strDirURL.Mid(nFind+1));
// 		}
// 	}
// 
// 	m_sDDNSPort=0 ;
// 	m_strDDNSIP.Empty() ;
// 
// 	if(!strDDNSURL.IsEmpty())
// 	{
// 		nFind = strDDNSURL.Find(_T("ddns://"));
// 		if (nFind != -1)
// 		{
// 			strDDNSURL = strDDNSURL.Mid(nFind+7);
// 		}
// 
// 		nFind = strDDNSURL.Find(':');
// 		if (nFind != -1)
// 		{
// 			m_strDDNSIP   = strDDNSURL.Left(nFind);
// 			m_sDDNSPort   = _ttoi(strDDNSURL.Mid(nFind+1));
// 		}
// 
// 		if(m_strDDNSIP.Find(_T("192.168")) == 0 || m_strDDNSIP.Find(_T("10.0")) == 0 || m_strDDNSIP.Find(_T("172.0")) == 0)
// 		{
// 			m_strDDNSIP.Empty() ;
// 		}
// 	}
// 
// 	if(ConnectToServer() == CON_SUCCESS)
// 	{
// 		bRet = TRUE;
// 		bReCnntFlag = TRUE;
// 	}else
// 	{
// 		bReCnntFlag = FALSE;
// 	}
// 
// 	::PostMessage(m_hReport,WM_COMMAND,0x4000,(DWORD)bReCnntFlag);
// 
// 	m_pThreadDataRecv  = AfxBeginThread(ThreadDataRecv,this,THREAD_PRIORITY_NORMAL,0, CREATE_SUSPENDED);
// 	m_pThreadDataRecv->m_bAutoDelete = FALSE;
// 	m_pThreadDataRecv->ResumeThread();
// 
// 	m_pThreadVidPlay   = AfxBeginThread(ThreadVidPlay,this,THREAD_PRIORITY_NORMAL,0, CREATE_SUSPENDED);
// 	m_pThreadVidPlay->m_bAutoDelete = FALSE;
// 	m_pThreadVidPlay->ResumeThread();
// 
// 	m_pThreadAudPlay   = AfxBeginThread(ThreadAudPlay,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
// 	m_pThreadAudPlay->m_bAutoDelete = FALSE;
// 	m_pThreadAudPlay->ResumeThread();
// 
// 	m_pThreadHeartBeat = AfxBeginThread(ThreadHeartBeat,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED) ;
// 	m_pThreadHeartBeat->m_bAutoDelete = FALSE;
// 	m_pThreadHeartBeat->ResumeThread();
// 
// 	return bRet;
// }
// 
// void CGVAPPlayer::sendDataREQ(DWORD dwType,BOOL bStart)
// {
// 	CHAR *pType = "All";
// 	CHAR *pCmmd = "Start";
// 
// 	if(dwType == 1)
// 		pType = "Video";
// 
// 	if(dwType == 2)
// 		pType = "Audio";
// 
// 	if(!bStart)
// 		pCmmd = "Stop";
// 
// 	CGOSCAMProtocolAX Req ;
// 	Req.SetCommand(COMMAND_C_S_TRANSMIT_DATA_REQ) ;
// 	Req.AddNewFiled("Command",pCmmd);
// 	Req.AddNewFiled("Type",pType);
// 	
// 	if(m_bIsDDNS == TRUE )
// 	{	
// 		Req.AddNewFiled("StreamID","1");
// 		m_bSendStreamType = TRUE;
// 	}
// 
// 	if (m_iVideoType == LOCALVIDETYPE)
// 	{	
// 		if (g_Param.bLCBigStream)
// 		{	
// 			Req.AddNewFiled("StreamID","0");
// 			m_bSendStreamType = FALSE;
// 		}
// 		else
// 		{	
// 			Req.AddNewFiled("StreamID","1");
// 			m_bSendStreamType = TRUE;
// 		}
// 	}
// 
// 	sendCmd(&Req,1) ;
// }
// 
// void CGVAPPlayer::DoDomeCtrl(int nCmdID)
// {
// 	CGOSCAMProtocolAX Req ;
// 	Req.SetCommand(COMMAND_C_S_TRANSMIT_REQ) ;
// 
// 	if (nCmdID == 103)
// 	{
// 		if (!m_bSendStreamType)
// 		{
// 			if (m_nVideoFrameRate<=8)
// 			{
// 				Req.AddNewFiled("AVQVal","Mid");
// 
// 			}else if (m_nVideoFrameRate <=15)
// 			{
// 				Req.AddNewFiled("AVQVal","Max");
// 			}else
// 			{
// 				Req.AddNewFiled("AVQVal","Min");
// 			}
// 
// 		}else
// 		{
// 			if (m_nVideoFrameRate<=3)
// 			{
// 				Req.AddNewFiled("AVQVal","Mid");
// 
// 			}else if (m_nVideoFrameRate <=5)
// 			{
// 				Req.AddNewFiled("AVQVal","Max");
// 
// 			}else
// 			{
// 				Req.AddNewFiled("AVQVal","Min");
// 			}
// 		}
// 	}
// 
// 	if (nCmdID>9)
// 	{
// 		Req.AddNewFiled("Command_Param",nCmdID,2,FALSE);
// 	}
// 	else
// 	{
// 		Req.AddNewFiled("Command_Param",nCmdID);
// 	}
// 
// 	sendCmd(&Req,1) ;
// }
// 
// void CGVAPPlayer::Rest( DWORD dwIntervalStamp,DWORD dwIntervalFrame,DWORD dwIntervalDecodeAndShow )
// {
// 	if (dwIntervalStamp < dwIntervalFrame*2)
// 	{
// 		if (dwIntervalStamp > (dwIntervalDecodeAndShow+20))
// 		{
// 			Sleep(dwIntervalStamp - dwIntervalDecodeAndShow-20);
// 		}
// 	}
// }
// 
// void CGVAPPlayer::DoRecording(BOOL bCapJPGOnly)
// {
// 	if(m_pVidDecoder == NULL || m_pRecorder == NULL)
// 		return ;
// 
// 	CString strFileName = MakeRecordFileName(m_pDevInfo->szDevID,m_pDevInfo->szDevName,bCapJPGOnly) ;
// 	USES_CONVERSION;
// 	if(bCapJPGOnly)
// 	{
// 		m_pVidDecoder->DoCapture(W2A(strFileName));
// 		return ;
// 	}
// 
// 	if(m_bRecing)
// 	{
// 		m_bRecing = FALSE;
// 		m_pRecorder->StopRecord();
// 	}
// 	else
// 	{
// 		m_bRecing = TRUE;
// 		m_pRecorder->StartRecord(strFileName,A2W(m_pDevInfo->szDevID),A2W(m_pDevInfo->szDevName)) ;
// 	}
// 
// 	if (m_pVidDecoder->m_pYUVInfo)
// 	{
// 		m_pVidDecoder->m_pYUVInfo->bInRecording = m_bRecing;
// 	}
// }
// 
// void CGVAPPlayer::SendScaleCmd(int nX, int nY)
// {
// 	CGOSCAMProtocolAX Cnn ;
// 	Cnn.SetCommand(COMMAND_C_S_TRANSMIT_REQ) ;
// 	Cnn.AddNewFiled("Command_Param",PTZ_SCALE_REQ);
// 	Cnn.AddNewFiled("X_Axis",nX);
// 	Cnn.AddNewFiled("Y_Axis",nY);		
// 	sendCmd(&Cnn,1);
// }
// 
// void CGVAPPlayer::SetAudioProperty( BYTE *adtsData )
// {
// 	if(adtsData == NULL)
// 		return;
// 	BYTE adtsHeader[7] = {0};
// 	memcpy(adtsHeader,adtsData,7);
// 	if((adtsHeader[0] != 0xff) || ((adtsHeader[1] & 0xf0) != 0xf0))
// 		return;
// 	//根据ADST头确定sampling_frequency_index的位置
// 	//adts_fixed_header()
// 	//syncword						12bit
// 	//ID							1bit
// 	//layer							2bit
// 	//protection_absent				1bit
// 	//profile						2bit
// 	//sampling_frequency_index		4bit
// 	//private_bit					1bit
// 	//channel_configuration			3bit
// 	//original_copy					1bit
// 	//home							1bit
// // 	int sampling_frequency_index = (adtsHeader[2]&0x3c)>>2;
// // 	m_pAudDecoder->SetSamplesPerSecond(g_samplingFrequency[sampling_frequency_index]);
// 	int channel = ((adtsHeader[2]&0x01)<<2) + ((adtsHeader[3]&0xc0)>>6);
// 	m_pAudDecoder->SetChannels(channel);
// }
// 
// void CGVAPPlayer::ChangeDeviceInfo( LPCTSTR szDevName )
// {
// 	if(m_pDevInfo == NULL)
// 		return;
// 	USES_CONVERSION;
// 	ZeroMemory(m_pDevInfo->szDevName,128);
// 	strcpy(m_pDevInfo->szDevName,W2A(szDevName));
// }