
// CVedioDlg.h : 头文件
//

#pragma once
#include "stdafx.h"

#include<GdiPlus.h>
#pragma (lib,"gdiplus.lib")
using namespace Gdiplus;

extern "C"
{
	#include <libavutil/opt.h>
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
};

class CVedioDlg : public CDialogEx
{
// 构造
public:
	CVedioDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAINFRAME };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void									OnDestroy();
	afx_msg BOOL									OnEraseBkgnd(CDC* pDC);
	afx_msg void									OnSize(UINT nType, int cx, int cy);
	virtual LRESULT									WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	AVCodecContext									*m_aCodecCtx;  //声音相关的Codec上下文
	AVPacket										m_Packet;  //代表音视频数据帧，固有的属性是一些标记，时钟信息和压缩数据首地址，大小等信息

	//以下变量与视频图像相关
	AVFormatContext									*m_pFormatCtx;  //表示程序运行的当前文件容器格式使用的上下文，着重于所有文件容器共有的属性和关联其他结构的字段
	AVCodecContext									*m_pCodecCtx;  //AVCodecContext 表示程序运行的当前Codec使用的上下文,着重于所有Codec共有的属性和关联其他结构的字段
	AVFrame											*m_pFrame;
	AVFrame											*m_pFrameRGB;
	AVFrame											*m_pFrameYUV;

	int												m_dstNumBytes;
	int												m_videoIndex;
	uint8_t											*m_pDst;

	static boost::mutex								m_iobufMutex;
	static boost::asio::streambuf					m_ioBuffer;
	static BOOL										m_bRunning;
	static int										ReadIOBuffer(void * opaque, uint8_t *buf, int bufsize);
	static void										WirteIOBuffer(char *buf, int bufsize);

	HBITMAP											m_hbitmap;

	void											WorkThread();
	void											Receive();
	void											WriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void											ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void											BeginVedio();
	void											SaveFrameToImg();
	int												SaveFrameToBMP(char *pPicFile, uint8_t *pRGBBuffer, int nWidth, int nHeight, int nBitCount);
	void											Send(boost::asio::const_buffer& BufferToSend);

	boost::asio::streambuf							m_ReadBuffer;
	boost::mutex									m_sockMutex;				//asio::socket不是线程安全的，一把大锁解决问题
	boost::timer									heartTm;

	boost::thread									*m_pThreadBeginVedio;
	boost::thread									*m_pThreadWorkThread;

	boost::thread_group								m_WorkingThreadGroup;
	boost::asio::io_service							m_IoService;
	std::unique_ptr<boost::asio::ip::tcp::socket>	m_ptrSocket;
	void											Realease();

	HANDLE											m_hEvent;
	std::string										m_curIP;
	int												m_curPort;
	CWnd											*m_pCtrWnd;


public:
	void											StartVedio(CWnd *pCtrWnd, char* ipStr, int port);
	void											WaitForConnect();
	BOOL											IsRunning();
	void											Stop();
	void											SendTouchScreenMessage(boost::asio::const_buffer& Buffer);
	void											SendInputMethodOperationMessage(boost::asio::const_buffer& Buffer);
	HBITMAP											RotateBmp(bool bRotateLeft, BITMAPINFOHEADER bmpinfo, byte* pBmpData);
};
