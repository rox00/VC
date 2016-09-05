// BoxFinder.h : CBoxFinder ������

#pragma once
#include "resource.h"       // ������

#include "BoxManager_i.h"
#include "DeviceEnumerator.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CBoxFinder

class ATL_NO_VTABLE CBoxFinder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBoxFinder, &CLSID_BoxFinder>,
	public IConnectionPointContainerImpl<CBoxFinder>,
	public IDispatchImpl<IBoxFinder, &IID_IBoxFinder, &LIBID_BoxManagerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CBoxFinder()
	{
	}

	~CBoxFinder()
	{
		CDeviceEnumerator::GetInstance().WindowDetached(m_szCableName.c_str(), m_attachHwnd);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BOXFINDER)


BEGIN_COM_MAP(CBoxFinder)
	COM_INTERFACE_ENTRY(IBoxFinder)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CBoxFinder)
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(GetCable)(DWORD *nCableId, BSTR* bstrCableName);
	STDMETHOD(AttachDevice)(BSTR bstrCableId, LONG hWnd);
	STDMETHOD(DetachDevice)(BSTR lpszCableName, LONG hWnd);

private:
	String					m_szCableName;
	HWND					m_attachHwnd;
};

OBJECT_ENTRY_AUTO(__uuidof(BoxFinder), CBoxFinder)
