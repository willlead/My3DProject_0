#include "AsyncSelect.h"
#include "Sample.h"

bool AsyncSelect::NetConnect(int iPort, char* szIP)
{
	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_port = htons(iPort);
	m_SockAddr.sin_addr.s_addr = inet_addr(szIP);
	int iRet = connect(m_Sock, (SOCKADDR*)&m_SockAddr, sizeof(m_SockAddr));
	if (iRet == SOCKET_ERROR)
	{
		return false;			
	}
	m_netConnect = true;
	return true;
}

bool AsyncSelect::NetAsyncSelect(HWND hWnd)
{
	int iRet = WSAAsyncSelect(m_Sock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool AsyncSelect::NetworkProc(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam) != 0)
	{
		return false;			// 서버 오류
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:
		{
			NetRecv();
		}break;
		case FD_CLOSE:
		{
			return false;
		}break;
	}
	return true;
}

bool AsyncSelect::NetRecv()
{
	if (m_iRecvByte < PACKET_HEADER_SIZE)
	{
		int iRet = recv(m_Sock, &m_RecvBuffer[m_iRecvByte], PACKET_HEADER_SIZE - m_iRecvByte, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			return false;			//SOCKET_ERROR -> WSAEWOULDBLOCK: m_RecvBuffer, m_iRecvByte를 멤버로 기억하고 있으므로 FD_READ 발생 시 재실행
									//SOCKET_ERROR -> !WSAEWOULDBLOCK, 0: NetworkProc에서 서버 오류 확인 -> 접속 종료
		}
		m_iRecvByte += iRet;
	}
	UPACKET* pPacket = (UPACKET*)m_RecvBuffer;
	while (m_iRecvByte < pPacket->ph.len)
	{
		int iRet = recv(m_Sock, &m_RecvBuffer[m_iRecvByte], pPacket->ph.len - m_iRecvByte, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			return false;		//SOCKET_ERROR -> WSAEWOULDBLOCK: m_RecvBuffer, m_iRecvByte를 멤버로 기억하고 있으므로 FD_READ 발생 시 재실행
								//SOCKET_ERROR -> !WSAEWOULDBLOCK, 0: NetworkProc에서 서버 오류 확인 -> 접속 종료
		}
		m_iRecvByte += iRet;
	}
	pPacket = (UPACKET*)m_RecvBuffer;
	I_Sample.m_PacketPool.AddPacket(*pPacket);
	m_iRecvByte = 0;
	memset(m_RecvBuffer, 0, sizeof(m_RecvBuffer));
	return true;
}

bool AsyncSelect::NetSend(char* msg, int type, int iLen)
{
	UPACKET packet;
 	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	memcpy(packet.msg, msg, iLen);

	int iSendByte = send(m_Sock, (char*)&packet, packet.ph.len, 0);
	if (iSendByte == SOCKET_ERROR)
	{
		return false;			//SOCKET_ERROR -> WSAEWOULDBLOCK:진행 중인 상태이므로 시간이 지나면 처리됨
								//SOCKET_ERROR -> !WSAEWOULDBLOCK: NetworkProc에서 서버 오류 확인 -> 접속 종료
	}							
	TCHAR StrBuffer[MAX_PATH] = { 0, };
	_stprintf_s(StrBuffer, _T("%x\n"), type);
	OutputDebugString(StrBuffer);
	return true;
}

AsyncSelect::AsyncSelect()
{
}

AsyncSelect::~AsyncSelect()
{
}
