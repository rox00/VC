#pragma once

#include <wtypes.h>

#ifndef SOCKET
typedef UINT_PTR SOCKET;
#endif

#ifdef _UNICODE
#define ConnectToServer			ConnectToServerW
#else
#define ConnectToServer			ConnectToServerA
#endif


class CTSocket{
public:
	CTSocket();
	virtual ~CTSocket();

	static void Startup( );
	static SOCKET ConnectToServerA( LPCSTR pcsIp, const unsigned short uPort );
	static SOCKET ConnectToServerW( LPCWSTR pwsIp, const unsigned short uPort );
	static SOCKET CreateServer( const unsigned short uPort );
	static SOCKET Accept( SOCKET sock, BYTE* pbyIp = NULL, const unsigned int uSize = NULL );
	static int Send( SOCKET sock, const BYTE* pbyBuf, const int nSize );
	static int Recv( SOCKET sock, BYTE* pbyRecv, const int nSize, const unsigned int uMilliTimeOut );
	static int Recv( SOCKET sock, BYTE* pbyRecv, const int nSize );
	static void Close( SOCKET sock );

	static void Cleanup( );
};

