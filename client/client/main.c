#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <process.h>

#pragma comment( lib, "ws2_32.lib")
#include <WinSock2.h>

#define IPADDR "127.0.0.1"
#define PORT 8080

#include "User.c"
#include "SendObject.c"

void sendMsg();
void recvMsg();
void checkInputCount();

void printLine(int lineCount);

User getUserData() {
	char name[30];
	int age;

	printf("이름을 입력하세요 : ");
	gets(name);

	printf("나이를 입력하세요 : ");
	scanf_s("%d", &age);

	User userData, serverResultData;

	strcpy(userData.name, name);
	userData.age = age;

	getchar();

	return userData;
}

void gotoxy(int x, int y) {
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

User userData;
SOCKET hSocket;
int printYPos = 6;
int nowInputLen = 0;

void main() {
	userData = getUserData();

	WSADATA wsdata;
	int iRes = WSAStartup( MAKEWORD( 0x02, 0x02 ), &wsdata );
	if ( ERROR_SUCCESS != iRes ) return;
	
	//SOCKET hSocket;
	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	// 1. 소켓을 생성한다.
	if ( INVALID_SOCKET == hSocket) return;
	
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPADDR);
	servAddr.sin_port = htons(PORT);
	iRes = connect(hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	// 2. 서버에 연결을 요청한다.

	system("cls");

	printLine(30);
	printf("이름 : %s, 나이 : %d\n", userData.name, userData.age);
	printf("서버와의 연결을 시작합니다.\n");
	printLine(30);
	gotoxy(0, 5);
	printLine(30);


	send(hSocket, &userData, sizeof(userData), 0);
	
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)sendMsg, NULL, 0, NULL);
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)recvMsg, NULL, 0, NULL);

	while(1) {		

		/*gotoxy(0, chatY++);
		printf("")

		int resultLen = recv(hSocket, &serverResultData, 100, 0);
		printf("상대 : %s\n", serverResultData.msg);
		*/

	}
	
	closesocket(hSocket);
	// 4. 서버와 통신을 끝는다.
	WSACleanup();
}


void sendMsg() {
	while (1) {
		char msg[100];
		gotoxy(0, 4);
		printf("> ");
		gets(msg);

		send(hSocket, msg, sizeof(msg), 0);
	}
}

void recvMsg() {
	while (1) {
		SendObject result;
		int resultLen = recv(hSocket, &result, sizeof(SendObject), 0);
		if (resultLen > 0) {
			gotoxy(0, printYPos++);
			printf("%s : %s\n",
				strcmp(result.name, userData.name) == 0 ? "나" : result.name,
				result.msg
			);
			gotoxy(2, 4);
		}
	}
}

void printLine(int lineCount) {
	for (int i = 0; i < lineCount; i++) printf("―");
	printf("\n");
}

void checkInputCount() {
	while (1) {
		//gotoxy(0, 10);
		int cBuf = _getch();
		printf("%d\n", cBuf);
	}
	//nowInputLen
}