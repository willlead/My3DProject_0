#include "StreamPacket.h"
#include "Select.h"

void StreamPacket::Put(char* Buffer, int iRecvByte)
{
	char TmpBuffer[MAX_BUFFER_SIZE];
	
	if (m_iWritePos + iRecvByte > MAX_BUFFER_SIZE)
	{
		ZeroMemory(TmpBuffer, MAX_BUFFER_SIZE);
		memcpy(TmpBuffer, &m_RecvBuffer[m_iStartPos], m_iReadByte);
		ZeroMemory(m_RecvBuffer, MAX_BUFFER_SIZE);
		memcpy(m_RecvBuffer, TmpBuffer, m_iReadByte);
		m_iStartPos = 0;
		m_iWritePos = m_iReadByte;
	}
	memcpy(&m_RecvBuffer[m_iWritePos], Buffer, iRecvByte);
	m_iWritePos += iRecvByte;
	m_iReadByte += iRecvByte;

	if (m_iReadByte >= PACKET_HEADER_SIZE)
	{
		UPACKET packet;
		ZeroMemory(&packet, sizeof(packet));
		memcpy(&packet, &Buffer[m_iStartPos], PACKET_HEADER_SIZE);
		while (m_iReadByte >= packet.ph.len)
		{
			int iLen = packet.ph.len;
			memcpy(&packet, &Buffer[m_iStartPos], iLen);
			m_iStartPos += iLen;
			m_iReadByte -= iLen;

			I_Select.m_PacketPool.AddPacket(packet);
			
			if (m_iReadByte < PACKET_HEADER_SIZE)
			{
				if (m_iReadByte == 0)
				{
					m_iStartPos = 0;
				}
				break;
			}
			ZeroMemory(&packet, sizeof(packet));
			memcpy(&packet, &Buffer[m_iStartPos], PACKET_HEADER_SIZE);
		}
	}
}

StreamPacket::StreamPacket()
{
	m_iWritePos = 0;
	m_iStartPos = 0;
	m_iReadByte = 0;
	ZeroMemory(m_RecvBuffer, MAX_BUFFER_SIZE);
}


StreamPacket::~StreamPacket()
{
}
