//********************************************************************
// COMMONFUNTION.CPP 文件注释
// 文件名 　: COMMONFUNTION.CPP
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 14:33
// 文件描述 : 
//*********************************************************************
#include "StdAfx.h"
#include "CommonFuntion.h"


std::string  UTF8toASCII(std::string lstr)
{
	std::string lstrRet ; 
	lstrRet.clear() ;
	try
	{
		if(lstr.size())
		{
			int lnWBufLen = MultiByteToWideChar(CP_UTF8   ,0, lstr.c_str(), lstr.size()+1 ,NULL,0);
			if(lnWBufLen)
			{
				WCHAR* lpWBuf = new   WCHAR[lnWBufLen] ;
				memset(lpWBuf,0,lnWBufLen*sizeof(WCHAR));
				if(MultiByteToWideChar(CP_UTF8   ,0, lstr.c_str(), lstr.size()+1 ,lpWBuf,lnWBufLen))
				{
					int lnBufLen =  WideCharToMultiByte( CP_ACP , 0 , lpWBuf ,-1 , NULL , 0 , NULL, NULL ) ;
					if(lnBufLen)
					{
						CHAR*  lpBuf  = new   CHAR[lnBufLen+2];
						memset(lpBuf, 0, (lnBufLen+2)*sizeof(CHAR));
						if(WideCharToMultiByte( CP_ACP , 0 , lpWBuf ,-1 , lpBuf, lnBufLen , NULL, NULL ))
						{
							lstrRet = lpBuf ;
						}
						delete []lpBuf ;
					}
				}
				delete []lpWBuf ; 
			}
		}
		else
		{
			lstrRet = _T(" ") ;
		}
	}
	catch (...)
	{
		lstrRet.clear() ;
	}
	return lstrRet ;
}

std::string  ASCIItoUTF8(std::string lstr)
{
	std::string lstrRet ; 
	lstrRet.clear() ;
	try
	{
		if(lstr.size())
		{
			int lnWBufLen = MultiByteToWideChar(CP_ACP   ,0, lstr.c_str(),-1 ,NULL,0);
			if(lnWBufLen)
			{
				WCHAR* lpWBuf = new   WCHAR[lnWBufLen];
				memset(lpWBuf,0,lnWBufLen*sizeof(WCHAR));
				if(MultiByteToWideChar(CP_ACP   ,0, lstr.c_str(), -1 ,lpWBuf,lnWBufLen))
				{
					int lnBufLen =  WideCharToMultiByte( CP_UTF8 , 0 , lpWBuf ,-1 , NULL , 0 , NULL, NULL ) ;
					if(lnBufLen)
					{
						char*  lpBuf  = new   char[lnBufLen+2]; //防止字符过短出现问题
						memset(lpBuf, 0, (lnBufLen+2)*sizeof(char));
						if(WideCharToMultiByte( CP_UTF8 , 0 , lpWBuf ,-1 , lpBuf, lnBufLen , NULL, NULL ))
						{
							lstrRet = lpBuf ;
						}
						delete []lpBuf ;
					}
				}
				delete []lpWBuf ; 
			}
		}
		else
		{
			lstrRet = _T(" ") ;
		}
	}
	catch (...)
	{
		lstrRet.clear() ;
	}
	return lstrRet ;
}