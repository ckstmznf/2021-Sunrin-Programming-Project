#pragma once
#include <stdio.h>
#include <conio.h>
#include <string.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
		char msg[100];
		memset(msg, 0, sizeof(msg));

		int iRecv = recv( sockAccept, msg, sizeof(msg), 0 );

		if ( 0 < iRecv ) {
			printf("»ó´ë : %s\n", msg);
			
			if (strcmp("END", msg) == 0) {
				printf("EXIT!");
				break;
			}
		}
	}
	
	closesocket( hSocket );
	WSACleanup();
}
