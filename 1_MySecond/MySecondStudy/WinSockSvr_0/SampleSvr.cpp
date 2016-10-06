#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
//#include <Windows.h> 반드시 윈속 아래에 윈도우헤더 파일 
#include <stdio.h>

#pragma comment (lib,"ws2_32.lib") 

// 소켓 프로그래밍
void main()
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return; }

	SOCKET sock;
	//				주소체계, 소켓 타입,   프로토콜 지정
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//sock = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	// h to n s,  n to h l, ...

	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("127.0.0.1");
	//int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	int iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	// 동시에 접속하는 유저의 개수
	iRet = listen(sock, SOMAXCONN);

	SOCKADDR_IN clientAddr;
	SOCKET client;

	while (1)
	{
		
		int addlength = sizeof(clientAddr);
		//accept는 접속되어야만 반환한다.
		//블록형 함수 : 무슨일이 일어나기전에 반환하지 않는다.
		client = accept(sock, (SOCKADDR*)&clientAddr, &addlength);

		printf("Client Connect! ip = %s, port = %d\n",
			inet_ntoa(clientAddr.sin_addr),
				ntohs(clientAddr.sin_port));
	}

	char szBuffer[MAX_PATH] = { 0, };
	while (1)
	{
		ZeroMemory(szBuffer, sizeof(char*)*MAX_PATH);
		//recv 는 데이터를 보내줘야만 반환한다.
		//블록형 함수 : 무슨일이 일어나기전에 반환하지 않는다.
		int iRecvByte = recv(client, szBuffer, sizeof(char)*MAX_PATH, 0);

		if (iRecvByte == SOCKET_ERROR)
		{
			break;
		}
		printf("Message = %s", szBuffer);


	}

	// 윈속 해제
	WSACleanup();
}