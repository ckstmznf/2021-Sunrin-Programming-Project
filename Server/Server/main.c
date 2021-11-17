#pragma once
#include <stdio.h>
#include <conio.h>

#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib")

#define IPADDR "127.0.0.1"

void main() {
	WSADATA wsdata;
	int iRes = WSAStartup( MAKEWORD( 0x02, 0x02 ), &wsdata );
	if ( ERROR_SUCCESS != iRes ) return;
	
	SOCKET hSocket;
	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == hSocket ) return;
	
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPADDR);
	servAddr.sin_port = htons( 1234 );
	iRes = bind( hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	if ( ERROR_SUCCESS != iRes ) return;
	
	iRes = listen( hSocket, SOMAXCONN );
	if ( ERROR_SUCCESS != iRes ) return;
	
	struct sockaddr accept_addr;
	int iLen = sizeof( accept_addr );
	SOCKET sockAccept = accept( hSocket, &accept_addr, &iLen );
	if ( ERROR_SUCCESS != iRes ) return;


	while (1){
		char cBuff;
		memset( &cBuff, 0, sizeof( cBuff ) );

		int iRecv = recv( sockAccept, &cBuff, sizeof( cBuff ), 0 );

		if ( 0 < iRecv ) {
			printf("%c", cBuff);
			
			if ( 0x1b == cBuff ) {
				printf("EXIT");
				break;
			}
		}
	}
	
	closesocket( hSocket );
	WSACleanup();
}
