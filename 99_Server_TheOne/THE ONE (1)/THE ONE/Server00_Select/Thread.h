#pragma once
#include <windows.h>
#include <process.h>

class Thread
{
public:
	HANDLE					m_hThread;
	unsigned int			m_ThreadID;
	bool					m_bStart;

public:
	void					Create();

public:
	virtual bool			Run();

public:
	Thread();
	virtual ~Thread();
};

