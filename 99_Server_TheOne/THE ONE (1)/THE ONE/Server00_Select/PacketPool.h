#pragma once
#include "NetStd.h"

class PacketPool 
{
public:
	list<UPACKET>				 m_RecvPacketList;

public:
	void						AddPacket(UPACKET);
	void						PacketProcess();

public:
	virtual void				ProcessWork(UPACKET*);

public:
	PacketPool();
	virtual ~PacketPool();
};



