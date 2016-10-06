#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	while (1)
	{
		ZeroMemory(buf, sizeof(char) * 256);
		fgets(buf, 256, stdin);
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
		if (strlen(buf) == 0) break; // 엔터는 종료!
		int iSendByte = send(sock, buf, strlen(buf), 0);
		if (iSendByte == SOCKET_ERROR) break;
		printf("\t%d 바이트를 전송하였습니다.", iSendByte);
	}
	return 0;
}

int main(int argc, char* argv[])
{

	HANDLE hExcuteMutex = CreateMutex(NULL, FALSE, L"OnecClientMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExcuteMutex);
		MessageBox(NULL, L"이미 다른 인스턴스가 실행중입니다.", L"알림", MB_OK);
		return 1;
	}

	if (argc != 3)
	{
		printf("\n[사용방법] xxx.exe [port] [ip]");
		printf("\n[사용예시] xxx.exe 10000 192.168.0.100");
		//return 1;		
	}

	HANDLE hThread;
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	{
		SOCKET sock;
		//socket
		sock = socket(AF_INET, SOCK_STREAM, 0);
		{
			SOCKADDR_IN sa;
			ZeroMemory(&sa, sizeof(sa));
			sa.sin_family = AF_INET;
			sa.sin_port = htons(10000);
			//sa.sin_addr.s_addr = inet_addr("192.168.0.100");
			sa.sin_addr.s_addr = inet_addr("192.168.0.100");
			//sa.sin_addr.s_addr = inet_addr();

			//connect
			iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
			if (iRet == SOCKET_ERROR) { return 1; }

			DWORD dwRecvThreadID;

			//createthread
			hThread = CreateThread(
				0, 0,
				// sendthread 정의
				SendThread,
				(LPVOID)sock, 
				0,
				&dwRecvThreadID);
			{
				char szBuffer[MAX_PATH] = { 0, };
				int iLen = 0;
				while (1)
				{
					//recv
					int iRecvByte = recv(sock, szBuffer, sizeof(char)*MAX_PATH, 0);
					//int iSendByte = send(sock, szBuffer, sizeof(char)*MAX_PATH, 0);

					if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
					//if (iSendByte == 0 || iSendByte == SOCKET_ERROR)
					{
						printf("[연결종료]: %s	", "서버 문제로 인하여 종료되었다.");
						break;
					}
					//szBuffer[iRecvByte] = 0;
					szBuffer[iRecvByte] = '\0';
					//printf("\n%s", buf);
					//szBuffer[iSendByte] = 0;
					printf("\n [받은 메시지]: %s", szBuffer);
					printf("\n보낼 데이터를 입력하시오?	");

				}

			}

		}
		closesocket(sock);
	}
	CloseHandle(hThread);
	iRet = WSACleanup();
	CloseHandle(hExcuteMutex);
	return 0;
}