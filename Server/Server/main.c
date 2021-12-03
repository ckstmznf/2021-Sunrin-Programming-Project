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

#include "User.c"

SOCKET hSocket; //������ ����
SOCKET clients[10]; //Ŭ���̾�Ʈ�� ������ �����ϴ� �迭
int nowClientCount = 0; //���ӵ� Ŭ���̾�Ʈ ��

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

		clients[nowClientCount++] = sockAccept;
		printf("������ Ŭ���̾�Ʈ%d ����\n", nowClientCount);
	}
}

void gotoxy(int x, int y) {
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

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
	servAddr.sin_port = htons( 1234 ); //������ ����� ��Ʈ�� �����Ѵ�.
	iRes = bind( hSocket, (LPSOCKADDR)&servAddr, sizeof( servAddr ) );
	if ( ERROR_SUCCESS != iRes ) return;
	

	User serverData;
	strcpy(serverData.name, "����");
	serverData.age = 0;


	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)listenClient, NULL, 0, NULL);

	while (1){
		User resultUserData;
		char input[100];

		memset(&resultUserData, 0, sizeof(resultUserData));
		

		// 5. Ŭ���̾�Ʈ�� ���� �����͸� �޴´�.
		// int iRecv = recv( sockAccept, &resultUserData, sizeof(resultUserData), 0 );
		

		/*if ( 0 < iRecv ) {
			printf("%s(%d��) : %s\n", resultUserData.name, resultUserData.age, resultUserData.msg);
		}

		printf("�Է� : ");
		gets(input);
		strcpy(serverData.msg, input);
		send(sockAccept, &serverData, sizeof(serverData), 0);*/
	}
	
	closesocket( hSocket );
	// 6. ������ �ݴ´�.
	WSACleanup();
}
