#pragma once
#include "WServerObj.h"
#include "WWorkThread.h"

#define WORKER_THREAD_COUNT 2
class WServerIOCP : public WSingleton<WServerObj>, public WServerObj
{
private:
	friend class WSingleton<WServerIOCP>;
public:
	HANDLE m_hIOCP;
	WWorkThread m_WorkerThread[WORKER_THREAD_COUNT];
public:
	bool Init();
	bool Run();
	bool Release();
	void AddhandleToIOCP(HANDLE hClientSocket, DWORD dwReturnKey);

public:
	WServerIOCP();
	virtual ~WServerIOCP();
};
#define I_ServerIOCP WServerIOCP::GetInstance()
