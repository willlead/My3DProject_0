// ServerSample_0.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib" )

int main()
{
	WSADATA wsaData;
	SOCKET socketListen, socketClient;
	SOCKADDR_IN serverAddr;

	// ���� ���� 1: ���ͳ�, 2: TCP, 3: 
	int socketListen = socket(AF_INET, SOCK_STREAM, 0);

	//SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;// �йи�
	serverAddr.sin_port = htons(10000); // 
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // ��� �ּҿ��� ���°� �� �ްٴ�





	
	closesocket(socketListen);


    return 0;
}

