#include "WServerIOCP.h"


bool WServerIOCP::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE, 0, 0,
		WORKER_THREAD_COUNT);

	for (int iThread = 0; iThread < WORKER_THREAD_COUNT; iThread++)
	{
		m_WorkerThread[iThread].CreateThread();
	}
	return true;
}
bool WServerIOCP::Run()
{
	return true;
}
bool WServerIOCP::Release()
{
	return true;
}
void WServerIOCP::AddhandleToIOCP(HANDLE hClientSocket, DWORD dwReturnKey)
{
	::CreateIoCompletionPort(
		hClientSocket, m_hIOCP,
		dwReturnKey, 0);
		
	
}


WServerIOCP::WServerIOCP()
{
	m_hIOCP = NULL;
}


WServerIOCP::~WServerIOCP()
{
}
