#pragma once
#include "User.h"
#include "Network.h"

class Select : public Network, public Singleton<Select>
{
public:
	SOCKET				m_listensock;
	SOCKADDR_IN			m_sa;
	SOCKET				m_clientsock;
	SOCKADDR_IN			m_clientsa;
	int					m_isalen;
	list<User*>			m_UserList;
	FD_SET				m_rSet;
	FD_SET				m_wSet;
	int					m_iUserCnt;

public:
	PacketPool			m_PacketPool;
	StreamPacket		m_RecvStreamPacket;

public:
	void				Access();
	int					SendMsg(SOCKET, UPACKET*);
	//void				BroadCast(SOCKET, UPACKET*);
	void				BroadCast(UPACKET*);
	void				Erase();
	//void				UserDrop(SOCKET);

public:
	bool				Set(int);
	bool				Init();
	bool				Run();
	bool				Release();

public:
	Select();
	virtual ~Select();
};

#define I_Select Select::GetInstance()


