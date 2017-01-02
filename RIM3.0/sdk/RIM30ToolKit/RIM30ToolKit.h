//********************************************************************
// RIM30TOOLKIT.H 文件注释
// 文件名 　: RIM30TOOLKIT.H
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 14:28
// 文件描述 : 
//*********************************************************************
#pragma once

#include <string>
#include <vector>
using namespace std;

#include "SqliteDataDefine.h"

/////////////////////////////////////////////////////////	访问单个表	////////////////////////////////////////////////////////

/*查询监测站表*/
typedef bool						(*_pQueryAllStation)					(std::vector<RTMSTATION>					&lcArray, string &strError); 
/*查询设备表*/
typedef bool						(*_pQueryAllDevice)						(std::vector<RTMDEVICE>						&lcArray, string &strError); 
/*查询设备功能表*/
typedef bool						(*_pQueryAllRtmDeviceFunc)				(std::vector<RTMDEVICEFUNC>					&lcArray, string &strError);
/*查询设备所有功能表*/
typedef bool						(*_pQueryAllRTMRefDeviceFunc)			(std::vector<RTMREFDEVICEFUNC>				&lcArray, string &strError);
/*查询地区编码表*/
typedef bool						(*_pQueryAllComRegionNo)				(std::vector<COMREGIONNO>					&lcArray, string &strError);
/*查询设备协议表*/
typedef bool						(*_pQueryAllRtmRefDeviceProtocol)		(std::vector<RTMREFDEVICEPROTOCOL>			&lcArray, string &strError);
/*查询监测站类型表*/
typedef bool						(*_pQueryAllRimRefStationType)			(std::vector<RTMREFSTATIONTYPE>				&lcArray, string &strError);
/*查询设备类型表*/
typedef bool						(*_pQueryAllRimRefDeviceType)			(std::vector<RTMREFDEVICETYPE>				&lcArray, string &strError);
/*查询监测机构表*/
typedef bool						(*_pQueryAllRtmMonOrg)					(std::vector<RTMMONORG>						&lcArray, string &strError);
/*查询控制中心表*/
typedef bool						(*_pQueryAllRtmMonCtrlCenter)			(std::vector<RTMMONCTRLCENTER>				&lcArray, string &strError) ;
/*查询设备功能对应天线表*/
typedef bool						(*_pQueryAllRtmStationDeviceFuncAnt)	(std::vector<RTMSTATIONDEVICEFUNCANT>		&lcArray, string &strError);
/*查询天线表*/
typedef bool						(*_pQueryAllRtmAnt)						(std::vector<RTMANT>						&lcArray, string &strError);
/*查询设备型号表*/
typedef bool						(*_pQueryAllRtmRefDeviceModel)			(std::vector<RTMREFDEVICEMODEL>				&lcArray, string &strError);
/*查询设备功能参数表*/
typedef bool						(*_pQueryAllRtmRefDeviceModelFuncParam)	(std::vector<RTMREFDEVICEMODELFUNCPARAM>	&lcArray, string &strError);
/*查询远程控制箱表*/
typedef bool                        (*_pQueryAllRtmRnc)                     (std::vector<RIM30RTMRNC>                   &lcArray, string &strError);
/*查询监测站授权表*/
typedef bool                        (*_pQueryAllComAuthSysDataStation)      (std::vector<RIM30COMAUTHSYSDATASTATION>    &lcArray, string &strError);
/*查询系统功能授权表*/
typedef bool                        (*_pQueryAllComAuthSysFunc)				(std::vector<RIM30COMAUTHSYSFUNC>			&lcArray, string &strError);
/*查询用户信息列表*/
typedef bool						(*_pQueryAllRIM30UserInfo)				(std::vector<RIM30USERINFO>					&lcArray, string &strError);
/*查询用户所属机构的列表*/
typedef bool						(*_pQueryAllRIM30BranchInfo)			(std::vector<RIM30BRANCHINFO>				&lcArray, string &strError);


/////////////////////////////////////////////////////////	扩展功能	////////////////////////////////////////////////////////
typedef bool						(*_pUpdateDataBySql)					(const std::string &sSqlStr, string &strError);
/*查询具有指定功能的站点列表*/
typedef std::vector<ULONG>			(*_pQueryStationGroupByFuncID)			(ULONG lnFuncID);
/** 查询具有指定功能的区域编号 */
typedef std::vector<std::string>	(*_pQueryRegionGroupByFuncID)			(ULONG lnFuncID);
/** 查询指定站点ID的站点名称 */
typedef std::string					(*_pQueryStationName)					(ULONG lnStationID);
/** 查询指定ID的设备功能名称 */
typedef std::string                 (*_pQueryFuncName)						(ULONG lnFuncID);
/** 查询指定ID的站点类型名称 */
typedef std::string                 (*_pQueryStationTypeName)				(ULONG lnStationType);
/** 查询指定监测机构 */
typedef bool						(*_pQueryMonOrg)						(std::string	sOrgCode,	RTMMONORG &cOrg);
/** 查询指定监测站 */
typedef bool						(*_pQueryStation)						(std::string	sStationID, RTMSTATION &cStation);
/** 通过站点,设备ID查询设备型号 */
typedef bool						(*_pQueryDevice)						(ULONG			lnStationID, ULONG lnDeviceID, RTMDEVICE &lcDevice);
/** 通过设备型号及功能ID查询设备参数 */
typedef bool						(*_pQueryDeviceModelFuncParam)			(std::string	sDeviceModel, LONG nFuncID , std::string &sXMLParam);
/** 判断指定的设备组合是否存在 */
typedef bool                        (*_pIsRtmGroup)							(ULONG			lnStationID, ULONG lnDeviceID, ULONG lnFuncID);
/** 根据区域编码查询区域名称 */
typedef bool						(*_pQueryRegionName)					(std::string	sRegionNo, std::string &sRegionName);
