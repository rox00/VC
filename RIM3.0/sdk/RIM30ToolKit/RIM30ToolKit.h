//********************************************************************
// RIM30TOOLKIT.H �ļ�ע��
// �ļ��� ��: RIM30TOOLKIT.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 14:28
// �ļ����� : 
//*********************************************************************
#pragma once

#include <string>
#include <vector>
using namespace std;

#include "SqliteDataDefine.h"

/////////////////////////////////////////////////////////	���ʵ�����	////////////////////////////////////////////////////////

/*��ѯ���վ��*/
typedef bool						(*_pQueryAllStation)					(std::vector<RTMSTATION>					&lcArray, string &strError); 
/*��ѯ�豸��*/
typedef bool						(*_pQueryAllDevice)						(std::vector<RTMDEVICE>						&lcArray, string &strError); 
/*��ѯ�豸���ܱ�*/
typedef bool						(*_pQueryAllRtmDeviceFunc)				(std::vector<RTMDEVICEFUNC>					&lcArray, string &strError);
/*��ѯ�豸���й��ܱ�*/
typedef bool						(*_pQueryAllRTMRefDeviceFunc)			(std::vector<RTMREFDEVICEFUNC>				&lcArray, string &strError);
/*��ѯ���������*/
typedef bool						(*_pQueryAllComRegionNo)				(std::vector<COMREGIONNO>					&lcArray, string &strError);
/*��ѯ�豸Э���*/
typedef bool						(*_pQueryAllRtmRefDeviceProtocol)		(std::vector<RTMREFDEVICEPROTOCOL>			&lcArray, string &strError);
/*��ѯ���վ���ͱ�*/
typedef bool						(*_pQueryAllRimRefStationType)			(std::vector<RTMREFSTATIONTYPE>				&lcArray, string &strError);
/*��ѯ�豸���ͱ�*/
typedef bool						(*_pQueryAllRimRefDeviceType)			(std::vector<RTMREFDEVICETYPE>				&lcArray, string &strError);
/*��ѯ��������*/
typedef bool						(*_pQueryAllRtmMonOrg)					(std::vector<RTMMONORG>						&lcArray, string &strError);
/*��ѯ�������ı�*/
typedef bool						(*_pQueryAllRtmMonCtrlCenter)			(std::vector<RTMMONCTRLCENTER>				&lcArray, string &strError) ;
/*��ѯ�豸���ܶ�Ӧ���߱�*/
typedef bool						(*_pQueryAllRtmStationDeviceFuncAnt)	(std::vector<RTMSTATIONDEVICEFUNCANT>		&lcArray, string &strError);
/*��ѯ���߱�*/
typedef bool						(*_pQueryAllRtmAnt)						(std::vector<RTMANT>						&lcArray, string &strError);
/*��ѯ�豸�ͺű�*/
typedef bool						(*_pQueryAllRtmRefDeviceModel)			(std::vector<RTMREFDEVICEMODEL>				&lcArray, string &strError);
/*��ѯ�豸���ܲ�����*/
typedef bool						(*_pQueryAllRtmRefDeviceModelFuncParam)	(std::vector<RTMREFDEVICEMODELFUNCPARAM>	&lcArray, string &strError);
/*��ѯԶ�̿������*/
typedef bool                        (*_pQueryAllRtmRnc)                     (std::vector<RIM30RTMRNC>                   &lcArray, string &strError);
/*��ѯ���վ��Ȩ��*/
typedef bool                        (*_pQueryAllComAuthSysDataStation)      (std::vector<RIM30COMAUTHSYSDATASTATION>    &lcArray, string &strError);
/*��ѯϵͳ������Ȩ��*/
typedef bool                        (*_pQueryAllComAuthSysFunc)				(std::vector<RIM30COMAUTHSYSFUNC>			&lcArray, string &strError);
/*��ѯ�û���Ϣ�б�*/
typedef bool						(*_pQueryAllRIM30UserInfo)				(std::vector<RIM30USERINFO>					&lcArray, string &strError);
/*��ѯ�û������������б�*/
typedef bool						(*_pQueryAllRIM30BranchInfo)			(std::vector<RIM30BRANCHINFO>				&lcArray, string &strError);


/////////////////////////////////////////////////////////	��չ����	////////////////////////////////////////////////////////
typedef bool						(*_pUpdateDataBySql)					(const std::string &sSqlStr, string &strError);
/*��ѯ����ָ�����ܵ�վ���б�*/
typedef std::vector<ULONG>			(*_pQueryStationGroupByFuncID)			(ULONG lnFuncID);
/** ��ѯ����ָ�����ܵ������� */
typedef std::vector<std::string>	(*_pQueryRegionGroupByFuncID)			(ULONG lnFuncID);
/** ��ѯָ��վ��ID��վ������ */
typedef std::string					(*_pQueryStationName)					(ULONG lnStationID);
/** ��ѯָ��ID���豸�������� */
typedef std::string                 (*_pQueryFuncName)						(ULONG lnFuncID);
/** ��ѯָ��ID��վ���������� */
typedef std::string                 (*_pQueryStationTypeName)				(ULONG lnStationType);
/** ��ѯָ�������� */
typedef bool						(*_pQueryMonOrg)						(std::string	sOrgCode,	RTMMONORG &cOrg);
/** ��ѯָ�����վ */
typedef bool						(*_pQueryStation)						(std::string	sStationID, RTMSTATION &cStation);
/** ͨ��վ��,�豸ID��ѯ�豸�ͺ� */
typedef bool						(*_pQueryDevice)						(ULONG			lnStationID, ULONG lnDeviceID, RTMDEVICE &lcDevice);
/** ͨ���豸�ͺż�����ID��ѯ�豸���� */
typedef bool						(*_pQueryDeviceModelFuncParam)			(std::string	sDeviceModel, LONG nFuncID , std::string &sXMLParam);
/** �ж�ָ�����豸����Ƿ���� */
typedef bool                        (*_pIsRtmGroup)							(ULONG			lnStationID, ULONG lnDeviceID, ULONG lnFuncID);
/** ������������ѯ�������� */
typedef bool						(*_pQueryRegionName)					(std::string	sRegionNo, std::string &sRegionName);
