#pragma once
#include "PacketPool.h"

class StreamPacket
{
public:
	int			m_iWritePos;
	int			m_iStartPos;
	int			m_iReadByte;
	char		m_RecvBuffer[MAX_BUFFER_SIZE];

public:
	void		Put(char*, int);

public:
	StreamPacket();
	virtual ~StreamPacket();
};

