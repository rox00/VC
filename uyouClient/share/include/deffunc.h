#pragma once

#define SAFE_COPY_A( x, nmax, y )		{\
											(x)[min((nmax)-1, strlen((y)))] = 0;\
											memcpy( (x), (y), min((nmax)-1, strlen((y)))*sizeof(char) );\
										}

#define SAFE_CAT_A( x, nmax, y )		{\
											(x)[min((nmax)-1, strlen((x))+strlen((y)))] = 0;\
											memcpy( &(x)[strlen((x))], (y), min((nmax)-strlen((x))-1, strlen((y)))*sizeof(char) );\
										}

#define SAFE_COPY_W( x, nmax, y )		{\
											(x)[min((nmax)-1, wcslen((y)))] = 0;\
											memcpy( (x), (y), min((nmax)-1, wcslen((y)))*sizeof(WCHAR) );\
										}

#define SAFE_CAT_W( x, nmax, y )		{\
											(x)[min((nmax)-1, wcslen((x))+wcslen((y)))] = 0;\
											memcpy( &(x)[wcslen((x))], (y), min((nmax)-wcslen((x))-1, wcslen((y)))*sizeof(WCHAR) );\
										}

#define SAFE_RELEASE( x )				{\
											if( (x) != NULL )\
											{\
												free((x));\
												(x) = NULL;\
											}\
										}

#define SAFE_CLOSEHANDLE( x )			{\
											if( (x) != NULL )\
											{\
												CloseHandle( (x) );\
												(x) = NULL;\
											}\
										}

#define SAFE_DELETEOBJECT( x )			{\
	if( (x) != NULL )\
											{\
											::DeleteObject( (x) );\
											(x) = NULL;\
											}\
										}


#define SAFE_DELETEDC( x )				{\
	if( (x) != NULL )\
											{\
											::DeleteDC( (x) );\
											(x) = NULL;\
											}\
										}


#define CALLOC_COPY_CHAR_A( x, y )		{\
											SAFE_RELEASE( (x) );\
											if ( (y) != NULL )\
											{\
												(x) = (char*)calloc( strlen((y))+1, sizeof(char) );\
												memcpy( (x), (y), strlen(y)*sizeof(char) );\
											}\
										}

#define CALLOC_COPY_CHAR_W( x, y )		{\
											SAFE_RELEASE( (x) );\
											if ( (y) != NULL )\
											{\
												(x) = (WCHAR*)calloc( wcslen((y))+1, sizeof(WCHAR) );\
												memcpy( (x), (y), wcslen(y)*sizeof(WCHAR) );\
											}\
										}


#define WAIT_TERMINATE_THREAD( x, t )	{\
											if( (x) )\
											{\
												if ( WaitForSingleObject( (x), (t) ) != WAIT_OBJECT_0 )\
												{\
													TerminateThread( (x),(DWORD)0xFFFFFFFF );\
													CloseHandle( (x) );\
												}\
												(x) = NULL;\
											}\
										}

#if defined(_UNICODE)
#define SAFE_COPY			SAFE_COPY_W
#define SAFE_CAT			SAFE_CAT_W
#define CALLOC_COPY_CHAR	CALLOC_COPY_CHAR_W
#else
#define SAFE_COPY			SAFE_COPY_A
#define SAFE_CAT			SAFE_CAT_A
#define CALLOC_COPY_CHAR	CALLOC_COPY_CHAR_A
#endif

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

