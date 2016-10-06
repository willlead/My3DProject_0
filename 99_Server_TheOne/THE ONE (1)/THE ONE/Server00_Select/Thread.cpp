#include "Thread.h"

unsigned int WINAPI WorkerThread(LPVOID param)
{
	Thread* pThread = (Thread*)param;
	if (pThread != 0)
	{
		pThread->Run();
	}
	return true;
}

void Thread::Create()
{
	if (m_bStart == true)
	{
		return;
	}
	m_hThread =(HANDLE)::_beginthreadex(0, 0, WorkerThread, this, 0, &m_ThreadID);
	m_bStart = true;
}

bool Thread::Run()
{
	return true;
}

Thread::Thread()
{
	m_bStart = false;
}


Thread::~Thread()
{
	::CloseHandle(m_hThread);
}
