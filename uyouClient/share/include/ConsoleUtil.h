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
	saAttr.bInheritHandle = TRUE;                        //注意，这一行一定要为TRUE，子进程可以继承句柄设置
	saAttr.lpSecurityDescriptor = NULL; 
	HANDLE hRead, hWrite;
	if(!CreatePipe(&(hRead), &(hWrite),&saAttr,0))
	{
		return ERROR_RETURN;
	}

	STARTUPINFO siStartInfo;
	GetStartupInfo(&siStartInfo);                 //通过这个函数省了不少事    
	siStartInfo.hStdOutput = hWrite;  //设置输出句柄
	siStartInfo.hStdError = hWrite;  //设置错误句柄
	siStartInfo.cb = sizeof(STARTUPINFO);  //设置大小
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	siStartInfo.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION piProcInfo;
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	if(CreateProcess(NULL,      //第一个参数可为空，
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

	//返回的是ansi字符集
	CHAR buf[1024] = {0};
	DWORD len;
	CloseHandle(hWrite);       //注意，一定要在本端关闭句柄，否则子进程无法进行写操作

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
					ATLTRACE("关闭进程失败\n");
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
					ATLTRACE("关闭进程失败\n");
				break;
			}
		}

		tmEnd = clock();
	} while (dwRet != WAIT_OBJECT_0);
	//等待执行结束

	String strRes;
	while(ReadFile(hRead, buf, 1023, &len, NULL))
	{
		CString sz(buf);		//自动进行ansi->unicode的转换，如果必要的话
		strRes = sz;
	}
	CloseHandle(hRead);

	return strRes;
}

}