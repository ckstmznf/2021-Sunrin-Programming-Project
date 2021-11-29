#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib")

#define IPADDR "127.0.0.1"

#include "User.c"

void gotoxy(int x, int y) {
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void main() {
	//0. 소켓 라이브러리를 초기화 한다. 버전과 소켓 관련 정보를 전달하는 구조체를 받는다.
	WSADATA wsdata; //소켓 관련 정보를 저장한다.
	int iRes = WSAStartup( MAKEWORD( 0x02, 0x02 ), &wsdata );
	if ( ERROR_SUCCESS != iRes ) return;


	// 1. 소켓을 생성한다.
	SOCKET hSocket;
	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == hSocket ) return;
	

	// 2. 서버가 사용할 IP주소와 포트를 연결한다.
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPADDR); //서버가 사용할 IP주소를 설정한다.
	servAddr.sin_port = htons( 1234 ); //서버가 사용할 포트를 연결한다.
	iRes = bind( hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	if ( ERROR_SUCCESS != iRes ) return;
	

	// 3. 클라이언트의 연결 요청이 수신 되는지 확인한다.
	// (클라이언트의 연결 요청을 받는다.)
	iRes = listen( hSocket, SOMAXCONN );
	if ( ERROR_SUCCESS != iRes ) return;


	// 4. 클라이언트의 연결 요청이 들어오면 요청을 수락하고 데이터 통신을 위한 소켓을 생성한다.
	struct sockaddr accept_addr;
	int iLen = sizeof( accept_addr );
	SOCKET sockAccept = accept( hSocket, &accept_addr, &iLen );
	if ( ERROR_SUCCESS != iRes ) return;

	printf("서버에 클라이언트 접속\n");

	User serverData;
	strcpy(serverData.name, "서버");
	serverData.age = 0;


	while (1){
		User resultUserData;
		char input[100];

		memset(&resultUserData, 0, sizeof(resultUserData));
		

		// 5. 클라이언트로 부터 데이터를 받는다.
		int iRecv = recv( sockAccept, &resultUserData, sizeof(resultUserData), 0 );
		

		if ( 0 < iRecv ) {
			printf("%s(%d세) : %s\n", resultUserData.name, resultUserData.age, resultUserData.msg);
		}

		printf("입력 : ");
		gets(input);
		strcpy(serverData.msg, input);
		send(sockAccept, &serverData, sizeof(serverData), 0);
	}
	
	closesocket( hSocket );
	// 6. 소켓을 닫는다.
	WSACleanup();
}
