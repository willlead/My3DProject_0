#include "Select.h"
#include "Game.h"

volatile long		g_lSendByte = 0;
volatile long		g_lRecvByte = 0;
volatile long		g_lSendPacket = 0;
volatile long		g_lRecvPacket = 0;
volatile long		g_lUserCnt = 0;

void Select::Access()
{
	User* pUser = new User;
	pUser->m_connect = true;
	pUser->m_sock = m_clientsock;
	pUser->m_addr = m_clientsa;
	
	m_UserList.push_back(pUser);
	InterlockedIncrement(&g_lUserCnt);

	printf("클라이언트 접속 : IP: %s, PORT: %d\n", inet_ntoa(m_clientsa.sin_addr), ntohs(m_clientsa.sin_port));

	if (m_UserList.size() == 2)
	{
		I_Game.Init();
	}

	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.type = PACKET_GAME_JOIN;
	UPACKET_GAME_JOIN user;
	user.sock = m_clientsock;
	user.UserNum = m_iUserCnt++;
	memcpy(packet.msg, (char*)&user, sizeof(UPACKET_GAME_JOIN));
	packet.ph.len = PACKET_HEADER_SIZE + sizeof(UPACKET_GAME_JOIN);
	BroadCast(&packet);
	
	list<User*>::iterator iter;
	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		User* pUser0 = (*iter);
		if (pUser0->m_sock == m_clientsock)
		{
			break;
		}
		UPACKET packet;
		ZeroMemory(&packet, sizeof(packet));
		packet.ph.type = PACKET_GAME_JOIN;
		UPACKET_GAME_JOIN user;
		user.sock = pUser0->m_sock;
		user.UserNum = m_iUserCnt - 2;
		memcpy(packet.msg, (char*)&user, sizeof(UPACKET_GAME_JOIN));
		packet.ph.len = PACKET_HEADER_SIZE + sizeof(UPACKET_GAME_JOIN);
		pUser->m_SendpacketList.push_back(packet);
	}
}

int Select::SendMsg(SOCKET sock, UPACKET* pPacket)
{
	int i = sizeof(UPACKET_ENEMY_CREATE);
	int iSendByte = send(sock, (char*)pPacket, pPacket->ph.len, 0);
	if (iSendByte == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 1;
	}
	InterlockedIncrement(&g_lSendPacket);
	InterlockedExchangeAdd(&g_lSendByte, iSendByte);
	
	return 0;
}

void Select::BroadCast(UPACKET* pPacket)
{
	list<User*>::iterator iter;
	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		User* pUser = (*iter);
		if (pUser->m_connect == false)
		{
			continue;
		}
		pUser->m_SendpacketList.push_back(*pPacket);	
		int i = 0;
	}
}

void Select::Erase()
{
	list<User*>::iterator iter = m_UserList.begin();
	list<User*>::iterator deliter;
	while (iter != m_UserList.end())
	{
		User* pUser = (*iter);
		if (pUser->m_connect == false)
		{
			printf("클라이언트 접속 종료 : IP: %s, PORT: %d\n", inet_ntoa(pUser->m_addr.sin_addr), ntohs(pUser->m_addr.sin_port));
			InterlockedExchangeAdd(&g_lUserCnt, -1);

			SOCKET sock = pUser->m_sock;
			closesocket(pUser->m_sock);
			delete pUser;
			deliter = iter++;
			m_UserList.erase(deliter);

			if (m_UserList.size() == 0)
			{
				m_iUserCnt = 1;
			}

			UPACKET packet;
			ZeroMemory(&packet, sizeof(packet));
			packet.ph.type = PACKET_USER_DROP;
			memcpy(packet.msg, (char*)&sock, sizeof(packet.msg));
			packet.ph.len = PACKET_HEADER_SIZE + strlen(packet.msg);
			BroadCast(&packet);
			break;
		}
		else
		{
			iter++;
		}
	}
}

//void Select::UserDrop(SOCKET sock)
//{
//	list<User*>::iterator iter = m_UserList.begin();
//	list<User*>::iterator deliter;
//	while (iter != m_UserList.end())
//	{
//		User* pUser = (*iter);
//		if (pUser->m_sock == sock)
//		{
//			printf("클라이언트 접속 종료 : IP: %s, PORT: %d\n", inet_ntoa(pUser->m_addr.sin_addr), ntohs(pUser->m_addr.sin_port));
//			InterlockedExchangeAdd(&g_lUserCnt, -1);
//			closesocket(pUser->m_sock);
//			delete pUser;
//			deliter = iter++;
//			m_UserList.erase(deliter);
//		}
//	}
//}

DWORD WINAPI CalculateThread(LPVOID pData)
{
	while (1)
	{
		system("cls");
		printf("Current Byte Per Second sent : %lu [%lu]\n", g_lSendByte / 5, g_lSendByte);
		printf("Current Byte Per Second read : %lu [%lu]\n", g_lRecvByte / 5, g_lRecvByte);
		printf("Current Packet Per Second sent : %lu [%lu]\n", g_lSendPacket / 5, g_lSendPacket);
		printf("Current Packet Per Second read : %lu [%lu]\n", g_lRecvPacket / 5, g_lRecvPacket);
		printf("Current Connections : %lu\n", g_lUserCnt);
		InterlockedExchange(&g_lSendByte, 0);
		InterlockedExchange(&g_lRecvByte, 0);
		InterlockedExchange(&g_lSendPacket, 0);
		InterlockedExchange(&g_lRecvPacket, 0);
		Sleep(5000);
	}
	return 0;
}

bool Select::Set(int port)
{
	ZeroMemory(&m_sa, sizeof(m_sa));
	m_sa.sin_port = htons(port);
	return true;
}

bool Select::Init()
{
	if (Network::Init() == true)
	{
		m_listensock = Network::m_Sock;
		m_sa.sin_family = AF_INET;
		m_sa.sin_addr.s_addr = htonl(INADDR_ANY);

		int iBind = bind(m_listensock, (SOCKADDR*)&m_sa, sizeof(m_sa));
		if (iBind == SOCKET_ERROR)
		{
			DWORD Ret = GetLastError();
			return false;
		}

		int iListen = listen(m_listensock, SOMAXCONN);
		if (iListen == SOCKET_ERROR)
		{
			return false;
		}
		u_long on = 1;
		int iRet = ioctlsocket(m_listensock, FIONBIO, &on);
		if (iRet == SOCKET_ERROR)
		{
			return false;
		}
	}

	DWORD dwCalculateThreadID;
	CloseHandle(CreateThread(0, 0, CalculateThread, 0, 0, &dwCalculateThreadID));

	return true;
}

bool Select::Run()
{
	m_isalen = sizeof(m_clientsa);
	while (1)
	{
		FD_ZERO(&m_rSet);
		FD_ZERO(&m_wSet);

		FD_SET(m_listensock, &m_rSet);
		list<User*>::iterator iter;
		for (iter = m_UserList.begin();iter != m_UserList.end();iter++)
		{
			User* pUser = (*iter);
			FD_SET(pUser->m_sock, &m_rSet);
			if (pUser->m_SendpacketList.size() > 0)
			{
				FD_SET(pUser->m_sock, &m_wSet);
			}
		}

		timeval time;
		time.tv_sec = 0;
		time.tv_usec = 1;
		int iRet = select(0, &m_rSet, &m_wSet, NULL, &time);
		if (iRet == INVALID_SOCKET)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			//INVALID_SOCKET -> WSAEWOULDBLOCK:진행 중인 상태이므로 시간이 지나면 처리됨.
		}
		if (iRet > 0)
		{
			if (FD_ISSET(m_listensock, &m_rSet))
			{
				m_clientsock = accept(m_listensock, (SOCKADDR*)&m_clientsa, &m_isalen);
				if (m_clientsock == INVALID_SOCKET)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						return false;
					}
					//INVALID_SOCKET -> WSAEWOULDBLOCK:while문에서 accept 재호출
				}
				else 
				{
					if (I_Game.m_bStart == true)
					{
						InterlockedIncrement(&g_lUserCnt);
						printf("클라이언트 접속 : IP: %s, PORT: %d\n", inet_ntoa(m_clientsa.sin_addr), ntohs(m_clientsa.sin_port));

						UPACKET packet;
						ZeroMemory(&packet, sizeof(packet));
						packet.ph.type = PACKET_GAME_STARTED;
						packet.ph.len = PACKET_HEADER_SIZE;
						SendMsg(m_clientsock, &packet);

						printf("클라이언트 접속 종료 : IP: %s, PORT: %d\n", inet_ntoa(m_clientsa.sin_addr), ntohs(m_clientsa.sin_port));
						InterlockedExchangeAdd(&g_lUserCnt, -1);
						closesocket(m_clientsock);
					}
					else
					{
						Access();
					}
				}
			}
			for (iter = m_UserList.begin();iter != m_UserList.end();iter++)
			{
				User* pUser = (*iter);
				if (pUser->m_connect == false)
				{
					continue;
				}
				
				if (FD_ISSET(pUser->m_sock, &m_rSet))
				{					
					char Buffer[MAX_BUFFER_SIZE] = { 0, };
					int iRecvByte = 0;
					iRecvByte = recv(pUser->m_sock, Buffer, MAX_BUFFER_SIZE, 0);
					if (iRecvByte == SOCKET_ERROR)
					{
						if(WSAGetLastError() != WSAEWOULDBLOCK)
						{
							pUser->m_connect = false;
						}					
						//SOCKET_ERROR -> WSAEWOULDBLOCK:while문에서 recv 재호출
					}
					else if (iRecvByte == 0)
					{
						pUser->m_connect = false;
					}
					else 
					{
						m_RecvStreamPacket.Put(Buffer, iRecvByte);
						m_PacketPool.PacketProcess();
						InterlockedIncrement(&g_lRecvPacket);
						InterlockedExchangeAdd(&g_lRecvByte, iRecvByte);
					}
				}
				if (FD_ISSET(pUser->m_sock, &m_wSet))
				{
					list<UPACKET>::iterator iter = pUser->m_SendpacketList.begin();
					list<UPACKET>::iterator deliter;
					while(iter != pUser->m_SendpacketList.end())
					{
						UPACKET packet = (*iter);
						int iRet = SendMsg(pUser->m_sock, &packet);
						if (iRet == -1)
						{
							pUser->m_connect = false;
							return false;
						}
						else if (iRet == 1)			//SOCKET_ERROR -> WSAEWOULDBLOCK:while문에서 SendMsg 재호출 처리
						{
							iter++;
							continue;		
						}
						else if (iRet == 0)
						{
							deliter = iter++;
							pUser->m_SendpacketList.erase(deliter);
						}
					}
				}
			}
			Erase();
		}
	}
	return true;
}

bool Select::Release()
{
	Network::Release();
	return true;
}

Select::Select()
{
	m_iUserCnt = 1;
}


Select::~Select()
{
}
