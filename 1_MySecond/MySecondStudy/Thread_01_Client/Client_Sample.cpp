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
		if (strlen(buf) == 0) break; // ���ʹ� ����!
		int iSendByte = send(sock, buf, strlen(buf), 0);
		if (iSendByte == SOCKET_ERROR) break;
		printf("\t%d ����Ʈ�� �����Ͽ����ϴ�.", iSendByte);
	}
	return 0;
}

int main(int argc, char* argv[])
{

	HANDLE hExcuteMutex = CreateMutex(NULL, FALSE, L"OnecClientMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExcuteMutex);
		MessageBox(NULL, L"�̹� �ٸ� �ν��Ͻ��� �������Դϴ�.", L"�˸�", MB_OK);
		return 1;
	}

	if (argc != 3)
	{
		printf("\n[�����] xxx.exe [port] [ip]");
		printf("\n[��뿹��] xxx.exe 10000 192.168.0.100");
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
				// sendthread ����
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
						printf("[��������]: %s	", "���� ������ ���Ͽ� ����Ǿ���.");
						break;
					}
					//szBuffer[iRecvByte] = 0;
					szBuffer[iRecvByte] = '\0';
					//printf("\n%s", buf);
					//szBuffer[iSendByte] = 0;
					printf("\n [���� �޽���]: %s", szBuffer);
					printf("\n���� �����͸� �Է��Ͻÿ�?	");

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