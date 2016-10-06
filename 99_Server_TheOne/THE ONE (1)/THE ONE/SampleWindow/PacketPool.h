#pragma once
#include "NetStd.h"

class User;

class PacketPool 
{
public:
	CRITICAL_SECTION			 m_cs;
	list<UPACKET>				 m_RecvPacketList;

public:
	void						AddPacket(UPACKET);
	void						PacketProcess();
	User*						FindUser(SOCKET);

public:
	virtual void				ProcessWork(UPACKET*);

public:
	PacketPool();
	virtual ~PacketPool();
};



