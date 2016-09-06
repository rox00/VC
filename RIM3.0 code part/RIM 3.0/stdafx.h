
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#include <afxsock.h>     // 功能区和控件条的 MFC 支持

#include <afxrich.h>            // MFC Rich Edit 类
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

//声音
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


#include "聊天\ThreadUDPMonitor.h"
#include "聊天\ThreadTCPMonitor.h"
#include "聊天\ThreadDataProcess.h"
#include "聊天\NetData.h"
#include "聊天\ImageDataObject.h"
#include "聊天\TapBitmap.h"
#include "聊天\EnBitmap.h"
#include "聊天\SendFileListTree.h"
#include "聊天\ChatRichView.h"
#include "聊天\ChatDoc.h"


#ifdef _DEBUG
#pragma comment(lib,"stdsoap2D.lib")
#else
#pragma comment(lib,"stdsoap2.lib")
#endif
