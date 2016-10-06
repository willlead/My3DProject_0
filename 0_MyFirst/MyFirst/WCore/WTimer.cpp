#include "WTimer.h"
float g_fSecondPerFrame = 0.f;

bool WTimer::Init()
{
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_dwFrameCounter = 0;
	m_fAccumulation = 0.0f;
	m_fFrameTime = 0.0f;

	return true;
}

bool WTimer::Frame()
{
	DWORD dwCurrentTick = timeGetTime();

	// ���� ƽ���� ����ƽ�� ����  ���� ���°� ���
	DWORD dwElapseTick = dwCurrentTick - m_dwBeforeTick;

	//ƽ��  1000���� ������ 1�ʰ� �ȴ�.
	m_fSecondPerFrame = dwElapseTick / 1000.0f;
	g_fSecondPerFrame = m_fSecondPerFrame;
	m_fAccumulation += m_fSecondPerFrame;
	m_fFrameTime += m_fSecondPerFrame;

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
	fTime += g_fSecondPerFrame;
	if (fTime >= 1.0f)
	{
		_stprintf_s(m_cs_Buffer,
			L"FPS=[%d] %10.f % 10.4f",
			m_iFPS, g_fSecondPerFrame, m_fAccumulation);
		OutputDebugString(m_cs_Buffer);
		fTime = 0.0f;
	}
#endif
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
