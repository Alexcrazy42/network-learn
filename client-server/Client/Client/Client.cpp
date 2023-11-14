#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

#pragma warning(disable: 4996)

SOCKET connection;

enum Packet 
{
	P_ChatMessage
};

bool ProcessPacket(Packet packetType)
{
	switch (packetType)
	{
	case P_ChatMessage:
	{
		int msg_size;
		recv(connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(connection, msg, msg_size, NULL);
		cout << msg << endl;
		delete[] msg;
		break;
	}
	default:
		cout << "Unrecognized packet: " << packetType << endl;
		break;
	}
	return true;
}

void ClientHandler()
{
	Packet packetType;
	while (true)
	{
		recv(connection, (char*)&packetType, sizeof(Packet), NULL);
		if (!ProcessPacket(packetType))
		{
			break;
		}
	}
	closesocket(connection);
}

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

	connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		cout << "Error: failed connect to server" << endl;
		return 1;
	}
	cout << "Connected!" << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	string msg1;
	while (true)
	{
		std::getline(std::cin, msg1);
		int msg_size = msg1.size();
		Packet packetType = P_ChatMessage;
		send(connection, (char*)&packetType, sizeof(Packet), NULL);
		send(connection, (char*)&msg_size, sizeof(int), NULL);
		send(connection, msg1.c_str(), msg_size, NULL);
		Sleep(10);
		
	}

	system("pause");
	return 0;
}