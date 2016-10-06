// UDPRecv.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment  (lib,"ws2_32.lib")

#ifndef Multi 

int main()
{
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKET sockSend = socket(AF_INET, SOCK_DGRAM, 0);
	int iLen, iRet;

	// Recv
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (SOCKADDR*)&addr, sizeof(addr));

	// 멀티캐스팅 그룹
	// 224.0.0.0 ~ 239.255.255.255
	struct ip_mreq mreq;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");

	// 소켓 옵션
	BOOL bEnable = TRUE;
	iRet = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	if (iRet == SOCKET_ERROR)
	{
		return 1;
	}

	// Send
	SOCKADDR_IN addrSend;
	ZeroMemory(&addrSend, sizeof(addrSend));
	addrSend.sin_family = AF_INET;
	addrSend.sin_port = htons(9000);
	addrSend.sin_addr.s_addr = inet_addr("235.7.8.9");

	SOCKADDR_IN peer;
	char buf[256 + 1] = { 0, };
	char bufSend[256] = "<<<<< Send BG!! >>>>>";
	while (1)
	{
		// Send
		//iLen = sizeof(peer);
		//iRet = sendto(sock, bufSend, strlen(bufSend), 0, (SOCKADDR*)&addrSend, sizeof(addrSend));

		// Recv
		iLen = sizeof(peer);
		iRet = recvfrom(sock, buf, 256, 0, (SOCKADDR*)&peer, &iLen);
		buf[iRet] = 0;
		printf("\n[Recv] %s:%d : %s", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), buf);

		Sleep(1000);
	}

	// 소켓 옵션
	iRet = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	if (iRet == SOCKET_ERROR)
	{
		return 1;
	}

	closesocket(sockSend);
	closesocket(sock);
	WSACleanup();
	return 0;
}
#else
int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return -1; }

	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	//setsockopt(sock,SOL_SOCKET,so)

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(9000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (SOCKADDR*)&sa, sizeof(sa));

	/*SOCKADDR_IN sendadd;
	sendadd.sin_family = AF_INET;
	sendadd.sin_port = htons(9000);
	sendadd.sin_addr.s_addr = htonl(INADDR_BROADC);*/

	SOCKADDR_IN peer;
	SOCKET client;
	char buff[256 + 1] = { 0, };
	while (1)
	{

		int addlength = sizeof(peer);
		//accept는 접속되어야만 반환한다.
		//블록형 함수 : 무슨일이 일어나기전에 반환하지 않는다.
		int iRet = recvfrom(sock, buff, 256, 0, (SOCKADDR*)&peer
			, &addlength);
		buff[iRet] = '\0';
		printf("\n%s", buff);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}
# endif



