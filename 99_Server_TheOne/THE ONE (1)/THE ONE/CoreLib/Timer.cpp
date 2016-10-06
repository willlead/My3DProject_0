#include "Timer.h"
#include "Sys.h"

bool Timer::Init()
{
	m_iFPS = 0;
	m_fSecondPerFrame = 0.0f;
	m_fAccumulation = 0;
	m_dwBeforeTick = timeGetTime();
	m_dwFrameCounter = 0;
	m_fFrameTime = 0.0f;
	return true;
}

bool Timer::Frame()
{
	DWORD dwCurrentTick = timeGetTime();
	DWORD dwElapseTick = dwCurrentTick - m_dwBeforeTick;
	m_fSecondPerFrame = dwElapseTick / 1000.0;
	m_fAccumulation += m_fSecondPerFrame;
	m_fFrameTime += m_fSecondPerFrame;

	if (m_fFrameTime >= 1.0)
	{
		m_iFPS = m_dwFrameCounter;
		m_fFrameTime -= 1.0;
		m_dwFrameCounter = 0;
	}

	m_dwFrameCounter++;
	m_dwBeforeTick = dwCurrentTick;
	return true;
}

bool Timer::Render()
{
#ifdef _DEBUG
	static float fTime = 0.0f;
	fTime += m_fSecondPerFrame;
	if (fTime >= 1.0f)
	{
		_stprintf_s(m_szBuffer, _T("FPS=[%d] %10.4f %10.4f"), m_iFPS, m_fSecondPerFrame, m_fAccumulation);
		fTime = 0.0;
	}
#endif
	return true;
}

bool Timer::Release()
{
	return true;
}


Timer::Timer()
{
}


Timer::~Timer()
{
}
