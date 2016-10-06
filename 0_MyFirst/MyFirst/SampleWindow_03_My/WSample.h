#pragma once
#include "WCore.h"
#include "WTimer.h"

class Sample : public WCore
{
public:
	WTimer m_Timer;
public:
	bool	 Init();
	bool	 Frame();
	bool	 Render();
	bool	 Release();
public:
	Sample();
	virtual ~Sample();
};

