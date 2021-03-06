#pragma once
#include "TServerObj.h"
#include "TProtocol.h"
#include "TStreamPacket.h"
#include "TAcceptor.h"
#include "TDebugString.h"
// 어떤 메세지인지 구분해야 한다. ( 필요성 )
// 프로토콜 생성
typedef struct _OVERLAPPED2 : OVERLAPPED
{
	enum { MODE_RECV = 0, MODE_SEND = 1 };
	int		m_iFlags;
	_OVERLAPPED2()
	{
		m_iFlags = MODE_RECV;
	}

}OVERLAPPED2;

struct TUser
{
	OVERLAPPED2		m_ov;
	int				m_iEvent; // 이벤트 배열의 인덱스
	string			m_Name;
	SOCKET			m_Socket;
	WSABUF			m_wsaBuffer;
	char			m_strBuffer[2048];
	TStreamPacket	m_StreamPacket;
	bool			m_bUsed;
	void			Dispatch(DWORD deByteSize,
		LPOVERLAPPED ov)
	{
		OVERLAPPED2* pOV = (OVERLAPPED2*)ov;
		if (deByteSize == 0) return;
		// send
		// recv
		// 패킷처리
		if (pOV->m_iFlags == OVERLAPPED2::MODE_RECV)
		{
			m_StreamPacket.Put(
				m_wsaBuffer.buf,
				deByteSize,
				this);
		}
		WaitForpacketReceive();
	}
	void WaitForpacketReceive()
	{
		int iRet = 0;
		DWORD cbTransferred, flags = 0;
		m_ov.m_iFlags = OVERLAPPED2::MODE_RECV;
		m_wsaBuffer.buf = m_strBuffer;
		m_wsaBuffer.len = 2048;
		iRet = WSARecv(m_Socket,
			&(m_wsaBuffer), 1,
			&cbTransferred, &flags,
			(LPOVERLAPPED)&m_ov, NULL);
		if (iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				I_DebugStr.T_ERROR();
			}
		}
	}
};

class TServer : public TSingleton<TServer>//, public TThread
{
private:
	friend class TSingleton<TServer>;
public:
	TStreamPacket		m_StreamPacket;
	std::list<TUser*>	m_UserList;
	typedef std::list<TUser*>::iterator	m_ListItor;
	int			m_iClientNumber;
	HANDLE		m_Mutex;
	int			m_iEventCount;// 이벤트 배열의 개수
	WSABUF		m_wsaBuffer;
public:
	TAcceptor	m_Acceptor;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	void		ProcessPacket();
public:
	TUser*		GetUser(int iIndex);
	bool		DelUser(int iIndex);
public:
	TServer(void);
	~TServer(void);
};
#define I_Server TServer::GetInstance()