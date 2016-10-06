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
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return; }

	SOCKET sock;
	//				�ּ�ü��, ���� Ÿ��,   �������� ����
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
	// ���ÿ� �����ϴ� ������ ����
	iRet = listen(sock, SOMAXCONN);

	SOCKADDR_IN clientAddr;
	SOCKET client;

	while (1)
	{
		
		int addlength = sizeof(clientAddr);
		//accept�� ���ӵǾ�߸� ��ȯ�Ѵ�.
		//����� �Լ� : �������� �Ͼ������ ��ȯ���� �ʴ´�.
		client = accept(sock, (SOCKADDR*)&clientAddr, &addlength);

		printf("Client Connect! ip = %s, port = %d\n",
			inet_ntoa(clientAddr.sin_addr),
				ntohs(clientAddr.sin_port));
	}

	char szBuffer[MAX_PATH] = { 0, };
	while (1)
	{
		ZeroMemory(szBuffer, sizeof(char*)*MAX_PATH);
		//recv �� �����͸� ������߸� ��ȯ�Ѵ�.
		//����� �Լ� : �������� �Ͼ������ ��ȯ���� �ʴ´�.
		int iRecvByte = recv(client, szBuffer, sizeof(char)*MAX_PATH, 0);

		if (iRecvByte == SOCKET_ERROR)
		{
			break;
		}
		printf("Message = %s", szBuffer);


	}

	// ���� ����
	WSACleanup();
}