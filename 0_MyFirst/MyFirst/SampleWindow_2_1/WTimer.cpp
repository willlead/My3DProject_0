#include "WTimer.h"

bool     WTimer::Init()
{
	m_fSecondPerFrame = 0.0f;
	m_fAccumulation = 0.0f;
	m_fFrameTime = 0.0f;
	m_dwFramecouter = 0;
	m_iFPS = 0;
	dwBeforeCurrentTIck = timeGetTime();
	return true;
}; // 초기화
bool     WTimer::Frame() {
	//timeGetTime() (winmm.lib)	
	// 외부기호는 라이브러리가 없다. cpp가 없다.
	//(winmm.lib) 링커 입력에 추가 필요
	DWORD dwCurrentTIck = timeGetTime();
	DWORD dwElapseTick = dwCurrentTIck - dwBeforeCurrentTIck;

	m_fSecondPerFrame = dwElapseTick / 1000.0f;	//틱은 1000 이 1초
	m_fAccumulation += m_fSecondPerFrame;
	m_fFrameTime += m_fSecondPerFrame;
	
	//FPS 계산
	if (m_fFrameTime >= 1.0f)
	{// 1초가 지났다면??
		m_iFPS = m_dwFramecouter;
		m_fFrameTime -= 1.0f;
	}
	m_dwFramecouter++;


	return true;
};
bool     WTimer::Render() {
#ifdef _DEBUG
	//DWORD dwTick = m_fAccumulation;
	static float fTime = 0.0f;
	fTime += m_fSecondPerFrame;
	if (fTime >= 1.0f)
	{
		_stprintf_s(m_csBuffer, L" FPS:[%d] %10.4f Timer:[%10.4f]\n",
			m_iFPS, m_fSecondPerFrame, m_fAccumulation);
		OutputDebugString(m_csBuffer);
		fTime = 0.0f;
	}
	
	#endif
	

	return true;
};
bool     WTimer::Release() {
	return true;
};

WTimer::WTimer()
{
}


WTimer::~WTimer()
{
}
