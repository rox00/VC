#include "stdafx.h"
#include "TSocket.h"
#include <deffunc.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")


CTSocket::CTSocket()
{
}

CTSocket::~CTSocket()
{
}

void CTSocket::Startup( )
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(1,1), &WSAData);
}

SOCKET CTSocket::ConnectToServerA( LPCSTR pcsIp, const unsigned short uPort )
{

	SOCKET sockRet = NULL;

	if ( pcsIp != NULL && uPort < 65536 )
	{
		SOCKET sock;
		struct sockaddr_in stSockAddrInfo;
		int nSize = sizeof( struct sockaddr_in );
		memset( &stSockAddrInfo, 0, sizeof( stSockAddrInfo ));
		stSockAddrInfo.sin_family = AF_INET;
		stSockAddrInfo.sin_port = htons( uPort );
		stSockAddrInfo.sin_addr.s_addr = inet_addr( pcsIp );
		sock = socket( AF_INET, SOCK_STREAM, 0 );
		if ( sock > 0 )
		{
			// ¿ªÆôKeepAlive
			BOOL bKeepAlive = TRUE;
			::setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));


			if( connect( sock, (struct sockaddr *)&stSockAddrInfo, nSize ) < 0 )
			{
				CTSocket::Close( sock );
			}
			else
			{
				sockRet = sock;
			}
		}
	}
	return sockRet;
}

SOCKET CTSocket::ConnectToServerW( LPCWSTR pwsIp, const unsigned short uPort )
{
	if ( pwsIp != NULL )
	{
		char szIp[100];
		memset( szIp, 0, sizeof(szIp) );
		WideCharToMultiByte( CP_ACP, 0, pwsIp, wcslen(pwsIp)+1, szIp, _countof(szIp), NULL, NULL );
		return CTSocket::ConnectToServerA( szIp, uPort );
	}
	return NULL;
}

SOCKET CTSocket::CreateServer( const unsigned short uPort )
{

	SOCKET sockRet = NULL;
	if ( uPort < 65536 )
	{
		SOCKADDR_IN addr;
		BOOL  bDontLinger = FALSE;
		SOCKET sock = socket( AF_INET,SOCK_STREAM,IPPROTO_IP );
		if ( sock != INVALID_SOCKET )
		{
			setsockopt( sock, SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));
			addr.sin_family = AF_INET;
			addr.sin_port = htons( (short)uPort );
			addr.sin_addr.s_addr= htonl(INADDR_ANY);
			if ( SOCKET_ERROR == bind( sock, (struct sockaddr*)&addr, sizeof(struct sockaddr) ) )
			{
				closesocket( sock );
			}
			else
			{
				if ( SOCKET_ERROR == listen(sock, SOMAXCONN) )
				{
					closesocket( sock );
				}
				else
				{
					sockRet = sock;
				}
			}
		}
	}
	return sockRet;
}

SOCKET CTSocket::Accept( SOCKET sock, BYTE* pbyIp, const unsigned int uSize )
{
	SOCKET sockRet = NULL;
	if ( sock != NULL )
	{
		SOCKADDR_IN addrclient;
		int nLen = sizeof(struct sockaddr);
		sockRet = accept( sock, (struct sockaddr*)&(addrclient), &nLen );

		// ¿ªÆôKeepAlive
		BOOL bKeepAlive = TRUE;
		::setsockopt(sockRet, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));

		if ( pbyIp != NULL )
		{
			BYTE byTemp[4];
			byTemp[0] = addrclient.sin_addr.S_un.S_un_b.s_b1;
			byTemp[1] = addrclient.sin_addr.S_un.S_un_b.s_b2;
			byTemp[2] = addrclient.sin_addr.S_un.S_un_b.s_b3;
			byTemp[3] = addrclient.sin_addr.S_un.S_un_b.s_b4;
			for ( unsigned int i = 0; i < min(4, uSize); i++ )
			{
				pbyIp[i] = byTemp[i];
			}
		}
	}
	return sockRet;
}

int CTSocket::Send( SOCKET sock, const BYTE* pbyBuf, const int nSize )
{

	int nSendLen = -1;
	if ( sock != NULL )
	{
		nSendLen = 0;
		while ( nSendLen < nSize )
		{
			int nTemp = send( sock, (char*)&pbyBuf[nSendLen], nSize - nSendLen, 0 );
			if ( nTemp <= 0 )
			{
				break;
			}
			nSendLen += nTemp;
		}
	}
	return nSendLen;
}

int CTSocket::Recv( SOCKET sock, BYTE* pbyRecv, const int nSize )
{
	int nReaded = -1;
	if ( sock )
	{
		nReaded = recv( sock, (char*)pbyRecv, nSize, FALSE );
		if ( nReaded == 0 )
		{
			nReaded = -1;
		}
	}
	return nReaded;
}


int CTSocket::Recv ( SOCKET sock, BYTE* pbyRecv, const int nSize, const unsigned int uMilliTimeOut )
{
	int nReaded = -1;
	if ( sock )
	{
		nReaded = 0;
		int nSelect = 0;
		fd_set fdRead;
		struct timeval stTimeOut;
		FD_ZERO( &fdRead );
		FD_SET( sock, &fdRead );
		stTimeOut.tv_sec = uMilliTimeOut/1000;
		stTimeOut.tv_usec = uMilliTimeOut%1000;

		nSelect = select( sock+1, &fdRead, 0, 0, &stTimeOut );
		if ( nSelect > 0 && FD_ISSET( sock, &fdRead ) )
		{
			int nTemp = recv( sock, (char*)pbyRecv, nSize, FALSE );
			if ( nTemp <= 0 )
			{
				nReaded = -1;
			}
			else
			{
				nReaded += nTemp;
			}
		}
	}
	return nReaded;
}

void CTSocket::Close( SOCKET sock )
{
	if ( sock != 0 )
	{
		shutdown( sock, 0x02 );
		closesocket( sock );
	}
}

void CTSocket::Cleanup()
{
	WSACleanup();
}
