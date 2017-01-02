#pragma once


extern COLORREF										g_transAlphaClr;				//Layer窗口做透明的颜色
extern HBRUSH										g_hTransAlphaBrush;				//用来刷透明色的画刷


#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#define 	ConnToStr(x,y)							x##y
#define 	ToChar(x)								#@x
#define		ToString(x)								#x


////【zy 2014-12-23 ↓】///自定义消息
#define		WM_SHOWDOCKBOUND						WM_USER + 1							//停靠消息		
#define		WM_LOGON								WM_USER + 2							//登录消息			wParam = 1 登陆成功		
#define		WM_SHOWMAINWND							WM_USER + 3							//隐藏和显示登录部分界面	
#define		WM_UPDATECONFIG							WM_USER + 4							//立即更新配置
#define		WM_UPDATECONFIGFLAG						WM_USER + 5							//更新标志的可用状态wParam  1 状态可用需要更新配置 0 状态不可用不需要更新配置
#define		WM_PRESEND								WM_USER + 6							//发送聊天信息前通知窗口 0是聊天消息， 1是文件，2是文件夹, 3是图片
#define		WM_UPDATEUSERLIST						WM_USER + 8							//更新用户列表
#define		WM_UPDATESTATIONSTATUS					WM_USER + 9							//更新站点状态
#define		WM_FLICKERNOTIFYICON					WM_USER + 10						//闪烁托盘图标		wParam 0 不闪烁图标   1 执行闪烁图标 2 消息通知，3文件消息通知, lParam  用户信息指针
#define		WM_REPOSITIONSUBDLG						WM_USER + 11						//重新放置子对话框的位置
#define		WM_PROGRESS								WM_USER + 12						//进度消息			wParam 代表进度值，具体值跟业务相关

////【zy 2014-12-23 ↓】//托盘图标涉及消息
#define 	WM_ICON									WM_USER + 100						//托盘消息 
#define 	WM_ICONSHOWMINWND  						WM_USER + 101						//托盘菜单显示主窗口消息
#define 	WM_ICONSET 								WM_USER + 102						//
#define 	WM_ICONOPEN  							WM_USER + 103
#define 	WM_ICONEXITUSER 						WM_USER + 104
#define 	WM_ICONEXIT  							WM_USER + 105
////【zy 2014-12-23 ↑】

////【zy 2015-05-28 ↓】//用来做索引基数
#define 	WM_INDEXBASE  							WM_USER + 1000
////【zy 2015-05-28 ↑】

////【zy 2014-12-23 ↓】//托盘图标显示菜单ID
#define 	ICONSHOWMINWND_STR						_T("最小化")
#define 	ICONSET_STR								_T("设置")
#define 	ICONOPEN_STR							_T("显示主界面")
#define 	ICONEXITUSER_STR						_T("切换用户")
#define 	ICONEXIT_STR							_T("退出")
////【zy 2014-12-23 ↑】

////【zy 2014-12-23 ↓】//各配置文件相对路径
#define 	CFG_RELATIVEPATH_RIMCONFIG				_T(".\\Config\\RIMConfig.xml")		////RIM配置文件
//#define 	CFG_RELATIVEPATH_SYSTEMSET				_T(".\\Config\\SystemSet.xml")		////系统配置文件
#define 	VEDIO_RELATIVEPATH_MSG					_T(".\\res\\sound\\Message.wav")	////消息提示声明
//#define 	CFG_PATH_RIMCONFIG			_T(".\\Config\\RIMConfig.xml")
////【zy 2014-12-23 ↑】

#define		UDP_CHATSERVER_PORT						6969								////UDP上下线监听端口
#define		TCP_CHATSERVER_PORT						7070								////TCP聊天文件传输端口
#define		TCP_MSGSERVER_PORT						8181								////子系统间通信端口

#define		PACKVERSION(major,minor)				MAKELONG(minor,major)

typedef struct _lldiv_tRIM {
	long long quot;
	long long rem;
} lldiv_tRIM;

//************************************
// Method:    MemPaintPng
// FullName:  MemPaintPng
// Access:    public 
// Returns:   void
// Qualifier:									向内存DC上以填充的方式绘制可调节透明度的图片
// Parameter: const CDC & memDC					内存DC 
// Parameter: const CRect & rcClient			目的区域
// Parameter: Image * pImage					图像对象指针
// Parameter: float Alpha0_1					图像的透明度 0~1.0 1.0指不透明
//************************************
void											MemPaintPng(const CDC &memDC, const CRect &rcClient, Image* pImage, float Alpha0_1 = 1.0);
//************************************
// Method:    MemDrawText
// FullName:  MemDrawText
// Access:    public 
// Returns:   void
// Qualifier:									向内存DC上绘制文字字符串
// Parameter: const CDC & memDC					内存DC对象 
// Parameter: const CRect & rcClient			目的区域
// Parameter: const CString & textStr   		要绘制的字符串
// Parameter: const DWORD & mode				绘制字符串的对齐方式 与DrawText的对齐方式相同
//************************************
void											MemDrawText(const CDC &memDC, const CRect &rcClient, const CString &textStr, const DWORD &mode);
//************************************
// Method:    AlphaImageToRgn
// FullName:  AlphaImageToRgn
// Access:    public 
// Returns:   HRGN								返回获得的区域。
// Qualifier:									将图片一定范围内的像素点根据透明度排除后，剩余像素所组成的区域
// Parameter: Image * pImage					图片对象指针
// Parameter: BYTE offAlphaRange				排除透明度范围 从0开始
//************************************
HRGN											AlphaImageToRgn(Image* pImage, BYTE offAlphaRange = 0);
//************************************
// Method:    IsDicExist
// FullName:  IsDicExist
// Access:    public 
// Returns:   BOOL
// Qualifier:									判断目录文件夹是否存在
// Parameter: LPCTSTR lpPath
//************************************
BOOL 											IsDicExist(LPCTSTR lpPath);
//************************************
// Method:    IsFileExist
// FullName:  IsFileExist
// Access:    public 
// Returns:   BOOL
// Qualifier:									判断文件是否存在
// Parameter: LPCTSTR lpFile
//************************************
BOOL 											IsFileExist(LPCTSTR lpFile);
//************************************
// Method:    GetMd5
// FullName:  GetMd5
// Access:    public 
// Returns:   BOOL
// Qualifier:									获取文件的MD5值
// Parameter: tstring FileDirectory				文件路径
// Parameter: tstring & strFileMd5				返回的MD5码
//************************************
BOOL											GetMd5(tstring FileDirectory, tstring &strFileMd5);
//************************************
// Method:    ReadRegKeyStr
// FullName:  ReadRegKeyStr
// Access:    public 
// Returns:   tstring
// Qualifier:									读取注册表键中项对应的字符串值
// Parameter: HKEY hKey							注册表项的句柄
// Parameter: tstring subKey					子键的名称
// Parameter: tstring valueName					子键中项的名称
//************************************
tstring											ReadRegKeyStr(HKEY hKey, tstring subKey, tstring valueName);
//************************************
// Method:    ReadRegKeyDWord
// FullName:  ReadRegKeyDWord
// Access:    public 
// Returns:   LONG								返回错注册表访问误码  ERROR_SUCCESS 成功
// Qualifier:									读取注册表键中关键字对应的DWORD值
// Parameter: HKEY hKey							注册表键的句柄
// Parameter: tstring subKey					子键的名称
// Parameter: tstring valueName					子键中对应项的名称
// Parameter: DWORD & regValue					所要设置项的DWORD值
//************************************
LONG											ReadRegKeyDWord(HKEY hKey, tstring subKey, tstring valueName, DWORD& regValue);
//************************************
// Method:    WriteRegKeyStr
// FullName:  WriteRegKeyStr
// Access:    public 
// Returns:   void
// Qualifier:									向注册表键中写入对应的项值，没有该键则自动创建
// Parameter: HKEY hKey							注册表键的句柄
// Parameter: tstring subKey					子键的名称
// Parameter: tstring valueName					子键中对应项的名称
// Parameter: LPCTSTR lpStr						所要设置的字符串值
// Parameter: int strLen						所要设置字符串值的长度
//************************************
void											WriteRegKeyStr(HKEY hKey, tstring subKey, tstring valueName, tstring lpStr);
//************************************
// Method:    WriteRegKeyDWord
// FullName:  WriteRegKeyDWord
// Access:    public 
// Returns:   void
// Qualifier:									向注册表键中写入对应的项值，没有该键则自动创建
// Parameter: HKEY hKey							注册表键的句柄
// Parameter: tstring subKey					子键的名称
// Parameter: tstring valueName					子键中对应项的名称
// Parameter: DWORD dwRegValue					所要设置的DWORD值
//************************************	
void											WriteRegKeyDWord(HKEY hKey, tstring subKey, tstring valueName, DWORD dwRegValue);
//************************************
// Method:    SetAppAutoRun
// FullName:  SetAppAutoRun
// Access:    public 
// Returns:   void
// Qualifier:									写入注册表键值，设置EXE应用程序自动启动
// Parameter: BOOL bAutoRun						1 设置为自动启动 0 设置为非自动启动
//************************************
void											SetAppAutoRun( BOOL bAutoRun );
//************************************
// Method:    IsAppAutoRun
// FullName:  IsAppAutoRun
// Access:    public 
// Returns:   BOOL								1 开机自动运行  2 开机不自动运行
// Qualifier:									判断应用程序是否开机自动运行
//************************************
BOOL											IsAppAutoRun();
//************************************
// Method:    RelativeToFullPath
// FullName:  RelativeToFullPath
// Access:    public 
// Returns:   tstring
// Qualifier:									相对路径转换成绝对路径
// Parameter: const tstring & strRelativePath	相对路径字符串
//************************************
tstring											RelativeToFullPath(const tstring &strRelativePath);
//************************************
// Method:    SetRoundRgnWnd
// FullName:  SetRoundRgnWnd
// Access:    public 
// Returns:   void
// Qualifier:									设置窗口区域为圆角矩形
// Parameter: HWND hWnd							窗口句柄
// Parameter: int rX							圆角椭圆的宽度
// Parameter: int rY							圆角椭圆的高度
//************************************
void											SetRoundRgnWnd(HWND hWnd, int rX, int rY);
bool											GetAvalibleIpAddress(vector<tstring> &ipArray);
bool											GetAvalibleBroadcastAddress(vector<tstring> &broadCastIpArray);
int												SelectSocketRead(SOCKET sock, __in DWORD dwMilliseconds);
tstring											GetNewGuid();
void											FindAllFile(tstring dirc, vector<tstring> &pathVec);
BOOL 											IsRunAsAdmin();
BOOL 											ChangeWndMessageFilterOk(UINT nMessage, BOOL bAllow);
DWORD64											GetFolderSize(LPCTSTR szPath, DWORD *dwFiles = NULL, DWORD *dwFolders = NULL);
lldiv_tRIM __cdecl								lldiv_RIM(long long numer, long long denom);
long											FindStrNoCase(const tstring& strTxt, long nBegin, const tstring& strFnd);
DWORD											GetVersion(LPCTSTR lpszDllName);


/*
 函数功能：对指定文件在指定的目录下创建其快捷方式
 函数参数：
 lpszFileName    指定文件，为NULL表示当前进程的EXE文件。
 lpszLnkFileDir  指定目录，不能为NULL。
 lpszLnkFileName 快捷方式名称，为NULL表示EXE文件名。
 wHotkey         为0表示不设置快捷键
 pszDescription  备注
 iShowCmd        运行方式，默认为常规窗口
 */

//************************************
// Method:    CreateFileShortcut
// FullName:  CreateFileShortcut
// Access:    public 
// Returns:   BOOL
// Qualifier:									对指定文件或文件夹在指定的目录下创建其快捷方式
// Parameter: LPCSTR lpszFileOrDic				指定文件或文件夹，为NULL表示当前进程的EXE文件。
// Parameter: LPCSTR lpszLnkFileDir				指定目录，不能为NULL。
// Parameter: LPCSTR lpszLnkFileName			快捷方式名称，为NULL表示EXE文件名。
// Parameter: WORD wHotkey						为0表示不设置快捷键
// Parameter: LPCTSTR lpszDescription			备注		
// Parameter: int iShowCmd						运行方式，默认为常规窗口
//************************************
BOOL											CreateFileShortcut(LPCTSTR lpszFileOrDic, LPCTSTR lpszLnkFileDir, LPCTSTR lpszLnkFileName, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL);
wstring											ANSI2Unicode(const string & strin);
string 											Unicode2UTF8(const wstring & strin);
string 											ANSI2UTF8(const string & strin);
string 											Unicode2ANSI(const wstring & strin);
wstring											UTF82Unicode(const string & strin);
string											UTF82ANSI(const string & strin);
bool											IsValidIP(tstring strIP);
