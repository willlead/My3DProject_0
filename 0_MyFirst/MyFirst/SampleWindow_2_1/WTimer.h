#pragma once
#include "WBase.h"
class WTimer : public WBase
{
public:
	TCHAR		m_csBuffer[MAX_PATH];
	int m_iFPS; // 초당 프레임 개수
	float m_fSecondPerFrame;
	float m_fAccumulation;
	float m_fFrameTime;
	DWORD dwBeforeCurrentTIck;
	int m_dwFramecouter;

public:
	bool     Init(); // 초기화
	bool     Frame();// 계산
	bool     Render();// 드로우
	bool     Release();// 삭제, 소멸
public:
	WTimer();
	virtual ~WTimer();
};

