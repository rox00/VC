#include "StdAfx.h"
#include ".\register.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CRegister::CRegister()
{
	m_hKey = NULL;
}

CRegister::~CRegister()
{
	CloseKey();
}

LONG CRegister::CreateKey(HKEY hKey, 
							LPCTSTR lpSubKey,
							DWORD dwOptions,
							REGSAM samDesired, 
							LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
							LPDWORD lpdwDisposition)
{
	ASSERT(m_hKey==NULL);
	return RegCreateKeyEx(hKey, lpSubKey, 0, NULL, dwOptions, samDesired, lpSecurityAttributes, &m_hKey, lpdwDisposition);
}

LONG CRegister::OpenKey(	HKEY hKey,
							LPCTSTR lpSubKey,
							DWORD ulOptions,
							REGSAM samDesired)
{
	ASSERT(m_hKey==NULL);
	return RegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, &m_hKey);
}

LONG CRegister::CloseKey()
{
	if(m_hKey)
	{
		LONG lRst = RegCloseKey(m_hKey);
		m_hKey = NULL;

		return lRst;
	}
	else
		return ERROR_SUCCESS;
}


HKEY CRegister::Attach(HKEY hKey)
{
	ASSERT(hKey);

	HKEY hOldKey = m_hKey;
	m_hKey = hKey;
	return hOldKey;
}

HKEY CRegister::Dettach()
{
	HKEY hOldKey = m_hKey;
	m_hKey = NULL;
	return hOldKey;
}

HKEY CRegister::GetHandle()
{
	return m_hKey;
}

BOOL CRegister::IsValid()
{
	return (m_hKey != NULL);
}

LONG CRegister::GetStringValue(LPCTSTR szValueName, CString &strRegValue)
{
	ASSERT(m_hKey);

	DWORD	dwType;	
	DWORD	dwBufLen = 256 * sizeof(TCHAR);
	TCHAR	*pRegValue = new TCHAR[256];
	
	LONG lRet = RegQueryValueEx(m_hKey, szValueName, NULL, &dwType, LPBYTE(pRegValue), &dwBufLen);

	//如果分配的内存太小，按返回的大小重新分配
	if(lRet == ERROR_MORE_DATA)
	{
		delete []pRegValue;
		pRegValue = new TCHAR[dwBufLen/sizeof(TCHAR)];
		lRet = RegQueryValueEx(m_hKey, szValueName, NULL, &dwType, LPBYTE(pRegValue), &dwBufLen);
	}
	
	if(lRet == ERROR_SUCCESS)
	{
		if(dwType == REG_SZ)
		{
			strRegValue = pRegValue;
		}
		else
			lRet = ERROR_BAD_FORMAT;
	}
	
	delete []pRegValue;

	return lRet;
}

LONG CRegister::SetStringValue(LPCTSTR szValueName, LPCTSTR szRegValue)
{
	ASSERT(m_hKey);
		
	return RegSetValueEx(m_hKey, szValueName, NULL, REG_SZ, LPBYTE(szRegValue), (_tcslen(szRegValue)+1) * sizeof(TCHAR));
}

LONG CRegister::GetDWordValue(LPCTSTR szValueName, LPDWORD pRegValue)
{
	ASSERT(m_hKey);
	ASSERT(pRegValue);

	DWORD	dwType;
	DWORD	dwBufLen = sizeof(DWORD);
	
	LONG lRet = RegQueryValueEx(m_hKey, szValueName, NULL, &dwType, (LPBYTE)pRegValue, &dwBufLen);
	if(lRet == ERROR_SUCCESS)
	{
		if(dwType != REG_DWORD)
			lRet = ERROR_BAD_FORMAT;
	}

	return lRet;
}

LONG CRegister::SetDWordValue(LPCTSTR szValueName, DWORD dwRegValue)
{
	ASSERT(m_hKey);

	return RegSetValueEx(m_hKey, szValueName, NULL, REG_DWORD, LPBYTE(&dwRegValue), sizeof(DWORD));
}

LONG CRegister::GetBinaryValue(LPCTSTR szValueName, LPBYTE pBuffer, DWORD &dwLen)
{
	ASSERT(m_hKey);

	return RegQueryValueEx(m_hKey, szValueName, NULL, NULL, pBuffer, &dwLen);
}

LONG CRegister::SetBinaryValue(LPCTSTR szValueName, LPBYTE pBuffer, DWORD dwLen)
{
	ASSERT(m_hKey);
	return RegSetValueEx(m_hKey, szValueName, NULL, REG_BINARY, pBuffer, dwLen);
}
