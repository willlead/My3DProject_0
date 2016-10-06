// MakePack_Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

HANDLE g_hEvent;
SOCKET sock;

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));

	sendmsg.ph.type = type;

	memcpy(sendmsg.msg, msg, strlen(msg));

	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;

	int iRet = send(sock, (char*)&sendmsg, sendmsg.ph.len, 0);
	return iRet;
}


DWORD WINAPI ConnectThread(LPVOID arg)
{
	//socket
	sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);

	sa.sin_addr.s_addr = inet_addr("192.168.0.6");
	//connect
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR) { return 1; }

	linger optval;
	//struct  linger {
	//	u_short l_onoff;                /* option on/off */
	//	u_short l_linger;               /* linger time */
	//};
	int iLength = sizeof(optval);
	getsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optval, &iLength);
	optval.l_onoff = 1;
	optval.l_linger = 1000;
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optval, iLength);

	SetEvent(g_hEvent);
	return 0;
}

DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char szBuffer[256] = { 0, };
	while (1)
	{
		ZeroMemory(szBuffer, sizeof(char) * 256);
		fgets(szBuffer, 256, stdin);
		if (szBuffer[strlen(szBuffer) - 1] == '\n')
			szBuffer[strlen(szBuffer) - 1] = 0;

		if (strlen(szBuffer) == 0 ||
			_stricmp(szBuffer, "quit") == 0)
		{
			SendMsg(sock, szBuffer, PACKET_GAME_END);
			break; // 엔터는 종료!
		}

		int iSendByte = send(sock, szBuffer, strlen(szBuffer), 0);
		if (iSendByte == SOCKET_ERROR) break;
		printf("\t%d 바이트를 전송하였습니다.", iSendByte);
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 0;
}


DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char szBuffer[2048] = { 0, };
	printf("\n보낼 데이터를 입력하시오?	");

	int iRecvByte = 0;
	int iStartByte = 0;
	ZeroMemory(szBuffer, sizeof(char) * 2048);

	while (1)
	{
		int iLen = 0;
		iRecvByte += recv(sock, &szBuffer[iStartByte], PACKET_HEADER_SIZE - iRecvByte, 0);
		//int iSendByte = send(sock, szBuffer, sizeof(char)*MAX_PATH, 0);

		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
			//if (iSendByte == 0 || iSendByte == SOCKET_ERROR)
		{
			printf("[연결종료]: %s	", "서버 문제로 인하여 종료되었다.");
			break;
		}

		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			UPACKET* pPacket = (UPACKET*)&szBuffer;
			while (iRecvByte < pPacket->ph.len)
			{
				iRecvByte += recv(sock, &szBuffer[iRecvByte],
					pPacket->ph.len - iRecvByte, 0);
			}
			if (iRecvByte == pPacket->ph.len)
			{
				UPACKET recvmsg;
				memset(&recvmsg, 0, sizeof(recvmsg));
				memcpy(&recvmsg, pPacket, pPacket->ph.len);
				switch (recvmsg.ph.type)
				{
				default:
					break;
				case PACKET_CHAT_MSG:
				{
					printf("\n[받은메세지]: %s", recvmsg.msg);
					printf("\n보낼 데이터 입력하시오?");
				}break;
				}
				iStartByte = iRecvByte = 0;
			}
		}
		else
		{
			iStartByte += iRecvByte;
		}
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 0;
}


int main(int argc, char* argv[])
{

	HANDLE hExcuteMutex = 0; /*= CreateMutex(NULL, FALSE, L"OnecClientMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExcuteMutex);
		MessageBox(NULL, L"이미 다른 인스턴스가 실행중입니다.", L"알림", MB_OK);
		return 1;
	}*/

	g_hEvent = CreateEvent(NULL, FALSE, FALSE, L"ConnectEvnet");

	if (argc != 3)
	{
		printf("\n[사용방법] xxx.exe [port] [ip]");
		printf("\n[사용예시] xxx.exe 10000 192.168.0.100");
		//return 1;		
	}

	HANDLE hSendThread, hRecvThread, hConnectThread;
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	{
		//SOCKET sock;
		////socket
		//sock = socket(AF_INET, SOCK_STREAM, 0);
		//{
		//	SOCKADDR_IN sa;
		//	ZeroMemory(&sa, sizeof(sa));
		//	sa.sin_family = AF_INET;
		//	sa.sin_port = htons(10000);
		//	//sa.sin_addr.s_addr = inet_addr("192.168.0.100");
		//	sa.sin_addr.s_addr = inet_addr("192.168.0.100");
		//	//sa.sin_addr.s_addr = inet_addr();

		//	//connect
		//	iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
		//	if (iRet == SOCKET_ERROR) { return 1; }
			// 정상 접속
		DWORD dwConnectThreadID;

		hConnectThread = CreateThread(
			0, 0,
			// sendthread 정의
			ConnectThread,
			(LPVOID)0,
			0,
			&dwConnectThreadID);

		WaitForSingleObject(g_hEvent, INFINITE);

		{
			DWORD dwSendThreadID;
			hSendThread = CreateThread(
				0, 0,
				SendThread,
				(LPVOID)sock,
				0,
				&dwSendThreadID);

			DWORD dwRecvThreadID;			  //char szBuffer[MAX_PATH] = { 0, };
			hRecvThread = CreateThread(		  //int iLen = 0;
				0, 0,						  //while (1)
				RecvThread,					  //{
				(LPVOID)sock,				  	////recv
				0,							  	//int iRecvByte = recv(sock, szBuffer, sizeof(char)*MAX_PATH, 0);
				&dwRecvThreadID);			  	////int iSendByte = send(sock, szBuffer, sizeof(char)*MAX_PATH, 0);

												//if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
													////if (iSendByte == 0 || iSendByte == SOCKET_ERROR)
												//{
													//printf("[연결종료]: %s	", "서버 문제로 인하여 종료되었다.");
													//break;
												//}
												////szBuffer[iRecvByte] = 0;
												//szBuffer[iRecvByte] = '\0';
												////printf("\n%s", buf);
												////szBuffer[iSendByte] = 0;
												//printf("\n [받은 메시지]: %s", szBuffer);
												//printf("\n보낼 데이터를 입력하시오?	");


		}

	}

	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);


	CloseHandle(hSendThread);
	CloseHandle(hRecvThread);

	iRet = WSACleanup();
	CloseHandle(hExcuteMutex);
	CloseHandle(g_hEvent);
	return iRet;
}