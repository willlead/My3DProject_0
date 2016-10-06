#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Network.h"

#define WM_SOCKET (WM_USER+1)

class AsyncSelect : public Singleton<AsyncSelect>, public Network
{
public:
	bool						NetConnect(int, char*);
	bool						NetAsyncSelect(HWND);
	bool						NetworkProc(WPARAM, LPARAM);
	bool						NetRecv();
	bool						NetSend(char*, int, int);

public:
	AsyncSelect();
	virtual ~AsyncSelect();
};

#define I_AsyncSelect AsyncSelect::GetInstance()

