#pragma once
#include "WCore.h"
#include "TObject.h"
#include "TClient.h"
#include "TGameUser.h"
#include "TUdpSocket.h"

class Sample :public WCore
{
public:
	TUdpSocket m_Udp;
	std::vector<TGameUser> m_UserList;
	TClient m_Client;
	bool m_bLogin;
	int m_iSerIndex;
	POINT m_CursorPos;
public:
	bool				Init();
	bool				PreRender();
	bool				Render();
	bool				PostRender();
	bool				Frame();
	bool				Release();
public:
	Sample();
	virtual ~Sample();
};

