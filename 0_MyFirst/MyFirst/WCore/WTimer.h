#pragma once
#include "WBase.h"

class WTimer :public WBase
{
public:
	int m_iFPS;
	float m_fSecondPerFrame;
	float m_fAccumulation;
public:
	TCHAR m_cs_Buffer[MAX_PATH];
	DWORD m_dwBeforeTick;
	float m_fFrameTime;
	DWORD m_dwFrameCounter;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	WTimer();
	virtual ~WTimer();
};

