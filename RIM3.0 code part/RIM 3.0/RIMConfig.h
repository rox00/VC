//********************************************************************
//	RIMCONFIG.H �ļ�ע��
//	�ļ���		: RIMCONFIG.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	����		: ����
//	����ʱ��	: 2014/12/5 9:05
//	�ļ�����	: 
//*********************************************************************	

//CREATE TABLE MAIN.[T_RIM30_USERINFO] 
//( 
// [USERID] TEXT , 
// [USERNAME] TEXT , 
// [EMPNO] TEXT , 
// [AREACODE] TEXT , 
// [DEPTID] TEXT , 
// [DEPTNAME] TEXT , 
// [PARENTDEPTID] TEXT  
// ) ;

#pragma once
#include "StdAfx.h"

typedef struct tagFILEINFO
{
public:
	tagFILEINFO()
	{
		name	= _T("");
		hash	= _T("");
		size	= 0;
	}

	tstring 	name;			//�ļ����·��
	tstring 	hash;			//�ļ���MD5��ֵ
	DWORD  		size;			//�ļ��Ĵ�С�ߴ�
}FILEINFO, *LPFILEINFO;

class tagTRANSFILEINFO : public CQueueData
{
public:
	tagTRANSFILEINFO()
	{
		bDic					= FALSE;
		totalSize				= 0;
		applicationName			= _T("");
		guidStr					= GetNewGuid();
		version					= _T("0");

		transFlag				= TRANS_FLAG_PRETRANS;

		///CMDָ��
		updateCmdBase			= string("UPDATE:") + string(CT2A(guidStr.c_str())) + " ";
		beginCmd				= updateCmdBase + string("BEGIN");
		cancelCmd				= updateCmdBase + string("CANCEL");
		endCmd					= updateCmdBase + string("END");
		nameCmdBase				= updateCmdBase + string("NAME=");

		transError				= _T("");
		rateTemp				= 0;  //byte;
		transBytesTemp			= 0;
	}

	~tagTRANSFILEINFO()
	{
	}


	enum TRANSFLAG{
		TRANS_FLAG_PRETRANS		= 1,						//��ʼ���촫��״̬		Ԥ����״̬����Ҫ���ó�ʼ����
		TRANS_FLAG_TRANSBEGIN	= 2,						//׼�����ͻ����		��ʼ�����󣬿�ʼ������Ҫ�����û�����
		TRANS_FLAG_TRANSING		= 3,						//���ͻ���ս�����		��Ȩ����󣬵Ĵ���״̬
		TRANS_FLAG_TRANSCANCEL	= 4,						//ȡ������				�û�ȡ������
		TRANS_FLAG_TRANSPREEND	= 5,						//����Ԥ����			�������״̬����ȡ����Ĵ������״̬
		TRANS_FLAG_TRANSEND		= 6,						//�������				���д������ݿ�����յı�־
	};

protected:
	TRANSFLAG					transFlag;					//��ǰ����״̬
	CCriticalSection			m_cs;

protected:
	string						updateCmdBase;
	string						beginCmd;
	string						cancelCmd;
	string						endCmd;
	string						nameCmdBase;

	tstring						guidStr;					//����һ��Ψһ�Ե�GUID�ַ���
	BOOL						bDic;						//�Ƿ����ļ���
	tstring						applicationName;			//Ҫ���µ��ļ�������	�ļ���������������洢�Ϸŵ��ļ���Ŀ¼��
	DWORD64						totalSize;					//�����ܴ�С
	tstring						version;					//Ҫ���µ��ļ��İ汾	��������

public:

	string						GetBeginCmd()			{ return beginCmd;			};
	string						GetCancelCmd()			{ return cancelCmd;			};
	string						GetEndCmd()				{ return endCmd;			};
	string						GetNameBaseCmd()		{ return nameCmdBase;		};

	TRANSFLAG					GetTransFlag()			{ return transFlag;			};
	tstring						GetGuidStr()			{ return guidStr;			};
	BOOL						IsDic()					{ return bDic;				};
	DWORD64						GetTotalSize()			{ return totalSize;			};
	tstring						GetApplicationName()	{ return applicationName;	};
	tstring						GetVersion()			{ return version;			};

	BOOL						SafeModifyTransFlag(TRANSFLAG flag);
	static void					InitTransFileInfoByPath(tagTRANSFILEINFO* pLogfileInfo, tstring m_applicationName, tstring DicOrPath = _T(""));
	static string				MakeXmlStr(tagTRANSFILEINFO* pLogfileInfo);
	static bool					LoadFromXmlStr(string xmlStr, tagTRANSFILEINFO* pLogfileInfo);


	tstring						transError;					//�����������ֵ���������
	LONGLONG					transBytesTemp;				//�����˶����ֽ�
	LONGLONG					rateTemp;					//�����ٶ�
};
typedef tagTRANSFILEINFO  TRANSFILEINFO;
typedef tagTRANSFILEINFO*  LPTRANSFILEINFO;

typedef class tagINFOTYPE
{
public:

	enum{
		INFOTYPE_SUBMOUDLE		= 1,
		INFOTYPE_MEASURE		= 2,
		INFOTYPE_DEVICE			= 3,
		INFOTYPE_STATION		= 4,
		INFOTYPE_BRANCH			= 5,
		INFOTYPE_USER			= 6,
	};

	DWORD						type;
}INFOTYPE, *LPINFOTYPE;

//��ϵͳ��Ϣ
typedef class tagSUBMOUDLEINFO
{
public:
	INFOTYPE					infoType;
	tagSUBMOUDLEINFO()
	{
		infoType.type			= INFOTYPE::INFOTYPE_SUBMOUDLE;

		bShowInList				= TRUE;
		sysFuncNo				= _T("");
		bDefaultOpen			= FALSE;
		bSupportOutLine			= FALSE;
		moudleName				= _T("");
		moudleEXEPath			= _T("");
		selPath					= _T("");
		unselPath				= _T("");
		pImagesel				= NULL;
		pImageUnsel				= NULL;
		rect					= CRect(0, 0, 0, 0);
	}

	tagSUBMOUDLEINFO(const tagSUBMOUDLEINFO& ob)
	{
		*this = ob;
	}

	tagSUBMOUDLEINFO& operator=(const tagSUBMOUDLEINFO& ob)
	{
		bShowInList				= ob.bShowInList;
		sysFuncNo				= ob.sysFuncNo;
		bDefaultOpen			= ob.bDefaultOpen;
		bSupportOutLine			= ob.bSupportOutLine;
		moudleName				= ob.moudleName;
		selPath					= ob.selPath;
		unselPath				= ob.unselPath;
		//����������Ȼ���п�����ԭ���ϲ�Ӧ�������κ��޸�
		pImagesel				= ob.pImagesel;
		pImageUnsel				= ob.pImageUnsel;
		rect					= ob.rect;

		return *this;
	}

	BOOL						bShowInList;	//�Ƿ����б�����ʾ
	tstring						sysFuncNo;
	BOOL						bDefaultOpen;	//Ĭ�ϴ�
	BOOL						bSupportOutLine;//�Ƿ�֧���ѻ�
	tstring						moudleName;		//��ϵͳ����
	tstring						moudleEXEPath;	//��ϵͳ����
	tstring						selPath;		//ѡ��ʱ��ͼƬ���·��
	tstring						unselPath;		//δѡ��ʱ��ͼƬ���·��

	Image						*pImagesel;		//ѡ��ʱ��ͼƬ���غ�Ķ���
	Image						*pImageUnsel;	//δѡ��ʱ��ͼƬ���غ�Ķ���
	CRect						rect;
} SUBMOUDLEINFO, *LPSUBMOUDLEINFO;


/////////////////////////////////////////////////////////////////��zy 2014-11-19 ���� �������
//������Ϣ
typedef class tagMEASUREINFO
{
public:
	INFOTYPE					infoType;
	tagMEASUREINFO()
	{
		infoType.type			= INFOTYPE::INFOTYPE_MEASURE;

		pRtmDeviceFunTemp		= NULL;

		bShowInList				= TRUE;
		measueName				= _T("");
		measureID				= 0;
	}
	LPRTMDEVICEFUNC				pRtmDeviceFunTemp;

	BOOL						bShowInList;	//�Ƿ����б�����ʾ
	tstring						measueName;		//��������
	tstring						keyWord;		//��������
	int							measureID;		//����ID

} MEASUREINFO, *LPMEASUREINFO;

//�豸��Ϣ
typedef class tagDEVICEINFO
{
public:
	INFOTYPE					infoType;
	enum 
	{	
		DEVICE_STATE_0			= 0,					// �豸����
		DEVICE_STATE_1			= 1,					// �豸æ
		DEVICE_STATE_2			= 2,					// �豸����
		DEVICE_STATE_3			= 3,					// �豸δ֪
		DEVICE_STATE_4			= 4,					// �豸����������
	};
	tagDEVICEINFO()
	{
		infoType.type			= INFOTYPE::INFOTYPE_DEVICE;

		pRimDeviceTemp			= NULL;

		sProtocol				= _T("");
		sDeviceType				= _T("");
		bShowInList				= TRUE;
		deviceState				= DEVICE_STATE_3;
		measureInfos.clear();
	}
	~tagDEVICEINFO()
	{
		for (UINT i = 0; i < measureInfos.size(); ++i)
		{
			LPMEASUREINFO lpmeasure = measureInfos.at(i);
			delete lpmeasure;
		}
		measureInfos.clear();
	}

	LPRTMDEVICE					pRimDeviceTemp;

	tstring						sProtocol;
	tstring						sDeviceType;
	BOOL						bShowInList;	//�Ƿ����б�����ʾ
	int							deviceState;	//�豸״̬  -1:������ 0:������ 1:����
	vector<LPMEASUREINFO>		measureInfos;

} DEVICEINFO, *LPDEVICEINFO;

//վ����Ϣ
typedef class tagSTATIONINFO
{
public:
	INFOTYPE					infoType;
	tagSTATIONINFO()
	{
		infoType.type			= INFOTYPE::INFOTYPE_STATION;

		pRimStationTemp			= NULL;
		pRegionNoTemp			= NULL;

		sStationType			= _T("");
		bShowInList 			= TRUE;
		devicesInfos.clear();
	}
	~tagSTATIONINFO()
	{
		for (UINT i = 0; i < devicesInfos.size(); ++i)
		{
			LPDEVICEINFO lpdevice = devicesInfos.at(i);
			delete lpdevice;
		}
		devicesInfos.clear();
	}
	LPRTMSTATION				pRimStationTemp;
	LPCOMREGIONNO				pRegionNoTemp;

	tstring						sStationType;
	BOOL						bShowInList;	//�Ƿ����б�����ʾ
	vector<LPDEVICEINFO>		devicesInfos;

} STATIONINFO, *LPSTATIONINFO;

////////////////////////////////////////////////////////////////////////////��zy 2014-11-19 �����������

/////////////////////////////////////////////////////////////////��zy 2014-11-19 ���� �û����

typedef class tagBRANCHINFO
{
public:
	INFOTYPE					infoType;
	tagBRANCHINFO()
	{
		infoType.type			= INFOTYPE::INFOTYPE_BRANCH;
		
		Deptname				= _T("");	
		Parentid				= _T("");	
		Tel						= _T("");	
		Fax						= _T("");	
		Email					= _T("");	
		Manager					= _T("");	
		Note					= _T("");	
		Deptlevel				= _T("");	
		Deptid					= _T("");	
		Delflag					= _T("");	

		pParentBranch			= NULL;
	}
	~tagBRANCHINFO()
	{
	}

	tstring 					Deptname;	
	tstring 					Parentid;	
	tstring 					Tel;	
	tstring 					Fax;	
	tstring 					Email;	
	tstring 					Manager;	
	tstring 					Note;	
	tstring 					Deptlevel;	
	tstring 					Deptid;	
	tstring 					Delflag;	

	tagBRANCHINFO			*pParentBranch;
}BRANCHINFO, *LPBRANCHINFO;

//�û���Ϣ
/*�����û���Ϣ���˵�� 
�û�����		ID userTypeID  ֻ��������¼���û��Żḳ��USER_ID_UNKNOWN   Ĭ��Ϊ0��Ϊ�Ϸ��û���Ҫ��֤��
�û���֧��Ϣ��	�����ж�pBranchInfo��Ϊ�ռ�Ϊ�Ϸ��û������pBranchInfoΪ�գ�branch�����Ϊ������֧���û�Ϊ�Ƿ�֧�µ��û�
////�����û�ID��������ͬ
*/
typedef class tagUSERINFO
{
public:
	INFOTYPE					infoType;
	enum {	
		USER_ID_NORMAL			= 0,
		USER_ID_UNKNOWN			= 0xFFFFFFFF,
	};
	enum {	USER_STATE_OFFLINE	= 0,					// �û�����״̬
			USER_STATE_ONLINE	= 1,					// �û�����״̬
	};
	static const  tstring		UNKNOWNBRANCHSTR;		//�û�Ĭ�ϵ�δ֪��֧�ַ���
	static CReadWriteLock		IPListLock;				//���û�IP�б������  ��ʡ�ں˶���

	tagUSERINFO()
	{
		infoType.type			= INFOTYPE::INFOTYPE_USER;

		pChatThread				= NULL;
		bShowInList				= true;

		pBranchInfo				= NULL;
		ipList.clear();

		UserID					= _T("");
		UserName				= _T("");	
		EmpNO					= _T("");	
		AreaCode				= _T("");	
		DeptID					= _T("");	
		DeptName				= _T("");	

		userTypeID				= USER_ID_NORMAL;				//˵�����û�Ĭ��Ϊ�����û���ֻ��������¼���û����������ID
		branch					= _T("");	
		state					= 0;
		userToken				= _T("");

		onlinePngPath			= _T("");	
		offlinePngPath			= _T("");
		userUdpPortTemp			= UDP_CHATSERVER_PORT;
		userTcpPortTemp			= TCP_CHATSERVER_PORT;
		stateUpdateClk			= clock();
	}
	~tagUSERINFO()
	{
		while (receiveFileQueue.GetQueueCount())
		{	//�����Ϣ����
			CQueueData *pData = NULL;
			receiveFileQueue.Pop(&pData);
			((LPTRANSFILEINFO)pData)->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL);
			while (((LPTRANSFILEINFO)pData)->GetTransFlag() != tagTRANSFILEINFO::TRANS_FLAG_TRANSEND) Sleep(1); //�ȴ���������ÿ�
			delete pData;
		}

		while (sendFileQueue.GetQueueCount())
		{	//�����Ϣ����
			CQueueData *pData = NULL;
			sendFileQueue.Pop(&pData);
			((LPTRANSFILEINFO)pData)->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL);
			while (((LPTRANSFILEINFO)pData)->GetTransFlag() != tagTRANSFILEINFO::TRANS_FLAG_TRANSEND) Sleep(1); //�ȴ���������ÿ�
			delete pData;
		}

		///////ע�⣬���պͷ��Ͷ���Ҫ�����߳̽���ǰ���֮ 
		if (pChatThread)
		{
			pChatThread->PostThreadMessage(WM_QUIT, 0, 0);
			while (pChatThread) Sleep(1);		//�ȴ��߳� �Զ�ɾ�����ÿ�
		}

		while(receiveMsgQueue.GetQueueCount())
		{	//�����Ϣ����
			CQueueData *pData = NULL;
			receiveMsgQueue.Pop(&pData);
			delete pData;
		}
	}
	LPBRANCHINFO				pBranchInfo;		//������Ϣ

	///////�������ݿ��ֶ�
	tstring						UserID;				//�û���Ψһ��ʶ
	tstring						UserName;			//�û�����
	tstring 					EmpNO;	
	tstring 					AreaCode;	
	tstring 					DeptID;				//������GUID
	tstring 					DeptName;			//��������������
	

	tstring						onlinePngPath;		//����ͼƬ
	tstring						offlinePngPath;		//����ͼƬ

	DWORD						userTypeID;			//�û�ID���
	tstring						branch;				//������ϼ����ŵ�����
	int							state;				//�û�״̬
	tstring						userToken;			

	//������Դ���ݻ�ȡ

	set<tstring>				ipList;				//ͬһ�û���¼�Ĳ�ͬ���Ե�IP�б�;
	short						userUdpPortTemp;	//�û��󶨵Ķ˿�
	short						userTcpPortTemp;	//�û��󶨵Ķ˿�

	clock_t						stateUpdateClk;		//�û�״̬ʵʱ����ʱ��

	CWinThread					*pChatThread;		//���û����ڵ�����Ի����߳�
	CUserDataQueue				receiveMsgQueue;	//��ǰ���յ���Ϣ����
	bool						bShowInList;		//�Ƿ����б�����ʾ

	CUserDataQueue				receiveFileQueue;	//��ǰ���յ��ļ�����
	CUserDataQueue				sendFileQueue;		//��ǰ���͵��ļ�����	////Ҳ�������ļ��б�����ʾ

} USERINFO, *LPUSERINFO;

////////////////////////////////////////////////////////////////////////////��zy 2014-11-19 �����û����

////��zy 2014-12-23 ����
////ϵͳ����
typedef class tagSYSTEMSET
{
public:
	tagSYSTEMSET()
	{
		lastUserID				= _T("");			
		lastPass				= _T("");			
		lastDmsIP				= _T("");		
		lastRmsIP				= _T("");		
		lastMsaIP				= _T("");		
		bRememberPass			= FALSE;		
		bAutoLogon				= FALSE;			
	}
	~tagSYSTEMSET()
	{
	}

	tstring						lastUserID;			//�ϴε�¼������
	tstring						lastPass;			//�ϴε�¼������
	tstring						lastDmsIP;			//�ϴε�¼�ķ���˵�IP	

	tstring						lastRmsIP;			//�������IP	
	tstring						lastMsaIP;			//ͳ�Ʒ����IP	

	BOOL						bRememberPass;		//�Ƿ��ס����
	BOOL						bAutoLogon;			//�Ƿ��Զ���¼
	BOOL						bOffLineLogon;		//�Ƿ��Я��¼

} SYSTEMSET, *LPSYSTEMSET;

////�û�����
typedef class tagUSERSET
{
public:
	tagUSERSET()
	{
		
	}
	~tagUSERSET()
	{
	}


} USERSET, *LPUSERSET;
////��zy 2014-12-23 ����

////����½ṹ
typedef class tagTABLEUPDATE
{
public:
	tagTABLEUPDATE()
	{
		tableName	= "";
		updateTime	= "";
	}
	~tagTABLEUPDATE()
	{
	}
	string			tableName;
	string			updateTime;
	
} TABLEUPDATE, *LPTABLEUPDATE;


class RIMConfig
{
public:
	~RIMConfig(void);

protected:
	RIMConfig(void);						//��ֹ��������
	static RIMConfig*						m_pRIMConfig;	//�ڲ�ʵ��ָ��
	HMODULE									m_hToolkitLibrary;
	CGsoapServer							m_gsoapServer;
	CStringArray							m_macArray;		//����MAC����

	//************************************
	// Method:    ParseConfig
	// FullName:  RIMConfig::ParseConfig
	// Access:    protected 
	// Returns:   BOOL						�ɹ�����TRUE,ʧ�ܷ���FALSE
	// Qualifier:							����RIMȫ�������ļ� 
	// Parameter: tstring path				�����ļ�·��
	//************************************
	BOOL									ParseConfig(tstring path);
	void									ParseSqlTableNodes(TiXmlNode* moudleNodes);
	//************************************
	// Method:    ParseModuleNodes
	// FullName:  RIMConfig::ParseModuleNodes
	// Access:    protected 
	// Returns:   void
	// Qualifier:							������ϵͳģ�����µ�������Ϣ
	// Parameter: TiXmlNode * statNodes
	//************************************
	void									ParseModuleNodes(TiXmlNode* moudleNodes);



	BOOL 									ParseLogonFile(tstring path);
	BOOL									ParseRecentFile(tstring path);
	void									ParseStationsInfo();

	BOOL 									SaveMoudles(tstring path);
	BOOL									SaveSqlTablesNodes(TiXmlNode* sqlTableNodes);
	BOOL									SaveModulesNodes(TiXmlNode* moudleNodes);
	BOOL									SaveLogonFile(tstring path);
	BOOL									SaveRecentFile(tstring path);

	void 									ClearAll();
	void 									ClearMoudleInfo();
	void 									ClearStationInfo();
	void 									ClearUserBranchInfo();

	void									ClearTableUpdateInfo();
	void									ClearWebAddrInfo();

	tstring									m_rimConfigPath;
	tstring									m_logonFilePath;
	tstring									m_recentFilePath;

	vector<RTMSTATION>						m_rtmStationVec;		//վ���
	vector<RTMDEVICE>						m_rtmDeviceVec;			//�豸��
	vector<RTMDEVICEFUNC>					m_rtmDeviceFuncVec;		//�豸���ܱ�
	vector<RTMREFDEVICEFUNC>				m_rtmRefDeviceFuncVec;	//�����豸���ܱ�
	vector<COMREGIONNO>						m_comRegionNoVec;		//���������
	vector<RTMREFSTATIONTYPE>				m_stationTypeVec;		//վ������
	vector<RTMREFDEVICETYPE>				m_deviceTypeVec;		//�豸����
	vector<RTMREFDEVICEPROTOCOL>			m_deviceProtocolVec;	//�豸Э������
	vector<RIM30COMAUTHSYSFUNC>				m_authSysFunc;			//ϵͳ������Ȩ��

public:
	static LPUSERINFO						m_pLogonUser;			//��¼�������û�
	static RIMConfig* 						GetInstance();
	static void 							ReleaseInstance();

	//************************************
	// Method:    ReInitStationInfo
	// FullName:  RIMConfig::ReInitStationInfo
	// Access:    public 
	// Returns:   void
	// Qualifier:							���³�ʼ��վ����Ϣ
	//************************************
	//void									ReInitStationInfo();
	//************************************
	// Method:    DoLogon
	// FullName:  RIMConfig::DoLogon
	// Access:    public 
	// Returns:   void
	// Qualifier:							�û���¼�������������������
	// Parameter: BOOL bAreadyLogon			�Ƿ��¼��TRUE ��¼ FALSE �ǳ�
	// Parameter: BOOL bOffLine				�Ƿ��Ǳ�Я��¼��TRUE ��Я�� FALSE �Ǳ�Я
	// Parameter: tstring & errorInfo		������Ϣ��ʾ
	// Parameter: HWND processHwnd			�����������Ĵ��ڣ�������ʾ����֪ͨ
	//************************************
	void									DoLogon(BOOL bLogon, BOOL bOffLine, tstring& errorInfo, HWND processHwnd);

	vector<LPSUBMOUDLEINFO>					m_moudleInfoArray;		//��ģ����Ϣ����
	vector<LPSTATIONINFO>					m_stationArray;			//վ����Ϣ���� �����豸���豸�������Ĳ���������Ϣ
	vector<LPUSERINFO>						m_userArray;			//�û���Ϣ����
	vector<LPBRANCHINFO>					m_branchArray;			//�û����ڵĻ�������
	map<string, map<string, vector<long>>>	m_recentMap;			//���ʹ����Ϣ���� �ֱ��Ӧ վ���ţ��豸��ţ����ܱ��


	vector<LPTABLEUPDATE>					m_tableUpdateArray;		//sqliteÿ����ĸ����б�
	map<DWORD, vector<LPWEBADDRINFO>>		m_webAddrInfoMap;		//���������ַ��Ϣ����

	//ϵͳ���ü���������
	SYSTEMSET								m_systemSet;
	USERSET									m_userSet;

	///UDP��������뷢����Ϣ�Ķ���
	CUserDataQueue							m_UDPReceiveQueue;		//��Ϣ���ն���
	CUserDataQueue							m_UDPSendQueue;			//��Ϣ���Ͷ���
	///TCP��������뷢����Ϣ�Ķ���
	CUserDataQueue							m_TCPReceiveQueue;		//��Ϣ���ն���
	CUserDataQueue							m_TCPSendQueue;			//��Ϣ���Ͷ���

	tstring									m_modifyPassTheme;		//�޸�����������ɫ
	tstring									m_welcomeText;			//��ӭ�ı�
	long									m_welcomeFontSize;		//��ӭ�ı�����Ĵ�С
};
