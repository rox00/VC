//********************************************************************
//	RIMCONFIG.H 文件注释
//	文件名		: RIMCONFIG.H
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/5 9:05
//	文件描述	: 
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

	tstring 	name;			//文件相对路径
	tstring 	hash;			//文件的MD5码值
	DWORD  		size;			//文件的大小尺寸
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

		///CMD指令
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
		TRANS_FLAG_PRETRANS		= 1,						//初始构造传输状态		预传输状态，需要做好初始工作
		TRANS_FLAG_TRANSBEGIN	= 2,						//准备发送或接收		初始工作后，开始传输需要经过用户允许
		TRANS_FLAG_TRANSING		= 3,						//发送或接收进行中		授权传输后，的传输状态
		TRANS_FLAG_TRANSCANCEL	= 4,						//取消传输				用户取消传输
		TRANS_FLAG_TRANSPREEND	= 5,						//传输预结束			传输结束状态，或经取消后的传输结束状态
		TRANS_FLAG_TRANSEND		= 6,						//传输结束				所有传输数据可以清空的标志
	};

protected:
	TRANSFLAG					transFlag;					//当前传输状态
	CCriticalSection			m_cs;

protected:
	string						updateCmdBase;
	string						beginCmd;
	string						cancelCmd;
	string						endCmd;
	string						nameCmdBase;

	tstring						guidStr;					//生成一个唯一性的GUID字符串
	BOOL						bDic;						//是否是文件夹
	tstring						applicationName;			//要更新的文件的名称	文件传输过程中用来存储拖放的文件或目录名
	DWORD64						totalSize;					//传输总大小
	tstring						version;					//要更新的文件的版本	保留无用

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


	tstring						transError;					//传输中所出现的问题描述
	LONGLONG					transBytesTemp;				//传送了多少字节
	LONGLONG					rateTemp;					//传输速度
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

//子系统信息
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
		//以下三者虽然进行拷贝，原则上不应当进行任何修改
		pImagesel				= ob.pImagesel;
		pImageUnsel				= ob.pImageUnsel;
		rect					= ob.rect;

		return *this;
	}

	BOOL						bShowInList;	//是否在列表中显示
	tstring						sysFuncNo;
	BOOL						bDefaultOpen;	//默认打开
	BOOL						bSupportOutLine;//是否支持脱机
	tstring						moudleName;		//子系统名称
	tstring						moudleEXEPath;	//子系统名称
	tstring						selPath;		//选中时的图片相对路径
	tstring						unselPath;		//未选中时的图片相对路径

	Image						*pImagesel;		//选中时的图片加载后的对象
	Image						*pImageUnsel;	//未选中时的图片加载后的对象
	CRect						rect;
} SUBMOUDLEINFO, *LPSUBMOUDLEINFO;


/////////////////////////////////////////////////////////////////【zy 2014-11-19 ↓】 测量相关
//测量信息
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

	BOOL						bShowInList;	//是否在列表中显示
	tstring						measueName;		//测量名称
	tstring						keyWord;		//测量名称
	int							measureID;		//测量ID

} MEASUREINFO, *LPMEASUREINFO;

//设备信息
typedef class tagDEVICEINFO
{
public:
	INFOTYPE					infoType;
	enum 
	{	
		DEVICE_STATE_0			= 0,					// 设备可用
		DEVICE_STATE_1			= 1,					// 设备忙
		DEVICE_STATE_2			= 2,					// 设备故障
		DEVICE_STATE_3			= 3,					// 设备未知
		DEVICE_STATE_4			= 4,					// 设备有任务运行
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
	BOOL						bShowInList;	//是否在列表中显示
	int							deviceState;	//设备状态  -1:不可用 0:不在线 1:在线
	vector<LPMEASUREINFO>		measureInfos;

} DEVICEINFO, *LPDEVICEINFO;

//站点信息
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
	BOOL						bShowInList;	//是否在列表中显示
	vector<LPDEVICEINFO>		devicesInfos;

} STATIONINFO, *LPSTATIONINFO;

////////////////////////////////////////////////////////////////////////////【zy 2014-11-19 ↑】测量相关

/////////////////////////////////////////////////////////////////【zy 2014-11-19 ↓】 用户相关

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

//用户信息
/*关于用户信息相关说明 
用户类型		ID userTypeID  只能匿名登录的用户才会赋以USER_ID_UNKNOWN   默认为0即为合法用户需要验证的
用户分支信息，	优先判断pBranchInfo不为空即为合法用户，如果pBranchInfo为空，branch如果不为匿名分支即用户为非分支下的用户
////匿名用户ID与名称相同
*/
typedef class tagUSERINFO
{
public:
	INFOTYPE					infoType;
	enum {	
		USER_ID_NORMAL			= 0,
		USER_ID_UNKNOWN			= 0xFFFFFFFF,
	};
	enum {	USER_STATE_OFFLINE	= 0,					// 用户在线状态
			USER_STATE_ONLINE	= 1,					// 用户下线状态
	};
	static const  tstring		UNKNOWNBRANCHSTR;		//用户默认的未知分支字符串
	static CReadWriteLock		IPListLock;				//对用户IP列表访问锁  结省内核对象

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

		userTypeID				= USER_ID_NORMAL;				//说明该用户默认为正常用户，只有匿名登录的用户才设成匿名ID
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
		{	//清空消息队列
			CQueueData *pData = NULL;
			receiveFileQueue.Pop(&pData);
			((LPTRANSFILEINFO)pData)->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL);
			while (((LPTRANSFILEINFO)pData)->GetTransFlag() != tagTRANSFILEINFO::TRANS_FLAG_TRANSEND) Sleep(1); //等待数组最后置空
			delete pData;
		}

		while (sendFileQueue.GetQueueCount())
		{	//清空消息队列
			CQueueData *pData = NULL;
			sendFileQueue.Pop(&pData);
			((LPTRANSFILEINFO)pData)->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL);
			while (((LPTRANSFILEINFO)pData)->GetTransFlag() != tagTRANSFILEINFO::TRANS_FLAG_TRANSEND) Sleep(1); //等待数组最后置空
			delete pData;
		}

		///////注意，接收和发送队列要先于线程结束前清空之 
		if (pChatThread)
		{
			pChatThread->PostThreadMessage(WM_QUIT, 0, 0);
			while (pChatThread) Sleep(1);		//等待线程 自动删除并置空
		}

		while(receiveMsgQueue.GetQueueCount())
		{	//清空消息队列
			CQueueData *pData = NULL;
			receiveMsgQueue.Pop(&pData);
			delete pData;
		}
	}
	LPBRANCHINFO				pBranchInfo;		//机构信息

	///////兼容数据库字段
	tstring						UserID;				//用户的唯一标识
	tstring						UserName;			//用户名称
	tstring 					EmpNO;	
	tstring 					AreaCode;	
	tstring 					DeptID;				//机构的GUID
	tstring 					DeptName;			//所属机构的名称
	

	tstring						onlinePngPath;		//在线图片
	tstring						offlinePngPath;		//离线图片

	DWORD						userTypeID;			//用户ID序号
	tstring						branch;				//最近的上级部门的名称
	int							state;				//用户状态
	tstring						userToken;			

	//不需由源数据获取

	set<tstring>				ipList;				//同一用户登录的不同电脑的IP列表;
	short						userUdpPortTemp;	//用户绑定的端口
	short						userTcpPortTemp;	//用户绑定的端口

	clock_t						stateUpdateClk;		//用户状态实时更新时间

	CWinThread					*pChatThread;		//该用户所在的聊天对话框线程
	CUserDataQueue				receiveMsgQueue;	//当前接收的消息队列
	bool						bShowInList;		//是否在列表中显示

	CUserDataQueue				receiveFileQueue;	//当前接收的文件队列
	CUserDataQueue				sendFileQueue;		//当前发送的文件队列	////也用于在文件列表中显示

} USERINFO, *LPUSERINFO;

////////////////////////////////////////////////////////////////////////////【zy 2014-11-19 ↑】用户相关

////【zy 2014-12-23 ↓】
////系统设置
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

	tstring						lastUserID;			//上次登录的名称
	tstring						lastPass;			//上次登录的密码
	tstring						lastDmsIP;			//上次登录的服务端的IP	

	tstring						lastRmsIP;			//监测服务的IP	
	tstring						lastMsaIP;			//统计服务的IP	

	BOOL						bRememberPass;		//是否记住密码
	BOOL						bAutoLogon;			//是否自动登录
	BOOL						bOffLineLogon;		//是否便携登录

} SYSTEMSET, *LPSYSTEMSET;

////用户设置
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
////【zy 2014-12-23 ↑】

////表更新结构
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
	RIMConfig(void);						//禁止创建副本
	static RIMConfig*						m_pRIMConfig;	//内部实例指针
	HMODULE									m_hToolkitLibrary;
	CGsoapServer							m_gsoapServer;
	CStringArray							m_macArray;		//网卡MAC集合

	//************************************
	// Method:    ParseConfig
	// FullName:  RIMConfig::ParseConfig
	// Access:    protected 
	// Returns:   BOOL						成功返回TRUE,失败返回FALSE
	// Qualifier:							解析RIM全局配置文件 
	// Parameter: tstring path				配置文件路径
	//************************************
	BOOL									ParseConfig(tstring path);
	void									ParseSqlTableNodes(TiXmlNode* moudleNodes);
	//************************************
	// Method:    ParseModuleNodes
	// FullName:  RIMConfig::ParseModuleNodes
	// Access:    protected 
	// Returns:   void
	// Qualifier:							解析子系统模块结点下的内容信息
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

	vector<RTMSTATION>						m_rtmStationVec;		//站点表
	vector<RTMDEVICE>						m_rtmDeviceVec;			//设备表
	vector<RTMDEVICEFUNC>					m_rtmDeviceFuncVec;		//设备功能表
	vector<RTMREFDEVICEFUNC>				m_rtmRefDeviceFuncVec;	//所有设备功能表
	vector<COMREGIONNO>						m_comRegionNoVec;		//地区编码表
	vector<RTMREFSTATIONTYPE>				m_stationTypeVec;		//站点类型
	vector<RTMREFDEVICETYPE>				m_deviceTypeVec;		//设备类型
	vector<RTMREFDEVICEPROTOCOL>			m_deviceProtocolVec;	//设备协议类型
	vector<RIM30COMAUTHSYSFUNC>				m_authSysFunc;			//系统功能授权表

public:
	static LPUSERINFO						m_pLogonUser;			//登录导航器用户
	static RIMConfig* 						GetInstance();
	static void 							ReleaseInstance();

	//************************************
	// Method:    ReInitStationInfo
	// FullName:  RIMConfig::ReInitStationInfo
	// Access:    public 
	// Returns:   void
	// Qualifier:							重新初始化站点信息
	//************************************
	//void									ReInitStationInfo();
	//************************************
	// Method:    DoLogon
	// FullName:  RIMConfig::DoLogon
	// Access:    public 
	// Returns:   void
	// Qualifier:							用户登录，并配置相关配置数据
	// Parameter: BOOL bAreadyLogon			是否登录，TRUE 登录 FALSE 登出
	// Parameter: BOOL bOffLine				是否是便携登录，TRUE 便携， FALSE 非便携
	// Parameter: tstring & errorInfo		错误信息提示
	// Parameter: HWND processHwnd			包含进度条的窗口，用于显示进度通知
	//************************************
	void									DoLogon(BOOL bLogon, BOOL bOffLine, tstring& errorInfo, HWND processHwnd);

	vector<LPSUBMOUDLEINFO>					m_moudleInfoArray;		//子模块信息数组
	vector<LPSTATIONINFO>					m_stationArray;			//站点信息数组 包括设备及设备所包含的测量功能信息
	vector<LPUSERINFO>						m_userArray;			//用户信息数组
	vector<LPBRANCHINFO>					m_branchArray;			//用户所在的机构数组
	map<string, map<string, vector<long>>>	m_recentMap;			//最近使用信息数组 分别对应 站点编号，设备编号，功能编号


	vector<LPTABLEUPDATE>					m_tableUpdateArray;		//sqlite每个表的更新列表
	map<DWORD, vector<LPWEBADDRINFO>>		m_webAddrInfoMap;		//各个服务地址信息数据

	//系统配置及个人配置
	SYSTEMSET								m_systemSet;
	USERSET									m_userSet;

	///UDP处理接收与发送消息的队列
	CUserDataQueue							m_UDPReceiveQueue;		//消息接收队列
	CUserDataQueue							m_UDPSendQueue;			//消息发送队列
	///TCP处理接收与发送消息的队列
	CUserDataQueue							m_TCPReceiveQueue;		//消息接收队列
	CUserDataQueue							m_TCPSendQueue;			//消息发送队列

	tstring									m_modifyPassTheme;		//修改密码界面的着色
	tstring									m_welcomeText;			//欢迎文本
	long									m_welcomeFontSize;		//欢迎文本字体的大小
};
