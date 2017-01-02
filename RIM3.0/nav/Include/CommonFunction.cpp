#include "StdAfx.h"
#include "CommonFunction.h"
#include <tchar.h>

#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "Iphlpapi.lib")

COLORREF	g_transAlphaClr = RGB(255, 255, 0);
HBRUSH		g_hTransAlphaBrush = CreateSolidBrush(g_transAlphaClr);






DWORD GetVersion(LPCTSTR lpszDllName)
{
	HINSTANCE hinstDll;
	DWORD dwVersion = 0;

	/* For security purposes, LoadLibrary should be provided with a fully qualified
	path to the DLL. The lpszDllName variable should be tested to ensure that it
	is a fully qualified path before it is used. */
	hinstDll = LoadLibrary(lpszDllName);

	if (hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

		/* Because some DLLs might not implement this function, you must test for
		it explicitly. Depending on the particular DLL, the lack of a DllGetVersion
		function can be a useful indicator of the version. */

		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;

			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);

			hr = (*pDllGetVersion)(&dvi);

			if (SUCCEEDED(hr))
			{
				dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
			}
		}
		FreeLibrary(hinstDll);
	}
	return dwVersion;
}

void MemPaintPng(const CDC &memDC, const CRect &rcClient, Image* pImage, float Alpha0_1/* = 1.0*/)
{
	if(pImage == NULL)
		return;

	Graphics graphics(memDC.GetSafeHdc());

	ImageAttributes imageattributes;

	ColorMatrix colormatrix={

		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	colormatrix.m[3][3] = Alpha0_1;

	imageattributes.SetColorMatrix(&colormatrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
	graphics.SetInterpolationMode(InterpolationModeHighQuality);

	CStringW	imagePathW	= CStringW(_T(""));
	UINT		iWidth		= 0;
	UINT		iHeight		= 0;
	Rect		desRect		= Rect(0, 0, 0, 0);
	Rect		sourceRect = Rect(0, 0, 0, 0);

	iWidth		= pImage->GetWidth();
	iHeight		= pImage->GetHeight();
	desRect		= Rect(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());	//填充
	sourceRect	= Rect(0, 0, pImage->GetWidth(), pImage->GetHeight());
	graphics.DrawImage(pImage,desRect,sourceRect.X,sourceRect.Y,sourceRect.Width,sourceRect.Height, UnitPixel, &imageattributes);

	graphics.ReleaseHDC(memDC.GetSafeHdc());
}

void MemDrawText(const CDC &memDC, const CRect &rcClient, const CString &textStr, const DWORD &mode)
{
	CRect rect = rcClient;
	::DrawText(memDC.GetSafeHdc(), textStr, textStr.GetLength(), rect, mode);
}

HRGN AlphaImageToRgn(Image* pImage, BYTE offAlphaRange /*= 0*/)
{
	if (pImage == NULL)
		return NULL;

	HRGN rTemp, hGoal;

	Bitmap *pBmpMain = (Bitmap*)pImage;
	Rect rcBmp;
	rcBmp.X = 0;
	rcBmp.Y = 0;
	rcBmp.Width = pBmpMain->GetWidth();
	rcBmp.Height = pBmpMain->GetHeight();

	hGoal = CreateRectRgn(0, 0, rcBmp.Width, rcBmp.Height);
	BitmapData bmData = { 0 };
	Status sts = pBmpMain->LockBits(&rcBmp, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bmData);
	if (sts == Ok)
	{
		for (UINT y = 0; y < bmData.Height; y++)
		{
			LPDWORD pData = LPDWORD(LPBYTE(bmData.Scan0) + bmData.Stride * y);
			for (UINT x = 0; x < bmData.Width; x++)
			{
				if (HIBYTE(HIWORD(pData[x])) <= offAlphaRange)
				{
					rTemp = CreateRectRgn(x, y, x + 1, y + 1);
					CombineRgn(hGoal, hGoal, rTemp, RGN_XOR);
					DeleteObject(rTemp);
				}
			}
		}
		pBmpMain->UnlockBits(&bmData);
	}

	return hGoal;
}

BOOL	IsDicExist(LPCTSTR lpPath)
{
	if(lpPath == NULL)
		return FALSE;

	DWORD dwAttrs = GetFileAttributes(lpPath);
	if (dwAttrs == INVALID_FILE_ATTRIBUTES)
		return FALSE;

	if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	return FALSE;
}

BOOL	IsFileExist(LPCTSTR lpFile)
{
	if(lpFile == NULL)
		return FALSE;

	DWORD dwAttrs = GetFileAttributes(lpFile);
	if (dwAttrs == INVALID_FILE_ATTRIBUTES)
		return FALSE;

	if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
		return FALSE;

	return TRUE;
}

DWORD64 GetFolderSize(LPCTSTR szPath, DWORD *dwFiles/* = NULL*/, DWORD *dwFolders/* = NULL*/)
{
	TCHAR			szFileFilter[MAX_PATH]	= { 0 };
	TCHAR			szFilePath[MAX_PATH]	= { 0 };
	HANDLE			hFind = NULL; 
	WIN32_FIND_DATA fileinfo; 
	DWORD64			dwSize = 0;

	_tcscpy_s(szFilePath, MAX_PATH, szPath);
	_tcscat_s(szFilePath, MAX_PATH, _T("//"));
	_tcscpy_s(szFileFilter, MAX_PATH, szFilePath);
	_tcscat_s(szFileFilter, MAX_PATH, _T("*.*"));

	hFind = FindFirstFile(szFileFilter, &fileinfo); do {
		if (fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    {
			if (!_tcscmp(fileinfo.cFileName, _T(".")) || !_tcscmp(fileinfo.cFileName, _T("..")))
			{
				//Do nothing for "." and ".." folders     
			}
			else
			{
				TCHAR sztmp[MAX_PATH] = { 0 };
				_tcscpy_s(sztmp, MAX_PATH, szFilePath);
				_tcscat_s(sztmp, MAX_PATH, fileinfo.cFileName);
				dwSize = dwSize + GetFolderSize(sztmp);
				if (dwFolders != NULL)
				{
					++(*dwFolders);
				}
			}
		}
		else
		{
			if (dwFiles != NULL)
			{
				++(*dwFiles);
			}
		}

		dwSize += fileinfo.nFileSizeLow;

	} while (FindNextFile(hFind, &fileinfo));

	FindClose(hFind); 
	return dwSize;
}

BOOL GetMd5(tstring FileDirectory, tstring &strFileMd5)
{
	HANDLE hFile = CreateFile(FileDirectory.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)                                        //如果CreateFile调用失败  
	{
		//提示CreateFile调用失败，并输出错误号。visual studio中可在“工具”>“错误查找”中利用错误号得到错误信息。  
		CloseHandle(hFile);
		return FALSE;
	}
	HCRYPTPROV hProv = NULL;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)       //获得CSP中一个密钥容器的句柄
	{
		return FALSE;
	}
	HCRYPTPROV hHash = NULL;
	//初始化对数据流的hash，创建并返回一个与CSP的hash对象相关的句柄。这个句柄接下来将被    CryptHashData调用。
	if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)
	{
		return FALSE;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);    //获取文件的大小
	if (dwFileSize == 0xFFFFFFFF)               //如果获取文件大小失败  
	{
		return FALSE;
	}
	byte* lpReadFileBuffer = new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile, lpReadFileBuffer, dwFileSize, &lpReadNumberOfBytes, NULL) == 0)        //读取文件  
	{
		return FALSE;
	}
	if (CryptHashData(hHash, lpReadFileBuffer, lpReadNumberOfBytes, 0) == FALSE)      //hash文件  
	{
		return FALSE;
	}
	delete[] lpReadFileBuffer;
	CloseHandle(hFile);          //关闭文件句柄
	BYTE *pbHash;
	DWORD dwHashLen = sizeof(DWORD);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0)) //我也不知道为什么要先这样调用CryptGetHashParam，这块是参照的msdn       
	{
		return FALSE;
	}
	pbHash = (byte*)malloc(dwHashLen);
	if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))//获得md5值 
	{
		for (DWORD i = 0; i < dwHashLen; i++)         //输出md5值 
		{
			TCHAR str[3] = { 0 };
			tstring strFilePartM = _T("");
			_stprintf_s(str, 3, _T("%02x"), pbHash[i]);
			strFileMd5 += str;
		}
	}

	if (pbHash)
		free(pbHash);

	//善后工作
	if (CryptDestroyHash(hHash) == FALSE)          //销毁hash对象  
	{
		return FALSE;
	}
	if (CryptReleaseContext(hProv, 0) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

tstring ReadRegKeyStr(HKEY hKey, tstring subKey, tstring valueName)
{
	tstring readStr		= _T("");
	TCHAR ch[MAX_PATH]	= {0};
	DWORD len			= MAX_PATH;
	HKEY hNewKey		= NULL;
	if(ERROR_SUCCESS == RegOpenKeyEx(hKey, subKey.c_str(), 0, KEY_READ, &hNewKey))//从注册表中读取版本号
	{
		RegQueryValueEx(hNewKey, valueName.c_str(), NULL, NULL, (LPBYTE)ch, &len);
		readStr = len == 0?_T(""):ch;
		RegCloseKey(hKey);
	}
	else
		readStr = _T("");

	return readStr;
}

LONG ReadRegKeyDWord(HKEY hKey, tstring subKey, tstring valueName, DWORD& regValue)
{
	HKEY	hNewKey		= NULL;

  	DWORD	dwType		= 0;
  	DWORD	dwBufLen	= sizeof(DWORD);
 
 	LONG lRet = RegOpenKeyEx(hKey, subKey.c_str(), 0, KEY_READ, &hNewKey);
  	if(ERROR_SUCCESS == lRet)//从注册表中读取版本号
  	{	
  		lRet = RegQueryValueEx(hNewKey, valueName.c_str(), NULL, &dwType, (LPBYTE)&regValue, &dwBufLen);
  		if(lRet == ERROR_SUCCESS)
  		{
  			if(dwType != REG_DWORD)
  				lRet = ERROR_BAD_FORMAT;
  		}
		RegCloseKey(hNewKey);
		return lRet;
  	}

 	return lRet;
 }

void WriteRegKeyStr(HKEY hKey, tstring subKey, tstring valueName, tstring str)
{

	HKEY	hNewKey		= NULL;
	TCHAR	cstr[100]	= {0};
	long	byte		= 0;
	if(ERROR_SUCCESS == RegCreateKey(hKey, subKey.c_str(), &hNewKey))
	{
		RegSetValueEx(hNewKey, valueName.c_str(), 0, REG_SZ, (LPBYTE)str.c_str(), str.size()*sizeof(TCHAR));
		RegCloseKey(hNewKey);
	}
}

void WriteRegKeyDWord(HKEY hKey, tstring subKey, tstring valueName, DWORD dwRegValue)
{
	HKEY	hNewKey		= NULL;
	TCHAR	cstr[100]	= {0};
	long	byte		= 0;
	if(ERROR_SUCCESS == RegCreateKey(hKey, subKey.c_str(), &hNewKey))
	{
		RegSetValueEx(hNewKey, valueName.c_str(), NULL, REG_DWORD, LPBYTE(&dwRegValue), sizeof(DWORD));
		RegCloseKey(hNewKey);
	}
}

void SetAppAutoRun( BOOL bAutoRun )
{
	HMODULE hModule = GetModuleHandle(NULL);
	ASSERT(hModule);

	TCHAR tcPath[_MAX_PATH];
	ZeroMemory(tcPath, _MAX_PATH);
	DWORD dwRet = GetModuleFileName(hModule, tcPath, _MAX_PATH);

	if(dwRet == 0)
		return;

	CString strAppPath = tcPath;

	HKEY hKey = NULL; 
	if(bAutoRun)
	{
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE|KEY_READ, &hKey) == ERROR_SUCCESS) 
		{ 
			DWORD cbData = (strAppPath.GetLength()+1)*2; 
			RegSetValueEx	(hKey, _T("RIM3.0"), 0, REG_SZ, (LPBYTE)strAppPath.GetBuffer(0), cbData);

			strAppPath.ReleaseBuffer();
		}
	}
	else
	{
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE|KEY_READ, &hKey) == ERROR_SUCCESS) 
		{
			RegDeleteValue(hKey, _T("RIM3.0"));
		}
	}	

	if(hKey)
		RegCloseKey(hKey); 
}


BOOL IsAppAutoRun()
{
	HMODULE hModule = GetModuleHandle(NULL);
	ASSERT(hModule);

	TCHAR tcPath[_MAX_PATH];
	ZeroMemory(tcPath, _MAX_PATH);
	DWORD dwRet = GetModuleFileName(hModule, tcPath, _MAX_PATH);

	if(dwRet == 0)
		return FALSE;

	CString strAppPath = tcPath;

	CString str;
	HKEY hKey; 
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE|KEY_READ, &hKey) == ERROR_SUCCESS) 
	{ 
		TCHAR *lpData = new TCHAR[512]; 
		ZeroMemory(lpData, 512 * sizeof(TCHAR));
		DWORD cbData = 512 * sizeof(TCHAR); 
		DWORD dwType = REG_SZ;

		if(RegQueryValueEx(hKey, _T("RIM3.0"), 0, &dwType, (LPBYTE)lpData, &cbData) == ERROR_SUCCESS) 
		{ 
			str = CString(lpData); //将lpData 转换成CString 类型 
		} 

		delete [] lpData; 
	} 

	RegCloseKey(hKey); 

	if(strAppPath.CompareNoCase(str) == 0)  //不要比较大小写
		return TRUE;
	else
		return FALSE;
}

void SetRoundRgnWnd(HWND hWnd, int rX, int rY)
{
	if (hWnd)
	{
		RECT cltRect;
		::GetClientRect(hWnd, &cltRect);

		HRGN hRgn = CreateRoundRectRgn(cltRect.left, cltRect.top, cltRect.right, cltRect.bottom, rX, rY);

		SetWindowRgn(hWnd, hRgn, TRUE);
	}
}

tstring RelativeToFullPath(const tstring &strRelativePath)
{
	TCHAR szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	TCHAR *pChar = _tcsrchr(szPath, _T('\\'));
	if(pChar)
		pChar[0] = _T('\0');
	tstring strPath = szPath;
	strPath += _T("\\");
	strPath += strRelativePath;

	_tfullpath(szPath, strPath.c_str(), MAX_PATH);
	return szPath;
}

tstring FullToRelativePath(tstring relativeDic,tstring strFullPath)
{
	TCHAR relative[MAX_PATH] = {0};
	PathRelativePathTo(relative, relativeDic.c_str(),FILE_ATTRIBUTE_DIRECTORY, strFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return relative;
}

bool GetAvalibleIpAddress(vector<tstring> &ipArray)
{
	ipArray.clear();

	// 	CStringArray ipAddresses;

	ULONG ulTalbeSize = 0;
	DWORD res = ::GetIpAddrTable(NULL, &ulTalbeSize, TRUE);
	MIB_IPADDRTABLE *pIpAddTalbe = (MIB_IPADDRTABLE*)new BYTE[ulTalbeSize];
	DWORD ret = ::GetIpAddrTable(pIpAddTalbe, &ulTalbeSize, TRUE);
	if(NO_ERROR == ret)
	{
		DWORD ipAddress = 0;
		for(UINT i = 0; i < pIpAddTalbe->dwNumEntries; i++)
		{
			ipAddress = pIpAddTalbe->table[i].dwAddr;
			if(0 == ipAddress)
				continue;

			// 			ipAddresses.Add(Dword2IpString(pIpAddTalbe->table[i].dwAddr));
			// MIB_IPADDRROW has a dwBCastAddr member but docs say it is broken (and it is!)
			// addr | ~mask = broadcast
			//ipAddress |= ~(pIpAddTalbe->table[i].dwMask);

			ipAddress = ntohl(ipAddress);

			TCHAR ch[MAX_PATH] = {0};
			_stprintf_s(ch, MAX_PATH, _T("%d.%d.%d.%d"), 
				(ipAddress>>24)&0x000000ff,
				(ipAddress>>16)&0x000000ff,
				(ipAddress>>8)&0x000000ff,
				ipAddress&0x000000ff);

			ipArray.push_back(ch);
		}

		if(pIpAddTalbe)
			delete[] pIpAddTalbe;
		pIpAddTalbe = NULL;

		return true;
	}
	else{

		if(pIpAddTalbe)
			delete[] pIpAddTalbe;
		pIpAddTalbe = NULL;
		return false;
	}
}

bool GetAvalibleBroadcastAddress(vector<tstring> &broadCastIpArray)
{
	broadCastIpArray.clear();

	// 	CStringArray ipAddresses;

	ULONG ulTalbeSize = 0;
	DWORD res = ::GetIpAddrTable(NULL, &ulTalbeSize, TRUE);
	MIB_IPADDRTABLE *pIpAddTalbe = (MIB_IPADDRTABLE*)new BYTE[ulTalbeSize];
	DWORD ret = ::GetIpAddrTable(pIpAddTalbe, &ulTalbeSize, TRUE);
	if(NO_ERROR == ret)
	{
		DWORD ipAddress = 0;
		for(UINT i = 0; i < pIpAddTalbe->dwNumEntries; i++)
		{
			ipAddress = pIpAddTalbe->table[i].dwAddr;
			if(0 == ipAddress)
				continue;

			ipAddress |= ~(pIpAddTalbe->table[i].dwMask);
			ipAddress = ntohl(ipAddress);

			TCHAR ch[MAX_PATH] = {0};
			_stprintf_s(ch, MAX_PATH, _T("%d.%d.%d.%d"), 
				(ipAddress>>24)&0x000000ff,
				(ipAddress>>16)&0x000000ff,
				(ipAddress>>8)&0x000000ff,
				ipAddress&0x000000ff);

			if(_tcscmp(ch, _T("127.255.255.255")) == 0)
				continue;

			broadCastIpArray.push_back(ch);
		}

		if(pIpAddTalbe)
			delete[] pIpAddTalbe;
		pIpAddTalbe = NULL;

		return true;
	}
	else{

		if(pIpAddTalbe)
			delete[] pIpAddTalbe;
		pIpAddTalbe = NULL;
		return false;
	}
}


int SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds)
{
	assert(sock != INVALID_SOCKET);
	TIMEVAL timeOut;
	timeOut.tv_sec	= 0;
	timeOut.tv_usec = dwMilliseconds * 1000;
	fd_set readFD;
	FD_ZERO(&readFD);
	FD_SET(sock, &readFD);
	int iRetVal = select(int(sock), &readFD, NULL, NULL, &timeOut);

	return iRetVal;
}

tstring GetNewGuid()
{
	TCHAR ch[MAX_PATH] = { 0 };
	GUID guid;

	if (S_OK == CoCreateGuid(&guid))
	{
		_stprintf_s(ch, MAX_PATH,
				  _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
				  guid.Data1,
				  guid.Data2,
				  guid.Data3,
				  guid.Data4[0], guid.Data4[1],
				  guid.Data4[2], guid.Data4[3],
				  guid.Data4[4], guid.Data4[5],
				  guid.Data4[6], guid.Data4[7]);
	}
	return ch;
}

void FindAllFile(tstring dirc, vector<tstring> &pathVec)
{
	// 开始查找
	TCHAR curPath[MAX_PATH] = {0};
	TCHAR filePath[MAX_PATH] = {0};
	if(dirc.empty())
	{
		GetModuleFileName(NULL, curPath, MAX_PATH);
		TCHAR * pName = _tcsrchr(curPath, _T('\\')) + 1;
		*pName = 0;
		_tcscpy_s(filePath, MAX_PATH, curPath);
		_tcscat_s(filePath, MAX_PATH, _T("*.*"));
	}
	else
	{
		dirc += _T("\\");
		_tcscpy_s(curPath, MAX_PATH, dirc.c_str());

		_tcscpy_s(filePath, MAX_PATH, curPath);
		_tcscat_s(filePath, MAX_PATH, _T("*.*"));
	}

	//pathVec.clear();	///不能清空，循环调用会失去以前找到的文件

	WIN32_FIND_DATA findFileData;
	HANDLE hFind=::FindFirstFile(filePath, &findFileData);
	while(hFind != INVALID_HANDLE_VALUE)
	{
		if (findFileData.dwFileAttributes == INVALID_FILE_ATTRIBUTES)
			continue;
		else if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(_tcscmp(findFileData.cFileName, _T(".")) != 0 && _tcscmp(findFileData.cFileName, _T("..")) != 0)
			{
				tstring dicPath = tstring(curPath) + findFileData.cFileName;
				FindAllFile(dicPath, pathVec);
			}
		}
		else
		{
			tstring fileName = findFileData.cFileName;
			tstring desPath	=  tstring(curPath) + findFileData.cFileName;
			pathVec.push_back(desPath);
		}

		if (!FindNextFile(hFind, &findFileData)) 
			break;
	}

	::FindClose(hFind);

}

BOOL IsRunAsAdmin()
{
	BOOL fIsRunAsAdmin = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(
		&NtAuthority, 
		2, 
		SECURITY_BUILTIN_DOMAIN_RID, 
		DOMAIN_ALIAS_RID_ADMINS, 
		0, 0, 0, 0, 0, 0, 
		&pAdministratorsGroup))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

	// Determine whether the SID of administrators group is enabled in 
	// the primary access token of the process.
	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

Cleanup:
	// Centralized cleanup for all allocated resources.
	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	// Throw the error if something failed in the function.
	if (ERROR_SUCCESS != dwError)
	{
		throw dwError;
	}
	return fIsRunAsAdmin;
}

BOOL ChangeWndMessageFilterOk(UINT nMessage, BOOL bAllow)
{
	typedef BOOL(WINAPI * ChangeWindowMessageFilterOkFn)(UINT, DWORD);

	HMODULE hModUser32 = NULL;
	hModUser32 = LoadLibrary(_T("user32.dll"));
	if (hModUser32 == NULL) {
		return FALSE;
	}

	ChangeWindowMessageFilterOkFn pfnChangeWindowMessageFilter = (ChangeWindowMessageFilterOkFn)GetProcAddress(hModUser32, "ChangeWindowMessageFilter");
	if (pfnChangeWindowMessageFilter == NULL)
	{
		FreeLibrary(hModUser32);
		return FALSE;
	}

	FreeLibrary(hModUser32);

	return pfnChangeWindowMessageFilter(nMessage, bAllow ? MSGFLT_ADD : MSGFLT_REMOVE);
}



lldiv_tRIM __cdecl lldiv_RIM( long long numer, long long denom )
{
	lldiv_tRIM result;

	result.quot = numer / denom;
	result.rem = numer % denom;

	return result;
}

long FindStrNoCase(const tstring& strTxt, long nBegin, const tstring& strFnd)
{
	if((strTxt.empty()) || (strFnd.empty())){
		return tstring::npos;
	}
	if(nBegin >= long(strTxt.length())){
		return tstring::npos;
	}

	tstring strTxtlower = strTxt;
	tstring strFndlower = strFnd;
	std::transform(strTxtlower.begin(), strTxtlower.end(), strTxtlower.begin(), tolower);	//转换成小写
	std::transform(strFndlower.begin(), strFndlower.end(), strFndlower.begin(), tolower);
	return strTxtlower.find(strFndlower, (nBegin >= 0)? nBegin: 0);
}

BOOL CreateFileShortcut(LPCTSTR lpszFileOrDic, LPCTSTR lpszLnkFileDir, LPCTSTR lpszLnkFileName, WORD wHotkey, 
						LPCTSTR lpszDescription, int iShowCmd /*= SW_SHOWNORMAL*/)
{
	if (lpszLnkFileDir == NULL)
		return FALSE;

	if(lpszLnkFileName == NULL)
		return FALSE;

	if (lpszFileOrDic && (GetFileAttributes(lpszFileOrDic) == INVALID_FILE_ATTRIBUTES)) 
		return FALSE;
	

	LPTSTR			pExePath = NULL;
#ifdef _UNICODE
	_get_wpgmptr(&pExePath);
#else
	_get_pgmptr(&pExePath);
#endif

	HRESULT			hr		= NULL;
	IShellLink     *pLink	= NULL;  //IShellLink对象指针
	IPersistFile   *ppf		= NULL; //IPersisFil对象指针

	//创建IShellLink对象
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
	if (FAILED(hr))
		return FALSE;

	//从IShellLink对象中获取IPersistFile接口
	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hr))
	{
		pLink->Release();
		return FALSE;
	}

	//目标
	if (lpszFileOrDic == NULL)
		pLink->SetPath(pExePath);
	else
		pLink->SetPath(lpszFileOrDic);

	//快捷键
	if (wHotkey != 0)
		pLink->SetHotkey(wHotkey);

	//备注
	if (lpszDescription != NULL)
		pLink->SetDescription(lpszDescription);

	//显示方式
	pLink->SetShowCmd(iShowCmd);

	//快捷方式的路径 + 名称
	TCHAR szBuffer[MAX_PATH] = {0};
	if (lpszLnkFileName != NULL) //指定了快捷方式的名称
		_stprintf_s(szBuffer, _T("%s\\%s"), lpszLnkFileDir, lpszLnkFileName);

	hr = ppf->Save(CT2OLE(szBuffer), TRUE);

	ppf->Release();
	pLink->Release();
	return SUCCEEDED(hr);
}

wstring ANSI2Unicode(const string & strin)
{
	wstring strout;

	// 预计算所需空间大小（已包含结束字符）,单位wchar_t
	int dwNum = MultiByteToWideChar (CP_ACP, 0,strin.c_str(), -1, 0 , 0);
	wchar_t * pBuffer = new wchar_t[dwNum + 1];
	if (!pBuffer)
	{
		return strout;
	}
	memset(pBuffer, 0, (dwNum + 1)*sizeof(wchar_t));

	if(MultiByteToWideChar(CP_ACP, 0, strin.c_str(),-1,pBuffer,dwNum) >= 0)
	{
		strout = pBuffer;
	}

	delete[] pBuffer;
	return strout;
}

string Unicode2UTF8(const wstring & strin)
{
	string strout;

	//测试所需存储空间大小（已包含结束字符）,单位char
	int dwNum = WideCharToMultiByte(CP_UTF8,0,strin.c_str(),-1,0,0,0,0);
	char* pBuffer = new char[dwNum + 1];
	if (!pBuffer)
	{
		return strout;
	}
	memset(pBuffer, 0, dwNum + 1);

	if(WideCharToMultiByte(CP_UTF8,0,strin.c_str(),-1,pBuffer,dwNum,0,0) >= 0)
	{
		strout = pBuffer;
	}

	delete[] pBuffer;
	return strout;
}

string ANSI2UTF8(const string & strin)
{
	return Unicode2UTF8(ANSI2Unicode(strin));
}

string Unicode2ANSI(const wstring & strin)
{
	string strout;

	//测试所需存储空间大小（已包含结束字符）,单位char
	int dwNum = WideCharToMultiByte(CP_ACP,0,strin.c_str(),-1,0,0,0,0);
	char* pBuffer = new char[dwNum + 1];
	if (!pBuffer)
	{
		return strout;
	}
	memset(pBuffer, 0 , dwNum + 1);

	BOOL use_def_char = FALSE;
	if(WideCharToMultiByte(CP_ACP,0,strin.c_str(),-1,pBuffer,dwNum,"?",&use_def_char) >= 0)
	{
		strout = pBuffer;
	}

	delete[] pBuffer;
	return strout;
}

wstring UTF82Unicode(const string & strin)
{
	wstring strout;

	// 预计算所需空间大小（已包含结束字符）,单位wchar_t
	int dwNum = MultiByteToWideChar (CP_UTF8, 0,strin.c_str(), -1, 0 , 0);
	wchar_t* pBuffer = new wchar_t[dwNum + 1];
	if (!pBuffer)
	{
		return strout;
	}
	memset(pBuffer, 0, (dwNum + 1)*sizeof(wchar_t));

	if(MultiByteToWideChar(CP_UTF8, 0, strin.c_str(), -1, pBuffer, dwNum) >= 0)
	{
		strout = pBuffer;
	}

	delete[] pBuffer;
	return strout;
}

string UTF82ANSI(const string & strin)
{
	return Unicode2ANSI(UTF82Unicode(strin));
}

bool IsValidIP(tstring strIP)
{
	string ip		= CT2A(strIP.c_str());
	char *pszIPAddr	= (char*)ip.c_str();

	if(NULL	==	pszIPAddr) 
		return false;

	int 	lenIPAddr	= strlen(pszIPAddr);
	int 	startIndex	= 0;
	int 	endIndex	= 0;
	//去掉前后空格
	for(startIndex = 0; startIndex < lenIPAddr; startIndex++)
		if(pszIPAddr[startIndex] != ' ')
			break;
	for(endIndex = lenIPAddr - 1; endIndex >= 0; endIndex--)
		if(pszIPAddr[endIndex] != ' ')
			break;
	if(endIndex <= startIndex) 
		return false;

	char	inputStr[200] 	= {0};
	int		lenInputStr		= endIndex - startIndex + 1;
	strncpy_s(inputStr, 200, &pszIPAddr[startIndex], lenInputStr);
	inputStr[lenInputStr] = '\0';
	//查找点的合法性
	int countDot = 0;
	for(int i=0; i<lenInputStr; i++)
	{
		if(inputStr[i]=='.')
		{
			countDot++;
			if(i == 0 || i == lenInputStr - 1) 
				return false;
			if(inputStr[i-1] < '0'|| inputStr[i-1] > '9')
				return false;
			if(inputStr[i+1] < '0'|| inputStr[i+1] > '9')
				return false;
		}
	}

	if(countDot != 3) return false;

	////【zy 2015-05-27 ↓】////检查四个部分是否合法
	char *pCut		= NULL;
	char *pCharPos  = NULL;
	pCut = strtok_s(inputStr, ".", &pCharPos);
	while(NULL != pCut)
	{	
		int num;
		sscanf_s(pCut, "%d", &num);
		if(num != 0 && pCut[0] == '0')
			return false;
		if(num>255) 
			return false;

		pCut = strtok_s(NULL,".", &pCharPos);
	}
	////【zy 2015-05-27 ↑】

	return true;
}
