#pragma once

#include <types.h>
#include "../../Common/AdbUtils.h"

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
		static ConnectionPtr							Connect(SVCChannel& rChannel, UINT nPort, LPCSTR lpszReadDelimeter = NULL);
		void											Close();
		void											Send(boost::asio::const_buffer& Buffer);
		void											Receive();

	protected:	//handler
		void											ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);
		void											WriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);
		void											WorkThread();

	private:
		SVCChannel&										m_rChannel;
		UINT											m_nPort;
		LPCSTR											m_lpszReadDelimeter;
		std::unique_ptr<boost::asio::ip::tcp::socket>	m_ptrSocket;
		boost::asio::io_service							m_IoService;
		boost::asio::streambuf							m_ReadBuffer;
		boost::mutex									m_Mutex;				//asio::socket�����̰߳�ȫ�ģ�һ�Ѵ����������

	};

public:
	SVCChannel(LPCTSTR lpszExePath);
	~SVCChannel();	
public:
	BOOL Initialize(HWND hNotifyWnd, LPCTSTR lpcDeviceId);

	void SendTouchScreenMessage(boost::asio::const_buffer& Buffer) 
	{
		try
		{
			if (m_ptrTouchScreenEmulationConnection)
			{
				if (m_ptrTouchScreenEmulationConnection->m_ptrSocket->is_open())
				{
					m_ptrTouchScreenEmulationConnection->Send(Buffer);
				}
			}
		}
		catch (...)
		{
			
		}				
	}

	void SendInputMethodOperationMessage(boost::asio::const_buffer& Buffer)
	{
		try
		{
			if (m_ptrInputMethodOperationConnection)
			{
				if (m_ptrInputMethodOperationConnection->m_ptrSocket->is_open())
				{
					m_ptrInputMethodOperationConnection->Send(Buffer);
				}
			}
		}
		catch (...)
		{
			
		}							
	}

	void Destroy()
	{
		m_bExitInitThread = true;
		try
		{
			if (m_ptrTouchScreenEmulationConnection)
			{
				m_ptrTouchScreenEmulationConnection->Close();
				m_ptrTouchScreenEmulationConnection.reset();
			}
			if (m_ptrInputMethodOperationConnection)
			{
				m_ptrInputMethodOperationConnection->Close();
				m_ptrInputMethodOperationConnection.reset();
			}
		}
		catch (...){

		}
		//�ȴ������̣߳�Ӧ�����������˳�
		m_WorkingThreadGroup.join_all();
	}

protected:
	void									InitializeThread();
	BOOL									Connect();

	bool									m_bExitInitThread;										////�Ƿ��˳���ʼ���߳�
	boost::thread_group						m_WorkingThreadGroup;									////�߳���

	HANDLE									m_hConnectMutex;										//��ֹ���̼�ͬʱ������ͬ�Ķ˿ڡ�
	boost::shared_mutex						m_notifyReConnectLock;									////��������־�ķ��ʽ��м���
	BOOL									m_bNotifyReConnect;										////֪ͨ������־��TRUE ֪ͨ����  FALSE ��֪ͨ
	AdbUtils								m_AdbUtil;												////ADB������
	ConnectionPtr							m_ptrTouchScreenEmulationConnection;					////��ָ�������
	ConnectionPtr							m_ptrInputMethodOperationConnection;					////�����ַ��������
	HWND									m_hNotifyWnd;											////����֪ͨ�Ŀؼ�����

public:
	std::wstring							PushFileToDevice(LPCTSTR fileName, LPCTSTR destPath);
	BOOL									RebootDevice();
	BOOL									DeleteFile(LPCTSTR fileName);
	BOOL									InstallAPP(LPCTSTR fileName);
};