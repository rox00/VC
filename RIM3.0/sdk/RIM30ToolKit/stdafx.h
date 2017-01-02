// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
#include <tchar.h>
#include <stdlib.h>
#include <io.h>
#include <string>
#include "SQLite3/sqlite3.h"

#ifdef _DEBUG
#pragma comment(lib,"sqlite3D.lib")
#else
#pragma comment(lib,"sqlite3.lib")
#endif

#include "CommonFuntion.h"
extern string RIM_RTK_BSD_DB_FILE;