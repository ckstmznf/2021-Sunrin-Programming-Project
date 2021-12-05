#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <process.h>

#define MAXLEN 1024

#pragma comment (lib, "ws2_32.lib")

#define BUF_SIZE 100
#define NAME_SIZE 20
#define IPADDR "127.0.0.1"
#define PORT 8080

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);

int main() {
	WSADATA wsaData;
	SOCKADDR_IN addr;
	SOCKET sockfd;

	HANDLE hTh[2];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
		return 1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		return 1;
	}

	memset((void*)&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IPADDR;
	addr.sin_port = htons(PORT);

	if (connect(sockfd, (SOCKADDR*)&addr, sizrof(addr)) == SOCKET_ERROR) {
		return 1;
	}

	printf("서버와 연결\n");

	hTh[0] = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&sockfd, 0, NULL);
	hTh[1] = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&sockfd, 0, NULL);

	WaitForSingleObject(hTh[0], INFINITE);
	WaitForSingleObject(hTh[1], INFINITE);

	CloseHandle(hTh[0]);
	CloseHandle(hTh[1]);

	closesocket(sockfd);
	WSACleanup();
	return 0;
}

unsigned WINAPI SendMsg(void* arg)
{
	SOCKET hSock = *((SOCKET*)arg);
	char msg[BUF_SIZE];
	
	while (1) {
		fgets(msg, BUF_SIZE, stdin);
		send(hSock, msg, strlen(msg), 0);
	}

	return 0;
}

unsigned WINAPI RecvMsg(void* arg)
{
	int hSock = *((SOCKET*)arg);
	char msg[BUF_SIZE];
	int strLen;

	while ((strLen = recv(hSock, msg, sizeof(msg), 0)) != 0) {
		if (strLen == -1) {
			return -1;
		}

		msg[strLen] = 0;
		printf("[recv]");
		fputs(msg, stdout);
	}

	return 0;
}
