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
		Sleep(50);	////�������Ҫ��ϵͳ��ʱ���ڲ�һ���ܼ�¼��Ʒ״̬
		ReleaseMutex(rChannel.m_hConnectMutex);

		if (nLocalPort != 0)
		{
			SVCChannel::ConnectionPtr ptrConnection(new SVCConnection(rChannel));
			ptrConnection->m_nPort = nPort;
			ptrConnection->m_lpszReadDelimeter = lpszDelimeter;	//���ǻὨ���������ӣ�һ���Ƕ�����Э��ģ�һ�����ı�Э��ġ��ı�Э����Ҫ�ж������������Э��û�У������ΪNULL��

			//��boost::asioͬ���Ľ������ӣ�����ʧ�����׳��쳣
			ptrConnection->m_ptrSocket->connect(boost::asio::ip::tcp::endpoint(
				boost::asio::ip::address::from_string("127.0.0.1"), nLocalPort));

			//adb�����ԣ���ʹ��Android��X�˿�û���κγ����ڼ�����ֻҪadb forward�ɹ�������connect�ɹ�
			//�����Android��δ�����˶˿ڣ����������һ��read��write�����ͻ����
			//����������ʱ������׼ȷ���жϾ����ǲ���������ӳɹ���



			//�����µ��̣߳������첽����
			rChannel.m_WorkingThreadGroup.create_thread(std::bind(&SVCChannel::SVCConnection::WorkThread, ptrConnection));

			//�첽�Ĵ�socket�����ȡ
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

	//���������ر�socket������ʹ�����߳��˳����˳���connection��shared_ptr���ü������㣬�����Զ�delete
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
	{	//�ж�������ı�Э�飬��ȡ�������Ϊֹ
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
	{	//��������			
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

	//׼����һ�ζ���Ҫ��ͣ�Ķ�������ioservice::run���˳���
	Receive();
}

void SVCChannel::SVCConnection::WriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{	
	if(ec)
	{	//д������
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
		//TODO����д����Ĵ���
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
	//TODO: ����Ӧ��֪ͨ���߳��Ѿ��˳���	
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
	m_hConnectMutex = CreateMutex(NULL, FALSE, _T("{64A55AEE-09C0-46b4-83BE-3982EE65895B}"));//��֤��Ӧ��Ӧ�ó���Ψһ
}

SVCChannel::~SVCChannel()
{
	Destroy();
}

BOOL SVCChannel::Initialize(HWND hNotifyWnd, LPCTSTR lpcDeviceId)
{
	m_hNotifyWnd = hNotifyWnd;

	//�����BoxManager�����ˣ�adb�Ѿ��ҵ�Ŀ���豸��
	m_bExitInitThread = false;
	m_AdbUtil.SetCurrentSerialno(lpcDeviceId);

	//����adb�豸
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
