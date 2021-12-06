#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <process.h>

#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib")

#define IPADDR "127.0.0.1"
#define PORT 8080

// color상수 지정 
#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15

#include "User.c"
#include "SendObject.c"

void recvUserMsg(int userIdx);
void listenClient();

void printLine(int lineCount, int txtColor, int bgColor);
void textcolor(int foreground, int background);

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

SOCKET hSocket; //서버의 소켓
User clients[10]; //클라이언트의 정보를 저장한는 배열
int nowClientCount = 0; //접속된 클라이언트 수

void main() {
	//0. 소켓 라이브러리를 초기화 한다. 버전과 소켓 관련 정보를 전달하는 구조체를 받는다.
	WSADATA wsdata; //소켓 관련 정보를 저장한다.
	int iRes = WSAStartup( MAKEWORD( 0x02, 0x02 ), &wsdata );
	if ( ERROR_SUCCESS != iRes ) return;

	// 1. 소켓을 생성한다.
	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == hSocket ) return;
	

	// 2. 서버가 사용할 IP주소와 포트를 연결한다.
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPADDR); //서버가 사용할 IP주소를 설정한다.
	servAddr.sin_port = htons(PORT); //서버가 사용할 포트를 연결한다.
	iRes = bind( hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	if ( ERROR_SUCCESS != iRes ) return;
	

	User serverData;
	strcpy(serverData.name, "서버");
	serverData.age = 0;

	textcolor(BLUE, WHITE);
	printf("[LOG] 서버를 작동시킵니다.\n");
	textcolor(WHITE, BLACK);

	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)listenClient, NULL, 0, NULL);

	while (1){
		// 5. 클라이언트로 부터 데이터를 받는다.
		/*
		int iRecv = recv( sockAccept, &resultUserData, sizeof(resultUserData), 0 );
		send(sockAccept, &serverData, sizeof(serverData), 0);
		*/
	}
	
	closesocket( hSocket );
	// 6. 소켓을 닫는다.
	WSACleanup();
}

void recvUserMsg(int userIdx) {
	User client = clients[userIdx];
	while (1) {
		char msg[100];
		int resultLen = recv(client.socket, msg, sizeof(msg), 0);
		if (resultLen > 0) {
			printf("[Chat] %s : %s\n", client.name, msg);

			if (0) {

			}
			else {
				strcpy(msg, "제가 잘 이해한건지 모르겠네요.");
			}

			send(client.socket, msg, sizeof(msg), 0);
		}
	}
}

void listenClient() {
	while (1) {
		// 3. 클라이언트의 연결 요청이 수신 되는지 확인한다.
		// (클라이언트의 연결 요청을 받는다.)
		int iRes = listen(hSocket, SOMAXCONN);
		if (iRes != ERROR_SUCCESS) return;

		// 4. 클라이언트의 연결 요청이 들어오면 요청을 수락하고
		// 데이터 통신을 위한 소켓을 생성한다.
		struct sockaddr accept_addr;
		int iLen = sizeof(accept_addr);
		SOCKET sockAccept = accept(hSocket, &accept_addr, &iLen);
		if (ERROR_SUCCESS != iRes) return;

		int userSize = recv(sockAccept, clients + nowClientCount, sizeof(User), 0);

		if (userSize > 0) {
			clients[nowClientCount].socket = sockAccept;

			printLine(30, RED, BLACK);
			printf("클라이언트%d 접속\n", nowClientCount);
			printf("이름 : %s\n", clients[nowClientCount].name);
			printLine(30, RED, BLACK);

			_beginthreadex(NULL, 0, (_beginthreadex_proc_type)recvUserMsg, (void*)nowClientCount, 0, NULL);
			nowClientCount++;
		}
	}
}

void printLine(int lineCount, int txtColor, int bgColor) {
	textcolor(txtColor, bgColor);
	for (int i = 0; i < lineCount; i++) printf("―");
	textcolor(WHITE, BLACK);
	printf("\n");
}

void textcolor(int foreground, int background){
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}