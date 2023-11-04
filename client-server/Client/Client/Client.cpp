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

	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		cout << "Error: failed connect to server" << endl;
		return 1;
	}
	cout << "Connected!" << endl;
	char msg[256];
	recv(connection, msg, sizeof(msg), NULL);
	cout << msg << endl;

	system("pause");
	return 0;
}