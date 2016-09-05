#pragma once

#include "RawData.h"
#include "../Channel/SVCChannel.h"
#include "ScreenZoomHandler.h"
#include "MouseOperationHandler.h"
#include "InputHookHelper.h"

typedef enum
{
	TOUCH_ID_MOUSE_CLICK,
	TOUCH_ID_FIRE_MAPPING = TOUCH_ID_MOUSE_CLICK,		//开火键和一般的鼠标点击都是用鼠标左键操作的，共享一个手指id即可
	TOUCH_ID_ARRAW_KEY,
	TOUCH_ID_ZOOM_1,
	TOUCH_ID_ZOOM_2,
	TOUCH_ID_AIM_MAPPING,
	TOUCH_ID_KEY_MAPPING_BEGIN,
	TOUCH_ID_KEY_MAPPING_MAX = 9,						//finger id > 9，发出的消息是无效的
	TOUCH_ID_LEFT_KEY_MAPPING = TOUCH_ID_ZOOM_1,		//键盘映射的时候一般不会同时进行缩放，共享一个手指id即可
	TOUCH_ID_RIGHT_KEY_MAPPING = TOUCH_ID_ZOOM_2
}TOUCH_FINGER_ID;

struct KeyActionInfo
{
	UINT	nVirtualKey;								//虚拟键值
	UINT	nAction;									//键的操作
};

class InputManager : public SinglePattern
{
	DECLARE_SINGLETON(InputManager)
	friend class MouseOperationHandler;
public:
	//模版参数是mapping函数的类型
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

		//decorator模式，可以映射，也可以不映射
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
	void											ClientToDeviceScreen(int&x, int&y);		//从窗口坐标转换为android系统坐标
	void											DeviceScreenToScreen(int&x, int&y);		//从android系统坐标转换为窗口坐标

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
	void											KeyActionHandler(UINT nVirtualKey, UINT nAction, unsigned int repeat);		//nAction：WM_KEYDOWN或WM_KEYUP

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
	map<UINT, string>								m_KeyNameTable;	//一些固定的键位，直接发给android端

	boost::thread_group								m_threadGroup;				////线程容器
	bool											m_bEndKeyProc;				////是否结束按键处理函数

	list<boost::shared_ptr<KeyActionInfo>>			m_directKeyVec;				////用于暂存按键信息的列表
	boost::shared_mutex								m_directKeyInfoMutex;		////用于同步访问按键信息列表的互斥量
	boost::condition_variable_any					m_condDirectGet;			////同步获取按键信息的条件变量

	list<boost::shared_ptr<KeyActionInfo>>			m_normalKeytVec;			////用于暂存按键信息的列表
	boost::shared_mutex								m_normalKeyInfoMutex;		////用于同步访问按键信息列表的互斥量
	boost::condition_variable_any					m_condNormalGet;			////同步获取按键信息的条件变量
};