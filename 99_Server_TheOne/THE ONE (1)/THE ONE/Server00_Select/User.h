#pragma once
#include "StreamPacket.h"
#include "Bullet.h"

class User
{
public:
	char						m_name[10];
	bool						m_connect;
	SOCKET						m_sock;
	SOCKADDR_IN					m_addr;
	float						m_fX;
	float						m_fY;
	int							m_iDirection;

public:
	//list<Bullet*>				m_BulletList;
	int							m_BulletCreate;
	int							m_iBulletNum;
	float						m_fBulletfX;
	float						m_fBulletfY;

public:
	list<UPACKET>				m_SendpacketList;
	
public:
	User();
	virtual ~User();
};

