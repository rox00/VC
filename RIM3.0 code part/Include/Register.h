//********************************************************************
//	REGISTER.H 文件注释
//	文件名		: REGISTER.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\INCLUDE/
//	作者		: 张永
//	创建时间	: 2015/1/5 15:39
//	文件描述	: 
//*********************************************************************	
#pragma once



class CRegister
{
public:
	CRegister(void);
	~CRegister(void);

	//创建键
	LONG 						CreateKey(HKEY hKey, 
										  LPCTSTR lpSubKey,
										  DWORD dwOptions,
										  REGSAM samDesired, 
										  LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
										  LPDWORD lpdwDisposition);

	//打开键
	LONG 						OpenKey(HKEY hKey,
										LPCTSTR lpSubKey,
										DWORD ulOptions,
										REGSAM samDesired);

	//关闭注册表句柄
	LONG						CloseKey();	
	
	//attach注册表句柄
	HKEY						Attach(HKEY hKey);
	HKEY						Dettach();

	//获取句柄
	HKEY						GetHandle();
	//句柄是否有效
	BOOL						IsValid();

	//该键下的字符串值操作，读取、写入
	LONG						GetStringValue(LPCTSTR szValueName, CString &strRegValue);
	LONG						SetStringValue(LPCTSTR szValueName, LPCTSTR szRegValue);
	
	//该键下的DWORD值操作，读取、写入
	LONG						GetDWordValue(LPCTSTR szValueName, LPDWORD pRegValue);
	LONG						SetDWordValue(LPCTSTR szValueName, DWORD dwRegValue);	

	//该键下的二进制值操作，读取、写入
	LONG						GetBinaryValue(LPCTSTR szValueName, LPBYTE pBuffer, DWORD &dwLen);
	LONG						SetBinaryValue(LPCTSTR szValueName, LPBYTE pBuffer, DWORD dwLen);

protected:
	HKEY m_hKey;
};
