#include "WThread.h"

void WThread::CreateThread()
{
	if (m_bStarted == true) return;
	m_hThread = _beginthreadex(NULL, 0, HandleRunner,
		(LPVOID)this, 0, &m_iThreadID);
	m_bStarted = true;
}
UINT WINAPI WThread::HandleRunner(LPVOID parameter)
{
	WThread* pThread = (WThread*)parameter;
	pThread->Run();
	return 0;
}
bool WThread::Run()
{
	return true;
}


WThread::WThread() : m_bStarted(false)
{
}


WThread::~WThread()
{
	CloseHandle((HANDLE)m_hThread);
}
