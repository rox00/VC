#pragma once

#include <string>

#ifdef UNICODE
typedef std::wstring String;
#else
typedef std::string String;
#endif	
