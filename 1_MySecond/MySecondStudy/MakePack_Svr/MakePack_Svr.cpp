// MakePack_Svr.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

CRITICAL_SECTION g_crit;
HANDLE g_hMutex;
SOCKET g_allClientSocket[100];
int g_iNumClient = 0;

int g_PrnByte = 0;
int g_PrnPacketCount = 0;

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;
	//1
	memcpy(sendmsg.msg, msg, strlen(msg));
	//2
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;// size 4
	
	//send
	int iRet = send(sock, (char*)&sendmsg, sendmsg.ph.len, 0);
	g_PrnByte += iRet;
	g_PrnPacketCount = ++g_PrnPacketCount;
	return iRet;
}

void GreetMessage(SOCKET sock)
{
	char message[MAX_PATH] = { 0, };
	sprintf_s(message, "[서버] 환영합니다. \n");
	//send(sock, message, strlen(message), 0);
	SendMsg(sock, message, PACKET_CHAT_MSG);	
	g_allClientSocket[g_iNumClient] = sock;  //??
	g_iNumClient++;

}

DWORD WINAPI PrintThread(LPVOID arg)
{
	while (1)
	{
		
		printf("byte = %d count = %d\n", g_PrnByte / 3, g_PrnPacketCount / 3);
		g_PrnPacketCount = 0;
		g_PrnByte = 0;
		Sleep(3000);
	}

	return 0;

}

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char szBuffer[MAX_PATH] = { 0, };
	SOCKADDR_IN clientaddr;
	while (1)
	{
		ZeroMemory(&szBuffer, sizeof(char)*MAX_PATH);
		// recv
		int iRecvByte = recv(sock, szBuffer, sizeof(char)*MAX_PATH, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("클라이언트 접속 종료\n");
			break;
		}
		szBuffer[iRecvByte] = 0;
	
		//printf("\n%s", szBuffer);

		//브로드 캐스팅
		DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);
		if (dwRet != WAIT_TIMEOUT)
		{
			for (int iClient = 0; iClient < g_iNumClient; iClient++)
			{
				int iSendByte = SendMsg(g_allClientSocket[iClient], szBuffer, PACKET_CHAT_MSG);
				//int iSendByte = send(g_allClientSocket[iClient], buf, iRecvByte, 0);

				if (iSendByte == SOCKET_ERROR)
				{
					printf("소켓 오류\n");
					break; // 클라이언트 종료
				}
			}
			
		}
		//LeaveCriticalSection(&g_crit);
		ReleaseMutex(g_hMutex);
	}

	// EnterCriticalSection(&g_crit);
	WaitForSingleObject(g_hMutex, INFINITE); // 대기함수

	for (int iClient = 0; iClient < g_iNumClient; iClient++)
	{
		if (g_allClientSocket[iClient] == sock)
		{
			for (int iUser = iClient; iUser < g_iNumClient; iUser++)
			{
				g_allClientSocket[iUser] = g_allClientSocket[iUser + 1];
			}
			break;
		}
	}
	g_iNumClient--;
	ReleaseMutex(g_hMutex);
	closesocket(sock);
	return 0;

}

int main(int argc, char* argv[])
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnecMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		return 1;
	}

	InitializeCriticalSection(&g_crit);
	g_hMutex = CreateMutex(NULL, FALSE, L"CHAT_MUTEX");

	unsigned short iPort = 10000;

	WSADATA wsa;
	//	typedef struct WSAData {
	//		WORD                    wVersion;
	//		WORD                    wHighVersion;		
	//		char                    szDescription[WSADESCRIPTION_LEN + 1];
	//		char                    szSystemStatus[WSASYS_STATUS_LEN + 1];
	//		unsigned short          iMaxSockets;
	//		unsigned short          iMaxUdpDg;
	//		char FAR *              lpVendorInfo;
	//	} WSADATA, FAR * LPWSADATA;
	int iRet;
	HANDLE hThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	{
		//socket
		SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
		//socket(
		//	_In_ int af,
		//	_In_ int type,
		//	_In_ int protocol
		//	);
		{
			SOCKADDR_IN sa;
			ZeroMemory(&sa, sizeof(sa));			//SOCKADDR_IN
			sa.sin_family = AF_INET;					//ADDRESS_FAMILY sin_family;
			sa.sin_port = htons(iPort);					//USHORT sin_port;
			sa.sin_addr.s_addr = htonl(INADDR_ANY);		//IN_ADDR sin_addr;
														//CHAR sin_zero[8];		

			iRet = bind(listenSock, (SOCKADDR*)&sa, sizeof(sa));
			if (iRet == SOCKET_ERROR) return -1;
			// listen
			iRet = listen(listenSock, SOMAXCONN);
			if (iRet == SOCKET_ERROR) return -1;

			SOCKADDR_IN clientaddr;
			SOCKET client;

			//출력==========================================================================
			DWORD dwPrintThreadID;
			hThread = CreateThread(0, 0,
				PrintThread,
				(LPVOID)0,
				0,
				&dwPrintThreadID);
			//==============================================================================


			while (1)
			{
				int addlen = sizeof(clientaddr);
				


				//accept
				client = accept(listenSock, (SOCKADDR*)&clientaddr, &addlen);
				/*printf("클라이언트 접속: IP: %s, PORT: %d\n",
					inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port));*/

				//EnterCriticalSection(&g_crit);
				// 신호상태 일 경우만 진입
				WaitForSingleObject(g_hMutex, INFINITE);// 대기함수
				GreetMessage(client);
				//LeaveCriticalSection(&g_crit);
				ReleaseMutex(g_hMutex);// 비신호상태로 전환

				DWORD dwRecvThreadID;
				hThread = CreateThread(0, 0,
					ClientThread,
					(LPVOID)client,
					0,
					&dwRecvThreadID);
			}
		}
		closesocket(listenSock);
	}
	iRet = WSACleanup();
	DeleteCriticalSection(&g_crit);
	CloseHandle(g_hMutex);
	CloseHandle(hExecuteMutex);


	return 0;
}

