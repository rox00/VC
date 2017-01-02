// D:\RIM 3.0\src\nav\RIM 3.0\gsoap\GsoapServer.cpp : implementation file
//

#include "stdafx.h"
#include "GsoapServer.h"


// CGsoapServer


#define RIM_LOSHORT(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define RIM_HISHORT(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))

CGsoapServer::CGsoapServer()
{
	char szPath[MAX_PATH] = {0};
	HMODULE hModule = 0;
	GetModuleFileNameA(hModule, szPath, MAX_PATH);
	char *pChar = strrchr(szPath, '\\');
	if(pChar)
		pChar[0] = '\0';
	RIM_RTK_BSD_DB_FILE = szPath;
	RIM_RTK_BSD_DB_FILE += "\\data\\BOSD.SDB";

#ifdef _DEBUG
	m_hToolkitLibrary		= LoadLibrary(_T("RIM30ToolKitD.dll"));
#else
	m_hToolkitLibrary		= LoadLibrary(_T("RIM30ToolKit.dll"));
#endif

}

CGsoapServer::~CGsoapServer()
{
}

bool CGsoapServer::DeleteTable(std::string tableName)
{
	_pUpdateDataBySql UpdateDataBySql = (_pUpdateDataBySql)GetProcAddress(m_hToolkitLibrary, "UpdateDataBySql");

	char deleteStr[MAX_PATH] = {0};
	sprintf_s(deleteStr, MAX_PATH, "DELETE FROM '%s' ;", tableName.c_str());
	string strError = "";
	if(!UpdateDataBySql(deleteStr, strError))
		return false;

	return true;
}

bool CGsoapServer::UpdateTable(const vector<FILEDMAP*> &tableInfoVec, std::string tableName)
{
	if(tableInfoVec.size())
	{
		////【zy 2015-05-12 ↓】  构造插入语句的前半部分
		string strSqlBase = "INSERT INTO ";
		strSqlBase += tableName + " (";
		for (UINT i = 0; i < tableInfoVec.size(); ++i)
		{
			strSqlBase += tableInfoVec.at(i)->sFiled; 

			if(i != (tableInfoVec.size() - 1))
				strSqlBase+=", ";
		}
		strSqlBase += ") VALUES(";
		////【zy 2015-05-12 ↑】

		////【zy 2015-05-12 ↓】  构造插入语句的值部分
		string strSql = "";
		unsigned int valueCount = tableInfoVec[0]->valueList.size();
		for(UINT j = 0; j < valueCount; ++j)
		{
			string strSqlTemp = strSqlBase;
			for (UINT i = 0; i < tableInfoVec.size(); ++i)
			{
				strSqlTemp += "'";
				strSqlTemp += tableInfoVec[i]->valueList[j];
				strSqlTemp += "'";
				if(i != (tableInfoVec.size() - 1))
					strSqlTemp+=", ";
			}

			strSqlTemp += ");";
			strSql += strSqlTemp;
		}

		_pUpdateDataBySql UpdateDataBySql = (_pUpdateDataBySql)GetProcAddress(m_hToolkitLibrary, "UpdateDataBySql");

		string strError = "";
		if(!DeleteTable(tableName))
			return false;

		if(!UpdateDataBySql(strSql, strError))
			return false;
	}
	else
		return false;

	return true;
}

bool CGsoapServer::FieldMapT_RIM_RTM_STATION(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();

	Xdmrtm::_Xdmrtm__QueryStation			_Xdmrtm__querystation;
	Xdmrtm::_Xdmrtm__QueryStationResponse	_Xdmrtm__querystationresponse;
	int lnsoap = lpsvr->QueryStation(&_Xdmrtm__querystation, &_Xdmrtm__querystationresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__querystationresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_STATION_COLUMN_STATION_NO    	,
				BOSD_T_RIM_RTM_STATION_COLUMN_NAME          	,
				BOSD_T_RIM_RTM_STATION_COLUMN_CREATETIME    	,
				BOSD_T_RIM_RTM_STATION_COLUMN_ADDRESS       	,
				BOSD_T_RIM_RTM_STATION_COLUMN_LONGITUDE     	,
				BOSD_T_RIM_RTM_STATION_COLUMN_LATITUDE      	,
				BOSD_T_RIM_RTM_STATION_COLUMN_MONRANGE      	,
				BOSD_T_RIM_RTM_STATION_COLUMN_TYPE          	,
				BOSD_T_RIM_RTM_STATION_COLUMN_CONTACT       	,
				BOSD_T_RIM_RTM_STATION_COLUMN_DESC          	,
				BOSD_T_RIM_RTM_STATION_COLUMN_GPSADDR       	,
				BOSD_T_RIM_RTM_STATION_COLUMN_GPSPORT       	,
				BOSD_T_RIM_RTM_STATION_COLUMN_COMPASSADDR   	,
				BOSD_T_RIM_RTM_STATION_COLUMN_COMPASSPORT   	,
				BOSD_T_RIM_RTM_STATION_COLUMN_GRIDNO        	,
				BOSD_T_RIM_RTM_STATION_COLUMN_NETTYPE       	,
			};						
			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__querystationresponse.StationArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__querystationresponse.StationArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__Station* pItem = _Xdmrtm__querystationresponse.StationArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sStationNO);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					case 2:
						sprintf_s(ch, 64, "%lld", pItem->nCreateTime);
						pFiledMap->valueList.push_back(ch);
						break;
					case 3:
						pFiledMap->valueList.push_back(pItem->sAddress);
						break;
					case 4:
						sprintf_s(ch, 64, "%f", pItem->fLong);
						pFiledMap->valueList.push_back(ch);
						break;
					case 5:
						sprintf_s(ch, 64, "%f", pItem->fLati);
						pFiledMap->valueList.push_back(ch);
						break;
					case 6:
						sprintf_s(ch, 64, "%f", pItem->fMonRange);
						pFiledMap->valueList.push_back(ch);
						break;
					case 7:
						sprintf_s(ch, 64, "%d", pItem->nStationType);
						pFiledMap->valueList.push_back(ch);
						break;
					case 8:
						pFiledMap->valueList.push_back(pItem->sContact);
						break;
					case 9:
						pFiledMap->valueList.push_back(pItem->sDes);
						break;
					case 10:
						pFiledMap->valueList.push_back(pItem->sGPSAddr);
						break;
					case 11:
						sprintf_s(ch, 64, "%d", pItem->nGPSPort);
						pFiledMap->valueList.push_back(ch);
						break;
					case 12:
						pFiledMap->valueList.push_back(pItem->sCompassAddr);
						break;
					case 13:
						sprintf_s(ch, 64, "%d", pItem->nCompassPort);
						pFiledMap->valueList.push_back(ch);
						break;
					case 14:
						sprintf_s(ch, 64, "%d", pItem->nGridNO);
						pFiledMap->valueList.push_back(ch);
						break;
					case 15:
						pFiledMap->valueList.push_back(pItem->sNettype);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__querystationresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryStation接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_DEVICE(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryDevice			_Xdmrtm__querydevice;
	Xdmrtm::_Xdmrtm__QueryDeviceResponse	_Xdmrtm__querydeviceresponse;
	int lnsoap = lpsvr->QueryDevice(&_Xdmrtm__querydevice, &_Xdmrtm__querydeviceresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__querydeviceresponse.nResult) == 0)
		{

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_DEVICE_COLUMN_STATIONNO        	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_DEVNO            	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_MODEL            	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_NAME             	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_ESC              	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_TYPE             	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_BUYDATE          	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_SUPPLIER         	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_REPAIRTEL        	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_CURADDRESS       	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_CURSTATE         	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_RESPONER         	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_MAXRUNNUM        	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_SERVICEIP        	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_SERVICEPORT      	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_DEVPROTOCOL      	,
				BOSD_T_RIM_RTM_DEVICE_COLUMN_ISINTEGRATED		,

			};						
			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__querydeviceresponse.DeviceArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__querydeviceresponse.DeviceArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__Device* pItem = _Xdmrtm__querydeviceresponse.DeviceArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sStationNO);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sDeviceNO);
						break;
					case 2:
						pFiledMap->valueList.push_back(pItem->sDeviceModel);
						break;
					case 3:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					case 4:
						pFiledMap->valueList.push_back(pItem->sDesc);
						break;
					case 5:
						sprintf_s(ch, 64, "%d", pItem->nDeviceType);
						pFiledMap->valueList.push_back(ch);
						break;
					case 6:
						sprintf_s(ch, 64, "%lld", pItem->nBuyTime);
						pFiledMap->valueList.push_back(ch);
						break;
					case 7:
						pFiledMap->valueList.push_back(pItem->sSupplier);
						break;
					case 8:
						pFiledMap->valueList.push_back(pItem->sRepairTel);
						break;
					case 9:
						pFiledMap->valueList.push_back(pItem->sDeviceCurAddress);
						break;
					case 10:
						sprintf_s(ch, 64, "%d", pItem->nStatus);
						pFiledMap->valueList.push_back(ch);
						break;
					case 11:
						pFiledMap->valueList.push_back(pItem->sResponer);
						break;
					case 12:
						sprintf_s(ch, 64, "%d", pItem->nMaxRunNUM);
						pFiledMap->valueList.push_back(ch);
						break;
					case 13:
						pFiledMap->valueList.push_back(pItem->sDriverIP);
						break;
					case 14:
						sprintf_s(ch, 64, "%d", pItem->nDriverPort);
						pFiledMap->valueList.push_back(ch);
						break;
					case 15:
						sprintf_s(ch, 64, "%d", pItem->nDeviceProtocol);
						pFiledMap->valueList.push_back(ch);
						break;
					case 16:
						sprintf_s(ch, 64, "%d", pItem->bIsintegrated);
						pFiledMap->valueList.push_back(ch);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__querydeviceresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryDevice接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_REF_STATION_TYPE(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryRefStationType			_Xdmrtm__queryrefstationtype;
	Xdmrtm::_Xdmrtm__QueryRefStationTypeResponse	_Xdmrtm__queryrefstationtyperesponse;
	int lnsoap = lpsvr->QueryRefStationType(&_Xdmrtm__queryrefstationtype, &_Xdmrtm__queryrefstationtyperesponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryrefstationtyperesponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_REF_STATION_TYPE_COLUMN_NOID     	,
				BOSD_T_RIM_RTM_REF_STATION_TYPE_COLUMN_NAME     	,
			};						
			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryrefstationtyperesponse.RefStationTypeArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryrefstationtyperesponse.RefStationTypeArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__RefStationType* pItem = _Xdmrtm__queryrefstationtyperesponse.RefStationTypeArray->item[j];

					switch(filedIndex)
					{
					case 0:
						sprintf_s(ch, 64, "%d", pItem->nType);
						pFiledMap->valueList.push_back(ch);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryrefstationtyperesponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryRefStationType接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_REF_DEVICE_TYPE(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryRefDeviceType			_Xdmrtm__queryrefdevicetype;
	Xdmrtm::_Xdmrtm__QueryRefDeviceTypeResponse	_Xdmrtm__queryrefdevicetyperesponse;
	int lnsoap = lpsvr->QueryRefDeviceType(&_Xdmrtm__queryrefdevicetype, &_Xdmrtm__queryrefdevicetyperesponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryrefdevicetyperesponse.nResult) == 0)
		{

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_REF_DEVICE_TYPE_COLUMN_NOID      	,
				BOSD_T_RIM_RTM_REF_DEVICE_TYPE_COLUMN_NAME      	,
			};						
			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryrefdevicetyperesponse.RefDeviceTypeArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryrefdevicetyperesponse.RefDeviceTypeArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__RefDeviceType* pItem = _Xdmrtm__queryrefdevicetyperesponse.RefDeviceTypeArray->item[j];

					switch(filedIndex)
					{
					case 0:
						sprintf_s(ch, 64, "%d", pItem->nType);
						pFiledMap->valueList.push_back(ch);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryrefdevicetyperesponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryRefDeviceType接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_REF_DEVICE_PROTOCOL(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryRefDeviceProtocolType			_Xdmrtm__queryrefdeviceprotocoltype;
	Xdmrtm::_Xdmrtm__QueryRefDeviceProtocolTypeResponse	_Xdmrtm__queryrefdeviceprotocoltyperesponse;
	int lnsoap = lpsvr->QueryRefDeviceProtocolType(&_Xdmrtm__queryrefdeviceprotocoltype, &_Xdmrtm__queryrefdeviceprotocoltyperesponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryrefdeviceprotocoltyperesponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NOID       	,
				BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NAME       	,
				BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NOTE			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryrefdeviceprotocoltyperesponse.RefProtocolTypeArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryrefdeviceprotocoltyperesponse.RefProtocolTypeArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__RefDeviceProtocol* pItem = _Xdmrtm__queryrefdeviceprotocoltyperesponse.RefProtocolTypeArray->item[j];

					switch(filedIndex)
					{
					case 0:
						sprintf_s(ch, 64, "%d", pItem->nProtocolType);
						pFiledMap->valueList.push_back(ch);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					case 2:
						pFiledMap->valueList.push_back("");
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryrefdeviceprotocoltyperesponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryRefDeviceProtocolType接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryRefDeviceModelFuncParam			_Xdmrtm__queryrefdevicemodelfuncparam;
	Xdmrtm::_Xdmrtm__QueryRefDeviceModelFuncParamResponse	_Xdmrtm__queryrefdevicemodelfuncparamresponse;
	int lnsoap = lpsvr->QueryRefDeviceModelFuncParam(&_Xdmrtm__queryrefdevicemodelfuncparam, &_Xdmrtm__queryrefdevicemodelfuncparamresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryrefdevicemodelfuncparamresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_DEVICE        	,
				BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_FUNC          	,
				BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_PARAM  			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryrefdevicemodelfuncparamresponse.RefDeviceModelFuncArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryrefdevicemodelfuncparamresponse.RefDeviceModelFuncArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__RefDeviceModelFuncParam* pItem = _Xdmrtm__queryrefdevicemodelfuncparamresponse.RefDeviceModelFuncArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sModel);
						break;
					case 1:
						sprintf_s(ch, 64, "%d", pItem->nFunc);
						pFiledMap->valueList.push_back(ch);
						break;
					case 2:
						//pItem->sParam.replace();
						pFiledMap->valueList.push_back(pItem->sParam);
						//pFiledMap->valueList.push_back("<?xml version=\"1.0\" encoding=\"gb2312\"?><!-- 璁惧鍙傛暟妗嗘灦  --><config><device name=\"RXMD865P\"><param><");
						//pFiledMap->valueList.push_back("xml");
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryrefdevicemodelfuncparamresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryRefDeviceModelFuncParam接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_REF_DEVICE_MODEL(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryRefDeviceModel			_Xdmrtm__queryrefdevicemodel;
	Xdmrtm::_Xdmrtm__QueryRefDeviceModelResponse	_Xdmrtm__queryrefdevicemodelresponse;
	int lnsoap = lpsvr->QueryRefDeviceModel(&_Xdmrtm__queryrefdevicemodel, &_Xdmrtm__queryrefdevicemodelresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryrefdevicemodelresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NOID        	,
				BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NAME        	,
				BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NOTE 		,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryrefdevicemodelresponse.RefDeviceModelArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryrefdevicemodelresponse.RefDeviceModelArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__RefDeviceModel* pItem = _Xdmrtm__queryrefdevicemodelresponse.RefDeviceModelArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sModel);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					case 2:
						pFiledMap->valueList.push_back(pItem->sNote);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryrefdevicemodelresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryRefDeviceModel接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_REF_DEVICE_FUNC(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryRefDeviceFunc			_Xdmrtm__queryrefdevicefunc;
	Xdmrtm::_Xdmrtm__QueryRefDeviceFuncResponse	_Xdmrtm__queryrefdevicefuncresponse;
	int lnsoap = lpsvr->QueryRefDeviceFunc(&_Xdmrtm__queryrefdevicefunc, &_Xdmrtm__queryrefdevicefuncresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryrefdevicefuncresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_NOID        	,
				BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_NAME        	,
				BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_KEY  			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryrefdevicefuncresponse.RefDeviceFuncArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryrefdevicefuncresponse.RefDeviceFuncArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__RefDeviceFunc* pItem = _Xdmrtm__queryrefdevicefuncresponse.RefDeviceFuncArray->item[j];

					switch(filedIndex)
					{
					case 0:
						sprintf_s(ch, 64, "%d", pItem->nFuncID);
						pFiledMap->valueList.push_back(ch);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sFuncName);
						break;
					case 2:
						pFiledMap->valueList.push_back(pItem->sFuncKey);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryrefdevicefuncresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryRefDeviceFunc接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_MON_ORG(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryMonOrganization			_Xdmrtm__querymonorganization;
	Xdmrtm::_Xdmrtm__QueryMonOrganizationResponse	_Xdmrtm__querymonorganizationresponse;
	int lnsoap = lpsvr->QueryMonOrganization(&_Xdmrtm__querymonorganization, &_Xdmrtm__querymonorganizationresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__querymonorganizationresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_NO                  	,
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_NAME                	,
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_ADDRESS      			,
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_POST         			,
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_RESPONPERSON 			,
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_CONTACT      			,
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_BRIEF        			,
				BOSD_T_RIM_RTM_MON_ORG_COLUMN_NOTE         			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__querymonorganizationresponse.MonOrganizationArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__querymonorganizationresponse.MonOrganizationArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__MonOrganization* pItem = _Xdmrtm__querymonorganizationresponse.MonOrganizationArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sOrgNO);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					case 2:
						pFiledMap->valueList.push_back(pItem->sAddress);
						break;
					case 3:
						pFiledMap->valueList.push_back(pItem->sPost);
						break;
					case 4:
						pFiledMap->valueList.push_back(pItem->sResponpersion);
						break;
					case 5:
						pFiledMap->valueList.push_back(pItem->sContact);
						break;
					case 6:
						pFiledMap->valueList.push_back(pItem->sBrief);
						break;
					case 7:
						pFiledMap->valueList.push_back(pItem->sNote);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__querymonorganizationresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryMonOrganization接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_MON_CONTROL_CENTER(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryMonControlCenter			_Xdmrtm__querymoncontrolcenter;
	Xdmrtm::_Xdmrtm__QueryMonControlCenterResponse	_Xdmrtm__querymoncontrolcenterresponse;
	int lnsoap = lpsvr->QueryMonControlCenter(&_Xdmrtm__querymoncontrolcenter, &_Xdmrtm__querymoncontrolcenterresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__querymoncontrolcenterresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_NAME            	,
				BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_AREACODE        	,
				BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_ADDRESS  			,
				BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_LONG     			,
				BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_LATI     			,
				BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_NOTE     			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__querymoncontrolcenterresponse.MonControlCenterArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__querymoncontrolcenterresponse.MonControlCenterArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__MonControlCenter* pItem = _Xdmrtm__querymoncontrolcenterresponse.MonControlCenterArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sCenterNO);
						break;
					case 2:
						pFiledMap->valueList.push_back(pItem->sAddress);
						break;
					case 3:
						sprintf_s(ch, 64, "%f", pItem->fLong);
						pFiledMap->valueList.push_back(ch);
						break;
					case 4:
						sprintf_s(ch, 64, "%f", pItem->fLati);
						pFiledMap->valueList.push_back(ch);
						break;
					case 5:
						pFiledMap->valueList.push_back("");
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__querymoncontrolcenterresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryMonControlCenter接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_DEVICE_FUNC_ANTENNA(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryDeviceFuncAnt			_Xdmrtm__querydevicefuncant;
	Xdmrtm::_Xdmrtm__QueryDeviceFuncAntResponse	_Xdmrtm__querydevicefuncantresponse;
	int lnsoap = lpsvr->QueryDeviceFuncAnt(&_Xdmrtm__querydevicefuncant, &_Xdmrtm__querydevicefuncantresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__querydevicefuncantresponse.nResult) == 0)
		{

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_STATIONNO         	,
				BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_DEVICENO          	,
				BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_FUNCNO     			,
				BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_ANTKEYWORD 			,
				BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_ANTNAME    			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__querydevicefuncantresponse.DeviceFuncAntArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__querydevicefuncantresponse.DeviceFuncAntArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__DeviceFuncAntenna* pItem = _Xdmrtm__querydevicefuncantresponse.DeviceFuncAntArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sStationNO);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sDeviceNO);
						break;
					case 2:
						sprintf_s(ch, 64, "%d", pItem->nFunNO);
						pFiledMap->valueList.push_back(ch);
						break;
					case 3:
						pFiledMap->valueList.push_back(pItem->sDesc);
						break;
					case 4:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__querydevicefuncantresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryDeviceFuncAnt接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_DEVICE_FUNC(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryDeviceFunc			_Xdmrtm__querydevicefunc;
	Xdmrtm::_Xdmrtm__QueryDeviceFuncResponse	_Xdmrtm__querydevicefuncresponse;
	int lnsoap = lpsvr->QueryDeviceFunc(&_Xdmrtm__querydevicefunc, &_Xdmrtm__querydevicefuncresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__querydevicefuncresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_STATIONNO       	,
				BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_DEVICENO        	,
				BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_FUNCNO   			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__querydevicefuncresponse.DeviceFuncArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__querydevicefuncresponse.DeviceFuncArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__DeviceFunc* pItem = _Xdmrtm__querydevicefuncresponse.DeviceFuncArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sStationNO);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sDeviceNO);
						break;
					case 2:
						sprintf_s(ch, 64, "%d", pItem->nFunNO);
						pFiledMap->valueList.push_back(ch);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__querydevicefuncresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryDeviceFunc接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_ANTENNA(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryAnt			_Xdmrtm__queryant;
	Xdmrtm::_Xdmrtm__QueryAntResponse	_Xdmrtm__queryantresponse;
	int lnsoap = lpsvr->QueryAnt(&_Xdmrtm__queryant, &_Xdmrtm__queryantresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryantresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_ANTENNA_COLUMN_STATIONNO          	,
				BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTNAME            	,
				BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTFACTOR   			,
				BOSD_T_RIM_RTM_ANTENNA_COLUMN_STARTFREQ   			,
				BOSD_T_RIM_RTM_ANTENNA_COLUMN_STOPFREQ    			,
				BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTHEIGHT   			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryantresponse.AntArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryantresponse.AntArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__Ant* pItem = _Xdmrtm__queryantresponse.AntArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sStationNO);
						break;
					case 1:
						pFiledMap->valueList.push_back(pItem->sAntName);
						break;
					case 2:
						pFiledMap->valueList.push_back(pItem->sAntFactor);
						break;
					case 3:
						sprintf_s(ch, 64, "%lld", pItem->nBeginFreq);
						pFiledMap->valueList.push_back(ch);
						break;
					case 4:
						sprintf_s(ch, 64, "%lld", pItem->nEndFreq);
						pFiledMap->valueList.push_back(ch);
						break;
					case 5:
						sprintf_s(ch, 64, "%d", pItem->nAntHeight);
						pFiledMap->valueList.push_back(ch);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryantresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryAnt接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_COM_FREQ_LAYOUT(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	Xdmrtm::_Xdmrtm__QueryFreqLayout			_Xdmrtm__queryfreqlayout;
	Xdmrtm::_Xdmrtm__QueryFreqLayoutResponse	_Xdmrtm__queryfreqlayoutresponse;
	int lnsoap = lpsvr->QueryFreqLayout(&_Xdmrtm__queryfreqlayout, &_Xdmrtm__queryfreqlayoutresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryfreqlayoutresponse.nResult) == 0)
		{
			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_LAYOUTNAME         	,
				BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_STARTFREQ          	,
				BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_ENDFREQ     			,
				BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_GROUPNAME   			,
				BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_DISTINCE    			,
				BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_EMITWIDTH   			,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryfreqlayoutresponse.FreqLayoutArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryfreqlayoutresponse.FreqLayoutArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__FreqLayout* pItem = _Xdmrtm__queryfreqlayoutresponse.FreqLayoutArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sName);
						break;
					case 1:
						sprintf_s(ch, 64, "%lld", pItem->nBeginFreq);
						pFiledMap->valueList.push_back(ch);
						break;
					case 2:
						sprintf_s(ch, 64, "%lld", pItem->nEndFreq);
						pFiledMap->valueList.push_back(ch);
						break;
					case 3:
						pFiledMap->valueList.push_back(pItem->sGroupName);
						break;
					case 4:
						sprintf_s(ch, 64, "%lld", pItem->nStep);
						pFiledMap->valueList.push_back(ch);
						break;
					case 5:
						sprintf_s(ch, 64, "%lld", pItem->nEmitWidth);
						pFiledMap->valueList.push_back(ch);
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryfreqlayoutresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryFreqLayout接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_COM_AUTH_SYS_DATA_STATION(const std::string lstrUser, const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& errorInfo)
{
	std::auto_ptr<Xdmrtm::XdmrtmProxy> lpsvr(new Xdmrtm::XdmrtmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();


	char user[512] = {0};
	sprintf_s(user, 512, "%s", lstrUser.c_str());
	Xdmrtm::_Xdmrtm__QueryUserLicenseStation			_Xdmrtm__queryuserlicensestation;
	_Xdmrtm__queryuserlicensestation.sUser				= user;
	Xdmrtm::_Xdmrtm__QueryUserLicenseStationResponse	_Xdmrtm__queryuserlicensestationresponse;
	int lnsoap = lpsvr->QueryUserLicenseStation(&_Xdmrtm__queryuserlicensestation, &_Xdmrtm__queryuserlicensestationresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		if(RIM_LOSHORT(_Xdmrtm__queryuserlicensestationresponse.nResult) == 0)
		{

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION_COLUMN_NO             	,
				BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION_COLUMN_NOTE           	,
			};						

			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _Xdmrtm__queryuserlicensestationresponse.StationArray->__sizeitem; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _Xdmrtm__queryuserlicensestationresponse.StationArray->__sizeitem; ++j)
				{
					Xdmrtm::Xdmrtm__LicenseStation* pItem = _Xdmrtm__queryuserlicensestationresponse.StationArray->item[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(pItem->sStationNO);
						break;
					case 1:
						pFiledMap->valueList.push_back("");
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else
		{
			errorInfo	= _Xdmrtm__queryuserlicensestationresponse.sResult;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "QueryUserLicenseStation接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM30_USERINFO(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo)
{
	std::auto_ptr<Bsdsysservice::BSDSYSSoap12Proxy> lpsvr(new Bsdsysservice::BSDSYSSoap12Proxy(SOAP_C_UTFSTRING));
	lpsvr->soap_endpoint = lstrURL.c_str();

	Bsdsysservice::_Bsdsysservice__GetUserList3			_bsdsysservice__getuserlist3;
	_bsdsysservice__getuserlist3.update					= updateDate;
	Bsdsysservice::_Bsdsysservice__GetUserList3Response	_bsdsysservice__getuserlistresponse;
	int lnsoap = lpsvr->GetUserList3(&_bsdsysservice__getuserlist3, &_bsdsysservice__getuserlistresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		string result	= UTF82ANSI(_bsdsysservice__getuserlistresponse.GetUserList3Result->Result.c_str());
		string msg		= UTF82ANSI(_bsdsysservice__getuserlistresponse.GetUserList3Result->Msg.c_str());
		if (atoi(result.c_str()) == 1)
		{
			updateDate = _bsdsysservice__getuserlistresponse.GetUserList3Result->UpdateDate;

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM30_USERINFO_USERID	        		,
				BOSD_T_RIM30_USERINFO_USERNAME					,
				BOSD_T_RIM30_USERINFO_EMPNO						,
				BOSD_T_RIM30_USERINFO_AREACODE					,
				BOSD_T_RIM30_USERINFO_DEPTID					,
				BOSD_T_RIM30_USERINFO_DEPTNAME					,
			};	

			int filedCount = sizeof(fields)/256;
			for (int filedIndex = 0; filedIndex < filedCount && _bsdsysservice__getuserlistresponse.GetUserList3Result->Data->__sizeV_USCOREComm_USCOREUserListEntity; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _bsdsysservice__getuserlistresponse.GetUserList3Result->Data->__sizeV_USCOREComm_USCOREUserListEntity; ++j)
				{
					Bsdsysservice::Bsdsysservice__V_USCOREComm_USCOREUserListEntity* pItem = _bsdsysservice__getuserlistresponse.GetUserList3Result->Data->V_USCOREComm_USCOREUserListEntity[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->UserID));
						break;
					case 1:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->UserName));
						break;
					case 2:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->EmpNO));
						break;
					case 3:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->AreaCode));
						break;
					case 4:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->DeptID));
						break;
					case 5:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->DeptName));
						break;
					}
				}
				tableInfoVec.push_back(pFiledMap);
			}
		}
		else if (atoi(result.c_str()) == 3)		/////无数据，无需更新
		{
		}
		else
		{
			errorInfo	= msg;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "GetUserList接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM30_BRANCHINFO(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo)
{
	std::auto_ptr<Bsdsysservice::BSDSYSSoap12Proxy> lpsvr(new Bsdsysservice::BSDSYSSoap12Proxy(SOAP_C_UTFSTRING));
	lpsvr->soap_endpoint = lstrURL.c_str();

	Bsdsysservice::_Bsdsysservice__GetDepartmentList3			_bsdsysservice__getdepartmentlist;
	_bsdsysservice__getdepartmentlist.update					= updateDate;
	Bsdsysservice::_Bsdsysservice__GetDepartmentList3Response	_bsdsysservice__getdepartmentlistresponse;
	int lnsoap = lpsvr->GetDepartmentList3(&_bsdsysservice__getdepartmentlist, &_bsdsysservice__getdepartmentlistresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		string result	= UTF82ANSI(_bsdsysservice__getdepartmentlistresponse.GetDepartmentList3Result->Result.c_str());
		string msg		= UTF82ANSI(_bsdsysservice__getdepartmentlistresponse.GetDepartmentList3Result->Msg.c_str());
		if (atoi(result.c_str()) == 1)
		{
			updateDate = _bsdsysservice__getdepartmentlistresponse.GetDepartmentList3Result->UpdateDate;

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM30_BRANCHINFO_DEPTNAME	  			,
				BOSD_T_RIM30_BRANCHINFO_PARENTID	    		,
				BOSD_T_RIM30_BRANCHINFO_TEL						,
				BOSD_T_RIM30_BRANCHINFO_FAX						,
				BOSD_T_RIM30_BRANCHINFO_EMAIL					,
				BOSD_T_RIM30_BRANCHINFO_MANAGER					,
				BOSD_T_RIM30_BRANCHINFO_NOTE					,
				BOSD_T_RIM30_BRANCHINFO_DEPTLEVEL				,
				BOSD_T_RIM30_BRANCHINFO_DEPTID					,
				BOSD_T_RIM30_BRANCHINFO_DELFLAG					,	
			};	

			int filedCount = sizeof(fields)/256;
			for (int filedIndex = 0; filedIndex < filedCount && _bsdsysservice__getdepartmentlistresponse.GetDepartmentList3Result->Data->__sizeComm_USCOREdepartmentEntity; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _bsdsysservice__getdepartmentlistresponse.GetDepartmentList3Result->Data->__sizeComm_USCOREdepartmentEntity; ++j)
				{
					Bsdsysservice::Bsdsysservice__Comm_USCOREdepartmentEntity* pItem = _bsdsysservice__getdepartmentlistresponse.GetDepartmentList3Result->Data->Comm_USCOREdepartmentEntity[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Deptname));
						break;
					case 1:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Parentid));
						break;
					case 2:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Tel));
						break;
					case 3:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Fax));
						break;
					case 4:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Email));
						break;
					case 5:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Manager));
						break;
					case 6:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Note));
						break;
					case 7:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Deptlevel));
						break;
					case 8:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Deptid));
						break;
					case 9:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Delflag));
						break;
					}
				}
				tableInfoVec.push_back(pFiledMap);
			}
		}
		else if (atoi(result.c_str()) == 3)		/////无数据，无需更新
		{
		}
		else
		{
			errorInfo	= msg;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "GetDepartmentList接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_RTM_RNC(const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo)
{
	std::auto_ptr<Bsdrtmservice::BSDRTMSoap12Proxy> lpsvr(new Bsdrtmservice::BSDRTMSoap12Proxy(SOAP_C_UTFSTRING));
	lpsvr->soap_endpoint = lstrURL.c_str();

	Bsdrtmservice::_Bsdrtmservice1__GetAllRNCs3			_bsdrtmservice1__getallrncs;
	_bsdrtmservice1__getallrncs.update					= updateDate;
	Bsdrtmservice::_Bsdrtmservice1__GetAllRNCs3Response	_bsdrtmservice1__getallrncsresponse;
	int lnsoap = lpsvr->GetAllRNCs3(&_bsdrtmservice1__getallrncs, &_bsdrtmservice1__getallrncsresponse);


	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		string result	= UTF82ANSI(_bsdrtmservice1__getallrncsresponse.GetAllRNCs3Result->Result.c_str());
		string msg		= UTF82ANSI(_bsdrtmservice1__getallrncsresponse.GetAllRNCs3Result->Msg.c_str());
		if (atoi(result.c_str()) == 1)
		{
			updateDate = _bsdrtmservice1__getallrncsresponse.GetAllRNCs3Result->UpdateDate;

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_RTM_RNC_COLUMN_STATIONNO          	,
				BOSD_T_RIM_RTM_RNC_COLUMN_RNCNAME            	,
				BOSD_T_RIM_RTM_RNC_COLUMN_RNCIP              	,
				BOSD_T_RIM_RTM_RNC_COLUMN_RNCPORT            	,
				BOSD_T_RIM_RTM_RNC_COLUMN_CTRLDEVICENAME     	,
				BOSD_T_RIM_RTM_RNC_COLUMN_CTRLPORT           	,
				BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSHAS          	,
				BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSIP           	,
				BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSPORT         	,
				"CTRLTYPE"										,

			};						
			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _bsdrtmservice1__getallrncsresponse.GetAllRNCs3Result->Data->__sizevRtmRncPort2Entity; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _bsdrtmservice1__getallrncsresponse.GetAllRNCs3Result->Data->__sizevRtmRncPort2Entity; ++j)
				{
					Bsdrtmservice::Bsdrtmservice1__vRtmRncPort2Entity* pItem = _bsdrtmservice1__getallrncsresponse.GetAllRNCs3Result->Data->vRtmRncPort2Entity[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Monstationno));
						break;
					case 1:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Rncname));
						break;
					case 2:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Rncip));
						break;
					case 3:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Rncport));
						break;
					case 4:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Devname));
						break;
					case 5:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Powerport));
						break;
					case 6:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Hasos));
						break;
					case 7:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Osip));
						break;
					case 8:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Osport));
						break;
					case 9:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem->Rnctype));
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else if (atoi(result.c_str()) == 3)		/////无数据，无需更新
		{
		}
		else
		{
			errorInfo	= msg;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "GetAllRNCs接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

bool CGsoapServer::FieldMapT_RIM_COM_AUTH_SYS_FUNC(const std::string lstrUser, const std::string lstrURL, vector<FILEDMAP*> &tableInfoVec, std::string tableName, std::string& updateDate, std::string& errorInfo)
{
	std::auto_ptr<Bsdsysservice::BSDSYSSoap12Proxy> lpsvr(new Bsdsysservice::BSDSYSSoap12Proxy(SOAP_C_UTFSTRING));
	lpsvr->soap_endpoint = lstrURL.c_str();

	Bsdsysservice::_Bsdsysservice__GetFunctions3			_bsdsysservice__getfunctions;
	_bsdsysservice__getfunctions.userID						= lstrUser;
	_bsdsysservice__getfunctions.update						= updateDate;
	Bsdsysservice::_Bsdsysservice__GetFunctions3Response	_bsdsysservice__getfunctionsresponse;
	int lnsoap = lpsvr->GetFunctions3(&_bsdsysservice__getfunctions, &_bsdsysservice__getfunctionsresponse);

	bool bRet = true;
	if(lnsoap == SOAP_OK)
	{
		string result	= UTF82ANSI(_bsdsysservice__getfunctionsresponse.GetFunctions3Result->Result.c_str());
		string msg		= UTF82ANSI(_bsdsysservice__getfunctionsresponse.GetFunctions3Result->Msg.c_str());
		if (atoi(result.c_str()) == 1)
		{
			updateDate = _bsdsysservice__getfunctionsresponse.GetFunctions3Result->UpdateDate;

			char ch[64]			= {0}; 
			char fields[][256]	= {
				BOSD_T_RIM_COM_AUTH_SYS_FUNC_COLUMN_NO            	,
				BOSD_T_RIM_COM_AUTH_SYS_FUNC_COLUMN_NOTE          	,
			};						
			int filedCount = sizeof(fields)/256;
			////////先构造  字段----值列表    vector		
			for (int filedIndex = 0; filedIndex < filedCount && _bsdsysservice__getfunctionsresponse.GetFunctions3Result->Data->__sizestring; ++filedIndex)					
			{
				FILEDMAP* pFiledMap		= new FILEDMAP;
				pFiledMap->sFiled		= fields[filedIndex];

				for (int j = 0; j < _bsdsysservice__getfunctionsresponse.GetFunctions3Result->Data->__sizestring; ++j)
				{
					string pItem = _bsdsysservice__getfunctionsresponse.GetFunctions3Result->Data->string[j];

					switch(filedIndex)
					{
					case 0:
						pFiledMap->valueList.push_back(UTF82ANSI(pItem));
						break;
					case 1:
						pFiledMap->valueList.push_back(UTF82ANSI(""));
						break;
					}
				}

				tableInfoVec.push_back(pFiledMap);
			}
		}
		else if (atoi(result.c_str()) == 3)		/////无数据，无需更新
		{
		}
		else
		{
			errorInfo	= msg;
			bRet		= false;
		}
	}
	else
	{
		errorInfo		= "GetFunctions3接口调用出错！";
		bRet = false;
	}

	lpsvr->destroy();

	return bRet;
}

/////////////////////////开放接口///////////////////////////
bool CGsoapServer::VerifyUser(const std::string lstrURL, const std::string userName, std::string password, std::string& userToken, std::string& errorInfo)
{
	std::auto_ptr<Bsdsysservice::BSDSYSSoap12Proxy> lpsvr(new Bsdsysservice::BSDSYSSoap12Proxy(SOAP_C_UTFSTRING));
	lpsvr->soap_endpoint = lstrURL.c_str();

	Bsdsysservice::_Bsdsysservice__VerifyUser			_bsdsysservice__verifyuser;
	_bsdsysservice__verifyuser.userID					= userName;
	_bsdsysservice__verifyuser.password					= password;
	Bsdsysservice::_Bsdsysservice__VerifyUserResponse	_bsdsysservice__verifyuserresponse;
	int lnsoap = lpsvr->VerifyUser(&_bsdsysservice__verifyuser, &_bsdsysservice__verifyuserresponse);
	if(lnsoap != SOAP_OK)
	{
		errorInfo = "服务验证失败!";
		return false;
	}

	string result	= UTF82ANSI(_bsdsysservice__verifyuserresponse.VerifyUserResult->Result);
	errorInfo		= UTF82ANSI(_bsdsysservice__verifyuserresponse.VerifyUserResult->Msg);
	if(atoi(result.c_str()) == 1)
		userToken		= UTF82ANSI(_bsdsysservice__verifyuserresponse.VerifyUserResult->Data->Comm_USCOREuserEntity[0]->UserToken);

	lpsvr->destroy();
	return atoi(result.c_str()) == 1;
}

bool CGsoapServer::SysncSqlite(const std::string lstrUser, const std::string lstrURL, std::string tableName, std::string& updateDate, std::string& errorInfo)
{
	if(false == ::PathIsURLA(lstrURL.c_str()))
		return false;

	bool				bReturn = true;
	vector<FILEDMAP*>	tableInfoVec;

	do 
	{
		if(tableName == BOSD_T_RIM_RTM_STATION)
		{
			bReturn = FieldMapT_RIM_RTM_STATION(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_REF_STATION_TYPE)
		{
			bReturn = FieldMapT_RIM_RTM_REF_STATION_TYPE(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_REF_DEVICE_TYPE)
		{
			bReturn = FieldMapT_RIM_RTM_REF_DEVICE_TYPE(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL)
		{
			bReturn = FieldMapT_RIM_RTM_REF_DEVICE_PROTOCOL(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM		||
				tableName == BOSD_T_RIM_RTM_REF_DEVICE_MODEL				||
				tableName == BOSD_T_RIM_RTM_REF_DEVICE_FUNC		
			)
		{
			////因为有外键   先清包含外键的表，再清外键表，再写外键表，再写包含外键的表
			if(!DeleteTable(tableName))
				return false;

			{	//更新表 BOSD_T_RIM_RTM_REF_DEVICE_MODEL
				if(	!FieldMapT_RIM_RTM_REF_DEVICE_MODEL(lstrURL, tableInfoVec, BOSD_T_RIM_RTM_REF_DEVICE_MODEL, errorInfo))
				{
					bReturn = false;
					break;
				}

				if(tableInfoVec.size() && !UpdateTable(tableInfoVec, BOSD_T_RIM_RTM_REF_DEVICE_MODEL))
				{
					char ch[MAX_PATH] = {0};
					sprintf_s(ch, MAX_PATH, "更新表%s失败！", BOSD_T_RIM_RTM_REF_DEVICE_MODEL);
					errorInfo = ch;
					bReturn = false;
					break;
				}

				for(UINT i = 0; i < tableInfoVec.size(); ++i)
				{
					FILEDMAP *pFileMap = tableInfoVec.at(i);
					if(pFileMap)
						delete pFileMap;
				}
				tableInfoVec.clear();
			}

			{	//更新表 BOSD_T_RIM_RTM_REF_DEVICE_FUNC
				if(	!FieldMapT_RIM_RTM_REF_DEVICE_FUNC(lstrURL, tableInfoVec, BOSD_T_RIM_RTM_REF_DEVICE_FUNC, errorInfo))
				{
					bReturn = false;
					break;
				}

				if(tableInfoVec.size() && !UpdateTable(tableInfoVec, BOSD_T_RIM_RTM_REF_DEVICE_FUNC))
				{
					char ch[MAX_PATH] = {0};
					sprintf_s(ch, MAX_PATH, "更新表%s失败！", BOSD_T_RIM_RTM_REF_DEVICE_FUNC);
					errorInfo = ch;
					bReturn = false;
					break;
				}

				for(UINT i = 0; i < tableInfoVec.size(); ++i)
				{
					FILEDMAP *pFileMap = tableInfoVec.at(i);
					if(pFileMap)
						delete pFileMap;
				}
				tableInfoVec.clear();
			}
			
			tableName = BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM;
			if(!FieldMapT_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM(lstrURL, tableInfoVec, tableName, errorInfo))
			{
				bReturn = false;
				break;
			}
		}
		else if(tableName == BOSD_T_RIM_RTM_MON_ORG)
		{
			//有外键存在于 T_RIM_COM_REGION_NO 中  但认为T_RIM_COM_REGION_NO表永远恒定不变
			bReturn = FieldMapT_RIM_RTM_MON_ORG(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_MON_CONTROL_CENTER)
		{
			//有外键存在于 T_RIM_COM_REGION_NO 中  但认为T_RIM_COM_REGION_NO表永远恒定不变
			bReturn = FieldMapT_RIM_RTM_MON_CONTROL_CENTER(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA)
		{
			bReturn = FieldMapT_RIM_RTM_DEVICE_FUNC_ANTENNA(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_DEVICE_FUNC)
		{
			bReturn = FieldMapT_RIM_RTM_DEVICE_FUNC(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_DEVICE)
		{
			bReturn = FieldMapT_RIM_RTM_DEVICE(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_ANTENNA)
		{
			bReturn = FieldMapT_RIM_RTM_ANTENNA(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_COM_FREQ_LAYOUT)
		{
			bReturn = FieldMapT_RIM_COM_FREQ_LAYOUT(lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if (tableName == BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION)
		{	////DMS 实时更新表
			bReturn = FieldMapT_RIM_COM_AUTH_SYS_DATA_STATION(lstrUser, lstrURL, tableInfoVec, tableName, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_COM_AUTH_SYS_FUNC)
		{	////WEB SYS
			bReturn = FieldMapT_RIM_COM_AUTH_SYS_FUNC(lstrUser, lstrURL, tableInfoVec, tableName, updateDate, errorInfo);
		}
		else if(tableName == BOSD_T_RIM30_USERINFO)
		{	////WEB SYS
			bReturn = FieldMapT_RIM30_USERINFO(lstrURL, tableInfoVec, tableName, updateDate, errorInfo);
		}
		else if(tableName == BOSD_T_RIM30_BRANCHINFO)
		{	////WEB SYS
			bReturn = FieldMapT_RIM30_BRANCHINFO(lstrURL, tableInfoVec, tableName, updateDate, errorInfo);
		}
		else if(tableName == BOSD_T_RIM_RTM_RNC)
		{	////WEB RTM
			bReturn = FieldMapT_RIM_RTM_RNC(lstrURL, tableInfoVec, tableName, updateDate, errorInfo);
		}

		if(bReturn && tableInfoVec.size() && !UpdateTable(tableInfoVec, tableName))
		{
			char ch[MAX_PATH] = {0};
			sprintf_s(ch, MAX_PATH, "更新表%s失败！", tableName.c_str());
			errorInfo = ch;

			bReturn = false;
			break;
		}

	} while (0);

	for(UINT i = 0; i < tableInfoVec.size(); ++i)
	{
		FILEDMAP *pFileMap = tableInfoVec.at(i);
		if(pFileMap)
			delete pFileMap;
	}
	tableInfoVec.clear();

	return bReturn;
}

bool CGsoapServer::SysncT_RIM_COM_AUTH_SYS_DATA_STATION(const std::string lstrUser, const std::string lstrURL, std::string tableName, std::string& errorInfo)
{
	if(false == ::PathIsURLA(lstrURL.c_str()))
		return false;

	bool				bReturn = true;
	vector<FILEDMAP*>	tableInfoVec;

	do 
	{
		if(tableName == BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION)
		{
			bReturn = FieldMapT_RIM_COM_AUTH_SYS_DATA_STATION(lstrUser, lstrURL, tableInfoVec, tableName, errorInfo);
		}

		if(bReturn && tableInfoVec.size() && !UpdateTable(tableInfoVec, tableName))
		{
			char ch[MAX_PATH] = {0};
			sprintf_s(ch, MAX_PATH, "更新表%s失败！", tableName.c_str());
			errorInfo = ch;
			bReturn = false;
			break;
		}

	} while (0);

	for(UINT i = 0; i < tableInfoVec.size(); ++i)
	{
		FILEDMAP *pFileMap = tableInfoVec.at(i);
		if(pFileMap)
			delete pFileMap;
	}
	tableInfoVec.clear();

	return bReturn;
}

bool CGsoapServer::QueryAllSubSysAddr(const std::string lstrURL, map<DWORD, vector<LPWEBADDRINFO>> &m_webAddrInfoMap, std::string& errorInfo)
{
	DWORD		subIdArray[]	= {	
		Xdmsdm::Xdmsdm__SubSystemID__xDMS,		
		Xdmsdm::Xdmsdm__SubSystemID__xRMS,		
		Xdmsdm::Xdmsdm__SubSystemID__xMSA,		
		Xdmsdm::Xdmsdm__SubSystemID__xECS,		
		Xdmsdm::Xdmsdm__SubSystemID__xBSD,			
		Xdmsdm::Xdmsdm__SubSystemID__xFRS, 
		Xdmsdm::Xdmsdm__SubSystemID__xSMS, 
		Xdmsdm::Xdmsdm__SubSystemID__xLES, 
		Xdmsdm::Xdmsdm__SubSystemID__xOAS, 
		Xdmsdm::Xdmsdm__SubSystemID__xGIS, 
		Xdmsdm::Xdmsdm__SubSystemID__xNULL,
		99,		
	};

	int totalCnt = sizeof(subIdArray)/4;

	std::auto_ptr<Xdmsdm::XdmsdmProxy> lpsvr(new Xdmsdm::XdmsdmProxy());
	lpsvr->soap_endpoint = lstrURL.c_str();

	for (int i = 0; i < totalCnt; ++i)
	{
		Xdmsdm::_Xdmsdm__QueryWebService			_xdmsdm__querywebservice;
		_xdmsdm__querywebservice.eSubSystemID		= Xdmsdm::Xdmsdm__SubSystemID(subIdArray[i]);
		_xdmsdm__querywebservice.nSubSystemModID	= -1;
		Xdmsdm::_Xdmsdm__QueryWebServiceResponse	_xdmsdm__querywebserviceresponse;
		int lnsoap = lpsvr->QueryWebService(&_xdmsdm__querywebservice, &_xdmsdm__querywebserviceresponse);
		if(lnsoap != SOAP_OK)
		{
			errorInfo = "获取服务地址信息失败!";
			return false;
		}

		for (int j = 0; j < _xdmsdm__querywebserviceresponse.WebServiceInfoArray->__sizeitem; ++j)
		{
			Xdmsdm::Xdmsdm__WebServiceInfo* pItem = _xdmsdm__querywebserviceresponse.WebServiceInfoArray->item[j];
			
			LPWEBADDRINFO pInfo = new WEBADDRINFO;
			pInfo->nSubModeID	= pItem->nSubModeID;
			pInfo->sName		= pItem->sName;
			pInfo->sLocation	= pItem->sLocation;

			m_webAddrInfoMap[subIdArray[i]].push_back(pInfo);
		}
	}
	
	lpsvr->destroy();
	return true;
}

