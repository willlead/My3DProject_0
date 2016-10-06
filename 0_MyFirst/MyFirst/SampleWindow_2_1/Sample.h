#pragma once
#include "WCore.h"
#include "WTimer.h"
#include "WInput.h"

class Sample : public WCore
{
public:
	WTimer   m_Timer;
	WInput	m_Input;
public:
	void MsgEvent(MSG msg);
	bool	 Init();
	bool	 Frame();
	bool	 Render();
	bool	 Release();
public:
	Sample();
	virtual ~Sample();
};

