#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <windows.h>
#include <process.h>

#define BUFSIZE 512
#define IPADDR "127.0.0.1"
#define PORT 8080

unsigned WINAPI HandleClnt(void* arg);
unsigned WINAPI serveMsg(void* arg);

int main() {
	WSADATA wsaData;
	SOCKADDR_IN serveAddr, clientAddr;
	SOCKET hservSock, hclientSock;
	int szClientAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("ERROR\n");
		return 0;
	}

	hservSock = socket(AF_INET, SOCK_STREAM, 0);
	if (hservSock == INVALID_SOCKET) {
		printf("ERROR\n");
		return 1;
	}

	msmset(&serveAddr, 0, sizeof(serveAddr));
	serveAddr.sin_family = AF_INET;
	serveAddr.sin_port = htons(PORT);
	serveAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(hservSock, (SOCKADDR*)&serveAddr, sizeof(serveAddr)) == SOCKET_ERROR) {
		printf("Bind Error");
	}

	if (listen(hservSock, 5) == SOCKET_ERROR) {
		printf("listen Error");
	}

	printf("서버를 시작합니다.\n");

	HANDLE hThread[2];
	
	while (1) {
		szClientAddr = sizeof(clientAddr);
		hclientSock = accept(hservSock, (SOCKADDR*)&clientAddr, &szClientAddr);

		hThread[0] = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hclientSock, 0, NULL);
		hThread[1] = (HANDLE)_beginthreadex(NULL, 0, serveMsg, (void*)&hclientSock, 0, NULL);

		WaitForMultipleObjects(1, hThread, TRUE, INFINITE);

		printf("서버를 종료합니다.");
		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);
	}

	closesocket(hservSock);
	WSACleanup();
	return 0;
}

unsigned WINAPI HandleClnt(void* arg) {
	SOCKET hclientSock = *((SOCKET*)arg);
	int strLen;
	char msg[BUFSIZE];

	while ((strLen = recv(hclientSock, msg, sizeof(msg), 0)) != 0) {
		msg[strLen] = 0;
		printf("[%d][recv], %s", strLen, msg);
	}

	closesocket(hclientSock);
	return 0;
}


unsigned WINAPI serveMsg(void* arg) {
	SOCKET hclientSocket = *((SOCKET*)arg);
	char nameMsg[BUFSIZE];
	char testMsg[BUFSIZE];

	while (1) {
		fgets(testMsg, BUFSIZE, stdin);

		sprintf_s(nameMsg, "%s", testMsg);
		send(hclientSocket, nameMsg, strlen(nameMsg), 0);
	}

	return 0;
}