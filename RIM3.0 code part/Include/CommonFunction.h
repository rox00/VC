#pragma once


extern COLORREF										g_transAlphaClr;				//Layer������͸������ɫ
extern HBRUSH										g_hTransAlphaBrush;				//����ˢ͸��ɫ�Ļ�ˢ


#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#define 	ConnToStr(x,y)							x##y
#define 	ToChar(x)								#@x
#define		ToString(x)								#x


////��zy 2014-12-23 ����///�Զ�����Ϣ
#define		WM_SHOWDOCKBOUND						WM_USER + 1							//ͣ����Ϣ		
#define		WM_LOGON								WM_USER + 2							//��¼��Ϣ			wParam = 1 ��½�ɹ�		
#define		WM_SHOWMAINWND							WM_USER + 3							//���غ���ʾ��¼���ֽ���	
#define		WM_UPDATECONFIG							WM_USER + 4							//������������
#define		WM_UPDATECONFIGFLAG						WM_USER + 5							//���±�־�Ŀ���״̬wParam  1 ״̬������Ҫ�������� 0 ״̬�����ò���Ҫ��������
#define		WM_PRESEND								WM_USER + 6							//����������Ϣǰ֪ͨ���� 0��������Ϣ�� 1���ļ���2���ļ���, 3��ͼƬ
#define		WM_UPDATEUSERLIST						WM_USER + 8							//�����û��б�
#define		WM_UPDATESTATIONSTATUS					WM_USER + 9							//����վ��״̬
#define		WM_FLICKERNOTIFYICON					WM_USER + 10						//��˸����ͼ��		wParam 0 ����˸ͼ��   1 ִ����˸ͼ�� 2 ��Ϣ֪ͨ��3�ļ���Ϣ֪ͨ, lParam  �û���Ϣָ��
#define		WM_REPOSITIONSUBDLG						WM_USER + 11						//���·����ӶԻ����λ��
#define		WM_PROGRESS								WM_USER + 12						//������Ϣ			wParam �������ֵ������ֵ��ҵ�����

////��zy 2014-12-23 ����//����ͼ���漰��Ϣ
#define 	WM_ICON									WM_USER + 100						//������Ϣ 
#define 	WM_ICONSHOWMINWND  						WM_USER + 101						//���̲˵���ʾ��������Ϣ
#define 	WM_ICONSET 								WM_USER + 102						//
#define 	WM_ICONOPEN  							WM_USER + 103
#define 	WM_ICONEXITUSER 						WM_USER + 104
#define 	WM_ICONEXIT  							WM_USER + 105
////��zy 2014-12-23 ����

////��zy 2015-05-28 ����//��������������
#define 	WM_INDEXBASE  							WM_USER + 1000
////��zy 2015-05-28 ����

////��zy 2014-12-23 ����//����ͼ����ʾ�˵�ID
#define 	ICONSHOWMINWND_STR						_T("��С��")
#define 	ICONSET_STR								_T("����")
#define 	ICONOPEN_STR							_T("��ʾ������")
#define 	ICONEXITUSER_STR						_T("�л��û�")
#define 	ICONEXIT_STR							_T("�˳�")
////��zy 2014-12-23 ����

////��zy 2014-12-23 ����//�������ļ����·��
#define 	CFG_RELATIVEPATH_RIMCONFIG				_T(".\\Config\\RIMConfig.xml")		////RIM�����ļ�
//#define 	CFG_RELATIVEPATH_SYSTEMSET				_T(".\\Config\\SystemSet.xml")		////ϵͳ�����ļ�
#define 	VEDIO_RELATIVEPATH_MSG					_T(".\\res\\sound\\Message.wav")	////��Ϣ��ʾ����
//#define 	CFG_PATH_RIMCONFIG			_T(".\\Config\\RIMConfig.xml")
////��zy 2014-12-23 ����

#define		UDP_CHATSERVER_PORT						6969								////UDP�����߼����˿�
#define		TCP_CHATSERVER_PORT						7070								////TCP�����ļ�����˿�
#define		TCP_MSGSERVER_PORT						8181								////��ϵͳ��ͨ�Ŷ˿�

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
// Qualifier:									���ڴ�DC�������ķ�ʽ���ƿɵ���͸���ȵ�ͼƬ
// Parameter: const CDC & memDC					�ڴ�DC 
// Parameter: const CRect & rcClient			Ŀ������
// Parameter: Image * pImage					ͼ�����ָ��
// Parameter: float Alpha0_1					ͼ���͸���� 0~1.0 1.0ָ��͸��
//************************************
void											MemPaintPng(const CDC &memDC, const CRect &rcClient, Image* pImage, float Alpha0_1 = 1.0);
//************************************
// Method:    MemDrawText
// FullName:  MemDrawText
// Access:    public 
// Returns:   void
// Qualifier:									���ڴ�DC�ϻ��������ַ���
// Parameter: const CDC & memDC					�ڴ�DC���� 
// Parameter: const CRect & rcClient			Ŀ������
// Parameter: const CString & textStr   		Ҫ���Ƶ��ַ���
// Parameter: const DWORD & mode				�����ַ����Ķ��뷽ʽ ��DrawText�Ķ��뷽ʽ��ͬ
//************************************
void											MemDrawText(const CDC &memDC, const CRect &rcClient, const CString &textStr, const DWORD &mode);
//************************************
// Method:    AlphaImageToRgn
// FullName:  AlphaImageToRgn
// Access:    public 
// Returns:   HRGN								���ػ�õ�����
// Qualifier:									��ͼƬһ����Χ�ڵ����ص����͸�����ų���ʣ����������ɵ�����
// Parameter: Image * pImage					ͼƬ����ָ��
// Parameter: BYTE offAlphaRange				�ų�͸���ȷ�Χ ��0��ʼ
//************************************
HRGN											AlphaImageToRgn(Image* pImage, BYTE offAlphaRange = 0);
//************************************
// Method:    IsDicExist
// FullName:  IsDicExist
// Access:    public 
// Returns:   BOOL
// Qualifier:									�ж�Ŀ¼�ļ����Ƿ����
// Parameter: LPCTSTR lpPath
//************************************
BOOL 											IsDicExist(LPCTSTR lpPath);
//************************************
// Method:    IsFileExist
// FullName:  IsFileExist
// Access:    public 
// Returns:   BOOL
// Qualifier:									�ж��ļ��Ƿ����
// Parameter: LPCTSTR lpFile
//************************************
BOOL 											IsFileExist(LPCTSTR lpFile);
//************************************
// Method:    GetMd5
// FullName:  GetMd5
// Access:    public 
// Returns:   BOOL
// Qualifier:									��ȡ�ļ���MD5ֵ
// Parameter: tstring FileDirectory				�ļ�·��
// Parameter: tstring & strFileMd5				���ص�MD5��
//************************************
BOOL											GetMd5(tstring FileDirectory, tstring &strFileMd5);
//************************************
// Method:    ReadRegKeyStr
// FullName:  ReadRegKeyStr
// Access:    public 
// Returns:   tstring
// Qualifier:									��ȡע���������Ӧ���ַ���ֵ
// Parameter: HKEY hKey							ע�����ľ��
// Parameter: tstring subKey					�Ӽ�������
// Parameter: tstring valueName					�Ӽ����������
//************************************
tstring											ReadRegKeyStr(HKEY hKey, tstring subKey, tstring valueName);
//************************************
// Method:    ReadRegKeyDWord
// FullName:  ReadRegKeyDWord
// Access:    public 
// Returns:   LONG								���ش�ע����������  ERROR_SUCCESS �ɹ�
// Qualifier:									��ȡע�����йؼ��ֶ�Ӧ��DWORDֵ
// Parameter: HKEY hKey							ע�����ľ��
// Parameter: tstring subKey					�Ӽ�������
// Parameter: tstring valueName					�Ӽ��ж�Ӧ�������
// Parameter: DWORD & regValue					��Ҫ�������DWORDֵ
//************************************
LONG											ReadRegKeyDWord(HKEY hKey, tstring subKey, tstring valueName, DWORD& regValue);
//************************************
// Method:    WriteRegKeyStr
// FullName:  WriteRegKeyStr
// Access:    public 
// Returns:   void
// Qualifier:									��ע������д���Ӧ����ֵ��û�иü����Զ�����
// Parameter: HKEY hKey							ע�����ľ��
// Parameter: tstring subKey					�Ӽ�������
// Parameter: tstring valueName					�Ӽ��ж�Ӧ�������
// Parameter: LPCTSTR lpStr						��Ҫ���õ��ַ���ֵ
// Parameter: int strLen						��Ҫ�����ַ���ֵ�ĳ���
//************************************
void											WriteRegKeyStr(HKEY hKey, tstring subKey, tstring valueName, tstring lpStr);
//************************************
// Method:    WriteRegKeyDWord
// FullName:  WriteRegKeyDWord
// Access:    public 
// Returns:   void
// Qualifier:									��ע������д���Ӧ����ֵ��û�иü����Զ�����
// Parameter: HKEY hKey							ע�����ľ��
// Parameter: tstring subKey					�Ӽ�������
// Parameter: tstring valueName					�Ӽ��ж�Ӧ�������
// Parameter: DWORD dwRegValue					��Ҫ���õ�DWORDֵ
//************************************	
void											WriteRegKeyDWord(HKEY hKey, tstring subKey, tstring valueName, DWORD dwRegValue);
//************************************
// Method:    SetAppAutoRun
// FullName:  SetAppAutoRun
// Access:    public 
// Returns:   void
// Qualifier:									д��ע����ֵ������EXEӦ�ó����Զ�����
// Parameter: BOOL bAutoRun						1 ����Ϊ�Զ����� 0 ����Ϊ���Զ�����
//************************************
void											SetAppAutoRun( BOOL bAutoRun );
//************************************
// Method:    IsAppAutoRun
// FullName:  IsAppAutoRun
// Access:    public 
// Returns:   BOOL								1 �����Զ�����  2 �������Զ�����
// Qualifier:									�ж�Ӧ�ó����Ƿ񿪻��Զ�����
//************************************
BOOL											IsAppAutoRun();
//************************************
// Method:    RelativeToFullPath
// FullName:  RelativeToFullPath
// Access:    public 
// Returns:   tstring
// Qualifier:									���·��ת���ɾ���·��
// Parameter: const tstring & strRelativePath	���·���ַ���
//************************************
tstring											RelativeToFullPath(const tstring &strRelativePath);
//************************************
// Method:    SetRoundRgnWnd
// FullName:  SetRoundRgnWnd
// Access:    public 
// Returns:   void
// Qualifier:									���ô�������ΪԲ�Ǿ���
// Parameter: HWND hWnd							���ھ��
// Parameter: int rX							Բ����Բ�Ŀ��
// Parameter: int rY							Բ����Բ�ĸ߶�
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
 �������ܣ���ָ���ļ���ָ����Ŀ¼�´������ݷ�ʽ
 ����������
 lpszFileName    ָ���ļ���ΪNULL��ʾ��ǰ���̵�EXE�ļ���
 lpszLnkFileDir  ָ��Ŀ¼������ΪNULL��
 lpszLnkFileName ��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����
 wHotkey         Ϊ0��ʾ�����ÿ�ݼ�
 pszDescription  ��ע
 iShowCmd        ���з�ʽ��Ĭ��Ϊ���洰��
 */

//************************************
// Method:    CreateFileShortcut
// FullName:  CreateFileShortcut
// Access:    public 
// Returns:   BOOL
// Qualifier:									��ָ���ļ����ļ�����ָ����Ŀ¼�´������ݷ�ʽ
// Parameter: LPCSTR lpszFileOrDic				ָ���ļ����ļ��У�ΪNULL��ʾ��ǰ���̵�EXE�ļ���
// Parameter: LPCSTR lpszLnkFileDir				ָ��Ŀ¼������ΪNULL��
// Parameter: LPCSTR lpszLnkFileName			��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����
// Parameter: WORD wHotkey						Ϊ0��ʾ�����ÿ�ݼ�
// Parameter: LPCTSTR lpszDescription			��ע		
// Parameter: int iShowCmd						���з�ʽ��Ĭ��Ϊ���洰��
//************************************
BOOL											CreateFileShortcut(LPCTSTR lpszFileOrDic, LPCTSTR lpszLnkFileDir, LPCTSTR lpszLnkFileName, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL);
wstring											ANSI2Unicode(const string & strin);
string 											Unicode2UTF8(const wstring & strin);
string 											ANSI2UTF8(const string & strin);
string 											Unicode2ANSI(const wstring & strin);
wstring											UTF82Unicode(const string & strin);
string											UTF82ANSI(const string & strin);
bool											IsValidIP(tstring strIP);
