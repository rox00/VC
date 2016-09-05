// BoxFinder.cpp : CBoxFinder 的实现

#include "stdafx.h"
#include "BoxFinder.h"

// CBoxFinder

STDMETHODIMP CBoxFinder::GetCable(DWORD *nCableId, BSTR* bstrCableName)
{
	if (S_OK == CDeviceEnumerator::GetInstance().Refresh())
	{
		m_szCableName = CDeviceEnumerator::GetInstance().GetUnallocatedCable(nCableId);

		CComBSTR comStr(m_szCableName.c_str());
		*bstrCableName = comStr.Copy();
	}
	
	return S_OK;
}

STDMETHODIMP CBoxFinder::AttachDevice(BSTR lpszCableName, LONG hWnd)
{
	CDeviceEnumerator::GetInstance().WindowAttached(lpszCableName, (HWND)hWnd);
	m_attachHwnd = (HWND)hWnd;
	return S_OK;
}



STDMETHODIMP CBoxFinder::DetachDevice(BSTR lpszCableName, LONG hWnd)
{
	// TODO:  在此添加实现代码

	CDeviceEnumerator::GetInstance().WindowDetached(lpszCableName, (HWND)hWnd);

	return S_OK;
}
