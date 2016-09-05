#include <string>
#include <iostream>
#include <stdio.h>
#include <types.h>

namespace ConsoleUtil
{

static LPCTSTR ERROR_RETURN = _T("ERROR_RETURN");

static String Execute(LPCTSTR cmd, UINT nWaitTime = INFINITE) 
{
	SECURITY_ATTRIBUTES saAttr; 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE;                        //ע�⣬��һ��һ��ҪΪTRUE���ӽ��̿��Լ̳о������
	saAttr.lpSecurityDescriptor = NULL; 
	HANDLE hRead, hWrite;
	if(!CreatePipe(&(hRead), &(hWrite),&saAttr,0))
	{
		return ERROR_RETURN;
	}

	STARTUPINFO siStartInfo;
	GetStartupInfo(&siStartInfo);                 //ͨ���������ʡ�˲�����    
	siStartInfo.hStdOutput = hWrite;  //����������
	siStartInfo.hStdError = hWrite;  //���ô�����
	siStartInfo.cb = sizeof(STARTUPINFO);  //���ô�С
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	siStartInfo.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION piProcInfo;
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	if(CreateProcess(NULL,      //��һ��������Ϊ�գ�
		(LPTSTR)cmd,
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo  // receives PROCESS_INFORMATION
		) == FALSE)
	{
		return ERROR_RETURN;
	}

	//���ص���ansi�ַ���
	CHAR buf[1024] = {0};
	DWORD len;
	CloseHandle(hWrite);       //ע�⣬һ��Ҫ�ڱ��˹رվ���������ӽ����޷�����д����

	DWORD dwRet = 0;
	MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	clock_t  tmBegin	= clock();
	clock_t  tmEnd		= tmBegin;
	do 
	{
		dwRet = WaitForSingleObject(piProcInfo.hProcess, 50);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_CLOSE)
			{
				BOOL ret =  TerminateProcess(piProcInfo.hProcess, 0);
				if (ret == FALSE)
					ATLTRACE("�رս���ʧ��\n");
				break;			
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (UINT(tmEnd - tmBegin) > nWaitTime)
			{
				BOOL ret =  TerminateProcess(piProcInfo.hProcess, 0);
				if (ret == FALSE)
					ATLTRACE("�رս���ʧ��\n");
				break;
			}
		}

		tmEnd = clock();
	} while (dwRet != WAIT_OBJECT_0);
	//�ȴ�ִ�н���

	String strRes;
	while(ReadFile(hRead, buf, 1023, &len, NULL))
	{
		CString sz(buf);		//�Զ�����ansi->unicode��ת���������Ҫ�Ļ�
		strRes = sz;
	}
	CloseHandle(hRead);

	return strRes;
}

}