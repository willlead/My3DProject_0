#pragma once
#include "PacketPool.h"

class ServerPacketPool : public PacketPool, public Singleton<ServerPacketPool>
{
public:
	ServerPacketPool();
	virtual ~ServerPacketPool();
};

#define I_ServerPacketPool ServerPacketPool::GetInstance()

