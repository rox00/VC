// GVAPPlayer.h
#if !defined(AFX_GVAPPLAYER_H__A87071B0_7053_4C0C_ABC1_7D3A9DEAB2A5__INCLUDED_)
#define AFX_GVAPPLAYER_H__A87071B0_7053_4C0C_ABC1_7D3A9DEAB2A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

// #include "QueueBuf.h"
// #include "WaveBase.h"
// #include "GVAPWorker.h"
#include <windows.h>
#include <Vfw.h>
#pragma comment(lib,"Vfw32.lib")

#define MAX_PICCOUNT	128

typedef struct
{
	int     nWidth;
	int     nHeight;
	HWND    hShowWnd ;
	BOOL    bInUsing ;
	BOOL	bAlarm;
	BOOL    bFristFrame ;
	BOOL	bInRecording;
	DWORD   dwMode ;
	char	szCamName[512];
	char	szCamIP[16];
	DWORD   dwDecoder;
	LPCSTR  lpCapFile;
	LPBYTE  pY,pU,pV,pRGBbuf; 
	HDRAWDIB hDrawDIB;
	LPBITMAPINFO pBmpInfo;
	BITMAPINFOHEADER BMH ;
	void    *pRender ;
	void    *pPlayer ;
	void    *pCVenter ;

	HDC		srcHdc;
	HDC		bufHdc;
// 	CDC     *pSrcDC ;
// 	CDC     *pBufDC ;
// 	CBitmap *pBufBM ;

}YUVDataInfo;

typedef struct
{
	int type;		// 1:video; 100:audio
	int nIFrame;
	int height;
	int length;
	int framerate;	// 25
	DWORD dwTimeStamp;
}FrameHead;

typedef enum
{
	PUBVIDTYPE = 0,
	MYVIDTYPE,
	LOCALVIDETYPE

}VidConType;

// CString MakeRecordFileName(LPCSTR lpszCamID,LPCSTR lpszCamName,BOOL bCapFile=FALSE);

// typedef struct _DeleteFile
// {
// 	char strFileName[256];
// 	int nFileSize;
// 	CTime nCreateTime;
// }DelFile;


class CGOSCAMDecoder
{
public:
	CGOSCAMDecoder();
	virtual ~CGOSCAMDecoder();

	DWORD   m_dwDecIndex ;
	DWORD   m_dwWidth ;
	DWORD   m_dwHeight;
	
	YUVDataInfo *m_pYUVInfo ;
	CHAR    m_szCaptureFile[1024];

	YUVDataInfo		*SetDecoderInfo(void *pPlayer,HWND hShowWnd,LPCSTR lpszCamName);
	static void		CB_VideoPlay(char* pY,char* pCb,char* pCr,int iIndex,int nLineSize,int nLineSize1,int nX,int nY);
};


BOOL   ULIFE(DWORD dwDiskIndex, DWORD nLimitSize);
// 
// class CGOSCAMRecorder
// {	
// public:
// 	CGOSCAMRecorder();
// 	virtual ~CGOSCAMRecorder();
// 	
// //	void   WriteRecordData(BYTE *pData,DWORD dwSize,DWORD dwFrameType,DWORD dwIFrame=0,DWORD dwFrameRate=0,DWORD dwTimpStamp=0);
// 
// // 	void   StartRecord(LPCTSTR lpRecordFile, CString strID, CString strCamName) ;
// // 	void   StopRecord();
// // 	
// // 	BOOL   IsRecording();
// 	
// 	CFile  m_FileRec ;
// 	BOOL   m_bSavedIFrame ;
// 
// 	BOOL	m_bSplit;
// 	clock_t	m_start;
// 	CString m_strDevID;
// 	CString m_strDevName;
// };

enum
{
	CON_SERVER_ERROR=0,
	CON_SUCCESS,
	CON_PWD_ERROR,
	CON_CAM_OFFLINE,
	CON_CAM_MISS
};

// class CGVAPPlayer  
// {
// public:
// 	CGVAPPlayer();
// 	virtual ~CGVAPPlayer();
// 
// 	void SendScaleCmd(int nX, int nY);
// 
// 	
// 	void Rest(DWORD dwIntervalStamp,DWORD dwIntervalFrame,DWORD dwIntervalDecodeAndShow);
// 	int USleep(DWORD dwMsec);
// 	static UINT ThreadHeartBeat(LPVOID lpParam);
// 	static UINT ThreadDataRecv(LPVOID lpParam);
// 	static UINT ThreadVidPlay(LPVOID lpParam);
// 	static UINT ThreadAudPlay(LPVOID lpParam);
// 
// 	CWinThread* m_pThreadHeartBeat;
// 	CWinThread* m_pThreadDataRecv;
// 	CWinThread* m_pThreadVidPlay;
// 	CWinThread* m_pThreadAudPlay;
// 
// 
// 	void SetAudioProperty(BYTE *adstData);
// 	void AddVideoFrame(BYTE *pszBuf,stFrameHeader *pFrameHeader);
// 	void AddAudioFrame(BYTE *pszBuf,stFrameHeader *pFrameHeader);
// 	void PlayAVideoFrame() ;
// 	void PlayAAudioFrame() ;
// 	void DoRecording(BOOL bCapJPGOnly=FALSE);
// 	void DoDecKeyFrameOnly(BOOL bKFrameOnly=TRUE);
// 	
// 	void  StopConnect();
// 	
// 	DWORD RecvDataAndProcess(DWORD dwDataType=0);
// 	DWORD sendCmd(void *pReq,DWORD dwCmdDest=0);
// 	void  sendDataREQ(DWORD dwType=0,BOOL bStart=TRUE) ;
// 
// 	int  ConnectToServer();
// 	BOOL DoPlayDevice(DeviceInfo *pDevInfo,HWND hShowWnd) ;
// 	void DoDomeCtrl(int nCmdID) ;
// 	int  DoDealWithACK(LPCSTR lpAckString) ;
// 
// 	void ChangeDeviceInfo(LPCTSTR szDevName);
// 
// // 	CGOSCAMRecorder *m_pRecorder ;
// 	CGOSCAMDecoder	*m_pVidDecoder ;
// // 	CWaveOut	    *m_pAudDecoder ;
// // 	DeviceInfo      *m_pDevInfo ;
// // 
// // 	CQueueBuf		m_bufVidQueue;
// // 	CQueueBuf		m_bufAudQueue;
// 
// 	BOOL	m_bToLogin;
// 	BOOL	m_bAudio;
// 	BOOL	m_bRecing;
// 	BOOL	m_bShowCloseBtn ;
// 	CRect   m_rcVedio ;
// 	CRect   m_rcClsBtn ;
// 	CRect   m_rcVolBtn ;
// 	CRect   m_rcMotion ;
// 	VidConType	m_iVideoType;
// 	void ReSizeVideo();
// 	
// 	BOOL    m_bIsDDNS;
// 	int		m_nSokConType;
// 
// // 	AVFrameData	*m_pBufVideoPlay;
// // 	AVFrameData	*m_pBufAudioPlay;
// 
// 	BOOL		m_bDiscardFrame;
// 	BOOL		m_bFirstTime;
// 	BOOL		m_bOnlyKeFrame;
// 	DWORD		m_dwDecodeStartTime;
// 	DWORD		m_dwDecodeEndTime;
// 	DWORD		m_dwLastTimeStamp;
// 	DWORD		m_dwRecvErrTime;
// 
// 	HANDLE  m_hHeartBEvent ;
// 	HANDLE	m_hDataRecv;
// 	HANDLE  m_hPlayVidEvent ;
// 	HANDLE  m_hPlayAudEvent ;
// 	
// 	BOOL    m_bEndWork ;
// 	
// 	HWND    m_hReport  ;
// 	BOOL    m_bGetIRFrame;
// 
// 	SOCKET  m_nSockTrans ;
// 	CString m_strIP;
// 	WORD    m_sPort;
// 	CString m_strDDNSIP;
// 	WORD    m_sDDNSPort;
// 
// 	CHAR   *m_pszProtoBuf ;
// 
// 	BOOL bReCnntFlag;
// 	BOOL m_bSendStreamType;
// 	int  m_nVideoFrameRate;
// 	BOOL m_bPlayAlarmSound;
// };

extern BOOL  g_bRecOddFlag ;

#endif 
