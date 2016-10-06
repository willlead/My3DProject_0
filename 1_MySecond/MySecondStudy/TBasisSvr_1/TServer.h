#pragma once
#include "TServerObj.h"
#include "TProtocol.h"
#include "TStreamPacket.h"
#include "TAcceptor.h"
#include "TDebugString.h"
// � �޼������� �����ؾ� �Ѵ�. ( �ʿ伺 )
// �������� ����
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
	int				m_iEvent; // �̺�Ʈ �迭�� �ε���
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
		// ��Ŷó��
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
	int			m_iEventCount;// �̺�Ʈ �迭�� ����
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