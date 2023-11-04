#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

using namespace std;

#pragma warning(disable: 4996)

int main(int argc, char* argv[])
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "Error" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeOfAddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET socketListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(socketListen, (SOCKADDR*)&addr, sizeof(addr));

	listen(socketListen, SOMAXCONN);

	SOCKET newConn;
	newConn = accept(socketListen, (SOCKADDR*)&addr, &sizeOfAddr);

	if (newConn == 0)
	{
		cout << "Error 2\n" << endl;
	}
	else
	{
		cout << "Client connected" << endl;
		char msg[256] = "Hello, I'm server";
		send(newConn, msg, sizeof(msg), NULL);
	}

	return 0;
}