#pragma once
#include "TBasisStd.h"
#include <process.h>
#include "TProtocol.h"
class TStreamPacket
{
public:
	std::list<UPACKET>						m_PacketList;
	typedef std::list<UPACKET>::iterator	m_PackItor;
public:
	char		m_strBuffer[4096];// Recv����+ 1���� ��Ŷ ũ��
	int			m_iWritePos;
	P_UPACKET	m_pPacketData;
	int			m_iRecvPos;

	UPACKET		m_PacketData;
	int		m_iDataLength;
	PACKET_HEADER m_Header;
	void	Put(char* strBuffer, int iRet);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};

