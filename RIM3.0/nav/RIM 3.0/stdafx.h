
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#include <afxsock.h>     // �������Ϳؼ����� MFC ֧��

#include <afxrich.h>            // MFC Rich Edit ��
#include <afxhtml.h>
#include <afxinet.h>




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <tchar.h>

#include <Winnetwk.h>
#pragma comment(lib, "mpr.lib ")

#include <stdlib.h> 
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//����
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

#include "../Include/zip.h"
#include "../Include/unzip.h"
//#include "../Include/Lz77.h"
#include "../Include/CommonFunction.h"
#include "../Include/UserThread.h"
#include "../Include/UserDataQueue.h"

#include "../Include/Controls/PngButton.h"
#include "../Include/Controls/PngCheckBox.h"
#include "../Include/Controls/PNGGrayTipEdit.h"
#include "../Include/Controls/PngStatic.h"
#include "../Include/Controls/PngCheckButton.h"
#include "../Include/Controls/PngRadioButton.h"
#include "../Include/Controls/PngTreeCtrl.h"
#include "../Include/Controls/PngListBox.h"
#include "../Include/Controls/PngListCtrl.h"
#include "..\Include\Controls\PngScrollBar.h"

#define TIXML_USE_STL
#include "../Include/tinyxml/tinyxml.h"


#include "../../sdk/RmcpProtocol.h"
#include "../../sdk/RIM30MSG/IRIM30MsgServer.h"
#include "../../sdk/RIM30ToolKit/RIM30ToolKit.h "
//#include "../../sdk/RIM30ToolKit/RIM30ToolKitInterface.h "


//__declspec(dllimport) int ToolKitTest();
//#ifdef _DEBUG
//#pragma comment(lib,"RIM30ToolKitD.lib")
//#else
//#pragma comment(lib,"RIM30ToolKit.lib")
//#endif
//#include "PngPath_Define.h"

#include "gsoap\GsoapServer.h"


#include "ImageConfig.h"
#include "RIMConfig.h"
#include "RIMTreeCtrl.h"


#include "����\ThreadUDPMonitor.h"
#include "����\ThreadTCPMonitor.h"
#include "����\ThreadDataProcess.h"
#include "����\NetData.h"
#include "����\ImageDataObject.h"
#include "����\TapBitmap.h"
#include "����\EnBitmap.h"
#include "����\SendFileListTree.h"
#include "����\ChatRichView.h"
#include "����\ChatDoc.h"


#ifdef _DEBUG
#pragma comment(lib,"stdsoap2D.lib")
#else
#pragma comment(lib,"stdsoap2.lib")
#endif
