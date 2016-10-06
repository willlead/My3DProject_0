// InitWinsock_0.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")

#define SERVERPORT 10000
#define BUFSIZE 512
void err_quit(CHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf,
		msg, // ��Ƽ����Ʈ
		MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	int retval;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	//socket()

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "TCP ���� ���� ����", "�˸�", MB_OK);

	//bind()	

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, // �������ϱ� �ּ� destination���� �Լ��� �������ְ� ����
		sizeof(serveraddr)); // �������� ����
							 // SOCKADDR_IN ����ü ������ ����
	serveraddr.sin_family = AF_INET;				// ���ͳ�
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// ����( ȣ��Ʈ-> ��Ʈ��ũ)  
	//������ �ϳ��� ȣ��Ʈ��� ��������, 
	// ��Ʈ��ũ�� �����, ������ ���̴�.
	serveraddr.sin_port = htons(SERVERPORT);		// ��Ʈ��ȣ�� ����Ʈ ���� ��� �� �����, ��Ʈ��ũ ����Ʈ ����
	retval =
		bind(
			listen_sock,					// ����
			(SOCKADDR*)&serveraddr,		// ????
			sizeof(serveraddr)			// ??? ����
			);
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");


	SOCKET client_sock ;
	SOCKADDR_IN clientaddr;
	int addlen;
	char buf[BUFSIZE + 1];

	while (1)
	{
		//accept()

		addlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// Ŭ���̾�Ʈ�� ���� �ϸ� �ּҿ� ��Ʈ��ȣ�� ����Ѵ�.
		// ������ Ŭ���̾�Ʈ�� ��Ʈ��ȣ�� ���ƾ��� ������ ���� �����ϴ�. 10000
		printf("\n [TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ��ȣ = %d\n"
			,inet_ntoa(clientaddr.sin_addr), // inet_ntoa  ����ҋ��� ����Ʈ�� �ٲ۰��� �ٽ� ���ڷ� �ٲ����.
			ntohs(clientaddr.sin_port)); // ntohs
		
		// Ŭ���̾�Ʈ�� ������ ���
		while (1)
		{
			// ������ �ޱ�
			retval = 
				recv(client_sock,
					buf, // ���� �ּ�
					BUFSIZE, // ���ú� ������ ������ �ΰ��ΰ�?
					0);
			if (retval == SOCKET_ERROR)
			{
				err_quit("recv()");
				break;
			}
			else if (retval == 0) break;


			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port),buf);

			//������ ������
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}
		//closesocket()
		closesocket(client_sock);
		printf("[TCP  ����] Ŭ���̾�Ʈ ���� : IP ���� = %s, ��Ʈ��ȣ = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}



	closesocket(listen_sock);

	WSACleanup();
	return 0;
}

