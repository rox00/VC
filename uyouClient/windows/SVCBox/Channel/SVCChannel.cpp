#include "stdafx.h"
#include "SVCChannel.h"
#include "IMEAppProtocol.h"

#define FIND_ADB_DEVICE_TIME_INTERVAL	50


#include "../../Common/common.h"

//////////////////////////////////////////////////////////////////////////
//// SVCChannel::SVCConnection
//////////////////////////////////////////////////////////////////////////

SVCChannel::SVCConnection::SVCConnection(SVCChannel& rChannel)
	: m_rChannel(rChannel)
{	
	m_ptrSocket.reset(new boost::asio::ip::tcp::socket(m_IoService));
}

SVCChannel::SVCConnection::~SVCConnection()
{
}

SVCChannel::ConnectionPtr SVCChannel::SVCConnection::Connect(SVCChannel& rChannel, UINT nPort, LPCSTR lpszDelimeter)
{	
	try
	{
		do
		{
			DWORD ret = WaitForSingleObject(rChannel.m_hConnectMutex, 10);

			OutputDebugString(_T("DWORD ret = WaitForSingleObject(rChannel.m_hConnectMutex, 10);"));
			if (ret == WAIT_OBJECT_0)
				break;
		} while (TRUE);
		UINT nLocalPort = rChannel.m_AdbUtil.ForwardPort(nPort);
		Sleep(50);	////这个必须要，系统短时间内不一定能记录商品状态
		ReleaseMutex(rChannel.m_hConnectMutex);

		if (nLocalPort != 0)
		{
			SVCChannel::ConnectionPtr ptrConnection(new SVCConnection(rChannel));
			ptrConnection->m_nPort = nPort;
			ptrConnection->m_lpszReadDelimeter = lpszDelimeter;	//我们会建立两个连接，一个是二进制协议的，一个是文本协议的。文本协议需要有定界符，二进制协议没有（定界符为NULL）

			//用boost::asio同步的建立连接，建立失败则抛出异常
			ptrConnection->m_ptrSocket->connect(boost::asio::ip::tcp::endpoint(
				boost::asio::ip::address::from_string("127.0.0.1"), nLocalPort));

			//adb的特性：即使在Android的X端口没有任何程序在监听，只要adb forward成功，都能connect成功
			//但如果Android并未监听此端口，后面随便做一个read或write操作就会出错
			//所以这里暂时还不能准确的判断究竟是不是真的连接成功了



			//启动新的线程，进行异步操作
			rChannel.m_WorkingThreadGroup.create_thread(std::bind(&SVCChannel::SVCConnection::WorkThread, ptrConnection));

			//异步的从socket里面读取
			ptrConnection->Receive();
			return ptrConnection;
		}
	}
	catch (...)
	{
		
	}		
	return NULL;
}

void SVCChannel::SVCConnection::Close()
{
	boost::mutex::scoped_lock Lock(m_Mutex);

	//这里主动关闭socket，才能使工作线程退出。退出后，connection的shared_ptr引用计数清零，才能自动delete
	try
	{
		m_ptrSocket->cancel();
		m_ptrSocket->close();
	}
	catch (...)
	{
	}
}

void SVCChannel::SVCConnection::Receive()
{
	boost::mutex::scoped_lock Lock(m_Mutex);

	if(m_lpszReadDelimeter != NULL)
	{	//有定界符，文本协议，读取到定界符为止
		boost::asio::async_read_until(*m_ptrSocket, m_ReadBuffer, m_lpszReadDelimeter, 
			boost::bind(&SVCChannel::SVCConnection::ReadHandler, shared_from_this(), 
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		boost::asio::async_read(*m_ptrSocket, m_ReadBuffer, 
			boost::bind(&SVCChannel::SVCConnection::ReadHandler, 
			shared_from_this(), boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred));
	}
}

void SVCChannel::SVCConnection::Send(boost::asio::const_buffer& BufferToSend)
{
	boost::mutex::scoped_lock Lock(m_Mutex);

	if (!m_ptrSocket || !m_ptrSocket->is_open())
	{
		TRACE("SVCChannel::SVCConnection::Send(boost::asio::const_buffer& BufferToSend) ret \n");					
		return;
	}
	m_ptrSocket->async_send(boost::asio::buffer(BufferToSend), boost::bind(&SVCChannel::SVCConnection::WriteHandler, 
		shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void SVCChannel::SVCConnection::ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{	
	if(ec)
	{	//读出错了			
		string str = ec.message();
		throw std::exception((__FUNCTION__" : " + ec.message()).c_str());
	}

	auto lpszReadData = boost::asio::buffer_cast<LPCSTR>(m_ReadBuffer.data());
	auto nDataSize = m_ReadBuffer.size();
	auto lpszEndPos = strstr(lpszReadData, JSON_DATA_END);
	if(lpszEndPos)
	{
		nDataSize = lpszEndPos - lpszReadData + JSON_DATA_END_SIZE;
	}
	
	string szBuffer = lpszReadData;			
	auto&& Message = IMEAppProtocol::GetInstance().FromString(szBuffer.substr(0, nDataSize));
	if(Message)	//valid message
	{
		std::string msg = Message->second.c_str();
		TRACE("%s\n", szBuffer.substr(0, nDataSize).c_str());
		SendMessage(m_rChannel.m_hNotifyWnd, WM_NOTIFY_DEVICE_MESSAGE_RECEIVED, Message->first, (LPARAM)Message->second.c_str());
	}

	m_ReadBuffer.consume(nDataSize);

	//准备下一次读，要不停的读，否则ioservice::run就退出了
	Receive();
}

void SVCChannel::SVCConnection::WriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{	
	if(ec)
	{	//写出错了
		std::wstring message = CA2W(ec.message().c_str());
		Common::LogHelper::log(_T("SVCChannel::SVCConnection::WriteHandler"), L4CP_INFO_LOG_LEVEL,  message.c_str());
		TRACE("write error %s", ec.message().c_str());
		throw std::exception((__FUNCTION__" : " + ec.message()).c_str());
	}
}

void SVCChannel::SVCConnection::WorkThread()
{
	try
	{
		m_IoService.run();
	}
	catch(std::exception &e)
	{	
		TRACE("Working Thread Raised an Exception : %s\n", e.what());
		//TODO：读写出错的处理
		m_rChannel.m_notifyReConnectLock.lock();
		if (m_rChannel.m_bNotifyReConnect)
		{
			m_rChannel.m_bNotifyReConnect = FALSE;
			if (m_rChannel.m_ptrInputMethodOperationConnection)
			{
				if (m_rChannel.m_ptrInputMethodOperationConnection->m_ptrSocket->is_open())
					m_rChannel.m_ptrInputMethodOperationConnection->Close();
			}

			if (m_rChannel.m_ptrTouchScreenEmulationConnection)
			{
				if (m_rChannel.m_ptrTouchScreenEmulationConnection->m_ptrSocket->is_open())
					m_rChannel.m_ptrTouchScreenEmulationConnection->Close();
			}

			::PostMessage(m_rChannel.m_hNotifyWnd, WM_NOTIFY_CONNECT_STATE_CHANGED, ConnectStatus::DISCONNECT, 0);
		}
		m_rChannel.m_notifyReConnectLock.unlock();
	}
	TRACE("io_service working on port %u has finished\n", m_nPort);
	//TODO: 这里应该通知主线程已经退出了	
}

//////////////////////////////////////////////////////////////////////////
//// SVCChannel
//////////////////////////////////////////////////////////////////////////

SVCChannel::SVCChannel(LPCTSTR lpszExePath)
	: m_AdbUtil(((String)lpszExePath + ADB_EXE_RELATIVE_PATH).c_str())
	, m_bNotifyReConnect(FALSE)
	, m_bExitInitThread(FALSE)
	, m_hNotifyWnd(NULL)
{
	m_hConnectMutex = CreateMutex(NULL, FALSE, _T("{64A55AEE-09C0-46b4-83BE-3982EE65895B}"));//保证对应的应用程序唯一
}

SVCChannel::~SVCChannel()
{
	Destroy();
}

BOOL SVCChannel::Initialize(HWND hNotifyWnd, LPCTSTR lpcDeviceId)
{
	m_hNotifyWnd = hNotifyWnd;

	//如果有BoxManager，至此，adb已经找到目标设备了
	m_bExitInitThread = false;
	m_AdbUtil.SetCurrentSerialno(lpcDeviceId);

	//连接adb设备
	BOOL bRet = FALSE;
	try
	{
		bRet = Connect();
	}
	catch (...)
	{
	}

	return bRet;
}

BOOL SVCChannel::Connect()
{	
	try
	{
		if (m_ptrInputMethodOperationConnection)
		{
			if (m_ptrInputMethodOperationConnection->m_ptrSocket->is_open())
				m_ptrInputMethodOperationConnection->Close();

			m_ptrInputMethodOperationConnection.reset();
		}
		
		if (m_ptrTouchScreenEmulationConnection)
		{
			if (m_ptrTouchScreenEmulationConnection->m_ptrSocket->is_open())
				m_ptrTouchScreenEmulationConnection->Close();

			m_ptrTouchScreenEmulationConnection.reset();
		}

		m_notifyReConnectLock.lock();
		m_ptrTouchScreenEmulationConnection = SVCConnection::Connect(*this, TOUCH_SCREEN_EMULATION_PORT);
		m_ptrInputMethodOperationConnection = SVCConnection::Connect(*this, INPUT_METHOD_OPERATION_PORT, JSON_DATA_END);
		m_bNotifyReConnect					= TRUE;
		m_notifyReConnectLock.unlock();
	}
	catch (...)
	{
		return FALSE;
	}		

	if (m_ptrInputMethodOperationConnection == NULL || m_ptrTouchScreenEmulationConnection == NULL)
		return FALSE;

	return TRUE;
}

std::wstring SVCChannel::PushFileToDevice(LPCTSTR fileName, LPCTSTR destPath)
{
	return m_AdbUtil.PushFile(fileName, destPath);
}

BOOL SVCChannel::RebootDevice()
{
	return m_AdbUtil.RebootDevice();
}

BOOL SVCChannel::DeleteFile(LPCTSTR fileName)
{
	return m_AdbUtil.DeleteFile(fileName);
}

BOOL SVCChannel::InstallAPP(LPCTSTR fileName)
{
	return m_AdbUtil.InstallApk(fileName);
}
