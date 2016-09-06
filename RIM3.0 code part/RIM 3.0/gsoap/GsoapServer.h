#pragma once

#include "Xdmsdm\XdmsdmXdmsdmProxy.h"
#include "Xdmrtm\XdmrtmXdmrtmProxy.h"
#include "Bsdsysservice\BsdsysserviceBSDSYSSoap12Proxy.h"
#include "Bsdrtmservice\BsdrtmserviceBSDRTMSoap12Proxy.h"
// CGsoapServer

typedef class tagWEBADDRINFO
{
public:
	tagWEBADDRINFO()
	{
		sName					= "";
		nSubModeID				= 0;
		sLocation				= "";
	}
	~tagWEBADDRINFO()
	{
	}
	tagWEBADDRINFO::tagWEBADDRINFO(const tagWEBADDRINFO& ob)
	{
		*this					= ob;
	}

	tagWEBADDRINFO& tagWEBADDRINFO::operator=(const tagWEBADDRINFO& ob)
	{
		sName					= ob.sName;		
		nSubModeID				= ob.nSubModeID;	
		sLocation				= ob.sLocation;	

		return *this;
	}

	std::string					sName;					/* required element of type xsd:string	*/
	DWORD						nSubModeID;				/* required element of type xsd:int		*/
	std::string					sLocation;				/* required element of type xsd:string	*/
}WEBADDRINFO, *LPWEBADDRINFO;

class CGsoapServer
{
public:
	CGsoapServer();
	virtual ~CGsoapServer();
protected:
	struct FILEDMAP
	{
		string					sFiled;			////字段名称
		vector<string>			valueList;		////字段对应的每一行的值列表
	};

	HMODULE						m_hToolkitLibrary;
	string						RIM_RTK_BSD_DB_FILE;


	/*删除表*/
	bool						DeleteTable(std::string tableName);
	/*根据	字段--值列表更新表*/
	bool						UpdateTable(const vector<FILEDMAP*>	&tableInfoVec, std::string tableName);

	/*获取	字段--值列表*/
	//////////////////////////////////////////////DMS/////////////////////////////////////////
	bool 						FieldMapT_RIM_RTM_STATION						(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_DEVICE						(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_REF_STATION_TYPE				(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_REF_DEVICE_TYPE				(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_REF_DEVICE_PROTOCOL			(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM	(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_REF_DEVICE_MODEL				(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_REF_DEVICE_FUNC				(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_MON_ORG						(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_MON_CONTROL_CENTER			(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool						FieldMapT_RIM_RTM_DEVICE_FUNC_ANTENNA			(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool						FieldMapT_RIM_RTM_DEVICE_FUNC					(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_RTM_ANTENNA						(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_COM_FREQ_LAYOUT					(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);
	bool 						FieldMapT_RIM_COM_AUTH_SYS_DATA_STATION			(const std::string lstrUser, const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo);

	//////////////////////////////////////////////WEB  SYS/////////////////////////////////////////
	bool						FieldMapT_RIM_COM_AUTH_SYS_FUNC					(const std::string lstrUser, const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo);
	bool 						FieldMapT_RIM30_USERINFO						(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo);
	bool 						FieldMapT_RIM30_BRANCHINFO						(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo);

	//////////////////////////////////////////////WEB  RTM/////////////////////////////////////////
	bool						FieldMapT_RIM_RTM_RNC							(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo);
public:

	bool						VerifyUser(const std::string lstrURL, const std::string userName, std::string password, std::string& userToken, std::string& errorInfo);
	bool						SysncSqlite(const std::string lstrUser, const std::string lstrURL, std::string tableName, std::string& updateDate, std::string& errorInfo);
	bool						SysncT_RIM_COM_AUTH_SYS_DATA_STATION(const std::string lstrUser, const std::string lstrURL, std::string tableName, std::string& errorInfo);
	bool						QueryAllSubSysAddr(const std::string lstrURL, map<DWORD, vector<LPWEBADDRINFO>> &m_webAddrInfoMap, std::string& errorInfo);
};


