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

#include "User.c"

void gotoxy(int x, int y) {
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void main() {
	char name[30];
	int age;

	int chatY = 6;

	printf("�̸��� �Է��ϼ��� : ");
	gets(name);

	printf("���̸� �Է��ϼ��� : ");
	scanf_s("%d", &age);

	User userData, serverResultData;

	strcpy(userData.name, name, 10);
	userData.age = age;

	getchar();



	WSADATA wsdata;
	int iRes = WSAStartup( MAKEWORD( 0x02, 0x02 ), &wsdata );
	if ( ERROR_SUCCESS != iRes ) return;
	
	SOCKET hSocket;
	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	// 1. ������ �����Ѵ�.
	if ( INVALID_SOCKET == hSocket) return;
	
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPADDR);
	servAddr.sin_port = htons( 1234 );
	iRes = connect(hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	// 2. ������ ������ ��û�Ѵ�.

	printf("===================\n");
	printf("�̸� : %s, ���� : %d\n", userData.name, userData.age);
	printf("�������� ������ �����մϴ�.\n");
	printf("===================\n");

	send(hSocket, &userData, sizeof(userData), 0);
	

	while(1) {		
		char msg[100];
		printf("�Է� : ");
		gets(msg);

		//strcpy(userData.msg, msg, 100);

		send(hSocket, msg, sizeof(msg), 0);

		/*gotoxy(0, chatY++);
		printf("")

		int resultLen = recv(hSocket, &serverResultData, 100, 0);
		printf("��� : %s\n", serverResultData.msg);
		*/
	}
	
	closesocket(hSocket);
	// 4. ������ ����� ���´�.
	WSACleanup();
}