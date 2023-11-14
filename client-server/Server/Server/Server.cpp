#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

#pragma warning(disable: 4996)

SOCKET Connections[100];
int counter = 0;

enum Packet
{
	P_ChatMessage
};

bool ProcessPacket(int index, Packet packetType)
{
	switch (packetType) {
	case P_ChatMessage:
		{
			int msg_size;
			recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
			char* msg = new char[msg_size + 1];
			msg[msg_size] = '\0';
			recv(Connections[index], msg, msg_size, NULL);
			for (int i = 0; i < counter; i++)
			{
				if (i == index)
				{
					continue;
				}

				Packet msgType = P_ChatMessage;
				send(Connections[i], (char*)&msgType, sizeof(Packet), NULL);
				send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
				send(Connections[i], msg, msg_size, NULL);
			}

			delete[] msg;
			break;
		}
	default:
		cout << "Unrecognized packet: " << packetType << endl;
		break;
	}
	
	return true;
}

void ClientHandler(int index)
{
	Packet packetType;
	while (true)
	{
		recv(Connections[index], (char*)&packetType, sizeof(Packet), NULL);
		if (!ProcessPacket(index, packetType))
		{
			break;
		}
		
	}
	closesocket(Connections[index]);
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

	SOCKET socketListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(socketListen, (SOCKADDR*)&addr, sizeof(addr));

	cout << "Wait clients connections..." << endl;

	listen(socketListen, SOMAXCONN);

	SOCKET newConn;
	for (int i = 0; i < 100; i++)
	{
		newConn = accept(socketListen, (SOCKADDR*)&addr, &sizeOfAddr);
		

		if (newConn == 0)
		{
			cout << "Error 2\n" << endl;
		}
		else
		{
			printf("%s %d connected\n", inet_ntoa(addr.sin_addr), addr.sin_port);
			
			/*string msg = "Hello. It`s server message";
			int msg_size = msg.size();
			send(newConn, (char*)&msg_size, sizeof(int), NULL);
			send(newConn, msg.c_str(), msg_size, NULL);*/
			string msg = "Hello client!";
			int msg_size = msg.size();
			Packet msgType = P_ChatMessage;
			send(newConn, (char*)&msgType, sizeof(Packet), NULL);
			send(newConn, (char*)&msg_size, sizeof(int), NULL);
			send(newConn, msg.c_str(), msg_size, NULL);


			Connections[i] = newConn;
			counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");
	

	return 0;
}