//********************************************************************
// RIM30TOOLKIT.CPP 文件注释
// 文件名 　: RIM30TOOLKIT.CPP
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 14:29
// 文件描述 : 
//*********************************************************************
// RIM30ToolKit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RIM30ToolKit.h"
#include "RIM30Sqlite.h"

#include <map>
using namespace std;

//#define RIM_RTK_BSD_DB_FILE "BOSD.SDB"
bool UpdateDataBySql(const std::string &sSqlStr, string &strError) 
{
	if(sSqlStr.size() == 0)
		return false;

	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}

		if(!lpSQlite->ExeSql(ASCIItoUTF8(sSqlStr)))
		{
			strError = "执行SQL语句失败";
			return false;
		}

		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllStation(std::vector<tagRTMSTATION> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_STATION_NO	"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_NAME			"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_CREATETIME	"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_ADDRESS		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_LONGITUDE		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_LATITUDE		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_MONRANGE		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_TYPE			"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_CONTACT		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_DESC			"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_GPSADDR		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_GPSPORT		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_COMPASSADDR	"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_COMPASSPORT	"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_GRIDNO		"],"
			" t.["	BOSD_T_RIM_RTM_STATION_COLUMN_NETTYPE		"] "
			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_STATION"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 16 )
			{
				tagRTMSTATION lc ;
				lc.sStationNO	= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T("");
				lc.sName		= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T("");
				lc.nCreateTime	= sqlite3_column_int64(lpStmt,2);
				lc.sAddress		= sqlite3_column_bytes(lpStmt,3) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,3))) : _T("");
				lc.fLong 		= sqlite3_column_double(lpStmt,4);
				lc.fLati 		= sqlite3_column_double(lpStmt,5);
				lc.fMonRange	= sqlite3_column_double(lpStmt,6);
				lc.nStationType	= sqlite3_column_int(lpStmt,7);
				lc.sContact		= sqlite3_column_bytes(lpStmt,8) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,8))) : _T("");
				lc.sDes			= sqlite3_column_bytes(lpStmt,9) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,9))) : _T("");
				lc.sGPSAddr		= sqlite3_column_bytes(lpStmt,10) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,10))) : _T("");
				lc.nGPSPort		= sqlite3_column_int(lpStmt,11);
				lc.sCompassAddr	= sqlite3_column_bytes(lpStmt,12) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,12))) : _T("");
				lc.nCompassPort	= sqlite3_column_int(lpStmt,13);
				lc.nGridNO		= sqlite3_column_int(lpStmt,14);
				lc.sNettype		= sqlite3_column_bytes(lpStmt,15) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,15))) : _T("");

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllDevice(std::vector<tagRTMDEVICE> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_STATIONNO   	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_DEVNO       	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_MODEL       	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_NAME        	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_ESC         	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_TYPE        	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_BUYDATE     	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_SUPPLIER    	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_REPAIRTEL   	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_CURADDRESS  	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_CURSTATE    	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_RESPONER    	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_MAXRUNNUM   	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_SERVICEIP   	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_SERVICEPORT 	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_DEVPROTOCOL 	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_COLUMN_ISINTEGRATED	"] "
			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_DEVICE"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 17 )
			{
				RTMDEVICE lc ;
				lc.sStationNO			= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T("");
				lc.sDeviceNO			= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T("");
				lc.sDeviceModel			= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T("");
				lc.sName 				= sqlite3_column_bytes(lpStmt,3) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,3))) : _T("");
				lc.sDesc 				= sqlite3_column_bytes(lpStmt,4) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,4))) : _T("");
				lc.nDeviceType 			= sqlite3_column_int(lpStmt,5);
				lc.nBuyTime				= sqlite3_column_int64(lpStmt,6);
				lc.sSupplier			= sqlite3_column_bytes(lpStmt,7) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,7))) : _T("");
				lc.sRepairTel			= sqlite3_column_bytes(lpStmt,8) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,8))) : _T("");
				lc.sDeviceCurAddress	= sqlite3_column_bytes(lpStmt,9) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,9))) : _T("");
				lc.nStatus				= sqlite3_column_int(lpStmt,10);
				lc.sResponer			= sqlite3_column_bytes(lpStmt,11) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,11))) : _T("");
				lc.nMaxRunNUM			= sqlite3_column_int(lpStmt,12);
				lc.sDriverIP			= sqlite3_column_bytes(lpStmt,13) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,13))) : _T("");
				lc.nDriverPort			= sqlite3_column_int(lpStmt,14);
				lc.nDeviceProtocol		= sqlite3_column_int(lpStmt,15);
				lc.bIsintegrated		= sqlite3_column_int(lpStmt,16) ? true:false;

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRtmDeviceFunc(std::vector<RTMDEVICEFUNC> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_STATIONNO  	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_DEVICENO   	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_FUNCNO     	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_DEVICE_FUNC"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 3 )
			{
				RTMDEVICEFUNC lc ;
				lc.sStationNO				= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T("");
				lc.sDeviceNO				= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T("");
				lc.nFunNO					= sqlite3_column_int(lpStmt,2);

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRTMRefDeviceFunc(std::vector<RTMREFDEVICEFUNC> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_NOID  	"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_NAME  	"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_KEY   	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_REF_DEVICE_FUNC"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 3 )
			{
				RTMREFDEVICEFUNC lc ;
				lc.nFuncID					= sqlite3_column_int(lpStmt,0);
				lc.sFuncName				= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.sFuncKey					= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllComRegionNo(std::vector<COMREGIONNO> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_COM_REGION_NO_COLUMN_NO    	"],"
			" t.["	BOSD_T_RIM_COM_REGION_NO_COLUMN_NAME  	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_COM_REGION_NO"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 2 )
			{
				COMREGIONNO lc ;
				lc.sRegionNo					= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sRegionName					= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}


bool QueryAllRtmRefDeviceProtocol(std::vector<RTMREFDEVICEPROTOCOL> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NOID   	"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NAME   	"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NOTE   	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 3 )
			{
				RTMREFDEVICEPROTOCOL lc ;
				lc.nNoID					= sqlite3_column_int(lpStmt,0);
				lc.sName					= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.sNote					= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRimRefStationType(std::vector<RTMREFSTATIONTYPE> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_REF_STATION_TYPE_COLUMN_NOID   	"],"
			" t.["	BOSD_T_RIM_RTM_REF_STATION_TYPE_COLUMN_NAME   	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_REF_STATION_TYPE"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 2 )
			{
				RTMREFSTATIONTYPE lc ;
				lc.nTypeNo						= sqlite3_column_int(lpStmt,0); 
				lc.sTypeName					= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRimRefDeviceType(std::vector<RTMREFDEVICETYPE> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_TYPE_COLUMN_NOID   	"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_TYPE_COLUMN_NAME   	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_REF_DEVICE_TYPE"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 2 )
			{
				RTMREFDEVICETYPE lc ;
				lc.nTypeNo						= sqlite3_column_int(lpStmt,0); 
				lc.sTypeName					= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRtmMonOrg(std::vector<RTMMONORG> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_NO             	"],"
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_NAME           	"],"
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_ADDRESS        	"],"
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_POST           	"],"
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_RESPONPERSON   	"],"
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_CONTACT        	"],"
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_BRIEF          	"],"
			" t.["	BOSD_T_RIM_RTM_MON_ORG_COLUMN_NOTE           	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_MON_ORG"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 8 )
			{
				RTMMONORG lc ;
				lc.sOrgNO	 				= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sName	 				= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.sAddress	 				= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 
				lc.sPost					= sqlite3_column_bytes(lpStmt,3) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,3))) : _T(""); 
				lc.sResponpersion			= sqlite3_column_bytes(lpStmt,4) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,4))) : _T(""); 
				lc.sContact	 				= sqlite3_column_bytes(lpStmt,5) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,5))) : _T(""); 
				lc.sBrief 					= sqlite3_column_bytes(lpStmt,6) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,6))) : _T(""); 
				lc.sNote	 				= sqlite3_column_bytes(lpStmt,7) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,7))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}


bool QueryAllRtmMonCtrlCenter(std::vector<RTMMONCTRLCENTER> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_NAME       	"],"
			" t.["	BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_AREACODE   	"],"
			" t.["	BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_ADDRESS    	"],"
			" t.["	BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_LONG       	"],"
			" t.["	BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_LATI       	"],"
			" t.["	BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_NOTE       	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_MON_CONTROL_CENTER"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 6 )
			{
				RTMMONCTRLCENTER lc ;
				lc.sCenterNO 				= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sName	 				= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.sAddress	 				= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 
				lc.fLong 					= sqlite3_column_double(lpStmt,3); 
				lc.fLati	 				= sqlite3_column_double(lpStmt,4); 
				lc.sNote	 				= sqlite3_column_bytes(lpStmt,5) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,5))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRtmStationDeviceFuncAnt(std::vector<RTMSTATIONDEVICEFUNCANT> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_STATIONNO    	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_DEVICENO     	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_FUNCNO       	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_ANTKEYWORD   	"],"
			" t.["	BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_ANTNAME      	"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 5 )
			{
				RTMSTATIONDEVICEFUNCANT lc ;
				lc.sStationNO 					= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sDeviceNO  					= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.nFunNO   					= sqlite3_column_int(lpStmt,2); 
				lc.sAnt 						= sqlite3_column_bytes(lpStmt,3) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,3))) : _T(""); 
				lc.sDes 						= sqlite3_column_bytes(lpStmt,4) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,4))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRtmAnt(std::vector<RTMANT> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_ANTENNA_COLUMN_STATIONNO    		"],"
			" t.["	BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTNAME      		"],"
			" t.["	BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTFACTOR    		"],"
			" t.["	BOSD_T_RIM_RTM_ANTENNA_COLUMN_STARTFREQ    		"],"
			" t.["	BOSD_T_RIM_RTM_ANTENNA_COLUMN_STOPFREQ     		"],"
			" t.["	BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTHEIGHT    		"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_ANTENNA"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 6 )
			{
				RTMANT lc ;
				lc.sStationNO					= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sAntName						= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.sAntFactor					= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 
				lc.nBeginFreq					= sqlite3_column_int64(lpStmt,3);  
				lc.nEndFreq						= sqlite3_column_int64(lpStmt,4); 
				lc.nAntHeight					= sqlite3_column_int(lpStmt,5); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRtmRefDeviceModel(std::vector<RTMREFDEVICEMODEL> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NOID    		"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NAME    		"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NOTE    		"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_REF_DEVICE_MODEL"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 3 )
			{
				RTMREFDEVICEMODEL lc ;
				lc.sNoId							= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sName							= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.sNote							= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRtmRefDeviceModelFuncParam(std::vector<RTMREFDEVICEMODELFUNCPARAM> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_DEVICE    		"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_FUNC      		"],"
			" t.["	BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_PARAM     		"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 3 )
			{
				RTMREFDEVICEMODELFUNCPARAM lc ;
				lc.sModel								= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.nFuncID								= sqlite3_column_int(lpStmt,1);
				lc.sParam								= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}
bool QueryAllRtmRnc(std::vector<RIM30RTMRNC> &lcArray, string &strError) 
{
	bool bOk  = true;
	strError    = "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break; 
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_STATIONNO          	"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_RNCNAME            	"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_RNCIP					"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_RNCPORT            	"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_CTRLDEVICENAME     	"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_CTRLPORT           	"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSHAS          	"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSIP           	"],"
			" t.["         BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSPORT         	"],"
			" t.["         "CTRLTYPE"         								"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_RTM_RNC"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 10 )
			{
				RIM30RTMRNC lc ;
				lc.sSTATNo                                  = sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sRncName                                 = sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 
				lc.sRncAddr                                 = sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 
				lc.nRncPort                                 = sqlite3_column_int(lpStmt,3);  
				lc.sDevName                                 = sqlite3_column_bytes(lpStmt,4) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,4))) : _T(""); 
				lc.sCtrlPort								= sqlite3_column_int(lpStmt,5); 
				lc.bHasOS                                   = sqlite3_column_int(lpStmt,6) ? true:false;
				lc.sOSIP									= sqlite3_column_bytes(lpStmt,7) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,7))) : _T(""); 
				lc.nOSPort                                  = sqlite3_column_int(lpStmt,8); 
				lc.nType									= sqlite3_column_int(lpStmt,9); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllComAuthSysDataStation(std::vector<RIM30COMAUTHSYSDATASTATION> &lcArray, string &strError) 
{
	bool bOk  = true;
	strError    = "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["         BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION_COLUMN_NO  			"],"
			" t.["         BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION_COLUMN_NOTE			"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 2 )
			{
				RIM30COMAUTHSYSDATASTATION lc ;
				lc.sSTATNo                                  = sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sNote									= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllComAuthSysFunc(std::vector<RIM30COMAUTHSYSFUNC> &lcArray, string &strError) 
{
	bool bOk  = true;
	strError    = "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["         BOSD_T_RIM_COM_AUTH_SYS_FUNC_COLUMN_NO  		"],"
			" t.["         BOSD_T_RIM_COM_AUTH_SYS_FUNC_COLUMN_NOTE		"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_COM_AUTH_SYS_FUNC"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 2 )
			{
				RIM30COMAUTHSYSFUNC lc ;
				lc.sSysFuncNo                               = sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.sNote									= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRimComFreqLayout(std::vector<RIMCOMFREQLAYOUT> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_LAYOUTNAME      		"],"
			" t.["	BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_STARTFREQ     		"],"
			" t.["	BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_ENDFREQ         		"],"
			" t.["	BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_GROUPNAME     		"],"
			" t.["	BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_DISTINCE        		"],"
			" t.["	BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_EMITWIDTH       		"] "

			" FROM "
			" MAIN.["BOSD_T_RIM_COM_FREQ_LAYOUT"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 6 )
			{
				RIMCOMFREQLAYOUT lc ;
				lc.sName							= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.nStartFreq						= sqlite3_column_int(lpStmt,1);
				lc.nEndFreq							= sqlite3_column_int(lpStmt,2); 
				lc.sGroupName						= sqlite3_column_bytes(lpStmt,3) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,3))) : _T(""); 
				lc.nDistince						= sqlite3_column_int(lpStmt,4); 
				lc.nEmitWidth						= sqlite3_column_int(lpStmt,5); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRIM30UserInfo(std::vector<RIM30USERINFO> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM30_USERINFO_USERID	    		"],"
			" t.["	BOSD_T_RIM30_USERINFO_USERNAME    			"],"
			" t.["	BOSD_T_RIM30_USERINFO_EMPNO		    		"],"
			" t.["	BOSD_T_RIM30_USERINFO_AREACODE    			"],"
			" t.["	BOSD_T_RIM30_USERINFO_DEPTID	    		"],"
			" t.["	BOSD_T_RIM30_USERINFO_DEPTNAME    			"] "

			" FROM "
			" MAIN.["BOSD_T_RIM30_USERINFO"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 6 )
			{
				RIM30USERINFO lc ;
				lc.UserID								= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.UserName								= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T("");
				lc.EmpNO								= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 
				lc.AreaCode								= sqlite3_column_bytes(lpStmt,3) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,3))) : _T(""); 
				lc.DeptID								= sqlite3_column_bytes(lpStmt,4) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,4))) : _T(""); 
				lc.DeptName								= sqlite3_column_bytes(lpStmt,5) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,5))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}

bool QueryAllRIM30BranchInfo(std::vector<RIM30BRANCHINFO> &lcArray, string &strError) 
{
	bool bOk	= true;
	strError	= "";
	do 
	{
		std::auto_ptr<CRIM30Sqlite>  lpSQlite(new CRIM30Sqlite);
		if(!lpSQlite->Open(RIM_RTK_BSD_DB_FILE, false ,true))
		{
			strError = "打开基础支撑数据库失败";
			OutputDebugString(strError.c_str());
			bOk = false;
			break;
		}
		sqlite3_stmt   *lpStmt   =  NULL ;
		std::string     lstrSQL  =  " SELECT"  
			" t.["	BOSD_T_RIM30_BRANCHINFO_DEPTNAME	    		"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_PARENTID	  			"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_TEL			    		"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_FAX			  			"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_EMAIL		    		"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_MANAGER		    		"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_NOTE		    		"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_DEPTLEVEL	    		"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_DEPTID		  			"],"
			" t.["	BOSD_T_RIM30_BRANCHINFO_DELFLAG		  			"] "
							
			" FROM "
			" MAIN.["BOSD_T_RIM30_BRANCHINFO"] t ;"
			;
		if( sqlite3_prepare_v2(lpSQlite->Handle() , ASCIItoUTF8(lstrSQL).c_str(), -1, &lpStmt, NULL) != SQLITE_OK )
		{
			char ch[512] = {0};
			sprintf_s(ch, 512, "Prepare SQL:%s failure:%s\n",lstrSQL.c_str() , sqlite3_errmsg(lpSQlite->Handle()));
			strError = ch;
			OutputDebugString(strError.c_str());
			break;
		}

		while(sqlite3_step(lpStmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lpStmt) == 10 )
			{
				RIM30BRANCHINFO lc ;
				lc.Deptname								= sqlite3_column_bytes(lpStmt,0) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,0))) : _T(""); 
				lc.Parentid								= sqlite3_column_bytes(lpStmt,1) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,1))) : _T("");
				lc.Tel									= sqlite3_column_bytes(lpStmt,2) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,2))) : _T(""); 
				lc.Fax									= sqlite3_column_bytes(lpStmt,3) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,3))) : _T(""); 
				lc.Email								= sqlite3_column_bytes(lpStmt,4) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,4))) : _T(""); 
				lc.Manager								= sqlite3_column_bytes(lpStmt,5) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,5))) : _T(""); 
				lc.Note									= sqlite3_column_bytes(lpStmt,6) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,6))) : _T(""); 
				lc.Deptlevel							= sqlite3_column_bytes(lpStmt,7) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,7))) : _T(""); 
				lc.Deptid								= sqlite3_column_bytes(lpStmt,8) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,8))) : _T(""); 
				lc.Delflag								= sqlite3_column_bytes(lpStmt,9) ? (UTF8toASCII((char *)sqlite3_column_text(lpStmt,9))) : _T(""); 

				lcArray.push_back(lc);
			}
		}

		sqlite3_finalize(lpStmt);
		sqlite3_release_memory((int)sqlite3_memory_used());
	} while (0);

	return bOk;
}


/////////////////////////////////////////////////////////	扩展方法	////////////////////////////////////////////////////////

std::vector<ULONG>  QueryStationGroupByFuncID(ULONG lnFuncID)
{
	string strerror = "";

	vector<RTMDEVICEFUNC> vec;
	QueryAllRtmDeviceFunc(vec, strerror);

	map<string, int>	noArray;

	for (UINT i = 0; i < vec.size(); ++i)
	{	////查询每个站点下，有几个设备拥有该种功能
		if(vec.at(i).nFunNO == lnFuncID)
		{
			noArray[vec.at(i).sStationNO] ++;
		}
	}

	vector<ULONG> vecTemp;
	for (map<string, int>::iterator iter = noArray.begin(); iter != noArray.end(); ++iter)
	{	////将所有站点编号转换成ULONG
		ULONG temp = atoi(iter->first.c_str());
		vecTemp.push_back(temp);
	}

	vec.clear();
	noArray.clear();

	return vecTemp;
}

std::vector<std::string> QueryRegionGroupByFuncID(ULONG lnFuncID)
{
	string strerror = "";

	vector<RTMDEVICEFUNC> vec;
	QueryAllRtmDeviceFunc(vec, strerror);

	map<string, int>	noArray;

	for (UINT i = 0; i < vec.size(); ++i)
	{	////查询每个站点下，有几个设备拥有该种功能
		if(vec.at(i).nFunNO == lnFuncID)
		{
			noArray[vec.at(i).sStationNO.substr(0, 6)] ++;
		}
	}

	vector<string> vecTemp;
	for (map<string, int>::iterator iter = noArray.begin(); iter != noArray.end(); ++iter)
	{	////取前六位
		vecTemp.push_back(iter->first);
	}

	vec.clear();
	noArray.clear();

	return vecTemp;
}

std::string QueryStationName(ULONG lnStationID)
{
	string strerror = "";

	vector<RTMSTATION> vec;
	QueryAllStation(vec, strerror);

	ULONG idTemp = 0;
	for (UINT i = 0; i < vec.size(); ++i)
	{
		idTemp = atoi(vec[i].sStationNO.c_str());
		if (idTemp == lnStationID)
			return vec[i].sName;
	}

	return "";
}

std::string QueryFuncName(ULONG lnFuncID)
{
	string strerror = "";

	vector<RTMREFDEVICEFUNC> vec;
	QueryAllRTMRefDeviceFunc(vec, strerror);


	for (UINT i = 0; i < vec.size(); ++i)
	{
		if (lnFuncID == vec[i].nFuncID)
			return vec[i].sFuncName;
	}

	return "";
}

std::string QueryStationTypeName(ULONG lnStationType)
{
	string strerror = "";

	vector<RTMREFSTATIONTYPE> vec;
	QueryAllRimRefStationType(vec, strerror);


	for (UINT i = 0; i < vec.size(); ++i)
	{
		if (lnStationType == vec[i].nTypeNo)
			return vec[i].sTypeName;
	}

	return "";
}

bool QueryMonOrg(std::string sOrgCode , RTMMONORG &cOrg)
{
	string	strerror	= "";

	vector<RTMMONORG> vec;
	QueryAllRtmMonOrg(vec, strerror);

	for (UINT i = 0; i < vec.size(); ++i)
	{
		if (sOrgCode.compare(vec[i].sOrgNO.c_str()) == 0)
		{
			cOrg = vec[i];
			return true;
		}
	}

	return false;
}

bool QueryStation(std::string sStationID , RTMSTATION &cStation)
{
	string strerror = "";

	vector<RTMSTATION> vec;
	QueryAllStation(vec, strerror);

	for (UINT i = 0; i < vec.size(); ++i)
	{	////查询每个站点下，有几个设备拥有该种功能
		if (sStationID.compare(vec[i].sStationNO.c_str()) == 0)
		{
			cStation = vec[i];
			return true;
		}
	}

	return false;
}

bool QueryDevice(ULONG lnStationID , ULONG lnDeviceID, RTMDEVICE &lcDevice)
{
	string strerror = "";

	vector<RTMDEVICE> vec;
	QueryAllDevice(vec, strerror);

	for (UINT i = 0; i < vec.size(); ++i)
	{	////查询每个站点下，有几个设备拥有该种功能
		int stationID	= atoi(vec[i].sStationNO.c_str());
		int deviceID	= atoi(vec[i].sDeviceNO.c_str());
		if (lnStationID == stationID && lnDeviceID == deviceID)
		{
			lcDevice = vec[i];
			return true;
		}
	}

	return false;
}

bool QueryDeviceModelFuncParam(std::string sDeviceModel, LONG nFuncID , std::string &sXMLParam)
{
	string strerror = "";

	vector<RTMREFDEVICEMODELFUNCPARAM> vec;
	QueryAllRtmRefDeviceModelFuncParam(vec, strerror);

	for (UINT i = 0; i < vec.size(); ++i)
	{	////查询每个站点下，有几个设备拥有该种功能

		if (sDeviceModel.compare(vec[i].sModel.c_str()) == 0 && nFuncID == vec[i].nFuncID)
		{
			sXMLParam = vec[i].sParam;
			return true;
		}
	}

	return false;
}

bool IsRtmGroup(ULONG lnStationID,ULONG lnDeviceID,ULONG lnFuncID)
{
	string strerror = "";

	vector<RTMDEVICEFUNC> vec;
	QueryAllRtmDeviceFunc(vec, strerror);

	for (UINT i = 0; i < vec.size(); ++i)
	{	////查询每个站点下，有几个设备拥有该种功能
		int stationID	= atoi(vec[i].sStationNO.c_str());
		int deviceID	= atoi(vec[i].sDeviceNO.c_str());
		if (lnStationID == stationID && lnDeviceID == deviceID && lnFuncID == vec[i].nFunNO)
		{
			return true;
		}
	}

	return false;
}

bool QueryRegionName(std::string sRegionNo, std::string &sRegionName)
{
	string strerror = "";

	vector<COMREGIONNO> vec;
	QueryAllComRegionNo(vec, strerror);

	for (UINT i = 0; i < vec.size(); ++i)
	{	////查询每个站点下，有几个设备拥有该种功能

		if (sRegionNo == vec[i].sRegionNo)
		{
			sRegionName = vec[i].sRegionName;
			return true;
		}
	}

	return false;
}