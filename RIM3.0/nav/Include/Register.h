//********************************************************************
//	REGISTER.H �ļ�ע��
//	�ļ���		: REGISTER.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\INCLUDE/
//	����		: ����
//	����ʱ��	: 2015/1/5 15:39
//	�ļ�����	: 
//*********************************************************************	
#pragma once



class CRegister
{
public:
	CRegister(void);
	~CRegister(void);

	//������
	LONG 						CreateKey(HKEY hKey, 
										  LPCTSTR lpSubKey,
										  DWORD dwOptions,
										  REGSAM samDesired, 
										  LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
										  LPDWORD lpdwDisposition);

	//�򿪼�
	LONG 						OpenKey(HKEY hKey,
										LPCTSTR lpSubKey,
										DWORD ulOptions,
										REGSAM samDesired);

	//�ر�ע�����
	LONG						CloseKey();	
	
	//attachע�����
	HKEY						Attach(HKEY hKey);
	HKEY						Dettach();

	//��ȡ���
	HKEY						GetHandle();
	//����Ƿ���Ч
	BOOL						IsValid();

	//�ü��µ��ַ���ֵ��������ȡ��д��
	LONG						GetStringValue(LPCTSTR szValueName, CString &strRegValue);
	LONG						SetStringValue(LPCTSTR szValueName, LPCTSTR szRegValue);
	
	//�ü��µ�DWORDֵ��������ȡ��д��
	LONG						GetDWordValue(LPCTSTR szValueName, LPDWORD pRegValue);
	LONG						SetDWordValue(LPCTSTR szValueName, DWORD dwRegValue);	

	//�ü��µĶ�����ֵ��������ȡ��д��
	LONG						GetBinaryValue(LPCTSTR szValueName, LPBYTE pBuffer, DWORD &dwLen);
	LONG						SetBinaryValue(LPCTSTR szValueName, LPBYTE pBuffer, DWORD dwLen);

protected:
	HKEY m_hKey;
};
