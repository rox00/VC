// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ����

#ifdef _DEBUG
	#import "..\\output\\BoxManagerD.exe" no_namespace
#else
	#import "..\\output\\BoxManager.exe" no_namespace
#endif
// CBoxFinder ��װ����

class CBoxFinder : public COleDispatchDriver
{
public:
	CBoxFinder(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CBoxFinder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBoxFinder(const CBoxFinder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IBoxFinder ����
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
