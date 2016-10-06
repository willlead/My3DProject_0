#include "WTimer.h"
float g_fSecondPerFrame = 0.0f;

bool WTimer::Init()
{
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;	
	m_dwFrameCounter = 0;
	m_fAccumulation = 0.0f;
	m_fFrameTime = 0.0f;
	m_dwBeforeTick = timeGetTime();	
	return true;
}

bool WTimer::Frame()
{
	DWORD dwCurrentTick = timeGetTime();
	DWORD dwElapseTick = dwCurrentTick - m_dwBeforeTick;
	m_fSecondPerFrame = dwElapseTick / 1000.0f;
	g_fSecondPerFrame = m_fSecondPerFrame;
	m_fAccumulation += m_fSecondPerFrame;
	m_fFrameTime += m_fSecondPerFrame;
	

	//FPS
	if (m_fFrameTime >= 1.0f)
	{
		m_iFPS = m_dwFrameCounter;
		m_fFrameTime -= 1.0f;
		m_dwFrameCounter = 0;
	}
	m_dwFrameCounter++;
	m_dwBeforeTick = dwCurrentTick;
	return true;
}

bool WTimer::Render()
{
#ifdef _DEBUG
	static float fTime = 0.0f;
	fTime += m_fSecondPerFrame;
	if (fTime >= 1.0f)
	{
		_stprintf_s(m_csBuffer, L"FPS[%d] %10.4f %10.4f\n",
			m_iFPS, m_fSecondPerFrame, m_fAccumulation);
		//OutputDebugString(m_csBuffer);
		fTime = 0.0f;
	}
#endif // _DEBUG

	return true;
}

bool WTimer::Release()
{
	return true;
}

WTimer::WTimer()
{
}


WTimer::~WTimer()
{
}
