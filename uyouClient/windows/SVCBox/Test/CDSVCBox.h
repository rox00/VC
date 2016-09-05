// CDSVCBox.h : 由 Microsoft Visual C++ 创建的 ActiveX 控件包装器类的声明

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDSVCBox

class CDSVCBox : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDSVCBox)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x9175A113, 0xDD75, 0x4E95, { 0x8A, 0x27, 0xD0, 0x91, 0x7A, 0xC9, 0xDE, 0x10 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:

// 操作
public:

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	unsigned long Start(unsigned long bAutoReconnect)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, bAutoReconnect);
		return result;
	}
	unsigned long SendOperateCmd(unsigned long ulCmdType, unsigned long ulPara1, unsigned long ulPara2, unsigned long ulPara3, LPCTSTR data)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4 VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, ulCmdType, ulPara1, ulPara2, ulPara3, data);
		return result;
	}
	unsigned long SVCDowndloadApp(LPCTSTR strUrl, LPCTSTR strExtend, unsigned long ulExtent)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_UI4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, strUrl, strExtend, ulExtent);
		return result;
	}
	unsigned long SetIMEStatus(unsigned long ulIMEOpen)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, ulIMEOpen);
		return result;
	}
	unsigned long GetIMEStatus()
	{
		unsigned long result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetScreenResolution(LPCTSTR resolution)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, resolution);
	}
	void ResetDisplayMode()
	{
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	unsigned long GetUpgradeInfo(LPCTSTR version)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, version);
		return result;
	}
	void GetDeviceScreenSize()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetSDCardSpace()
	{
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void openApk(LPCTSTR packageName, LPCTSTR activityName)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, packageName, activityName);
	}
	unsigned long UpdatePCVersion(LPCTSTR filePath)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, filePath);
		return result;
	}
	STDMETHOD(LoadKeyMapping)(LPCTSTR packageName)
	{
		HRESULT result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, packageName);
		return result;
	}
	unsigned long SetKeyMappingOpen(short open)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, open);
		return result;
	}
	unsigned long SaveKeyMappingConfig(LPCTSTR configFile, LPCTSTR content)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, configFile, content);
		return result;
	}
	unsigned long SetRunningApp(LPCTSTR packageName)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, packageName);
		return result;
	}
	unsigned long PushUpgradeFileToBox(LPCTSTR boxFile)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, boxFile);
		return result;
	}
	unsigned long InputChar(long value)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, value);
		return result;
	}
	unsigned long GetSystemDPI(long * dpiValue)
	{
		unsigned long result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, dpiValue);
		return result;
	}
	unsigned long GetWorkAreaRect(long * left, long * top, long * right, long * bottom)
	{
		unsigned long result;
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, left, top, right, bottom);
		return result;
	}
	unsigned long GetAndroidVolume(long * volume)
	{
		unsigned long result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, volume);
		return result;
	}
	unsigned long RebootBox()
	{
		unsigned long result;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	unsigned long OpenTaskManager()
	{
		unsigned long result;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	unsigned long CallAndroidBack()
	{
		unsigned long result;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	unsigned long CallAndroidHomeKey()
	{
		unsigned long result;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	unsigned long SetVolume(long value)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, value);
		return result;
	}
	unsigned long Exit()
	{
		unsigned long result;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void ClearTouchedKey()
	{
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	unsigned long InstallAPP(LPCTSTR fileName)
	{
		unsigned long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, fileName);
		return result;
	}


};
