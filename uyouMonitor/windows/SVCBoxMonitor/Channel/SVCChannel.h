#pragma once

#include <types.h>
#include "../../Common/AdbUtils.h"
#include "../../Common/NetUtils.h"

#define TOUCH_SCREEN_EMULATION_PORT	17890
#define INPUT_METHOD_OPERATION_PORT		25556

class SVCChannel
{
public:
	typedef std::function<void(boost::asio::const_buffer&)> SendFunction;

private:
	class SVCConnection;
	typedef boost::shared_ptr<SVCConnection> ConnectionPtr;

private:
	class SVCConnection : public boost::enable_shared_from_this<SVCConnection>
	{
		friend class SVCChannel;

	public:
		SVCConnection(SVCChannel& rChannel);
		~SVCConnection();

	public:
		bool											Connect(UINT nPort, LPCSTR lpszReadDelimeter = NULL);
		void											Send(boost::asio::const_buffer& Buffer);
		void											Receive();

	protected:	//handler
		void											ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);
		void											WriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);
		void											WorkThread();
		void											HeartPacket();
	private:
		SVCChannel&										m_rChannel;
		UINT											m_nPort;
		LPCSTR											m_lpszReadDelimeter;
		std::unique_ptr<boost::asio::ip::tcp::socket>	m_ptrSocket;
		boost::asio::io_service							m_IoService;
		boost::asio::streambuf							m_ReadBuffer;
		boost::mutex									m_Mutex;				//asio::socket不是线程安全的，一把大锁解决问题
		boost::thread									*m_pThreadWork;

		boost::timer									heartTm;
	};

public:
	SVCChannel(LPCTSTR lpszExePath);
	~SVCChannel();	
public:
	BOOL Initialize(HWND hNotifyWnd, LPCTSTR lpcDeviceId);

	//void SendTouchScreenMessage(boost::asio::const_buffer& Buffer) 
	//{
	//	try
	//	{
	//		if (m_ptrTouchScreenEmulationConnection)
	//		{
	//			if (m_ptrTouchScreenEmulationConnection->m_ptrSocket->is_open())
	//			{
	//				m_ptrTouchScreenEmulationConnection->Send(Buffer);
	//			}
	//		}
	//	}
	//	catch (...)
	//	{
	//		
	//	}				
	//}

	//void SendInputMethodOperationMessage(boost::asio::const_buffer& Buffer)
	//{
	//	try
	//	{
	//		if (m_ptrInputMethodOperationConnection)
	//		{
	//			if (m_ptrInputMethodOperationConnection->m_ptrSocket->is_open())
	//			{
	//				m_ptrInputMethodOperationConnection->Send(Buffer);
	//			}
	//		}
	//	}
	//	catch (...)
	//	{
	//		
	//	}							
	//}

protected:
	void									InitializeThread();
	BOOL									Connect();

	bool									m_bExitInitThread;										////是否退出初始化线程
	boost::thread_group						m_WorkingThreadGroup;									////线程组

	IUtils									*m_pIUtil;
	//AdbUtils								m_AdbUtil;												////ADB操作类
	ConnectionPtr							m_ptrTouchScreenEmulationConnection;					////手指点击操作
	ConnectionPtr							m_ptrInputMethodOperationConnection;					////按键字符输入操作
	HWND									m_hNotifyWnd;											////接收通知的控件窗口

public:
	std::wstring							PushFileToDevice(LPCTSTR fileName, LPCTSTR destPath);
	BOOL									RebootDevice();
	BOOL									DeleteFile(LPCTSTR fileName);
	BOOL									InstallAPP(LPCTSTR fileName);
	void									Stop();
};