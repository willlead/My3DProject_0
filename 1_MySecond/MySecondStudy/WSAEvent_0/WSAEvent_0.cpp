// WSAEvent_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <winsock2.h>
#include <string>

#define BUFSIZE 100
void CompressSockets(SOCKET* hSockArray, int omitIndex, int total);
void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total);
void ErrorHandling(char *message);


int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET hServSock;
	SOCKADDR_IN servAddr;

	SOCKET hSockArray[WSA_MAXIMUM_WAIT_EVENTS];
	SOCKET hClntSock;
	int clntLen;
	SOCKADDR_IN clntAddr;

	WSAEVENT hEventArray[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvent;

	int sockTotal = 0;
	int index, i;
	char message[BUFSIZE];
	int strLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("wsastartup");

	hServSock = socket(AF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket()");

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind()");

	newEvent = WSACreateEvent();
	if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("EventSelect()");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen()");

	hSockArray[sockTotal] = hServSock;
	hEventArray[sockTotal] = newEvent;
	sockTotal++;
	
	while (1)
	{
		index = WSAWaitForMultipleEvents(sockTotal, hEventArray, FALSE, WSA_INFINITE, FALSE);
		index = index - WSA_WAIT_EVENT_0;
	}


    return 0;
}

