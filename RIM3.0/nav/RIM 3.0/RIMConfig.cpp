//********************************************************************
// RIMCONFIG.CPP 文件注释
// 文件名 　: RIMCONFIG.CPP
// 文件路径 : E:\PROJECT\RIM3.0\SRC\NAV\RIM 3.0/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 14:29
// 文件描述 : 
//*********************************************************************
#include "StdAfx.h"
#include "RIMConfig.h"
#include "ThreadEmsClient.h"

void tagTRANSFILEINFO::InitTransFileInfoByPath(tagTRANSFILEINFO* pLogfileInfo, tstring m_applicationName, tstring DicOrPath /*= _T("")*/)
{
	if(pLogfileInfo == NULL)
		return;

	pLogfileInfo->applicationName	= m_applicationName;
	//pLogfileInfo->m_version			= serverconfig.m_version;

	if(IsFileExist(DicOrPath.c_str()))
	{
		pLogfileInfo->bDic			= FALSE;

		HANDLE hFile; // the file handle 
		DWORD dwFileSize; // temporary storage for file sizes 

		hFile = CreateFile(DicOrPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
		if (hFile != INVALID_HANDLE_VALUE) 
		{ 
			dwFileSize = GetFileSize(hFile, NULL); 
			CloseHandle(hFile);
			if(dwFileSize != 0xFFFFFFFF && dwFileSize != 0)
			{
				tstring	MD5Str					= _T("");
				//GetMd5(DicOrPath, MD5Str);

				LPFILEINFO pFileInfo		= new FILEINFO();
				pFileInfo->name 			= DicOrPath;	//如果是路径，则直接存路径
				pFileInfo->hash 			= MD5Str;
				pFileInfo->size 			= dwFileSize;
				pLogfileInfo->totalSize		= dwFileSize;
			}
		} 
	}
	else if(IsDicExist(DicOrPath.c_str()))
	{
		pLogfileInfo->bDic			= TRUE;
		pLogfileInfo->totalSize		=  GetFolderSize(DicOrPath.c_str());
	}
}

BOOL tagTRANSFILEINFO::SafeModifyTransFlag(TRANSFLAG flag)
{
	CSingleLock sLock(&m_cs, TRUE);

	if(transFlag <= flag)
	{
		transFlag = flag;		//所有的状态不能被修改为比当前状态更前的状态;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

std::string tagTRANSFILEINFO::MakeXmlStr(tagTRANSFILEINFO* pLogfileInfo)
{
	if(pLogfileInfo == NULL)
		return "";

	try 
	{
		USES_CONVERSION;

		TiXmlDocument	logXMLDoc;

		logXMLDoc.LoadFile(TIXML_ENCODING_LEGACY);
		logXMLDoc.Clear();

		TiXmlElement * pRootNode, *pNode;
		TiXmlText *pTempText;

		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );
		logXMLDoc.LinkEndChild( decl );

		pRootNode = new TiXmlElement("transfile");			
		logXMLDoc.LinkEndChild(pRootNode);

		pNode = new TiXmlElement("guidStr");			
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(CT2A(pLogfileInfo->guidStr.c_str()));      
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("updateCmdBase");
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(pLogfileInfo->updateCmdBase.c_str());
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("beginCmd");
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(pLogfileInfo->beginCmd.c_str());
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("cancelCmd");
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(pLogfileInfo->cancelCmd.c_str());
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("endCmd");
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(pLogfileInfo->endCmd.c_str());
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("nameCmdBase");
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(pLogfileInfo->nameCmdBase.c_str());
		pNode->LinkEndChild(pTempText);

		char ch[MAX_PATH] = {0};
		sprintf_s(ch, "%d", pLogfileInfo->bDic);
		pNode = new TiXmlElement("bDic");			
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(ch);      
		pNode->LinkEndChild(pTempText);

		_i64toa_s(pLogfileInfo->totalSize, ch, MAX_PATH, 10);
		pNode = new TiXmlElement("totalSize");
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(ch);
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("applicationname");			
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(CT2A(pLogfileInfo->applicationName.c_str()));      
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("version");			
		pRootNode->LinkEndChild(pNode);
		pTempText = new TiXmlText(CT2A(pLogfileInfo->version.c_str()));      
		pNode->LinkEndChild(pTempText);

		//if(pLogfileInfo->filesInfoVec.size())
		//{
		//	pFiles = new TiXmlElement("files");			
		//	pRootNode->LinkEndChild(pFiles);

		//	for(UINT i = 0; i < pLogfileInfo->filesInfoVec.size(); ++i)
		//	{
		//		LPFILEINFO pFileInfo = pLogfileInfo->filesInfoVec[i];

		//		pfile = new TiXmlElement("file");			
		//		pFiles->LinkEndChild(pfile);
		//		pfile->SetAttribute("name", CT2A(pFileInfo->name.c_str()));
		//		pfile->SetAttribute("hash", CT2A(pFileInfo->hash.c_str()));
		//		pfile->SetAttribute("size", pFileInfo->size);
		//	}
		//}

		TiXmlPrinter printer;
		logXMLDoc.Accept(&printer);
		string strXMl = printer.CStr();
		return strXMl;
	}
	catch (...)
	{
		return "";
	}

	return "";
}

bool tagTRANSFILEINFO::LoadFromXmlStr(string xmlStr, tagTRANSFILEINFO* pLogfileInfo)
{
	try
	{
		if(pLogfileInfo == NULL)
			return false;
		TiXmlDocument xmldoc;
		if(!xmldoc.Parse(xmlStr.c_str(), 0, TIXML_ENCODING_LEGACY))
		{
			return false;
		}
		///////////////////
		USES_CONVERSION;

		string strValue;
		TiXmlNode* root = NULL;
		TiXmlNode* node = NULL;

		TiXmlNode*		nodeelement	= NULL;
		TiXmlNode*		files		= NULL;
		TiXmlNode*		file		= NULL;
		TiXmlElement*	element		= NULL;
		const char*		pTxt		= NULL;

		bool bContinue = true;
		//==========get the root of *.xml==========================================
		if((root = xmldoc.FirstChild("transfile")) == NULL)
		{
			return false;
		}

		//开始获取文件信息
		if((nodeelement = root->FirstChild("guidStr")) && (element = nodeelement->ToElement()) )
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->guidStr = CA2T(strValue.c_str());
		}

		if ((nodeelement = root->FirstChild("updateCmdBase")) && (element = nodeelement->ToElement()))
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->updateCmdBase = strValue.c_str();
		}

		if ((nodeelement = root->FirstChild("beginCmd")) && (element = nodeelement->ToElement()))
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->beginCmd = strValue.c_str();
		}

		if ((nodeelement = root->FirstChild("cancelCmd")) && (element = nodeelement->ToElement()))
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->cancelCmd = strValue.c_str();
		}

		if ((nodeelement = root->FirstChild("endCmd")) && (element = nodeelement->ToElement()))
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->endCmd = strValue.c_str();
		}

		if ((nodeelement = root->FirstChild("nameCmdBase")) && (element = nodeelement->ToElement()))
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->nameCmdBase = strValue.c_str();
		}

		if((nodeelement = root->FirstChild("bDic")) && (element = nodeelement->ToElement()) )
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->bDic = atoi(strValue.c_str());
		}

		if ((nodeelement = root->FirstChild("totalSize")) && (element = nodeelement->ToElement()))
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->totalSize = _atoi64(strValue.c_str());
		}

		if((nodeelement = root->FirstChild("applicationname")) && (element = nodeelement->ToElement()) )
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->applicationName = CA2T(strValue.c_str());
		}

		if((nodeelement = root->FirstChild("version")) && (element = nodeelement->ToElement()) )
		{
			pTxt		= element->GetText();
			strValue	= pTxt;

			pLogfileInfo->version = CA2T(strValue.c_str());
		} 

		/*pLogfileInfo->filesInfoVec.clear();
		if(files = root->FirstChild("files"))
		{
			if(file = files->FirstChild("file"))
			{
				while(file)
				{
					if(element = file->ToElement())
					{
						LPFILEINFO pFileInfo = new FILEINFO();

						if((pTxt = element->Attribute("name")))
							pFileInfo->name = CA2T(pTxt);
						if((pTxt = element->Attribute("hash")))
							pFileInfo->hash = CA2T(pTxt);
						if((pTxt = element->Attribute("size")))
							pFileInfo->size = atoi(pTxt);

						pLogfileInfo->filesInfoVec.push_back(pFileInfo);
					}
					file = file->NextSibling();
				}
			}
		} */
	}
	catch (...)
	{
		return false;
	}

	return true;
}


const tstring	tagUSERINFO::UNKNOWNBRANCHSTR	= _T("便携用户");
CReadWriteLock	tagUSERINFO::IPListLock;

RIMConfig* RIMConfig::m_pRIMConfig			= NULL;	//当前配置实例
LPUSERINFO RIMConfig::m_pLogonUser			= NULL;	//当前登录的用户

RIMConfig* RIMConfig::GetInstance()
{
	if(m_pRIMConfig == NULL)
	{
		m_pRIMConfig = new RIMConfig();

		if(	m_pRIMConfig->ParseConfig(m_pRIMConfig->m_rimConfigPath) == FALSE)		//配置文件加载错误
		{
			delete m_pRIMConfig;
			return NULL;
		}

		//解析系统设置文件
		m_pRIMConfig->ParseLogonFile(m_pRIMConfig->m_logonFilePath);
		m_pRIMConfig->ParseRecentFile(m_pRIMConfig->m_recentFilePath);
	}

	return m_pRIMConfig;
}

void RIMConfig::ReleaseInstance()
{
	if(m_pRIMConfig)
	{
		delete m_pRIMConfig;
		m_pRIMConfig = NULL;
	}
}


RIMConfig::RIMConfig(void)
{
	m_rimConfigPath 			= RelativeToFullPath(CFG_RELATIVEPATH_RIMCONFIG);
	TCHAR tempPath[MAX_PATH]	= { 0 }; GetTempPath(MAX_PATH, tempPath);
	m_logonFilePath 			= tstring(tempPath) + _T("logon.txt");
	m_recentFilePath			= tstring(tempPath) + _T("recentStation.txt");
	m_modifyPassTheme			= _T("");		
	m_welcomeText				= _T("");		
	m_welcomeFontSize			= 10;

	{	////获取所有网卡的MAC
		DWORD				dwError;
		PIP_ADAPTER_INFO	pAdapterInfo = NULL;
		ULONG				uLen = 0;
		DWORD				adapterCount = 0;

		dwError	= GetAdaptersInfo(pAdapterInfo, &uLen);
		if (dwError == ERROR_BUFFER_OVERFLOW)
		{
			if (pAdapterInfo != NULL)
			{
				delete pAdapterInfo;
				pAdapterInfo = NULL;
			}

			pAdapterInfo = (PIP_ADAPTER_INFO)new char[uLen];
			if ((0 == GetAdaptersInfo(pAdapterInfo, &uLen)) && (pAdapterInfo != NULL))
			{
				CString strMacAddress;
				PIP_ADAPTER_INFO pCurAdapterInfo = pAdapterInfo;
				while(pCurAdapterInfo)
				{
					CString strDesp;
					strDesp = pCurAdapterInfo->Description;
					//网卡名.包含了信息
					int num = strDesp.Find(_T(" - "));
					if(num > 0)
						strDesp = strDesp.Mid(0,num);

					CString str;
					str.Format(_T("%02X%02X%02X%02X%02X%02X"),    
						pAdapterInfo->Address[0],  
						pAdapterInfo->Address[1],  
						pAdapterInfo->Address[2],  
						pAdapterInfo->Address[3],  
						pAdapterInfo->Address[4],  
						pAdapterInfo->Address[5]);  
	
					m_macArray.Add(str);
					adapterCount++;
					pCurAdapterInfo = pCurAdapterInfo->Next;
				}
			}

			if (pAdapterInfo != NULL)
			{
				delete pAdapterInfo;
				pAdapterInfo = NULL;
			}
		}

	}


	{	////初始化站点，设备，及设备功能信息
#ifdef _DEBUG
		m_hToolkitLibrary		= LoadLibrary(_T("RIM30ToolKitD.dll"));
#else
		m_hToolkitLibrary		= LoadLibrary(_T("RIM30ToolKit.dll"));
#endif
		///////////////////////////////////////////////////////////////以下为测试/////////////////////////////////////////////////////////////////
		_pQueryAllRtmMonOrg						QueryAllRtmMonOrg					= (_pQueryAllRtmMonOrg)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmMonOrg");
		_pQueryAllRtmMonCtrlCenter				QueryAllRtmMonCtrlCenter			= (_pQueryAllRtmMonCtrlCenter)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmMonCtrlCenter");
		_pQueryAllRtmStationDeviceFuncAnt		QueryAllRtmStationDeviceFuncAnt		= (_pQueryAllRtmStationDeviceFuncAnt)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmStationDeviceFuncAnt");
		_pQueryAllRtmAnt						QueryAllRtmAnt						= (_pQueryAllRtmAnt)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmAnt");
		_pQueryAllRtmRefDeviceModel				QueryAllRtmRefDeviceModel			= (_pQueryAllRtmRefDeviceModel)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmRefDeviceModel");
		_pQueryAllRtmRefDeviceModelFuncParam	QueryAllRtmRefDeviceModelFuncParam	= (_pQueryAllRtmRefDeviceModelFuncParam)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmRefDeviceModelFuncParam");
		_pQueryAllRtmRnc						QueryAllRtmRnc						= (_pQueryAllRtmRnc)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmRnc");
		_pQueryAllComAuthSysDataStation			QueryAllComAuthSysDataStation		= (_pQueryAllComAuthSysDataStation)GetProcAddress(m_hToolkitLibrary, "QueryAllComAuthSysDataStation");
		_pQueryAllComAuthSysFunc				QueryAllComAuthSysFunc				= (_pQueryAllComAuthSysFunc)GetProcAddress(m_hToolkitLibrary, "QueryAllComAuthSysFunc");


		_pQueryStationGroupByFuncID				QueryStationGroupByFuncID			= (_pQueryStationGroupByFuncID)GetProcAddress(m_hToolkitLibrary, "QueryStationGroupByFuncID");
		_pQueryRegionGroupByFuncID				QueryRegionGroupByFuncID			= (_pQueryRegionGroupByFuncID)GetProcAddress(m_hToolkitLibrary, "QueryRegionGroupByFuncID");
		_pQueryStationName						QueryStationName					= (_pQueryStationName)GetProcAddress(m_hToolkitLibrary, "QueryStationName");
		_pQueryFuncName							QueryFuncName						= (_pQueryFuncName)GetProcAddress(m_hToolkitLibrary, "QueryFuncName");
		_pQueryStationTypeName					QueryStationTypeName				= (_pQueryStationTypeName)GetProcAddress(m_hToolkitLibrary, "QueryStationTypeName");
		_pQueryMonOrg							QueryMonOrg							= (_pQueryMonOrg)GetProcAddress(m_hToolkitLibrary, "QueryMonOrg");
		_pQueryStation							QueryStation						= (_pQueryStation)GetProcAddress(m_hToolkitLibrary, "QueryStation");
		_pQueryDevice							QueryDevice							= (_pQueryDevice)GetProcAddress(m_hToolkitLibrary, "QueryDevice");
		_pQueryDeviceModelFuncParam				QueryDeviceModelFuncParam			= (_pQueryDeviceModelFuncParam)GetProcAddress(m_hToolkitLibrary, "QueryDeviceModelFuncParam");
		_pIsRtmGroup							IsRtmGroup							= (_pIsRtmGroup)GetProcAddress(m_hToolkitLibrary, "IsRtmGroup");
		_pQueryRegionName						QueryRegionName						= (_pQueryRegionName)GetProcAddress(m_hToolkitLibrary, "QueryRegionName");

		///////////////////////////////////////////////////////////////以下为测试/////////////////////////////////////////////////////////////////

		{
			string strErrorex = "";
			//{
			//	vector<ULONG>	lcArray = QueryStationGroupByFuncID(10);
			//	lcArray.clear();
			//}

			//{
			//	vector<string>	lcArray = QueryRegionGroupByFuncID(10);
			//	lcArray.clear();
			//}

			//{
			//	string	lcArray = QueryStationName(44030006);
			//	lcArray = "";
			//}

			//{
			//	string	lcArray = QueryFuncName(15);
			//	lcArray = "";
			//}

			//{
			//	string	lcArray = QueryStationTypeName(5);
			//	lcArray = "";
			//}

			//{
			//	vector<RTMMONORG>	lcArray; 
			//	if(false == QueryAllRtmMonOrg(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	//lcArray.clear();
			//}

			//{
			//	vector<RTMMONCTRLCENTER>	lcArray; 
			//	if(false == QueryAllRtmMonCtrlCenter(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	//lcArray.clear();
			//}

			//{
			//	vector<RTMSTATIONDEVICEFUNCANT>	lcArray; 
			//	if(false == QueryAllRtmStationDeviceFuncAnt(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	//lcArray.clear();
			//}

			//{
			//	vector<RTMANT>	lcArray; 
			//	if(false == QueryAllRtmAnt(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	//lcArray.clear();
			//}

			//{
			//	vector<RTMREFDEVICEMODEL>	lcArray; 
			//	if(false == QueryAllRtmRefDeviceModel(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	//lcArray.clear();
			//}

			//{
			//	vector<RTMREFDEVICEMODELFUNCPARAM>	lcArray; 
			//	if(false == QueryAllRtmRefDeviceModelFuncParam(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	//lcArray.clear();
			//}		


			//{
			//	RTMMONORG lc;
			//	if(false == QueryMonOrg("440300", lc))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//}

			//{
			//	RTMSTATION lc;
			//	if(false == QueryStation("44030001", lc))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//}

			//{
			//	RTMDEVICE lc;
			//	if(false == QueryDevice(44030001, 101, lc))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//}

			//{
			//	string lc;
			//	if(false == QueryDeviceModelFuncParam("TCI735V", 22, lc))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//}

			//{

			//	if(false == IsRtmGroup(44030001, 101, 10))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//}
			//{
			//	string str = "";
			//	if(false == QueryRegionName("440300", str))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//}
			//{
			//	vector<RIM30RTMRNC>	lcArray; 
			//	if(false == QueryAllRtmRnc(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	lcArray.clear();
			//}

			//{
			//	vector<RIM30COMAUTHSYSDATASTATION>	lcArray; 
			//	if(false == QueryAllComAuthSysDataStation(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	lcArray.clear();
			//}
			//{
			//	vector<RIM30COMAUTHSYSFUNC>	lcArray; 
			//	if(false == QueryAllComAuthSysFunc(lcArray, strErrorex))
			//		::MessageBox(NULL, CT2A(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
			//	lcArray.clear();
			//}
		}
		
	}
}

RIMConfig::~RIMConfig(void)
{
	ClearAll();
}

void RIMConfig::ClearAll()
{
	CQueueData* pQueueData = NULL;
	while (m_UDPReceiveQueue.GetQueueCount())	{	m_UDPReceiveQueue.Pop(&pQueueData); delete pQueueData;}
	while (m_UDPSendQueue.GetQueueCount())		{	m_UDPSendQueue.Pop(&pQueueData);	delete pQueueData;}
	while (m_TCPReceiveQueue.GetQueueCount())	{	m_TCPReceiveQueue.Pop(&pQueueData);	delete pQueueData;}
	while (m_TCPSendQueue.GetQueueCount())		{	m_TCPSendQueue.Pop(&pQueueData);	delete pQueueData;}

	ClearMoudleInfo();
	ClearStationInfo();
	ClearUserBranchInfo();
	ClearTableUpdateInfo();
	ClearWebAddrInfo();

	m_rtmStationVec.clear();		
	m_rtmDeviceVec.clear();			
	m_rtmDeviceFuncVec.clear();		
	m_rtmRefDeviceFuncVec.clear();
	m_comRegionNoVec.clear();

	if(m_hToolkitLibrary)
		FreeLibrary(m_hToolkitLibrary);
}

void RIMConfig::ClearMoudleInfo()
{
	//清除子系统信息
	for (UINT i = 0; i < m_moudleInfoArray.size(); i++)
	{
		LPSUBMOUDLEINFO pSubMoudleInfo = m_moudleInfoArray.at(i);
		delete pSubMoudleInfo;
	}
	m_moudleInfoArray.clear();	
}

void RIMConfig::ClearStationInfo()
{
	//清除站点信息
	for (UINT i = 0; i < m_stationArray.size(); i++)
	{
		LPSTATIONINFO lpstationinfo = m_stationArray.at(i);
		delete lpstationinfo;
	}
	m_stationArray.clear();	
}

void RIMConfig::ClearUserBranchInfo()
{
	//清除用户信息
	for (UINT i = 0; i < m_userArray.size(); i++)
	{
		LPUSERINFO lpuserinfo = m_userArray.at(i);
		delete lpuserinfo;
	}
	m_userArray.clear();

	for (UINT i = 0; i < m_branchArray.size(); i++)
	{
		LPBRANCHINFO lpbranchinfo = m_branchArray.at(i);
		delete lpbranchinfo;
	}
	m_branchArray.clear();
}

void RIMConfig::ClearTableUpdateInfo()
{
	//清除用户信息
	for (UINT i = 0; i < m_tableUpdateArray.size(); i++)
	{
		LPTABLEUPDATE lptableupdate = m_tableUpdateArray.at(i);
		delete lptableupdate;
	}
	m_tableUpdateArray.clear();
}

void RIMConfig::ClearWebAddrInfo()
{
	for (map<DWORD, vector<LPWEBADDRINFO>>::iterator it = m_webAddrInfoMap.begin(); it != m_webAddrInfoMap.end(); ++it)
	{
		for (UINT i = 0; i < it->second.size(); ++i)
		{
			delete it->second.at(i);
		}
	}

	m_webAddrInfoMap.clear();		//各个服务地址信息数据
}

BOOL RIMConfig::ParseLogonFile(tstring path)
{
	///////////////////////读取配置文件 

	//tstring keyName			= _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RIM 3.0");
	//m_systemSet.lastName 	= ReadRegKeyStr(HKEY_LOCAL_MACHINE, keyName, _T("lastName"));
	//m_systemSet.lastPass 	= ReadRegKeyStr(HKEY_LOCAL_MACHINE, keyName, _T("lastPass"));
	//m_systemSet.lastServeIP = ReadRegKeyStr(HKEY_LOCAL_MACHINE, keyName, _T("lastServeIP"));
	//DWORD dWordTemp = 0;
	//ReadRegKeyDWord(HKEY_LOCAL_MACHINE, keyName, _T("bRememberPass"), dWordTemp);
	//m_systemSet.bRememberPass = BOOL(dWordTemp);
	//ReadRegKeyDWord(HKEY_LOCAL_MACHINE, keyName, _T("bAutoLogon"), dWordTemp);
	//m_systemSet.bAutoLogon = BOOL(dWordTemp);

	USES_CONVERSION;

	string strXMLFile = CT2A(path.c_str()); 
	tstring strError;
	TiXmlDocument xmldoc;
	if(!xmldoc.LoadFile(strXMLFile, TIXML_ENCODING_LEGACY))
	{
		strError = _T("读取登录文件失败,导入失败!");
		//MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}
	///////////////////

	string strValue;
	TiXmlNode* root = NULL;
	TiXmlNode* node = NULL;

	TiXmlNode* subnode = NULL;
	TiXmlElement *element = NULL;
	const char * pTxt = NULL;

	bool bContinue = true;
	//==========get the root of *.xml==========================================
	if((root = xmldoc.FirstChild("root")) == NULL)
	{
		strError = _T("配置文件格式不正确,缺少root,导入失败!");
		MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}

	//开始获取文件信息
	if(node = root->FirstChild("logonset"))
	{
		string	lastPassTemp = "";
		if((subnode = node->FirstChild("lastName")) && (element = subnode->ToElement()))
		{
			if(pTxt = element->GetText())
				m_systemSet.lastUserID	= CA2T(pTxt);
		}

		if((subnode = node->FirstChild("lastPass")) && (element = subnode->ToElement()))
		{

			if(pTxt = element->GetText())
				lastPassTemp			= CA2T(pTxt);
		}

		if((subnode = node->FirstChild("lastServeIP")) && (element = subnode->ToElement()))
		{
			if(pTxt = element->GetText())
				m_systemSet.lastDmsIP = CA2T(pTxt);
		}

		if((subnode = node->FirstChild("lastRmsIP")) && (element = subnode->ToElement()))
		{
			if(pTxt = element->GetText())
				m_systemSet.lastRmsIP = CA2T(pTxt);
		}

		if((subnode = node->FirstChild("lastMsaIP")) && (element = subnode->ToElement()))
		{
			if(pTxt = element->GetText())
				m_systemSet.lastMsaIP = CA2T(pTxt);
		}

		if((subnode = node->FirstChild("bRememberPass")) && (element = subnode->ToElement()))
		{
			if(pTxt = element->GetText())
				m_systemSet.bRememberPass = atoi(pTxt);
		}

		if((subnode = node->FirstChild("bAutoLogon")) && (element = subnode->ToElement()))
		{
			if(pTxt = element->GetText())
				m_systemSet.bAutoLogon	= atoi(pTxt);
		}

		if((subnode = node->FirstChild("bOutLineLogon")) && (element = subnode->ToElement()))
		{
			if(pTxt = element->GetText())
				m_systemSet.bOffLineLogon	= atoi(pTxt);
		}

		////字符串解密
		for(UINT i = 0;i<lastPassTemp.size();i++)
			lastPassTemp[i] = lastPassTemp[i]^255;

		if(m_macArray.GetSize())
		{
			for (int i = 0; i < m_macArray.GetSize(); ++i)
			{
				tstring subStr = m_macArray.GetAt(i) + _T("_"); 
				if(tstring::npos != lastPassTemp.find(subStr, 0))
				{
					lastPassTemp = lastPassTemp.substr(subStr.size());
					break;
				}
			}
		}

		m_systemSet.lastPass = CA2T(lastPassTemp.c_str());
	}

	return TRUE;
}


BOOL RIMConfig::ParseRecentFile(tstring path)
{
	///////////////////////读取配置文件 
	USES_CONVERSION;

	string strXMLFile = CT2A(path.c_str()); 
	tstring strError;
	TiXmlDocument xmldoc;
	if(!xmldoc.LoadFile(strXMLFile, TIXML_ENCODING_LEGACY))
	{
		strError = _T("读取登录文件失败,导入失败!");
		//MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}
	///////////////////

	string strValue;
	TiXmlNode* root = NULL;
	TiXmlNode* nodeStation	= NULL;
	TiXmlNode* nodeDevice	= NULL;
	TiXmlNode* nodeMeasure	= NULL;

	TiXmlNode* subnode = NULL;
	TiXmlElement *element = NULL;
	const char * pTxt = NULL;

	bool bContinue = true;
	//==========get the root of *.xml==========================================
	if((root = xmldoc.FirstChild("root")) == NULL)
	{
		strError = _T("配置文件格式不正确,缺少root,导入失败!");
		MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}

	//开始获取文件信息
	nodeStation = root->FirstChild("station");
	while(nodeStation)
	{
		string stationNo = nodeStation->ToElement()->Attribute("No");
		nodeDevice = nodeStation->FirstChild("device");
		while(nodeDevice)
		{
			string deviceNo = nodeDevice->ToElement()->Attribute("No");
			nodeMeasure = nodeDevice->FirstChild("measure");
			while(nodeMeasure)
			{
				string measureNo = nodeMeasure->ToElement()->Attribute("No");
				m_recentMap[stationNo][deviceNo].push_back(atoi(measureNo.c_str()));

				nodeMeasure = nodeMeasure->NextSibling();
			}
			nodeDevice = nodeDevice->NextSibling();
		}

		nodeStation = nodeStation->NextSibling();
	}

	return TRUE;
}

BOOL RIMConfig::SaveLogonFile(tstring path)
{

	//tstring keyName = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RIM 3.0");
	//WriteRegKeyStr(HKEY_LOCAL_MACHINE, keyName, _T("lastName"), m_systemSet.lastName);
	//WriteRegKeyStr(HKEY_LOCAL_MACHINE, keyName, _T("lastPass"), m_systemSet.lastPass);
	//WriteRegKeyStr(HKEY_LOCAL_MACHINE, keyName, _T("lastServeIP"), m_systemSet.lastServeIP);
	//WriteRegKeyDWord(HKEY_LOCAL_MACHINE, keyName, _T("bRememberPass"), m_systemSet.bRememberPass);
	//WriteRegKeyDWord(HKEY_LOCAL_MACHINE, keyName, _T("bAutoLogon"), m_systemSet.bAutoLogon);

	try 
	{
		tstring lastPass		= CT2A(m_systemSet.lastPass.c_str());
		if(m_macArray.GetSize())	////绑定网卡
			lastPass = tstring(m_macArray.GetAt(0)) + _T("_") + lastPass;

		////字符串加密
		string	lastPassTemp	= CT2A(lastPass.c_str());
		for(UINT i = 0;i<lastPassTemp.size();i++)
			lastPassTemp[i] = lastPassTemp[i]^255;

		string strXMLFile = CT2A(path.c_str());
		tstring strError;
		TiXmlDocument xmldoc;

		xmldoc.LoadFile(strXMLFile.c_str(), TIXML_ENCODING_LEGACY);
		xmldoc.Clear();

		TiXmlElement	*pRootNode	= NULL, *pNodes = NULL, *pNode = NULL;
		TiXmlText		*pTempText	= NULL;

		TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "GB2312", "");
		xmldoc.LinkEndChild(decl);

		pRootNode = new TiXmlElement("root");
		xmldoc.LinkEndChild(pRootNode);

		pNodes = new TiXmlElement("logonset");
		pRootNode->LinkEndChild(pNodes);

		pNode = new TiXmlElement("lastName");
		pNodes->LinkEndChild(pNode);
		pTempText = new TiXmlText(CT2A(m_systemSet.lastUserID.c_str()));
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("lastPass");
		pNodes->LinkEndChild(pNode);
		pTempText = new TiXmlText(lastPassTemp.c_str());
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("lastServeIP");
		pNodes->LinkEndChild(pNode);
		pTempText = new TiXmlText(CT2A(m_systemSet.lastDmsIP.c_str()));
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("lastRmsIP");
		pNodes->LinkEndChild(pNode);
		pTempText = new TiXmlText(CT2A(m_systemSet.lastRmsIP.c_str()));
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("lastMsaIP");
		pNodes->LinkEndChild(pNode);
		pTempText = new TiXmlText(CT2A(m_systemSet.lastMsaIP.c_str()));
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("bRememberPass");
		pNodes->LinkEndChild(pNode);
		char ch[MAX_PATH] ={ 0 };
		_itoa_s(int(m_systemSet.bRememberPass), ch, 10);
		pTempText = new TiXmlText(ch);
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("bAutoLogon");
		pNodes->LinkEndChild(pNode);
		_itoa_s(int(m_systemSet.bAutoLogon), ch, 10);
		pTempText = new TiXmlText(ch);
		pNode->LinkEndChild(pTempText);

		pNode = new TiXmlElement("bOutLineLogon");
		pNodes->LinkEndChild(pNode);
		_itoa_s(int(m_systemSet.bOffLineLogon), ch, 10);
		pTempText = new TiXmlText(ch);
		pNode->LinkEndChild(pTempText);

		xmldoc.SaveFile();

		//////下面是用于修改文件的代码
		//USES_CONVERSION;
		//if(!IsFileExist(path.c_str()))
		//{
		//	MakeSureDirectoryPathExists(CT2A(path.c_str()));
		//}


		//if(!xmldoc.LoadFile(strXMLFile, TIXML_ENCODING_LEGACY))
		//{
		//	strError = _T("读取系统配置文件失败,导入失败!");
		//	MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		//	return FALSE;
		//}
		/////////////////////

		//string strValue;
		//TiXmlNode* root = NULL;
		//TiXmlNode* node = NULL;

		//TiXmlNode* subnode = NULL;
		//TiXmlElement *element = NULL;
		//const char * pTxt = NULL;

		//bool bContinue = true;
		////==========get the root of *.xml==========================================
		//if((root = xmldoc.FirstChild("root")) == NULL)
		//{
		//	strError = _T("配置文件格式不正确,缺少root,导入失败!");
		//	MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		//	return FALSE;
		//}

		////设置结点的文本
		//if(node = root->FirstChild("logonset"))
		//{
		//	if((subnode = node->FirstChild("lastName")) && (subnode = subnode->FirstChild()))
		//	{
		//		subnode->SetValue(CT2A(m_systemSet.lastName.c_str()));
		//	}

		//	if((subnode = node->FirstChild("lastPass")) && (subnode = subnode->FirstChild()))
		//	{
		//		subnode->SetValue(CT2A(m_systemSet.lastPass.c_str()));
		//	}

		//	if((subnode = node->FirstChild("lastServeIP")) && (subnode = subnode->FirstChild()))
		//	{
		//		subnode->SetValue(CT2A(m_systemSet.lastServeIP.c_str()));
		//	}

		//	if((subnode = node->FirstChild("bRememberPass")) && (subnode = subnode->FirstChild()))
		//	{
		//		char ch[MAX_PATH] = {0};
		//		_itoa_s(int(m_systemSet.bRememberPass), ch, 10);
		//		subnode->SetValue(ch);
		//	}

		//	if((subnode = node->FirstChild("bAutoLogon")) && (subnode = subnode->FirstChild()))
		//	{
		//		char ch[MAX_PATH] = {0};
		//		_itoa_s(m_systemSet.bAutoLogon, ch, 10);
		//		subnode->SetValue(ch);
		//	}
		//}

		//xmldoc.SaveFile();
	}
	catch (...)
	{

	}

	return TRUE;
}

BOOL RIMConfig::SaveRecentFile(tstring path)
{
	try 
	{
		string strXMLFile = CT2A(path.c_str());
		tstring strError;
		TiXmlDocument xmldoc;

		xmldoc.LoadFile(strXMLFile.c_str(), TIXML_ENCODING_LEGACY);
		xmldoc.Clear();

		TiXmlElement	*pRootNode	= NULL, *pNodes = NULL, *pNode = NULL;
		TiXmlElement	*pNodeStation = NULL, *pNodeDevice = NULL, *pNodeMeasure = NULL;
		TiXmlText		*pTempText	= NULL;

		TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "GB2312", "");
		xmldoc.LinkEndChild(decl);

		pRootNode = new TiXmlElement("root");
		xmldoc.LinkEndChild(pRootNode);


		for (map<string, map<string, vector<long>>>::iterator iterStation = m_recentMap.begin(); iterStation != m_recentMap.end(); ++iterStation)
		{
			pNodeStation = new TiXmlElement("station");
			pRootNode->LinkEndChild(pNodeStation);
			pNodeStation->SetAttribute("No", iterStation->first);
			for (map<string, vector<long>>::iterator iterDevice = iterStation->second.begin(); iterDevice != iterStation->second.end(); ++iterDevice)
			{
				pNodeDevice = new TiXmlElement("device");
				pNodeStation->LinkEndChild(pNodeDevice);
				pNodeDevice->SetAttribute("No", iterDevice->first);
				for (vector<long>::iterator iterMeasure = iterDevice->second.begin(); iterMeasure != iterDevice->second.end(); ++iterMeasure)
				{
					pNodeMeasure = new TiXmlElement("measure");
					pNodeDevice->LinkEndChild(pNodeMeasure);
					pNodeMeasure->SetAttribute("No", *iterMeasure);
				}
			}
		}

		xmldoc.SaveFile();
	}
	catch (...)
	{

	}

	return TRUE;
}

BOOL RIMConfig::ParseConfig( tstring path )
{
	///////////////////////读取配置文件 
	USES_CONVERSION;

	string strXMLFile = CT2A(path.c_str()); 
	tstring strError;
	TiXmlDocument xmldoc;
	if(!xmldoc.LoadFile(strXMLFile, TIXML_ENCODING_LEGACY))
	{
		strError = _T("读取配置文件失败,导入失败!");
		MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}
	///////////////////

	string strValue;
	TiXmlNode		*root 		= NULL;
	TiXmlNode		*node 		= NULL;

	TiXmlNode		*subnode 	= NULL;
	TiXmlElement	*element 	= NULL;
	const char		*pTxt		= NULL;

	bool bContinue = true;
	//==========get the root of *.xml==========================================
	if((root = xmldoc.FirstChild("root")) == NULL)
	{
		strError = _T("配置文件格式不正确,缺少root,导入失败!");
		MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}

	//解析子模块信息
	if((node = root->FirstChild("welcomtext")))
	{
		if(element = node->ToElement())
		{
			if((pTxt = element->Attribute("text")))
				m_welcomeText = CA2T(pTxt);
			if((pTxt = element->Attribute("fontsize")))
				m_welcomeFontSize = atoi(pTxt);
		}
	} 

	if((node = root->FirstChild("modifypass")))
	{
		if(element = node->ToElement())
		{
			if((pTxt = element->Attribute("theme")))
				m_modifyPassTheme = CA2T(pTxt);
		}
	}
	
	//解析sqlite表更新的相关信息
	if((node = root->FirstChild("sqltables")))
	{
		ParseSqlTableNodes(node);
	} 

	//解析子模块信息
	if((node = root->FirstChild("moudles")))
	{
		ParseModuleNodes(node);
	} 

	return TRUE;
}

void RIMConfig::ParseSqlTableNodes(TiXmlNode* moudleNodes)
{
	/////数据的管理交由外部，如果数据未清除，不复加载
	if(moudleNodes == NULL || m_moudleInfoArray.size())
		return;

	USES_CONVERSION;

	TiXmlNode*		node	= NULL;

	TiXmlNode*		subnode = NULL;
	TiXmlElement*	element = NULL;
	const char*		pTxt	= NULL;

	if(subnode = moudleNodes->FirstChild("table"))
	{
		while(subnode)
		{
			if(element = subnode->ToElement())
			{
				LPTABLEUPDATE lptableupdate = new TABLEUPDATE();

				if((pTxt = element->Attribute("name")))
					lptableupdate->tableName = pTxt;
				if((pTxt = element->Attribute("updatetime")))
					lptableupdate->updateTime = pTxt;
				
				m_tableUpdateArray.push_back(lptableupdate);
			}
			subnode = subnode->NextSibling();
		}
	}
}

void RIMConfig::ParseModuleNodes(TiXmlNode* moudleNodes)
{
	/////数据的管理交由外部，如果数据未清除，不复加载
	if(moudleNodes == NULL || m_moudleInfoArray.size())
		return;

	USES_CONVERSION;

	TiXmlNode*		node	= NULL;

	TiXmlNode*		subnode = NULL;
	TiXmlElement*	element = NULL;
	const char*		pTxt	= NULL;

	if(subnode = moudleNodes->FirstChild("moudle"))
	{
		while(subnode)
		{
			if(element = subnode->ToElement())
			{
				SUBMOUDLEINFO* pSubMoudleInfo = new SUBMOUDLEINFO();

				if((pTxt = element->Attribute("name")))
					pSubMoudleInfo->moudleName = CA2T(pTxt);
				if((pTxt = element->Attribute("sysFuncNo")))
					pSubMoudleInfo->sysFuncNo = CA2T(pTxt);
				if((pTxt = element->Attribute("exePath")))
					pSubMoudleInfo->moudleEXEPath = CA2T(pTxt);
				if((pTxt = element->Attribute("bShowInList")))
					pSubMoudleInfo->bShowInList = atoi(pTxt);
				if((pTxt = element->Attribute("bDefaultOpen")))
					pSubMoudleInfo->bDefaultOpen = atoi(pTxt);
				if((pTxt = element->Attribute("bSupportOutLine")))
					pSubMoudleInfo->bSupportOutLine = atoi(pTxt);
				if((pTxt = element->Attribute("selpngpath")))
				{
					pSubMoudleInfo->selPath = CA2T(pTxt);
					pSubMoudleInfo->pImagesel = Image::FromFile(CStringW(RelativeToFullPath(pSubMoudleInfo->selPath).c_str()));
				}
				if((pTxt = element->Attribute("unselpngpath")))
				{
					pSubMoudleInfo->unselPath = CA2T(pTxt);
					pSubMoudleInfo->pImageUnsel = Image::FromFile(CStringW(RelativeToFullPath(pSubMoudleInfo->unselPath).c_str()));
				}

				m_moudleInfoArray.push_back(pSubMoudleInfo);
			}
			subnode = subnode->NextSibling();
		}
	}
}

void RIMConfig::ParseStationsInfo()
{
	if(m_stationArray.size() == 0)
	{
		LPRTMSTATION		lpRtmStationTemp		= NULL;
		LPRTMDEVICE			lpRtmDeviceTemp			= NULL;
		LPRTMDEVICEFUNC		lpRtmDeviceFunTemp		= NULL;
		LPRTMREFDEVICEFUNC	lpRtmRefDeviceFunTemp	= NULL;
		LPCOMREGIONNO		lpComRegionNoTemp		= NULL;
		for(UINT stationIndex = 0; stationIndex < m_rtmStationVec.size(); ++stationIndex)
		{
			lpRtmStationTemp = &m_rtmStationVec.at(stationIndex);

			LPSTATIONINFO pStationInfo = new STATIONINFO(); pStationInfo->pRimStationTemp = lpRtmStationTemp;

			for(UINT deviceIndex = 0; deviceIndex < m_rtmDeviceVec.size(); ++deviceIndex)
			{	////遍历所有站点
				lpRtmDeviceTemp = &m_rtmDeviceVec.at(deviceIndex);
				if(strcmp(lpRtmDeviceTemp->sStationNO.c_str(), lpRtmStationTemp->sStationNO.c_str()) == 0)
				{/////当设备属于该站点    必须与站点信息的  站点编号相同
					LPDEVICEINFO pDeviceInfo = new DEVICEINFO(); pDeviceInfo->pRimDeviceTemp = lpRtmDeviceTemp;

					for (UINT funIndex = 0; funIndex < m_rtmDeviceFuncVec.size(); ++funIndex)
					{////遍历所有设备对应的功能表
						lpRtmDeviceFunTemp = &m_rtmDeviceFuncVec.at(funIndex);
						if(	strcmp(lpRtmDeviceTemp->sStationNO.c_str(), lpRtmDeviceFunTemp->sStationNO.c_str()) == 0	&&
							strcmp(lpRtmDeviceTemp->sDeviceNO.c_str(), lpRtmDeviceFunTemp->sDeviceNO.c_str()) == 0
							)	/////当该设备功能属于该站点下的设备     必须与设备信息的  站点与设备编号相同
						{
							LPMEASUREINFO pMeasureInfo = new MEASUREINFO(); pMeasureInfo->pRtmDeviceFunTemp = lpRtmDeviceFunTemp;

							for (UINT descIndex = 0; descIndex < m_rtmRefDeviceFuncVec.size(); ++descIndex)
							{////查找该功能的相应的描述
								lpRtmRefDeviceFunTemp = &m_rtmRefDeviceFuncVec.at(descIndex);
								if(lpRtmDeviceFunTemp->nFunNO == lpRtmRefDeviceFunTemp->nFuncID)
								{/////当该功能表ID与 该设备功能ID相同时
									pMeasureInfo->measureID		= lpRtmRefDeviceFunTemp->nFuncID;
									pMeasureInfo->measueName	= CA2T(lpRtmRefDeviceFunTemp->sFuncName.c_str());
									pMeasureInfo->keyWord		= CA2T(lpRtmRefDeviceFunTemp->sFuncKey.c_str());
									break;	///////查找到即可不用再查找了
								}
							}
							pDeviceInfo->measureInfos.push_back(pMeasureInfo);
						}
					}

					for (UINT typeIndex = 0; typeIndex < m_deviceTypeVec.size(); ++typeIndex)
					{	////查找设备类型
						LPRTMREFDEVICETYPE lprtmrefdevicetype = &m_deviceTypeVec.at(typeIndex);
						if(lpRtmDeviceTemp->nDeviceType == lprtmrefdevicetype->nTypeNo)
						{
							pDeviceInfo->sDeviceType = CA2T(lprtmrefdevicetype->sTypeName.c_str());	break;	///////查找到即可不用再查找了
						}
					}

					for (UINT protocolIndex = 0; protocolIndex < m_deviceProtocolVec.size(); ++protocolIndex)
					{	////查找设备类型
						LPRTMREFDEVICEPROTOCOL lprtmrefdeviceprotocol = &m_deviceProtocolVec.at(protocolIndex);
						if(lpRtmDeviceTemp->nDeviceProtocol == lprtmrefdeviceprotocol->nNoID)
						{
							pDeviceInfo->sProtocol = CA2T(lprtmrefdeviceprotocol->sName.c_str());	break;	///////查找到即可不用再查找了
						}
					}

					pStationInfo->devicesInfos.push_back(pDeviceInfo);
				}
			}

			for (UINT regionIndex = 0; regionIndex < m_comRegionNoVec.size(); ++regionIndex)
			{	////赋值站点信息的  区域
				lpComRegionNoTemp = &m_comRegionNoVec.at(regionIndex);
				if(lpRtmStationTemp->sStationNO.compare(0, 6, lpComRegionNoTemp->sRegionNo.c_str()) == 0)
				{
					pStationInfo->pRegionNoTemp = lpComRegionNoTemp;	break;	///////查找到即可不用再查找了
				}
			}

			for (UINT typeIndex = 0; typeIndex < m_stationTypeVec.size(); ++typeIndex)
			{	////查找站点类型
				LPRTMREFSTATIONTYPE lprtmrefstationtype = &m_stationTypeVec.at(typeIndex);
				if(lpRtmStationTemp->nStationType == lprtmrefstationtype->nTypeNo)
				{
					pStationInfo->sStationType = CA2T(lprtmrefstationtype->sTypeName.c_str());	break;	///////查找到即可不用再查找了
				}
			}
			m_stationArray.push_back(pStationInfo);
		}
	}
}

BOOL RIMConfig::SaveMoudles(tstring path)
{
	///////////////////////读取配置文件 
	USES_CONVERSION;

	string strXMLFile = CT2A(path.c_str()); 
	tstring strError;
	TiXmlDocument xmldoc;
	if(!xmldoc.LoadFile(strXMLFile, TIXML_ENCODING_LEGACY))
	{
		strError = _T("读取配置文件失败,导入失败!");
		MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}
	///////////////////

	string strValue;
	TiXmlNode* root = NULL;
	TiXmlNode* node = NULL;

	TiXmlNode* subnode = NULL;
	TiXmlElement *element = NULL;
	const char * pTxt = NULL;

	bool bContinue = true;
	//==========get the root of *.xml==========================================
	if((root = xmldoc.FirstChild("root")) == NULL)
	{
		strError = _T("配置文件格式不正确,缺少root,导入失败!");
		MessageBox(NULL, strError.c_str(), _T("RIM3.0 错误"), MB_OK);
		return FALSE;
	}

	//保存数据库表更新信息
	if((node = root->FirstChild("sqltables")))
	{
		SaveSqlTablesNodes(node);
	}

	//保存子模块信息
	if((node = root->FirstChild("moudles")))
	{
		SaveModulesNodes(node);
	}


	xmldoc.SaveFile();
	return TRUE;
}

BOOL RIMConfig::SaveSqlTablesNodes(TiXmlNode* sqlTableNodes)
{
	if(sqlTableNodes == NULL)
		return FALSE;

	USES_CONVERSION;

	TiXmlNode*		node	= NULL;

	TiXmlNode*		subnode = NULL;
	TiXmlElement*	element = NULL;
	const char*		pTxt	= NULL;

	int i = 0;
	if(subnode = sqlTableNodes->FirstChild("table"))
	{
		while(subnode)
		{
			if(element = subnode->ToElement())
			{
				LPTABLEUPDATE lptableupdate = m_tableUpdateArray[i];

				element->SetAttribute("name", lptableupdate->tableName.c_str());
				element->SetAttribute("updatetime", lptableupdate->updateTime.c_str());
			}
			subnode = subnode->NextSibling();
			i++;
		}
	}

	return TRUE;
}

BOOL RIMConfig::SaveModulesNodes(TiXmlNode* moudleNodes)
{
	if(moudleNodes == NULL)
		return FALSE;

	USES_CONVERSION;

	TiXmlNode*		node	= NULL;

	TiXmlNode*		subnode = NULL;
	TiXmlElement*	element = NULL;
	const char*		pTxt	= NULL;

	int i = 0;
	if(subnode = moudleNodes->FirstChild("moudle"))
	{
		while(subnode)
		{
			if(element = subnode->ToElement())
			{
				SUBMOUDLEINFO* pSubMoudleInfo = m_moudleInfoArray[i];

				element->SetAttribute("name", CT2A(pSubMoudleInfo->moudleName.c_str()));
				element->SetAttribute("sysFuncNo", CT2A(pSubMoudleInfo->sysFuncNo.c_str()));
				element->SetAttribute("bShowInList", pSubMoudleInfo->bShowInList);
				element->SetAttribute("bDefaultOpen", pSubMoudleInfo->bDefaultOpen);
				element->SetAttribute("bSupportOutLine", pSubMoudleInfo->bSupportOutLine);
				element->SetAttribute("selpngpath", CT2A(pSubMoudleInfo->selPath.c_str()));
				element->SetAttribute("unselpngpath", CT2A(pSubMoudleInfo->unselPath.c_str()));
			}
			subnode = subnode->NextSibling();
			i++;
		}
	}

	return TRUE;
}

void RIMConfig::DoLogon(BOOL bLogon, BOOL bOffLine, tstring& errorInfo, HWND processHwnd)
{
	if (bLogon)
	{
		string	errorInfoTemp		= "";
		DWORD	process0_100		= 0;
		string  userToken			= "";


		string strWebSysUrl 		= "";
		string strWebRtmUrl 		= "";
		string strEmsUrl 			= "";

		if(bOffLine == FALSE)
		{	////非脱机登录
			if(!IsValidIP(m_systemSet.lastDmsIP))
			{
				errorInfo = _T("请输入有效的IP!");
				return;
			}

			if(processHwnd)
				PostMessage(processHwnd, WM_PROGRESS, process0_100 = 0, 0);

			//"http://192.168.108.241/rim3/rim3/BSDSYSService.asmx?"
			//"http://192.168.108.241:8100"
 
			string strDmsServers = "http://";
			strDmsServers += CT2A(m_systemSet.lastDmsIP.c_str());
			strDmsServers += ":8100";

			{////初始化各服务地址

				if(false == m_gsoapServer.QueryAllSubSysAddr(strDmsServers, m_webAddrInfoMap, errorInfoTemp))
				{
					errorInfo = CA2T(errorInfoTemp.c_str());
					return;
				}
			}

			if(processHwnd)
				PostMessage(processHwnd, WM_PROGRESS, process0_100 = 2, 0);

			{//////获取webservice地址 
				for(map<DWORD, vector<LPWEBADDRINFO>>::iterator it = m_webAddrInfoMap.begin(); it != m_webAddrInfoMap.end(); ++it)
				{
					if(	it->first == Xdmsdm::Xdmsdm__SubSystemID__xBSD)
					{
						for (UINT i = 0; i < it->second.size(); ++i)
						{
							if(it->second.at(i)->nSubModeID == 1)
							{
								strWebSysUrl = it->second.at(i)->sLocation;
							}
							else if(it->second.at(i)->nSubModeID == 2)
							{
								strWebRtmUrl = it->second.at(i)->sLocation;
							}
						}
					}
					else if( it->first == 99)
					{
						for (UINT i = 0; i < it->second.size(); ++i)
						{
							if(it->second.at(i)->nSubModeID == 9)
							{
								strEmsUrl = it->second.at(i)->sLocation;
							}
						}
					}
				}


				if(strWebSysUrl.size() == 0 || strWebRtmUrl.size() == 0 || strEmsUrl.size() == 0)
				{
					errorInfo = _T("获取服务信息失败!");
					return;
				}
			}
			//////验证用户的合法性
			string 	lastUserID	= CT2A(m_systemSet.lastUserID.c_str());
			string 	lastPass	= CT2A(m_systemSet.lastPass.c_str());
			if(FALSE == m_gsoapServer.VerifyUser(strWebSysUrl, lastUserID, lastPass, userToken, errorInfoTemp))
			{
				errorInfo = CA2T(errorInfoTemp.c_str());
				return; 
			}

			if(processHwnd)
				PostMessage(processHwnd, WM_PROGRESS, process0_100 = 5, 0);

			int step = 0;
			if((95 - 20)/m_tableUpdateArray.size())
				step = (95 - 20)/m_tableUpdateArray.size();

			string strUserID = CT2A(m_systemSet.lastUserID.c_str());
			{	///实时更新表T_RIM_COM_AUTH_SYS_DATA_STATION    监测站授权表
				string str = CT2A(m_systemSet.lastUserID.c_str());
				if(!m_gsoapServer.SysncT_RIM_COM_AUTH_SYS_DATA_STATION(str, strDmsServers, BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION, errorInfoTemp))
				{
					errorInfo = CA2T(errorInfoTemp.c_str());
					return; 
				}
			}

			for (UINT i = 0; i < m_tableUpdateArray.size(); ++i)
			{	////更新数据库 
				LPTABLEUPDATE lptableupdate = m_tableUpdateArray.at(i);

				if(	lptableupdate->tableName == BOSD_T_RIM30_USERINFO					|| 
					lptableupdate->tableName == BOSD_T_RIM30_BRANCHINFO					|| 
					lptableupdate->tableName == BOSD_T_RIM_COM_AUTH_SYS_FUNC
					)
				{	/////系统管理地址
					if(!m_gsoapServer.SysncSqlite(strUserID, strWebSysUrl, lptableupdate->tableName, lptableupdate->updateTime, errorInfoTemp))
					{
						errorInfo = CA2T(errorInfoTemp.c_str());
						return; 
					}
				}
				else if(lptableupdate->tableName == BOSD_T_RIM_RTM_RNC)
				{	////监测测向地址
					if(!m_gsoapServer.SysncSqlite(strUserID, strWebRtmUrl, lptableupdate->tableName, lptableupdate->updateTime, errorInfoTemp))
					{
						errorInfo = CA2T(errorInfoTemp.c_str());
						return; 
					}
				}
				else
				{	////DMS地址
					if(!m_gsoapServer.SysncSqlite(strUserID, strDmsServers, lptableupdate->tableName, lptableupdate->updateTime, errorInfoTemp))
					{
						errorInfo = CA2T(errorInfoTemp.c_str());
						return; 
					}
				}

				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += step, 0);
			}
		}
		else
		{	////脱机登录
			if(!IsValidIP(m_systemSet.lastRmsIP) || !IsValidIP(m_systemSet.lastMsaIP))
			{
				errorInfo = _T("请输入有效的IP!");
				return; 
			}

			{////初始化各服务地址
				const int	totalCnt				= 10;

				{
					LPWEBADDRINFO lpwebaddrinfo = new WEBADDRINFO;
					lpwebaddrinfo->nSubModeID	= 1;
					lpwebaddrinfo->sName		= "RMS 服务";
					lpwebaddrinfo->sLocation	= string("http://") + m_systemSet.lastRmsIP + ":8200";
					m_webAddrInfoMap[Xdmsdm::Xdmsdm__SubSystemID__xRMS].push_back(lpwebaddrinfo);
				}
				{
					LPWEBADDRINFO lpwebaddrinfo = new WEBADDRINFO;
					lpwebaddrinfo->nSubModeID	= 1;
					lpwebaddrinfo->sName		= "MSA 服务";
					lpwebaddrinfo->sLocation	= string("http://") + m_systemSet.lastRmsIP + ":8300";
					m_webAddrInfoMap[Xdmsdm::Xdmsdm__SubSystemID__xMSA].push_back(lpwebaddrinfo);

				}
			}
		}

		if(processHwnd)
			PostMessage(processHwnd, WM_PROGRESS, process0_100 = 80, 0);

		//////【zy 2015-01-25 ↓】 执行清除操作
		CQueueData* pQueueData = NULL;
		while (m_UDPReceiveQueue.GetQueueCount())	{ m_UDPReceiveQueue.Pop(&pQueueData);	delete pQueueData; }
		while (m_UDPSendQueue.GetQueueCount())		{ m_UDPSendQueue.Pop(&pQueueData);		delete pQueueData; }
		while (m_TCPReceiveQueue.GetQueueCount())	{ m_TCPReceiveQueue.Pop(&pQueueData);	delete pQueueData; }
		while (m_TCPSendQueue.GetQueueCount())		{ m_TCPSendQueue.Pop(&pQueueData);		delete pQueueData; }
		//////【zy 2015-01-25 ↑】


		{	////从数据库中获取站点列表信息
			_pQueryAllStation						QueryAllStation						= (_pQueryAllStation)GetProcAddress(m_hToolkitLibrary, "QueryAllStation");
			_pQueryAllDevice						QueryAllDevice						= (_pQueryAllDevice)GetProcAddress(m_hToolkitLibrary, "QueryAllDevice");
			_pQueryAllRtmDeviceFunc 				QueryAllRtmDeviceFunc				= (_pQueryAllRtmDeviceFunc)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmDeviceFunc");
			_pQueryAllRTMRefDeviceFunc				QueryAllRTMRefDeviceFunc			= (_pQueryAllRTMRefDeviceFunc)GetProcAddress(m_hToolkitLibrary, "QueryAllRTMRefDeviceFunc");
			_pQueryAllComRegionNo					QueryAllComRegionNo					= (_pQueryAllComRegionNo)GetProcAddress(m_hToolkitLibrary, "QueryAllComRegionNo");
			_pQueryAllRimRefStationType				QueryAllRimRefStationType			= (_pQueryAllRimRefStationType)GetProcAddress(m_hToolkitLibrary, "QueryAllRimRefStationType");
			_pQueryAllRimRefDeviceType				QueryAllRimRefDeviceType			= (_pQueryAllRimRefDeviceType)GetProcAddress(m_hToolkitLibrary, "QueryAllRimRefDeviceType");
			_pQueryAllRtmRefDeviceProtocol			QueryAllRtmRefDeviceProtocol		= (_pQueryAllRtmRefDeviceProtocol)GetProcAddress(m_hToolkitLibrary, "QueryAllRtmRefDeviceProtocol");
			_pQueryAllComAuthSysFunc				QueryAllComAuthSysFunc				= (_pQueryAllComAuthSysFunc)GetProcAddress(m_hToolkitLibrary, "QueryAllComAuthSysFunc");

			string	strErrorex		= "";
			{
				m_rtmStationVec.clear();
				if(false == QueryAllStation(m_rtmStationVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_rtmDeviceVec.clear();
				if(false == QueryAllDevice(m_rtmDeviceVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_rtmDeviceFuncVec.clear();
				if(false == QueryAllRtmDeviceFunc(m_rtmDeviceFuncVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_rtmRefDeviceFuncVec.clear();
				if(false == QueryAllRTMRefDeviceFunc(m_rtmRefDeviceFuncVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_comRegionNoVec.clear();
				if(false == QueryAllComRegionNo(m_comRegionNoVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_stationTypeVec.clear();
				if(false == QueryAllRimRefStationType(m_stationTypeVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_deviceTypeVec.clear();
				if(false == QueryAllRimRefDeviceType(m_deviceTypeVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_deviceProtocolVec.clear();
				if(false == QueryAllRtmRefDeviceProtocol(m_deviceProtocolVec, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			{
				m_authSysFunc.clear();
				if(false == QueryAllComAuthSysFunc(m_authSysFunc, strErrorex))
					::MessageBox(NULL, CA2T(strErrorex.c_str()), _T("RIM3.0 错误"), MB_OK);
				if(processHwnd)
					PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
			}

			ParseStationsInfo();	////组织站点信息。
		}

		{	////根据站点信息清除多余的最近使用记录
			map<string, map<string, vector<long>>>::iterator iterStation = m_recentMap.begin();
			while( iterStation != m_recentMap.end())
			{
				BOOL bFind = FALSE;
				for (UINT i = 0; i < m_stationArray.size(); ++i)
				{
					LPSTATIONINFO lpstationinfo = m_stationArray.at(i);

					if(lpstationinfo->pRimStationTemp->sStationNO == iterStation->first)
					{
						bFind = TRUE;
						break;
					}
				}

				if(bFind == FALSE)
					iterStation = m_recentMap.erase(iterStation);
				else
					++iterStation;
			}
		}

		for(UINT i = 0; i < m_moudleInfoArray.size(); ++i)
		{	////重新确定模块列表是否要显示
			LPSUBMOUDLEINFO lpsubmoudleinfo = m_moudleInfoArray.at(i);
			lpsubmoudleinfo->bShowInList = FALSE;

			for(UINT j  = 0; j < m_authSysFunc.size(); ++j)
			{
				if(_tcscmp(lpsubmoudleinfo->sysFuncNo.c_str(), m_authSysFunc[j].sSysFuncNo.c_str()) == 0)
					lpsubmoudleinfo->bShowInList = TRUE;
			}
		}

		{	////从数据库装载用户及分支机构信息
			string strError	= "";
			
			if(m_branchArray.size() == 0)
			{
				_pQueryAllRIM30BranchInfo		QueryAllRIM30BranchInfo		= (_pQueryAllRIM30BranchInfo)GetProcAddress(m_hToolkitLibrary, "QueryAllRIM30BranchInfo");

				if(QueryAllRIM30BranchInfo)
				{
					vector<RIM30BRANCHINFO>		lcArray;
					QueryAllRIM30BranchInfo(lcArray, strError);

					for (UINT i = 0; i < lcArray.size(); ++i)
					{
						LPRIM30BRANCHINFO	lprim30branchinfo	= &(lcArray.at(i));
						LPBRANCHINFO		lpbranchinfo		= new BRANCHINFO;

						lpbranchinfo->Deptname					= CA2T(lprim30branchinfo->Deptname.c_str());	
						lpbranchinfo->Parentid					= CA2T(lprim30branchinfo->Parentid.c_str());	
						lpbranchinfo->Tel						= CA2T(lprim30branchinfo->Tel.c_str());	
						lpbranchinfo->Fax						= CA2T(lprim30branchinfo->Fax.c_str());	
						lpbranchinfo->Email						= CA2T(lprim30branchinfo->Email.c_str());	
						lpbranchinfo->Manager					= CA2T(lprim30branchinfo->Manager.c_str());	
						lpbranchinfo->Note						= CA2T(lprim30branchinfo->Note.c_str());	
						lpbranchinfo->Deptlevel					= CA2T(lprim30branchinfo->Deptlevel.c_str());	
						lpbranchinfo->Deptid					= CA2T(lprim30branchinfo->Deptid.c_str());	
						lpbranchinfo->Delflag					= CA2T(lprim30branchinfo->Delflag.c_str());	

						m_branchArray.push_back(lpbranchinfo);
					}
				}

				for (UINT i = 0; i < m_branchArray.size(); ++i)
				{	/////初始化父机构信息
					LPBRANCHINFO lpbranchinfo = m_branchArray.at(i);

					for (UINT j = 0; j < m_branchArray.size(); ++j)
					{
						LPBRANCHINFO lpbranchinfoTemp = m_branchArray.at(j);
						if (lpbranchinfo->Parentid == lpbranchinfoTemp->Deptid)
						{
							lpbranchinfo->pParentBranch = lpbranchinfoTemp;
							break;
						}
					}
				}
			}

			if(processHwnd)
				PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);

			if(m_userArray.size() == 0)
			{	
				_pQueryAllRIM30UserInfo			QueryAllRIM30UserInfo		= (_pQueryAllRIM30UserInfo)GetProcAddress(m_hToolkitLibrary, "QueryAllRIM30UserInfo");

				if(QueryAllRIM30UserInfo)
				{
					vector<RIM30USERINFO>		lcArray;
					QueryAllRIM30UserInfo(lcArray, strError);

					for (UINT i = 0; i < lcArray.size(); ++i)
					{
						LPRIM30USERINFO		lprim30userinfo		= &(lcArray.at(i));
						LPUSERINFO			lpuserinfo			= new USERINFO;

						lpuserinfo->UserID						= CA2T(lprim30userinfo->UserID.c_str());
						lpuserinfo->UserName					= CA2T(lprim30userinfo->UserName.c_str());
						lpuserinfo->EmpNO						= CA2T(lprim30userinfo->EmpNO.c_str());
						lpuserinfo->AreaCode					= CA2T(lprim30userinfo->AreaCode.c_str());
						lpuserinfo->DeptID						= CA2T(lprim30userinfo->DeptID.c_str());
						lpuserinfo->DeptName					= CA2T(lprim30userinfo->DeptName.c_str());

						m_userArray.push_back(lpuserinfo);
					}
				}
				
				for (UINT i = 0; i < m_userArray.size(); ++i)
				{	////////初始化用户 所属机构
					LPUSERINFO lpuserinfo = m_userArray.at(i);

					for (UINT j = 0; j < m_branchArray.size(); ++j)
					{
						LPBRANCHINFO lpbranchinfo = m_branchArray.at(j);
						if ((lpuserinfo->DeptID == lpbranchinfo->Deptid) && (lpuserinfo->DeptName == lpbranchinfo->Deptname))
						{
							lpuserinfo->pBranchInfo = lpbranchinfo;
							lpuserinfo->branch		= lpbranchinfo->Deptname;
							break;
						}
					}
				}
			}

			if(processHwnd)
				PostMessage(processHwnd, WM_PROGRESS, process0_100 += 2, 0);
		}


		if(bOffLine)
		{
			LPUSERINFO lpuserinfo = new USERINFO();

			lpuserinfo->userTypeID	= USERINFO::USER_ID_UNKNOWN;
			HOSTENT *phosent		= gethostbyname(NULL);
			if (phosent)
			{
				lpuserinfo->UserName= _T("RX_") + CString(phosent->h_name);
				lpuserinfo->UserID	= lpuserinfo->UserName;
			}
			else
			{
				lpuserinfo->UserName= GetNewGuid();
				lpuserinfo->UserID	= lpuserinfo->UserName;
			}
			lpuserinfo->branch		= USERINFO::UNKNOWNBRANCHSTR;
			lpuserinfo->state		= USERINFO::USER_STATE_ONLINE;

			m_userArray.push_back(lpuserinfo);

			//设置当前登录用户
			m_pLogonUser = lpuserinfo;
			m_pLogonUser->ipList.insert(_T("127.0.0.1"));		/////上线了
			USERINFO::IPListLock.unlock();
		}
		else
		{
			for (vector<LPUSERINFO>::iterator it = m_userArray.begin(); it != m_userArray.end(); it++)
			{	//这里以后是要通过接口来进行判断的
				if(	_tcscmp((*it)->UserID.c_str(), m_systemSet.lastUserID.c_str())		== 0)
				{
					m_pLogonUser = (*it);
					m_pLogonUser->ipList.insert(_T("127.0.0.1"));		/////上线了
					break;
				} 
			}
			USERINFO::IPListLock.unlock();
		}
	
		////登录成功后  要保存当前的用户帐户信息
		if(m_pLogonUser)
		{
			m_pLogonUser->userToken = CA2T(userToken.c_str());
			SaveLogonFile(m_logonFilePath);
			errorInfo = _T("登录成功");

			if(bOffLine == FALSE)
			{	////启用站点状态获取服务
				USHORT	nPort			= 0;		//用于保存目标HTTP服务端口  
				DWORD	dwServiceType	= 0;		//dwServiceType用于保存服务类型，dwRet用于保存提交GET请求返回的状态号  
				CString strServerIP		= _T(""); 	//strServer用于保存服务器地址，strObject用于保存文件对象名称  
				CString strObject		= _T(""); 	//strServer用于保存服务器地址，strObject用于保存文件对象名称  

				//解析URL，获取信息  
				if(!AfxParseURL(CA2T(strEmsUrl.c_str()), dwServiceType, strServerIP, strObject, nPort))  
				{  
					::MessageBox(NULL, _T("EMS服务解析错误！"), _T("RIM3.0 错误"), MB_OK);
				}  

				CThreadEmsClient::GetInstance()->Begin(string(CT2A(strServerIP)), 8601);
			}
		}
		else
		{
			errorInfo = _T("用户不存在");
		}

		if(processHwnd)
			PostMessage(processHwnd, WM_PROGRESS, process0_100 = 100, 0);
	}
	else
	{
		CThreadEmsClient::GetInstance()->End();
		CThreadEmsClient::GetInstance()->ReleaseInstance();

		{//保存系统设置文件

			SaveMoudles(m_rimConfigPath);
			SaveRecentFile(m_recentFilePath);	////此时最近使用列表已经更新保存到数组中，可以将最新的记录保存到文件 中。
		}

		////【zy 2015-01-25 ↓】 执行清除操作
		CQueueData* pQueueData = NULL;
		while (m_UDPReceiveQueue.GetQueueCount())	{ m_UDPReceiveQueue.Pop(&pQueueData);	delete pQueueData; }
		while (m_UDPSendQueue.GetQueueCount())		{ m_UDPSendQueue.Pop(&pQueueData);		delete pQueueData; }
		while (m_TCPReceiveQueue.GetQueueCount())	{ m_TCPReceiveQueue.Pop(&pQueueData);	delete pQueueData; }
		while (m_TCPSendQueue.GetQueueCount())		{ m_TCPSendQueue.Pop(&pQueueData);		delete pQueueData; }

		////使下次再使用用户信息时，就会重新初始化用户信息
		ClearUserBranchInfo();
		ClearStationInfo();
		////【zy 2015-01-25 ↑】
		ClearWebAddrInfo();
	}
}
