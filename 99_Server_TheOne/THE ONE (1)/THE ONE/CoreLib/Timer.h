#pragma once
#include "Std.h"

class Timer : public Singleton<Timer>
{
public:
	int			m_iFPS;
	float		m_fSecondPerFrame;
	float		m_fAccumulation;

public:		// 임시변수
	DWORD		m_dwBeforeTick;
	DWORD		m_dwFrameCounter;
	float		m_fFrameTime;
	TCHAR		m_szBuffer[MAX_PATH];		

public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

public:
	Timer();
	virtual ~Timer();
};

#define I_Timer Timer::GetInstance()