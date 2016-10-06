#pragma once
#include "Protocol.h"

class Network
{
public:
	SOCKET				m_Sock;
	SOCKADDR_IN			m_SockAddr;
	bool				m_netConnect;

public:
	char				m_RecvBuffer[MAX_PATH];
	int					m_iRecvByte;
	list<UPACKET>		m_RecvList;
	
public:
	bool				Init();
	bool				Release();

public:
	Network();
	virtual ~Network();
};

