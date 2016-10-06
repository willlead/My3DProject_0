#include "Network.h"

bool Network::Init()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	return true;
}

bool Network::Release()
{
	closesocket(m_Sock);
	WSACleanup();
	return true;
}

Network::Network()
{
	ZeroMemory(&m_SockAddr, sizeof(m_SockAddr));
	ZeroMemory(m_RecvBuffer, MAX_PATH);
	m_netConnect = false;
	m_iRecvByte = 0;
}

Network::~Network()
{
}
