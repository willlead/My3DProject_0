// ServerSample_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib" )

int main()
{
	WSADATA wsaData;
	SOCKET socketListen, socketClient;
	SOCKADDR_IN serverAddr;

	// 소켓 생성 1: 인터넷, 2: TCP, 3: 
	int socketListen = socket(AF_INET, SOCK_STREAM, 0);

	//SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;// 패밀리
	serverAddr.sin_port = htons(10000); // 
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 주소에서 오는걸 다 받겟다





	
	closesocket(socketListen);


    return 0;
}

