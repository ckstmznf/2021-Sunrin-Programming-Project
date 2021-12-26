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

void printLine(int lineCount);
void clearInput(int len);
void gotoxy(int x, int y);

User getUserData() {
	char name[30];
	int age;

	printLine(32);
	printf("|\t\t\t\t\t\t\t\t|\n");
	printLine(32);
	gotoxy(1, 1);
	printf("�̸��� �Է��ϼ��� : ");
	gets(name);

	
	system("cls");


	printLine(32);
	printf("|\t\t\t\t\t\t\t\t|\n");
	printLine(32);

	gotoxy(0, 3);
	printf("�̸� : %s", name);

	gotoxy(1, 1);
	printf("���̸� �Է��ϼ��� : ");
	scanf_s("%d", &age);

	User userData, serverResultData;

	strcpy(userData.name, name);
	userData.age = age;

	getchar();

	return userData;
}

void gotoxy(int x, int y) {
	COORD pos = { x , y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

User userData;
SOCKET hSocket;
int printYPos = 6;

void main() {
	userData = getUserData();

	WSADATA wsdata;
	int iRes = WSAStartup( MAKEWORD( 0x02, 0x02 ), &wsdata );
	if ( ERROR_SUCCESS != iRes ) return;
	
	//SOCKET hSocket;
	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	// 1. ������ �����Ѵ�.
	if ( INVALID_SOCKET == hSocket) return;
	
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPADDR);
	servAddr.sin_port = htons(PORT);
	iRes = connect(hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	// 2. ������ ������ ��û�Ѵ�.

	system("cls");

	printLine(30);
	printf("�̸� : %s, ���� : %d\n", userData.name, userData.age);
	printf("�������� ������ �����մϴ�.\n");
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
		printf("��� : %s\n", serverResultData.msg);
		*/

	}
	
	closesocket(hSocket);
	// 4. ������ ����� ���´�.
	WSACleanup();
}


void sendMsg() {
	while (1) {
		char msg[100];
		gotoxy(0, 4);
		printf("> ");
		gets(msg);

		
		gotoxy(0, printYPos++);
		printf("> �� : %s", msg);
		clearInput(strlen(msg));

		send(hSocket, msg, sizeof(msg), 0);
	}
}

void recvMsg() {
	while (1) {
		char resultMsg[100];
		int resultLen = recv(hSocket, resultMsg, sizeof(resultMsg), 0);
		if (resultLen > 0) {
			gotoxy(0, printYPos++);

			printf("%s : %s\n", "ê��", resultMsg);

			gotoxy(2, 4);
		}
	}
}

void printLine(int lineCount) {
	for (int i = 0; i < lineCount; i++) printf("��");
	printf("\n");
}

void clearInput(int len) {
	gotoxy(2, 4);
	while (len--) printf(" ");
	gotoxy(2, 4);
}