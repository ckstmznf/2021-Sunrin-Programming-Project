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

// color��� ���� 
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

SOCKET hSocket; //������ ����
User clients[10]; //Ŭ���̾�Ʈ�� ������ �����Ѵ� �迭
int nowClientCount = 0; //���ӵ� Ŭ���̾�Ʈ ��

void main() {
	//0. ���� ���̺귯���� �ʱ�ȭ �Ѵ�. ������ ���� ���� ������ �����ϴ� ����ü�� �޴´�.
	WSADATA wsdata; //���� ���� ������ �����Ѵ�.
	int iRes = WSAStartup( MAKEWORD( 0x02, 0x02 ), &wsdata );
	if ( ERROR_SUCCESS != iRes ) return;

	// 1. ������ �����Ѵ�.
	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == hSocket ) return;
	

	// 2. ������ ����� IP�ּҿ� ��Ʈ�� �����Ѵ�.
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPADDR); //������ ����� IP�ּҸ� �����Ѵ�.
	servAddr.sin_port = htons(PORT); //������ ����� ��Ʈ�� �����Ѵ�.
	iRes = bind( hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	if ( ERROR_SUCCESS != iRes ) return;
	

	User serverData;
	strcpy(serverData.name, "����");
	serverData.age = 0;

	textcolor(BLUE, WHITE);
	printf("[LOG] ������ �۵���ŵ�ϴ�.\n");
	textcolor(WHITE, BLACK);

	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)listenClient, NULL, 0, NULL);

	while (1){
		// 5. Ŭ���̾�Ʈ�� ���� �����͸� �޴´�.
		/*
		int iRecv = recv( sockAccept, &resultUserData, sizeof(resultUserData), 0 );
		send(sockAccept, &serverData, sizeof(serverData), 0);
		*/
	}
	
	closesocket( hSocket );
	// 6. ������ �ݴ´�.
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
				strcpy(msg, "���� �� �����Ѱ��� �𸣰ڳ׿�.");
			}

			send(client.socket, msg, sizeof(msg), 0);
		}
	}
}

void listenClient() {
	while (1) {
		// 3. Ŭ���̾�Ʈ�� ���� ��û�� ���� �Ǵ��� Ȯ���Ѵ�.
		// (Ŭ���̾�Ʈ�� ���� ��û�� �޴´�.)
		int iRes = listen(hSocket, SOMAXCONN);
		if (iRes != ERROR_SUCCESS) return;

		// 4. Ŭ���̾�Ʈ�� ���� ��û�� ������ ��û�� �����ϰ�
		// ������ ����� ���� ������ �����Ѵ�.
		struct sockaddr accept_addr;
		int iLen = sizeof(accept_addr);
		SOCKET sockAccept = accept(hSocket, &accept_addr, &iLen);
		if (ERROR_SUCCESS != iRes) return;

		int userSize = recv(sockAccept, clients + nowClientCount, sizeof(User), 0);

		if (userSize > 0) {
			clients[nowClientCount].socket = sockAccept;

			printLine(30, RED, BLACK);
			printf("Ŭ���̾�Ʈ%d ����\n", nowClientCount);
			printf("�̸� : %s\n", clients[nowClientCount].name);
			printLine(30, RED, BLACK);

			_beginthreadex(NULL, 0, (_beginthreadex_proc_type)recvUserMsg, (void*)nowClientCount, 0, NULL);
			nowClientCount++;
		}
	}
}

void printLine(int lineCount, int txtColor, int bgColor) {
	textcolor(txtColor, bgColor);
	for (int i = 0; i < lineCount; i++) printf("��");
	textcolor(WHITE, BLACK);
	printf("\n");
}

void textcolor(int foreground, int background){
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}