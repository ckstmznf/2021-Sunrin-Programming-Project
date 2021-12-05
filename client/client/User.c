#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib")

typedef struct User {
	char name[10];
	int age;
}User;