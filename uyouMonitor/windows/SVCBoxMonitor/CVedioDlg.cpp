
// ffmpegopecvtestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SVCBoxMonitor.h"
#include "CVedioDlg.h"
#include "afxdialogex.h"
#include "GVAPPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ULONG_PTR				gdiplusToken;
CGOSCAMDecoder			*g_pVidDecoder  = NULL;
BOOL					CVedioDlg::m_bRunning	= FALSE;
boost::mutex			CVedioDlg::m_iobufMutex;
boost::asio::streambuf	CVedioDlg::m_ioBuffer;
CVedioDlg::CVedioDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVedioDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hEvent						= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadBeginVedio				= NULL;
	m_pThreadWorkThread				= NULL;

	m_ptrSocket.reset(new boost::asio::ip::tcp::socket(m_IoService));

	m_ioBuffer.prepare(0x10000);
	m_ReadBuffer.prepare(0x10000);

	ZeroMemory(&m_Packet, sizeof(AVPacket));
	m_aCodecCtx						= NULL;

	m_pFormatCtx					= NULL;
	m_pCodecCtx						= NULL;
	m_pFrame						= NULL;
	m_pFrameRGB						= NULL;
	m_pFrameYUV						= NULL;

	m_dstNumBytes					= 0;
	m_videoIndex					= 0;
	m_pDst							= NULL;
	m_bRunning						= FALSE;


	m_curIP							= "";
	m_curPort						= 0;
	m_pCtrWnd						= NULL;
}

void CVedioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVedioDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CffmpegopecvtestDlg ��Ϣ�������
int CVedioDlg::ReadIOBuffer(void * opaque, uint8_t *buf, int bufsize)
{

	int	realBufSize			= 0;
	int emptyBeginClk		= clock();
	try
	{
		while (m_bRunning)
		{
			{
				boost::mutex::scoped_lock Lock(m_iobufMutex);


				int		iobufsize	= m_ioBuffer.size();
				{
					CString str;
					str.Format(_T(" functionname: %s iobufsize:%d \n"), _T("ReadIOBuffer"), iobufsize);
					OutputDebugString(str);
				}

				if (iobufsize > bufsize)
				{
					memcpy(buf, boost::asio::buffer_cast<const void*>(m_ioBuffer.data()), bufsize);
					m_ioBuffer.consume(bufsize);
					realBufSize = bufsize;
					break;
				}
				else if (iobufsize > 0)
				{
					memcpy(buf, boost::asio::buffer_cast<const void*>(m_ioBuffer.data()), iobufsize);
					m_ioBuffer.consume(iobufsize);
					realBufSize = iobufsize;
					break;
				}
 				else if (iobufsize == 0 && (clock() - emptyBeginClk) > 10000)
 				{	////����3��û�н��յ��κ�����
 					m_bRunning = FALSE;
 					break;
 				}
			}

			Sleep(1);
		}
	}
	catch (...)
	{
	}

	return	realBufSize;
}

void CVedioDlg::WirteIOBuffer(char *buf, int bufsize)
{
	boost::mutex::scoped_lock Lock(m_iobufMutex);
	m_ioBuffer.prepare(bufsize);
	m_ioBuffer.sputn(buf, bufsize);

	{
		int		iobufsize	= m_ioBuffer.size();
		CString str;
		str.Format(_T(" functionname: %s iobufsize:%d \n"), _T("WirteIOBuffer"), iobufsize);
		OutputDebugString(str);
	}
}

BOOL CVedioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	SetWindowText(_T("Movie"));

	if (g_pVidDecoder == NULL)
	{
		g_pVidDecoder = new CGOSCAMDecoder();
		if (g_pVidDecoder)
		{
			g_pVidDecoder->SetDecoderInfo(NULL, GetSafeHwnd(), "");
		}
	}

	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

// ��DC��תһ���ĽǶ�
inline int RotateDC(HDC hDc, int iAngle, POINT centerPt)
{
	int nGraphicsMode = SetGraphicsMode(hDc, GM_ADVANCED);
	XFORM xform;
	if (iAngle != 0)
	{
		double fangle = (double)iAngle / 180. * 3.1415926;
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(centerPt.x - cos(fangle)*centerPt.x + sin(fangle)*centerPt.y);
		xform.eDy = (float)(centerPt.y - cos(fangle)*centerPt.y - sin(fangle)*centerPt.x);
		SetWorldTransform(hDc, &xform);
	}
	return nGraphicsMode;
}

// �ָ���ת����DC
inline void RestoreRotatedDC(HDC hDc, int nGraphicsMode)
{
	XFORM xform;
	xform.eM11 = (float)1.0;
	xform.eM12 = (float)0;
	xform.eM21 = (float)0;
	xform.eM22 = (float)1.0;
	xform.eDx = (float)0;
	xform.eDy = (float)0;

	SetWorldTransform(hDc, &xform);
	SetGraphicsMode(hDc, nGraphicsMode);
}

void CVedioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		try
		{
			if (m_hbitmap && m_pCodecCtx)
			{
				CRect rcClient;
				GetClientRect(rcClient);

				//if (g_IsHorizontalMode)
				//{
				//	CDC memDc;
				//	memDc.CreateCompatibleDC(&dc);
				//	memDc.SelectObject(m_hbitmap);

				//	dc.StretchBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDc, 0, 0, m_pCodecCtx->width, m_pCodecCtx->height, SRCCOPY);
				//}
				//else
				//{
				//	Bitmap *pBmp	= Bitmap::FromHBITMAP(m_hbitmap, nullptr);

				//	int newWidth	= m_pCodecCtx->height;
				//	int newHeight	= m_pCodecCtx->width;
				//	CBitmap newBitmap;
				//	newBitmap.CreateCompatibleBitmap(&dc, newWidth, newHeight);

				//	CDC newDc;
				//	newDc.CreateCompatibleDC(&dc);
				//	newDc.SelectObject(newBitmap);
				//	Graphics graphics(newDc.GetSafeHdc());
				//	graphics.RotateTransform(0, MatrixOrderAppend);
				//	graphics.TranslateTransform(m_pCodecCtx->height, 0, MatrixOrderAppend);
				//	graphics.DrawImage(pBmp, 0, 0, 0, 0, m_pCodecCtx->width, m_pCodecCtx->height, UnitPixel);

				//	SetStretchBltMode(dc.m_hDC, HALFTONE);
				//	dc.StretchBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &newDc, 0, 0, newWidth, newHeight, SRCCOPY);

				//	delete pBmp;
				//	graphics.ReleaseHDC(newDc.GetSafeHdc());
				//	newBitmap.DeleteObject();
				//	newDc.DeleteDC();
				//}

				CDC memDc;
				memDc.CreateCompatibleDC(&dc);
				memDc.SelectObject(m_hbitmap);

				SetStretchBltMode(dc.m_hDC, HALFTONE);
				if (g_IsHorizontalMode)
					dc.StretchBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDc, 0, 0, m_pCodecCtx->width, m_pCodecCtx->height, SRCCOPY);
				else
					dc.StretchBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDc, 0, 0, m_pCodecCtx->height, m_pCodecCtx->width, SRCCOPY);

				memDc.DeleteDC();

				//////��zy 2015-12-30 ����
				//{
				//static int i= 0;
				//char ch[MAX_PATH] ={ 0 };
				//sprintf_s(ch, MAX_PATH, "\n(zyo): OnPaint:%d clock:%d name3:%d Index(i):%d \n", 0, clock(), 0, i++);
				//OutputDebugStringA(ch);//***zy***
				//}
				//////��zy 2015-12-30 ����
			}
		}
		catch (...)
		{
		}

	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVedioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVedioDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
}

LRESULT CVedioDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO:  �ڴ����ר�ô����/����û���
	switch (message)
	{
	case WM_NCHITTEST:
	{
		RECT    rc;
		GetClientRect(&rc);
		int     agWidth				= 3; //�սǿ��
		POINT    pt;
		pt.x	= lParam & 0xFFFF;
		pt.y	= lParam >> 16;

		ScreenToClient(&pt);

		if (pt.x < agWidth				||
			pt.x > rc.right - agWidth	||
			pt.y < agWidth				||
			pt.y > rc.bottom - agWidth
			)
			return HTTRANSPARENT;
	}
	break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CVedioDlg::Send(boost::asio::const_buffer& BufferToSend)
{
	heartTm = boost::timer();
	boost::mutex::scoped_lock Lock(m_sockMutex);

	if (!m_ptrSocket || !m_ptrSocket->is_open())
	{
		TRACE("SVCChannel::SVCConnection::Send(boost::asio::const_buffer& BufferToSend) ret \n");
		return;
	}

	m_ptrSocket->async_send(boost::asio::buffer(BufferToSend), boost::bind(&CVedioDlg::WriteHandler,
		this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

}

void CVedioDlg::Receive()
{
	boost::mutex::scoped_lock Lock(m_sockMutex);
	try
	{
		m_ptrSocket->async_read_some(m_ReadBuffer.prepare(4096), boost::bind(&CVedioDlg::ReadHandler,
			this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		//boost::asio::async_read(*m_ptrSocket, m_ReadBuffer, boost::bind(&CffmpegopecvtestDlg::ReadHandler,
		//	this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	catch (...)
	{
		CString str = _T("");
		str.Format(_T(" functionname: %s  ���������쳣��\n"), _T("Receive"));
		OutputDebugString(str);
	}
}

// ����֡����ΪBMPͼƬ
int CVedioDlg::SaveFrameToBMP(char *pPicFile, uint8_t *pRGBBuffer, int nWidth, int nHeight, int nBitCount)
{
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	memset(&bmpheader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpinfo, 0, sizeof(BITMAPINFOHEADER));

	FILE *fp = NULL;
	fopen_s(&fp, pPicFile, "wb");
	if (NULL == fp)
	{
		printf("file open error %s\n", pPicFile);
		return -1;
	}
	// set BITMAPFILEHEADER value
	bmpheader.bfType = ('M' << 8) | 'B';
	bmpheader.bfReserved1 = 0;
	bmpheader.bfReserved2 = 0;
	bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpheader.bfSize = bmpheader.bfOffBits + nWidth * nHeight * nBitCount / 8;
	// set BITMAPINFOHEADER value
	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.biWidth = nWidth;
	bmpinfo.biHeight = 0 - nHeight;
	bmpinfo.biPlanes = 1;
	bmpinfo.biBitCount = nBitCount;
	bmpinfo.biCompression = BI_RGB;
	bmpinfo.biSizeImage = 0;
	bmpinfo.biXPelsPerMeter = 100;
	bmpinfo.biYPelsPerMeter = 100;
	bmpinfo.biClrUsed = 0;
	bmpinfo.biClrImportant = 0;
	// write pic file
	fwrite(&bmpheader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(pRGBBuffer, nWidth * nHeight * nBitCount / 8, 1, fp);
	fclose(fp);
	return 0;
}
HBITMAP CVedioDlg::RotateBmp(bool bRotateLeft, BITMAPINFOHEADER bmpinfo, byte* pBmpData)
{
	int bmpHeight			= abs(bmpinfo.biHeight);
	int bmpWidth			= abs(bmpinfo.biWidth);

	int byteCount			= bmpinfo.biBitCount/8;													//һ�����ؼ����ֽ�
	int iAddLineData		= (4 - (bmpHeight * byteCount) % 4) % 4;							//��ת��һ��׷���ֽ���

	int eddySize			= (bmpHeight * byteCount + iAddLineData) * bmpWidth;		//��ת����Ҫ����ڴ�
	byte *bEddyData			= new byte[eddySize];

	int bEddyDataRowCount	= bmpHeight * byteCount + iAddLineData;							//��ǰһ��׷���ֽ���

	int iBmpAddLineData		= (4 - bmpWidth * byteCount % 4) % 4;								//��ת��һ����Ҫ׷�Ӽ����ֽ�

	//�����ݽ��в���
	int iImageSize			= (bmpWidth * byteCount + iBmpAddLineData) * bmpHeight;	//��ǰͼ�����ݶ���ڴ�
	for (int i = 0; i< bmpHeight; i++)
	{

		for (int j = 0; j < bmpWidth * byteCount; j += byteCount)
		{
			int bmDataIndex			= 0;
			int bEddyDataRowIndex	= 0;		//��ת��������
			int bEddyDataColIndex	= 0;		//��ת��������
			int locIndex			= 0;		//��ת���ڴ��е�����ֵ

			if (bRotateLeft)
			{
				bmDataIndex			= (bmpWidth * byteCount + iBmpAddLineData) * i;				//��ǰ�������ڵ�ǰͼ�������е��ڴ�λ��
				bEddyDataRowIndex	= (bmpWidth - 1 - (j / byteCount));							//��ת���������������
				bEddyDataColIndex	= i;																	//��ת���������������
				locIndex			= bEddyDataRowIndex  * (bmpHeight * byteCount + iAddLineData) + bEddyDataColIndex * byteCount;
			}
			else
			{
				bmDataIndex			= (bmpWidth * byteCount + iBmpAddLineData) * i;				//��ǰ�������ڵ�ǰͼ�������е��ڴ�λ��
				bEddyDataRowIndex	= j / byteCount;														//��ת���������������
				bEddyDataColIndex	= bmpHeight - 1 - i;											//��ת���������������
				locIndex			= bEddyDataRowIndex  * (bmpHeight * byteCount + iAddLineData) + bEddyDataColIndex * byteCount;

			}
			bEddyData[locIndex]		= pBmpData[j+bmDataIndex];   //B
			bEddyData[locIndex+1]	= pBmpData[j+bmDataIndex+1]; //G
			bEddyData[locIndex+2]	= pBmpData[j+bmDataIndex+2]; //R

		}
	}
	//byte * bTemp = bmData;
	//bmData = bEddyData;
	////ɾ����ʱ��Ϣ
	//deletebTemp;
	////����ͷ�ļ���Ϣ
	//bmpheader.bfSize				= bmpheader.bfOffBits + eddySize;

	bmpinfo.biWidth					= bmpHeight;
	bmpinfo.biHeight				= bmpinfo.biHeight > 0?bmpWidth : -bmpWidth;

	CClientDC dc(NULL);
	HBITMAP hBitmap					= CreateDIBitmap(dc.GetSafeHdc(),				//�豸�����ĵľ�� 
										(LPBITMAPINFOHEADER)&bmpinfo,				//λͼ��Ϣͷָ�� 
										(long)CBM_INIT,								//��ʼ����־ 
										bEddyData,									//��ʼ������ָ�� 
										(LPBITMAPINFO)&bmpinfo,						//λͼ��Ϣָ�� 
										DIB_RGB_COLORS);

	delete[] bEddyData;


	return hBitmap;
}

void CVedioDlg::SaveFrameToImg()
{
	try
	{
		int decode_frames	= 0;
		while (m_bRunning)
		{
			int frames	= -1;
			if (m_pFormatCtx)
				frames = av_read_frame(m_pFormatCtx, &m_Packet);
			if (frames < 0)
				break;

			if (m_Packet.stream_index == m_videoIndex)//�ж��Ƿ�����Ƶ��
			{
				int			got_frame;
				int ret = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got_frame, &m_Packet);//���н���
				if (ret < 0)
					break;
				SwsContext	*img_convert_ctx	= sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height, PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

				if (got_frame)
				{
					decode_frames++;

					//g_pVidDecoder->CB_VideoPlay(\
										//	(char *)m_pFrame->data[0],
					//	(char *)m_pFrame->data[1],
					//	(char *)m_pFrame->data[2],
					//	0,
					//	m_pFrame->linesize[0], //++�����в���
					//	m_pFrame->linesize[1], //++ɫ���в���
					//	m_pCodecCtx->width,   //++ͼ��ʵ�ʿ��
					//	m_pCodecCtx->height); //++



					sws_scale(img_convert_ctx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);

					if (g_bScreenShot)
					{
						TCHAR szTime[MAX_PATH] ={ 0 };
						TCHAR szName[MAX_PATH] ={ 0 };
						::GetModuleFileName(NULL, szName, sizeof(szName));

						TCHAR * pName = _tcsrchr(szName, _T('\\')) + 1;
						*pName = _T('\0');

						SYSTEMTIME sysTime;   // Win32 time information
						GetLocalTime(&sysTime);
						COleDateTime dateTime(sysTime);
						_stprintf_s(szTime, MAX_PATH, _T("%02d_%02d_%02d.bmp"), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
						_tcscat_s(szName, MAX_PATH, szTime);

						SaveFrameToBMP(CT2A(szName), m_pFrameRGB->data[0], m_pCodecCtx->width, m_pCodecCtx->height, 24);
						g_bScreenShot = FALSE;
					}

					{
						BITMAPFILEHEADER bmpheader;
						BITMAPINFOHEADER bmpinfo;
						memset(&bmpheader, 0, sizeof(BITMAPFILEHEADER));
						memset(&bmpinfo, 0, sizeof(BITMAPINFOHEADER));

						// set BITMAPFILEHEADER value
						bmpheader.bfType		= ('M' << 8) | 'B';
						bmpheader.bfReserved1	= 0;
						bmpheader.bfReserved2	= 0;
						bmpheader.bfOffBits		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
						bmpheader.bfSize		= bmpheader.bfOffBits + m_pCodecCtx->width * m_pCodecCtx->height * 24 / 8;
						// set BITMAPINFOHEADER value
						bmpinfo.biSize			= sizeof(BITMAPINFOHEADER);
						bmpinfo.biWidth			= m_pCodecCtx->width;
						bmpinfo.biHeight		= 0 - m_pCodecCtx->height;
						bmpinfo.biPlanes		= 1;
						bmpinfo.biBitCount		= 24;
						bmpinfo.biCompression	= BI_RGB;
						bmpinfo.biSizeImage		= 0;
						bmpinfo.biXPelsPerMeter = 100;
						bmpinfo.biYPelsPerMeter = 100;
						bmpinfo.biClrUsed		= 0;
						bmpinfo.biClrImportant	= 0;

						if (m_hbitmap)
							::DeleteObject(m_hbitmap);
						if (g_IsHorizontalMode)
						{
							CClientDC dc(NULL);
							m_hbitmap = CreateDIBitmap(dc.GetSafeHdc(),							//�豸�����ĵľ�� 
								(LPBITMAPINFOHEADER)&bmpinfo,				//λͼ��Ϣͷָ�� 
								(long)CBM_INIT,								//��ʼ����־ 
								m_pFrameRGB->data[0],						//��ʼ������ָ�� 
								(LPBITMAPINFO)&bmpinfo,						//λͼ��Ϣָ�� 
								DIB_RGB_COLORS);							//��ɫ���ݵ�ʹ�÷�ʽ 
						}
						else
						{
							m_hbitmap = RotateBmp(false, bmpinfo, m_pFrameRGB->data[0]);
						}

						::InvalidateRect(m_hWnd, NULL, FALSE);
					}
				}
				if (img_convert_ctx)
				{
					sws_freeContext(img_convert_ctx);
				}
			}
			av_free_packet(&m_Packet);
		}
	}
	catch (...)
	{
	}
	
}

//void CVedioDlg::CB_VideoPlay(char* pY, char* pCb, char* pCr, int iIndex, int nLineSize, int nLineSize1, int nX, int nY)
//{
//	if (iIndex>=128 || s_pDecoders[iIndex]==NULL || nX==0 || nY==0)
//		return;
//
//	if ((g_dwOldWidth[iIndex] != 0 && g_dwOldHeight[iIndex] != 0)
//		&&	((g_dwOldWidth[iIndex] != nX) || (g_dwOldHeight[iIndex] != nY)))
//	{
//		YUVDataInfo *pInfo = GetYUVInfo(iIndex);
//		if (pInfo)
//		{
//			if (pInfo->pY)
//				free(pInfo->pY);
//			if (pInfo->pU)
//				free(pInfo->pU);
//			if (pInfo->pV)
//				free(pInfo->pV);
//			if (pInfo->pRGBbuf)
//				free(pInfo->pRGBbuf);
//			if (pInfo->pBmpInfo)
//				free(pInfo->pBmpInfo);
//
//			pInfo->nWidth = 0;
//			pInfo->nHeight = 0;
//		}
//	}
//
//	InitYUVInfo(iIndex, nX, nY);
//	YUVDataInfo *pInfo = GetYUVInfo(iIndex);
//	if (!pInfo)
//		return;
//
//	int i=0;
//	int nYUVLen = 0;
//	for (i=0; i<nY; i++)
//	{
//		memcpy(pInfo->pY+nYUVLen, pY+i*nLineSize, nX);
//		nYUVLen += nX;
//	}
//
//	nYUVLen = 0;
//	for (i=0; i<nY/2; i++)
//	{
//		memcpy(pInfo->pU+nYUVLen, pCb+i*nLineSize1, nX/2);
//		nYUVLen += nX/2;
//	}
//
//	nYUVLen = 0;
//	for (i=0; i<nY/2; i++)
//	{
//		memcpy(pInfo->pV+nYUVLen, pCr+i*nLineSize1, nX/2);
//		nYUVLen += nX/2;
//	}
//
//	ShowBmpData(pInfo);
//	g_dwOldWidth[iIndex] = nX;
//	g_dwOldHeight[iIndex] = nY;
//}

void  CVedioDlg::WriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	if (ec)
	{	//д������
		std::wstring message = CA2W(ec.message().c_str());
		//Common::LogHelper::log(_T("SVCChannel::SVCConnection::WriteHandler"), L4CP_INFO_LOG_LEVEL,  message.c_str());
		//TRACE("write error %s", ec.message().c_str());
		throw std::exception((__FUNCTION__" : " + ec.message()).c_str());
	}
}

void CVedioDlg::ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{

	if (ec)
	{	//��������		
		std::string str = ec.message();
		throw std::exception((__FUNCTION__" : " + ec.message()).c_str());
		OutputDebugString(_T("������"));
	}

	try
	{
		//////��zy 2015-12-30 ����
		//{
		//	static int readbytes= 0;
		//	readbytes+=bytes_transferred;
		//	static int i= 0;
		//	char ch[MAX_PATH] ={ 0 };
		//	sprintf_s(ch, MAX_PATH, "\n(zyo): ReadHandlerStart:%d readbytes:%d m_ipStr:%s Index(i):%d \n", 0, readbytes, m_curIP.c_str(), i++);
		//	OutputDebugStringA(ch);//***zy***
		//}

		m_ReadBuffer.commit(bytes_transferred);

		clock_t clk							= clock();
		int		packHeadLen					= sizeof(PACKAGEHEAD);
		int		curBufferLen				= m_ReadBuffer.size();
		if (curBufferLen > packHeadLen)
		{
			PACKAGEHEAD *pPackageHead = (PACKAGEHEAD *)boost::asio::buffer_cast<LPCSTR>(m_ReadBuffer.data());

			if (curBufferLen >= (packHeadLen + pPackageHead->len))
			{
				if (pPackageHead->head1 == 0xFF && pPackageHead->head2 == 0xFE)
				{
					if (pPackageHead->packType == PackTypeEnum_VEDIO)
					{
						WirteIOBuffer(((char*)pPackageHead + packHeadLen), pPackageHead->len);
						m_ReadBuffer.consume(packHeadLen + pPackageHead->len);	///ɾ������
					}
					else if (pPackageHead->packType == PackTypeEnum_MOUSE)
					{

					}
					else if (pPackageHead->packType == PackTypeEnum_KEYBOARD)
					{

					}
				}
				else
				{
					//AfxMessageBox(_T("���ݽ��մ���"));
				}
			}
		}

		////׼����һ�ζ���Ҫ��ͣ�Ķ�������ioservice::run���˳���
		//if (clock() - clk > 500)
		//	MessageBox(_T("���ݴ����ʱ��̫���ˣ�"));
		Receive();
	}
	catch (...)
	{
		CString str = _T("");
		str.Format(_T(" functionname: %s  ���մ����쳣��\n"), _T("ReadHandler"));
		OutputDebugString(str);
	}
	
}

void CVedioDlg::BeginVedio()
{
	try
	{
		do 
		{
			m_ptrSocket->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(m_curIP), m_curPort));
			Receive();
			Sleep(100);
			m_pThreadWorkThread = m_WorkingThreadGroup.create_thread(std::bind(&CVedioDlg::WorkThread, this));

			av_register_all();
			if (m_pFrame)
				av_frame_free(&m_pFrame);
			m_pFrame		= av_frame_alloc();
			
			m_pFormatCtx	= avformat_alloc_context();
			//pFormatCtx->max_analyze_duration = 1000;
			m_pFormatCtx->probesize		= 2048;
			unsigned char * iobuffer	= (unsigned char*)av_malloc(8192);
			AVIOContext *avio			= avio_alloc_context(iobuffer, 8192, 0, NULL, CVedioDlg::ReadIOBuffer, NULL, NULL);
			m_pFormatCtx->pb			= avio;
			if (avformat_open_input(&m_pFormatCtx, "nothing", NULL, NULL) != 0) //����Ƶ  ����Ҫ������������У���������������******
			{
				break;
			}

			if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)//��������Ϣ  
			{
				break;
			}

			AVCodec *pvCodec;    //AVCodec ��ʾ����Ƶ�������
			//AVCodec *avCodec;
			m_videoIndex = -1;
			for (UINT i=0; i<m_pFormatCtx->nb_streams; i++)//����Ƶ���ĸ���
			{
				if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) //AVMEDIA_TYPE_VIDEO ��Ƶ��
				{
					//cout<<"�ҵ���Ƶ��ID"<<endl;
					m_videoIndex = i;
				}
			}
			if (m_videoIndex == -1){
				//cout<<"δ�ҵ���Ƶ��"<<endl;
				break;
			}
			//���й��ڱ����������Ϣ���Ǳ����ǽ���"codec context"��������������ģ��Ķ����������������������ʹ�õĹ��ڱ��������������Ϣ
			/////////������Ƶ//////////////////////
			m_pCodecCtx = m_pFormatCtx->streams[m_videoIndex]->codec;
			pvCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);  //���ҽ�����  
			if (pvCodec == NULL)
			{
				break;
			}
			if (avcodec_open2(m_pCodecCtx, pvCodec, NULL) < 0)  //������Ƶ������
			{
				break;
			}
			m_pFrameRGB			= av_frame_alloc();
			m_dstNumBytes		= avpicture_get_size(PIX_FMT_BGR24, m_pCodecCtx->width, m_pCodecCtx->height);
			m_pDst				= new uint8_t[m_dstNumBytes*sizeof(uint8_t)];
			avpicture_fill((AVPicture *)m_pFrameRGB, m_pDst, PIX_FMT_BGR24, m_pCodecCtx->width, m_pCodecCtx->height);
			CString str;
			str.Format(_T("*********************��ǰ����Ƶ���ֱ����� %d, %d ****************************"), m_pCodecCtx->width, m_pCodecCtx->height);
			OutputDebugString(str);
			av_new_packet(&m_Packet, m_dstNumBytes);
			SetEvent(m_hEvent);	////˵���Ѿ���������


			while (m_bRunning)
			{
				SaveFrameToImg();
				Sleep(1);
			}
		} while (0);
	}
	catch (...)
	{
		Sleep(100);
		m_bRunning = FALSE;
	}

	if (m_pCtrWnd)
		::PostMessage(m_pCtrWnd->GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);
}

void CVedioDlg::WorkThread()
{
	try
	{
		m_IoService.reset();
		m_IoService.run();
	}
	catch (std::exception &e)
	{
		std::string str = e.what();
		TRACE("Working Thread Raised an Exception : %s\n", str.c_str());

		m_bRunning = FALSE;	
		if (m_pCtrWnd)
			::PostMessage(m_pCtrWnd->GetSafeHwnd(), WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);
		//MessageBox(_T("CffmpegopecvtestDlg �����ѽ�����"));
	}
}

void CVedioDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������
	Stop();

	if (m_hEvent)
		CloseHandle(m_hEvent);
	m_WorkingThreadGroup.join_all();

	if (g_pVidDecoder)
	{
		delete g_pVidDecoder;
		g_pVidDecoder = NULL;
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
}


BOOL CVedioDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//pDC->FillSolidRect(0, 0, 500, 500, RGB(255, 0, 0));
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CVedioDlg::Realease()
{
	try
	{
		ResetEvent(m_hEvent);

		av_free_packet(&m_Packet);
		if (m_pFrame)
			av_frame_free(&m_pFrame);
		if (m_pFrameRGB)
			av_frame_free(&m_pFrameRGB);
		if (m_pCodecCtx)
			avcodec_close(m_pCodecCtx);
		if (m_pFormatCtx)
			avformat_close_input(&m_pFormatCtx);

		ZeroMemory(&m_Packet, sizeof(m_Packet));
		m_pFrame		= NULL;
		m_pFrameRGB		= NULL;
		m_pCodecCtx		= NULL;
		m_pFormatCtx	= NULL;

		////���������
		m_ReadBuffer.consume(m_ReadBuffer.size());
		m_ioBuffer.consume(m_ioBuffer.size());
	}
	catch (...)
	{

	}
}

void CVedioDlg::StartVedio(CWnd *pCtrWnd, char* ipStr, int port)
{
	m_pCtrWnd			= pCtrWnd;
	m_curIP				= ipStr;
	m_curPort			= port;
	m_bRunning			= TRUE;

	Realease();
	m_bRunning			= TRUE;
	m_pThreadBeginVedio = m_WorkingThreadGroup.create_thread(std::bind(&CVedioDlg::BeginVedio, this));
}

BOOL CVedioDlg::IsRunning()
{
	return m_bRunning;
}

void CVedioDlg::Stop()
{
	try
	{
		m_bRunning = FALSE;
		if (m_ptrSocket->is_open())	m_ptrSocket->close();
		m_WorkingThreadGroup.join_all();
		m_WorkingThreadGroup.remove_thread(m_pThreadWorkThread);
	}
	catch (...)
	{
	}
}

void CVedioDlg::WaitForConnect()
{
	DWORD dwRet = 0;
	boost::timer tm;
	while (m_bRunning)
	{
		///��һ����������������������
		dwRet = WaitForSingleObject(m_hEvent, 0);
		if (dwRet == WAIT_OBJECT_0)
			break;

		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (tm.elapsed() > 10)//�Ҳ�����������cable����һ�¼�����
		{
			m_bRunning = FALSE;
			break;
		}

		Sleep(1);
	}
}

void CVedioDlg::SendTouchScreenMessage(boost::asio::const_buffer& Buffer)
{
	try
	{
		if (m_ptrSocket->is_open())
		{
			Send(Buffer);
		}
	}
	catch (...)
	{

	}
}

void CVedioDlg::SendInputMethodOperationMessage(boost::asio::const_buffer& Buffer)
{
	try
	{
		if (m_ptrSocket->is_open())
		{
			Send(Buffer);
		}
	}
	catch (...)
	{

	}
}