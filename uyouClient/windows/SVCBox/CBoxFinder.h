// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装器类

#ifdef _DEBUG
	#import "..\\output\\BoxManagerD.exe" no_namespace
#else
	#import "..\\output\\BoxManager.exe" no_namespace
#endif
// CBoxFinder 包装器类

class CBoxFinder : public COleDispatchDriver
{
public:
	CBoxFinder(){} // 调用 COleDispatchDriver 默认构造函数
	CBoxFinder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBoxFinder(const CBoxFinder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IBoxFinder 方法
public:
	CString GetCable(DWORD *nCableId)
	{
		static BYTE parms[] = VTS_PUI4;
		CString result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nCableId);
		return result;
	}
	void AttachDevice(LPCTSTR lpszCableName, long hWnd)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszCableName, hWnd);
	}
	void DetachDevice(LPCTSTR lpszCableName, long hWnd)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszCableName, hWnd);
	}
};
