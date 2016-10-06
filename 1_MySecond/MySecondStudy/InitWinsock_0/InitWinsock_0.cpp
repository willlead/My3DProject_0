// InitWinsock_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
		msg, // 멀티바이트
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
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	//socket()

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "TCP 소켓 생성 성공", "알림", MB_OK);

	//bind()	

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, // 목적지니까 주소 destination으로 함수로 설명해주고 있음
		sizeof(serveraddr)); // 목적지의 길이
							 // SOCKADDR_IN 구조체 데이터 설정
	serveraddr.sin_family = AF_INET;				// 인터넷
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// 서버( 호스트-> 네트워크)  
	//서버도 하나의 호스트라고 생각하자, 
	// 네트워크는 라우터, 공유기 등이다.
	serveraddr.sin_port = htons(SERVERPORT);		// 포트번호를 바이트 정렬 방식 빅 엔디언, 네트워크 바이트 정렬
	retval =
		bind(
			listen_sock,					// 소켓
			(SOCKADDR*)&serveraddr,		// ????
			sizeof(serveraddr)			// ??? 길이
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

		// 클라이언트가 접속 하면 주소와 포트번호를 출력한다.
		// 서버와 클라이언트의 포트번호가 같아야지 서버에 접속 가능하다. 10000
		printf("\n [TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트번호 = %d\n"
			,inet_ntoa(clientaddr.sin_addr), // inet_ntoa  출력할떄는 바이트로 바꾼것을 다시 숫자로 바꿔야함.
			ntohs(clientaddr.sin_port)); // ntohs
		
		// 클라이언트와 데이터 통신
		while (1)
		{
			// 데이터 받기
			retval = 
				recv(client_sock,
					buf, // 버퍼 주소
					BUFSIZE, // 리시브 버퍼의 사이즈 인것인가?
					0);
			if (retval == SOCKET_ERROR)
			{
				err_quit("recv()");
				break;
			}
			else if (retval == 0) break;


			// 받은 데이터 출력
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port),buf);

			//데이터 보내기
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}
		//closesocket()
		closesocket(client_sock);
		printf("[TCP  서버] 클라이언트 종료 : IP 조소 = %s, 포트번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}



	closesocket(listen_sock);

	WSACleanup();
	return 0;
}

