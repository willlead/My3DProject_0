#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
//#include <Windows.h> �ݵ�� ���� �Ʒ��� ��������� ���� 
#include <stdio.h>

#pragma comment (lib,"ws2_32.lib") 

// ���� ���α׷���
void main()
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	SOCKET sock;
	//				�ּ�ü��, ���� Ÿ��,   �������� ����
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//sock = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	// h to n s,  n to h l, ...
	
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));	
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	char szBuffer[MAX_PATH] = { 0, };
	while (1)
	{
		ZeroMemory(szBuffer, sizeof(char*)*MAX_PATH);
		fgets(szBuffer, MAX_PATH, stdin);
		int iMessageSize = strlen(szBuffer);
		int iSentbyte = send(sock, szBuffer, iMessageSize, 0);

		printf("\nMessage = %s", szBuffer);


	}


	getchar();
	// ���� ����
	WSACleanup();
}