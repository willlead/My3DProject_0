// Select_1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <vector>
#include <list>
#include "tprotocol.h"

#pragma comment (lib, "ws2_32.lib")
using namespace std;


struct TUser
{
	bool bConnect;
	SOCKET sock;
	SOCKADDR_IN ClientAddr;
};

std::list<TUser*> g_UserList;
CRITICAL_SECTION g_crit;

int g_RecvByte = 0;
int g_RecvCNT = 0;
int g_SendByte = 0;
int g_SendCNT = 0;

DWORD WINAPI PrintThread(LPVOID arg)
{
	while (1)
	{

		printf("Recvbyte = %d RecvCNT = %d\n", g_RecvByte / 3, g_RecvCNT / 3);
		printf("Sendbyte = %d SendCNT = %d\n", g_SendByte / 3, g_SendCNT / 3);

		g_RecvByte = 0;
		g_RecvCNT = 0;
		g_SendByte = 0;
		g_SendCNT = 0;

		Sleep(3000);
	}

	return 0;

}
void BroadCasting(TUser* pSendUser, UPACKET* pPacket)
{
	EnterCriticalSection(&g_crit);
	{
		int iUserCnt = 0;
		for (list<TUser*>::iterator itor = g_UserList.begin();
		itor != g_UserList.end(); itor++)
		{
			TUser * user = *itor;
			if (user->bConnect == false)continue;
			int iSendByte = send(user->sock, (char*)pPacket, pPacket->ph.len, 0);
			
			if (iSendByte == SOCKET_ERROR)
			{
				printf("\nClient Exit = [%s]", inet_ntoa(user->ClientAddr.sin_addr));
				break;
			}

			g_SendByte += iSendByte;
			g_SendCNT++;

			printf("\nSend=[%d][%d][%d][%d]",
				ntohs(user->ClientAddr.sin_port),
				pPacket->msg,
				pPacket->ph.type,
				pPacket->ph.len);
		}
	}
	LeaveCriticalSection(&g_crit);
}


void PacketProcess(TUser * pUser, UPACKET* pPacket)
{
	switch (pPacket->ph.type)
	{
	case PACKET_CHAR_MSG:
	{
		BroadCasting(pUser, pPacket);
		printf("\nRecvMessage =%s", pPacket->msg);
	}break;
	default:
		break;
	}
}

DWORD UserRecv(LPVOID data)
{
	TUser* tUser = (TUser*)data;
	SOCKET sock = tUser->sock;
	char szBuffer[MAX_PATH] = { 0, };
	int iRecvData = 0;
	bool bConnect = true;
	while (bConnect)
	{
		ZeroMemory(szBuffer, sizeof(char)*MAX_PATH);
		int iRet = recv(sock, &szBuffer[iRecvData], PACKET_HEADER_SIZE - iRecvData, 0);
		g_RecvByte += iRet;
		g_RecvCNT++;
		iRecvData += iRet;
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			bConnect = false;
			break;
		}
		if (iRecvData == PACKET_HEADER_SIZE)
		{
			UPACKET * pPacket = (UPACKET*)&szBuffer;
			int iRecvSize = pPacket->ph.len - PACKET_HEADER_SIZE;
			int iRet = 0;
			while (iRecvData < pPacket->ph.len)
			{
				iRecvSize -= iRet;
				iRet = recv(sock, &szBuffer[iRecvData], iRecvSize, 0);
				iRecvData += iRet;
				if (iRet == 0 || iRet == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			PacketProcess(tUser, pPacket);
			iRecvData = 0;
		}
		Sleep(1);
	}
	return 0;

}


int main()
{
	InitializeCriticalSection(&g_crit);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 0; }
	SOCKET listensock;
	listensock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR) { return 0; }
	long iNum = 0x7fffffff;

	iRet = listen(listensock, SOMAXCONN);
	if (iRet == SOCKET_ERROR) { return 0; }

	SOCKADDR_IN clientAddr;
	SOCKET client;
	FD_SET rSet, wSet;	
	//출력==========================================================================
	HANDLE hThread;
	DWORD dwPrintThreadID;
	hThread = CreateThread(0, 0,
		PrintThread,
		(LPVOID)0,
		0,
		&dwPrintThreadID);
	//==============================================================================


	while (1)
	{
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		FD_SET(listensock, &rSet);
		for (
			list<TUser*>::iterator itor = g_UserList.begin();
			itor != g_UserList.end(); itor++)
		{
			TUser* user = *itor;
			FD_SET(user->sock, &rSet);
		}

		timeval time;
		time.tv_sec = 0;
		time.tv_usec = 1;
		int iRet = select(0, &rSet, &wSet, NULL, &time);
		if (iRet == SOCKET_ERROR) { return 0; }
		if (iRet > 0)
		{
			if (FD_ISSET(listensock, &rSet))
			{
				int addlength = sizeof(clientAddr);
				client = accept(listensock, (SOCKADDR*)&clientAddr, &addlength);
				printf("\n Client Connect! ip = %s, port = %d",
					inet_ntoa(clientAddr.sin_addr),
					ntohs(clientAddr.sin_port));

				TUser* pUser = new TUser;
				pUser->sock = client;
				pUser->ClientAddr = clientAddr;
				g_UserList.push_back(pUser);

				for (list<TUser*>::iterator itor = g_UserList.begin();
				itor != g_UserList.end(); itor++)
				{
					TUser* user = *itor;
					if (FD_ISSET(user->sock, &rSet))
					{
						UserRecv(user);
					}
					if (FD_ISSET(user->sock, &wSet))
					{
						//UserSend(user);
					}

				}

			}
		}
	}
	closesocket(listensock);
	WSACleanup();
	DeleteCriticalSection(&g_crit);


	return 0;
}

