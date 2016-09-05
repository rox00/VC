#include "..\stdafx.h"
#include "InputManager.h"
#include "../Channel/IMEAppProtocol.h"
#include <atlconv.h>
#include "../../Common/Common.h"

void InputManager::InitialInstance()
{

}

void InputManager::DisposeInstance()
{
	DetachScreenFunc();
	m_KeyNameTable.clear();
	InputHookHelper::GetInstance().DisposeInstance();
}

void InputManager::Initialize(HWND hAttachWnd)
{	
	DisposeInstance();

	if (m_KeyNameTable.empty())		//��һ��ʹ�õ�ʱ���ʼ��
	{
		m_KeyNameTable.insert(make_pair(VK_BACK,	"backspace"));
		m_KeyNameTable.insert(make_pair(VK_RETURN,	"enter"));
		m_KeyNameTable.insert(make_pair(VK_HOME,	"home"));
		m_KeyNameTable.insert(make_pair(VK_END,		"end"));
		m_KeyNameTable.insert(make_pair(VK_DELETE,	"delete"));
		m_KeyNameTable.insert(make_pair(VK_INSERT,	"insert"));
		m_KeyNameTable.insert(make_pair(VK_LEFT,	"left"));
		m_KeyNameTable.insert(make_pair(VK_UP,		"up"));
		m_KeyNameTable.insert(make_pair(VK_RIGHT,	"right"));
		m_KeyNameTable.insert(make_pair(VK_DOWN,	"down"));
		m_KeyNameTable.insert(make_pair(VK_F9,		"f9"));
		m_KeyNameTable.insert(make_pair(VK_TAB,		"tab"));
	}

	m_hAttachWnd = hAttachWnd;
	InputHookHelper::GetInstance().Start(hAttachWnd);	
	
	m_MouseOperationHandler.Initialize(hAttachWnd);
	InputHookHelper::GetInstance().AddMouseEventHandler(boost::bind(&InputManager::MouseActionHandler, this, _1, _2, _3, _4));
	InputHookHelper::GetInstance().AddMouseEventHandler(boost::bind(&MouseOperationHandler::MouseActionHandler, &m_MouseOperationHandler, _1, _2, _3, _4));	
	InputHookHelper::GetInstance().AddMouseEventHandler(boost::bind(&ScreenZoomHandler::MouseActionHandler, &m_ScreenZoomHandler, _1, _2, _3, _4));
			
	InputHookHelper::GetInstance().AddMouseEventListener(WM_MOUSELEAVE, boost::bind(&InputManager::OnMouseLeave, this, _1, _2));
	InputHookHelper::GetInstance().AddMouseEventListener(WM_MOUSELEAVE, boost::bind(&MouseOperationHandler::OnMouseLeave, &m_MouseOperationHandler, _1, _2));
	InputHookHelper::GetInstance().AddMouseEventListener(WM_MOUSELEAVE, boost::bind(&ScreenZoomHandler::OnMouseLeave, &m_ScreenZoomHandler, _1, _2));
	InputHookHelper::GetInstance().AddMouseEventListener(WM_MOUSEWHEEL, boost::bind(&ScreenZoomHandler::OnMouseWheel, &m_ScreenZoomHandler, _1, _2));
}

void InputManager::SendTouchMessage(int x, int y, TOUCHTYPE Action, UINT nFingerId, UINT nDelayTime)
{
	//���ݲ��Ϸ����������ϴ�
	if (x < 0 || y < 0)
		return;
	try
	{

		tagPACKAGEHEAD				packHead;
		packHead.head1				= 0xFF;
		packHead.head2				= 0xFE;
		packHead.packType			= PackTypeEnum_MOUSE;
		packHead.len				= sizeof(tagMOUSEPACKET);

		tagMOUSEPACKET				mousePacket;
		mousePacket.x				= x;
		mousePacket.y				= y;
		mousePacket.touchType		= Action;
		mousePacket.nDelayTime		= nDelayTime;
		mousePacket.nFingerId		= nFingerId;
		//Message TouchMsg;
		//TouchMsg.set_x(x);
		//TouchMsg.set_y(y);
		//TouchMsg.set_actiontype(Action);
		//TouchMsg.set_delaytime(nDelayTime);
		//TouchMsg.set_id(nFingerId);

		//RawData Data(TouchMsg);
		//auto&& Buffer = Data.ToAsioBuffer();	//r-value reference�����Ч��
		int		headSize	= sizeof(tagPACKAGEHEAD);
		int		msgSize		= sizeof(tagMOUSEPACKET);

		int		bufSize		= headSize + msgSize;
		char	*pBuf		= new char[bufSize];

		memcpy(pBuf, &packHead, headSize);
		memcpy(pBuf + headSize, &mousePacket, msgSize);
		GetTouchScreen().m_SendFunction(boost::asio::const_buffer(pBuf, bufSize));

		//if (x != 0  && y != 0)
		//	TRACE("-----------------send touch event x:%d y:%d action:%d id:%d\n", x, y, Action, nFingerId);
	}
	catch (exception ex)
	{
		
	}
	catch (...){
	}
}

void InputManager::SendKeyMessage(int keyValue, KEYACTIONTYPE Action)
{
	//���ݲ��Ϸ����������ϴ�
	
	try
	{

		tagPACKAGEHEAD				packHead;
		packHead.head1				= 0xFF;
		packHead.head2				= 0xFE;
		packHead.packType			= PackTypeEnum_KEYBOARD;
		packHead.len				= sizeof(tagKEYPACKET);

		tagKEYPACKET				keyPacket;
		keyPacket.key				= keyValue;
		keyPacket.keyAction			= Action;

		int		headSize			= sizeof(tagPACKAGEHEAD);
		int		msgSize				= sizeof(tagKEYPACKET);

		int		bufSize				= headSize + msgSize;
		char	*pBuf				= new char[bufSize];

		memcpy(pBuf, &packHead, headSize);
		memcpy(pBuf + headSize, &keyPacket, msgSize);
		GetInputMethodOperationDevice().m_SendFunction(boost::asio::const_buffer(pBuf, bufSize));

	}
	catch (exception ex)
	{

	}
	catch (...){
	}
}

BOOL InputManager::MouseActionHandler(int mouseButton, int x, int y, TOUCHTYPE Type)
{	
	boost::mutex::scoped_lock Lock(GetTouchScreen().m_Mutex);

	ClientToDeviceScreen(x, y);
	
	if(GetTouchScreen().m_SendFunction)
	{
		if (!GetInputMethodOperationDevice().m_MouseMappingFunction)
		{	////û��������ӳ�书�ܵĻ�����������¼�
			if (mouseButton == InputHookHelper::LEFT_MOUSE_BUTTON)	//������¡������϶����������ﴦ��				
			{
				SendTouchMessage(x, y, Type, TOUCH_ID_MOUSE_CLICK);
				return TRUE;	//��Ϣ��������ˣ�������ת��������TRUE
			}
		}
	}

	return FALSE;
}

void InputManager::OnMouseLeave(WPARAM, LPARAM)
{
	SendTouchMessage(0, 0, TOUCHTYPE_UP, TOUCH_ID_MOUSE_CLICK);
}

void InputManager::KeyActionHandler(UINT nVirtualKey, UINT nAction, UINT repeat)
{
	////ע�������ż������Դ�����ӳ����У������������ƣ�����д�������ZY��
	if (nAction == WM_KEYDOWN			&&
		repeat							&&
			nVirtualKey	!= VK_BACK			&&
			nVirtualKey	!= VK_LEFT			&&
			nVirtualKey	!= VK_RIGHT			&&
			nVirtualKey	!= VK_UP			&&
			nVirtualKey	!= VK_DOWN
			)
		{	
			return;
		}
	//{
	//	KeyActionProcess(nVirtualKey, nAction, repeat);
	//	return;
	//}

	{
		if (	GetInputMethodOperationDevice().m_IsDirectKeyFunc	&& 
				GetInputMethodOperationDevice().m_IsDirectKeyFunc(nVirtualKey)
			)
		{
			if (m_directKeyVec.size() > 6 && nAction == WM_KEYDOWN)
			{
				return;
			}

			boost::unique_lock<boost::shared_mutex> g(m_directKeyInfoMutex);
			tagKEYPACKET	*pInfo	= new tagKEYPACKET;
			pInfo->key				= nVirtualKey;
			pInfo->keyAction		= nAction == WM_KEYDOWN ? KEYACTION_DOWN : KEYACTION_UP;

			m_directKeyVec.push_back(boost::shared_ptr<tagKEYPACKET>(pInfo));
			m_condDirectGet.notify_one();
		}
		else
		{
			boost::unique_lock<boost::shared_mutex> g(m_normalKeyInfoMutex);
			tagKEYPACKET *pInfo		= new tagKEYPACKET;
			pInfo->key				= nVirtualKey;
			pInfo->keyAction		= nAction == WM_KEYDOWN ? KEYACTION_DOWN : KEYACTION_UP;

			m_normalKeytVec.push_back(boost::shared_ptr<tagKEYPACKET>(pInfo));
			m_condNormalGet.notify_one();
		}

		return;
	}

}

void InputManager::KeyActionProcess(UINT nVirtualKey, KEYACTIONTYPE nAction)
{

	bool  bHasMapping = false;		////�����Ƿ��Ѿ�����ӳ��
	if (GetInputMethodOperationDevice().m_MappingFunction && GetTouchScreen().m_SendFunction)
	{	//ֻ�е��û��ڽ����ϴ򿪰���ӳ�书�ܵ�ʱ�򣬲���Ҫ�������´���	
		auto&& TouchEvents = GetInputMethodOperationDevice().m_MappingFunction(nVirtualKey, nAction);
		tagMOUSEPACKET msg;
		int msgCnt = 0;
		for (auto iter = TouchEvents.begin(); iter != TouchEvents.end(); iter++, msgCnt++)
		{	//��һ������ӳ��Ϊһ����һϵ�д������������ҷ���ȥ			
			msg = (*iter);
			int x = msg.x;
			int y = msg.y;
			msg.x	= (x + msgCnt%3);
			msg.y	= (y + msgCnt%3);

			SendTouchMessage(x, y, (TOUCHTYPE)msg.touchType, msg.nFingerId, msg.nDelayTime);

			Common::SystemHelper::DelayMS(msg.nDelayTime);
			bHasMapping = true;
		}
	}

	if (nVirtualKey == VK_CAPITAL)
	{
		if (LOBYTE(GetKeyState(VK_CAPITAL)))
		{
			SendKeyMessage(KEYACTION_CAPSLOCKON, KEYACTION_FUNC);
		}
		else
		{
			SendKeyMessage(KEYACTION_CAPSLOCKOFF, KEYACTION_FUNC);
		}

	}
	else if (bHasMapping == false)
	{	//���û��ӳ�䣬�ٿ��Ƿ���һЩ�ض����ܼ�
		//auto iter = m_KeyNameTable.find(nVirtualKey);
		//if ((iter != m_KeyNameTable.end() && (nAction == WM_KEYDOWN) &&
		//	GetInputMethodOperationDevice().m_SendFunction))
		//{	//�ϱ�����������������µ�ʱ��Ҫ���͸�android��
		//	string&& strPacket = IMEAppProtocol::GetInstance().ToString(
		//		IMEAppProtocol::IMEAppMessage(IMEAppProtocol::EMV_OPER_INPUT_KEY, iter->second));
		//	GetInputMethodOperationDevice().m_SendFunction(boost::asio::buffer(strPacket.data(), strPacket.size()));
		//}
		SendKeyMessage(nVirtualKey, nAction);
	}
}

void InputManager::CharInputHandler(TCHAR InputChar)
{
	boost::mutex::scoped_lock Lock(GetInputMethodOperationDevice().m_Mutex);

	//TODO: �û������ڽ����ϴ򿪰���ӳ�书�ܣ�����򿪣����ﲻ��Ҫ����

	if(GetInputMethodOperationDevice().m_SendFunction)
	{
		//enter����backspace����Tab����ESC�����������ﴦ��
		if (InputChar == _T('\r') || InputChar == _T('\n') 
			|| InputChar == _T('\b') || InputChar == _T('\x1b') || InputChar == _T('\t'))
			return;

		CHAR CharBuffer[10] = { 0 };
		memcpy(CharBuffer, &InputChar, sizeof(InputChar));

#ifdef UNICODE
		//��unicode��ʽ�£�һ��WM_CHAR����ֱ������һ�����֣�תΪGBK��ʽ
		CW2A lpszDecode((WCHAR *)CharBuffer);
		strcpy_s(CharBuffer, lpszDecode);
#else
		//���������Ҫ��Unicode��ʽ����
		//��Ϊ�����ַ�ʽ�£���ʹ�����뷨�����������ģ�Ҳ�ǵ���WM_CHAR�����Ƚϼ�
		//������Ҫ����IME��ص�һϵ����Ϣ
		ASSERT(FALSE);
#endif

		//��r-value�������ڴ渴��
		string&& strPacket = IMEAppProtocol::GetInstance().ToString(
			IMEAppProtocol::IMEAppMessage(IMEAppProtocol::EMV_OPER_INPUT_CHAR, CharBuffer));

		GetInputMethodOperationDevice().m_SendFunction(boost::asio::buffer(strPacket.data(), strPacket.size()));
	}
}

void InputManager::ClearMapping()
{	//����mapping����ָͷ̧��
	for (int i = 0; i < 8; i++)
	{
		SendTouchMessage(0, 0, TOUCHTYPE_UP, i);
	}		
}

void InputManager::ResetMouseHandleStatus()
{
	m_MouseOperationHandler.Initialize(m_hAttachWnd);
}

void InputManager::AttachScreenFunc(std::function<void(boost::asio::const_buffer&)> funcTouch, std::function<void(boost::asio::const_buffer&)> funcInput)
{
	InputManager::GetInstance().GetTouchScreen().SetSendFunction(funcTouch);
	InputManager::GetInstance().GetInputMethodOperationDevice().SetSendFunction(funcInput);

	m_bEndKeyProc = false;
	m_threadGroup.create_thread(boost::bind(&InputManager::DirectKeyActionProc, this));
	m_threadGroup.create_thread(boost::bind(&InputManager::NormalKeyActionProc, this));
}

void InputManager::DetachScreenFunc()
{
	m_bEndKeyProc = true;
	m_condDirectGet.notify_all();			////���еȴ��̼߳���ִ��
	m_condNormalGet.notify_all();			////���еȴ��̼߳���ִ��
	m_threadGroup.join_all();

	InputManager::GetInstance().GetTouchScreen().SetSendFunction(static_cast<std::function<void(boost::asio::const_buffer&)>>(nullptr));
	InputManager::GetInstance().GetInputMethodOperationDevice().SetSendFunction(static_cast<std::function<void(boost::asio::const_buffer&)>>(nullptr));
}

void InputManager::DirectKeyActionProc()
{
	auto pIsGoOn = [&]()
	{
		return m_bEndKeyProc || (m_directKeyVec.size() != 0);
	};

	boost::shared_ptr<tagKEYPACKET> pInfo = NULL;
	do
	{
		////����������󲢵ȴ�֪ͨ��ʹ����һ���߳��쳣�ȴ���������֪ͨ���ֻ����һ�μ�ֻ����ȴ������е�֪ͨ���� ���pIsGoOn����TRUE�򲻱صȴ�, �ȴ��ɹ����������������ִ��
		m_condDirectGet.wait(m_directKeyInfoMutex, pIsGoOn);

		pInfo = NULL;
		if (m_directKeyVec.size())
		{
			pInfo = m_directKeyVec.front();
			m_directKeyVec.pop_front();
		}
		m_directKeyInfoMutex.unlock();

		if (pInfo)
		{
			KeyActionProcess(pInfo->key, (KEYACTIONTYPE)pInfo->keyAction);
		}

	} while (!m_bEndKeyProc);
}

void InputManager::NormalKeyActionProc()
{
	auto pIsGoOn = [&]()
	{
		return m_bEndKeyProc || (m_normalKeytVec.size() != 0);
	};

	boost::shared_ptr<tagKEYPACKET> pInfo = NULL;
	do
	{
		////����������󲢵ȴ�֪ͨ��ʹ����һ���߳��쳣�ȴ���������֪ͨ���ֻ����һ�μ�ֻ����ȴ������е�֪ͨ���� ���pIsGoOn����TRUE�򲻱صȴ�, �ȴ��ɹ����������������ִ��
		m_condNormalGet.wait(m_normalKeyInfoMutex, pIsGoOn);

		pInfo = NULL;
		if (m_normalKeytVec.size())
		{
			pInfo = m_normalKeytVec.front();
			m_normalKeytVec.pop_front();
		}
		m_normalKeyInfoMutex.unlock();

		if (pInfo)
		{
			KeyActionProcess(pInfo->key, (KEYACTIONTYPE)pInfo->keyAction);
		}

	} while (!m_bEndKeyProc);
}

InputManager::TouchScreenInputDevice& InputManager::GetTouchScreen()
{
	return m_TouchScreen;
}

InputManager::InputMethodOperationInputDevice& InputManager::GetInputMethodOperationDevice()
{
	return m_InputMethodOperation;
}

void InputManager::ClientToDeviceScreen(int&x, int&y)
{
	if (m_TouchScreen.m_MappingFunction) m_TouchScreen.m_MappingFunction(x, y);
}

void InputManager::SetDeviceScreenToClientFunc(std::function<void(int&, int&)> func)
{
	m_deviceScreenToClientFunc = func;
}

void InputManager::DeviceScreenToScreen(int&x, int&y)
{
	if (m_deviceScreenToClientFunc) m_deviceScreenToClientFunc(x, y);
}
