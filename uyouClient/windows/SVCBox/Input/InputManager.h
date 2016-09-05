#pragma once

#include "RawData.h"
#include "../Channel/SVCChannel.h"
#include "ScreenZoomHandler.h"
#include "MouseOperationHandler.h"
#include "InputHookHelper.h"

typedef enum
{
	TOUCH_ID_MOUSE_CLICK,
	TOUCH_ID_FIRE_MAPPING = TOUCH_ID_MOUSE_CLICK,		//�������һ�������������������������ģ�����һ����ָid����
	TOUCH_ID_ARRAW_KEY,
	TOUCH_ID_ZOOM_1,
	TOUCH_ID_ZOOM_2,
	TOUCH_ID_AIM_MAPPING,
	TOUCH_ID_KEY_MAPPING_BEGIN,
	TOUCH_ID_KEY_MAPPING_MAX = 9,						//finger id > 9����������Ϣ����Ч��
	TOUCH_ID_LEFT_KEY_MAPPING = TOUCH_ID_ZOOM_1,		//����ӳ���ʱ��һ�㲻��ͬʱ�������ţ�����һ����ָid����
	TOUCH_ID_RIGHT_KEY_MAPPING = TOUCH_ID_ZOOM_2
}TOUCH_FINGER_ID;

struct KeyActionInfo
{
	UINT	nVirtualKey;								//�����ֵ
	UINT	nAction;									//���Ĳ���
};

class InputManager : public SinglePattern
{
	DECLARE_SINGLETON(InputManager)
	friend class MouseOperationHandler;
public:
	//ģ�������mapping����������
	template<typename MAPPING_FUNC_TYPE> class InputDevice
	{
		friend class InputManager;
		friend class MouseOperationHandler;
		friend class ScreenZoomHandler;

	public:
		typedef MAPPING_FUNC_TYPE MappingFuncType;

	public:
		void										SetSendFunction(SVCChannel::SendFunction Function) 
		{
			boost::mutex::scoped_lock Lock(m_Mutex);
			m_SendFunction = Function;
		}

		//decoratorģʽ������ӳ�䣬Ҳ���Բ�ӳ��
		void										SetTouchMappingFunction(MAPPING_FUNC_TYPE Function) 
		{
			boost::mutex::scoped_lock Lock(m_Mutex);
			m_MappingFunction = Function;			
		}

		void										SetMouseMappingFunction(MAPPING_FUNC_TYPE function)
		{
			boost::mutex::scoped_lock Lock(m_Mutex);
			m_MouseMappingFunction = function;
		}
		
		void										SetIsDirectKeyFunction(std::function<bool(UINT&)> function)
		{
			boost::mutex::scoped_lock Lock(m_Mutex);
			m_IsDirectKeyFunc = function;
		}

	private:
		std::function<bool(UINT&)>					m_IsDirectKeyFunc;
		SVCChannel::SendFunction					m_SendFunction;
		MAPPING_FUNC_TYPE							m_MappingFunction;
		MAPPING_FUNC_TYPE							m_MouseMappingFunction;
		boost::mutex								m_Mutex;
	};	//InputDevice class

	typedef vector<TouchEvent::Message> TouchMessageVector;
	typedef InputDevice<std::function<void(int&, int&)>> TouchScreenInputDevice;
	typedef InputDevice<std::function<TouchMessageVector(UINT, UINT)>> InputMethodOperationInputDevice;

	std::function<void(int&, int&)>					m_deviceScreenToClientFunc;
public:

	void											SetDeviceScreenToClientFunc(std::function<void(int&, int&)> func);

	void											Initialize(HWND hAttachWnd);
	void											ResetMouseHandleStatus();	

	TouchScreenInputDevice&							GetTouchScreen();
	InputMethodOperationInputDevice&				GetInputMethodOperationDevice();
	void											ClientToDeviceScreen(int&x, int&y);		//�Ӵ�������ת��Ϊandroidϵͳ����
	void											DeviceScreenToScreen(int&x, int&y);		//��androidϵͳ����ת��Ϊ��������

protected: 
	virtual void									InitialInstance() { };
	virtual void									DisposeInstance() 
	{ 
		InputHookHelper::GetInstance().DisposeInstance();
	};

public:
	void											SendTouchMessage(int x, int y, TouchEvent::ActionType Action, UINT nFingerId, UINT nDelayTime = 0);
	BOOL											MouseActionHandler(int mouseEvent, int x, int y, TouchEvent::ActionType Type);
	void											CharInputHandler(TCHAR InputChar);
	void											KeyActionHandler(UINT nVirtualKey, UINT nAction, unsigned int repeat);		//nAction��WM_KEYDOWN��WM_KEYUP

	void											OnMouseLeave(WPARAM, LPARAM);	
	void											ClearMapping();

	void											AttachScreenFunc(SVCChannel::SendFunction funcTouch, SVCChannel::SendFunction funcInput);
	void											DetachScreenFunc();
	void											KeyActionProcess(UINT nVirtualKey, UINT nAction, UINT repeat);
	void											DirectKeyActionProc();
	void											NormalKeyActionProc();
private:
	TouchScreenInputDevice							m_TouchScreen;
	InputMethodOperationInputDevice					m_InputMethodOperation;	
	ScreenZoomHandler								m_ScreenZoomHandler;
	MouseOperationHandler							m_MouseOperationHandler;

	HWND											m_hAttachWnd;
	map<UINT, string>								m_KeyNameTable;	//һЩ�̶��ļ�λ��ֱ�ӷ���android��

	boost::thread_group								m_threadGroup;				////�߳�����
	bool											m_bEndKeyProc;				////�Ƿ��������������

	list<boost::shared_ptr<KeyActionInfo>>			m_directKeyVec;				////�����ݴ水����Ϣ���б�
	boost::shared_mutex								m_directKeyInfoMutex;		////����ͬ�����ʰ�����Ϣ�б�Ļ�����
	boost::condition_variable_any					m_condDirectGet;			////ͬ����ȡ������Ϣ����������

	list<boost::shared_ptr<KeyActionInfo>>			m_normalKeytVec;			////�����ݴ水����Ϣ���б�
	boost::shared_mutex								m_normalKeyInfoMutex;		////����ͬ�����ʰ�����Ϣ�б�Ļ�����
	boost::condition_variable_any					m_condNormalGet;			////ͬ����ȡ������Ϣ����������
};