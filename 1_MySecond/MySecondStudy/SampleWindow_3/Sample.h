#pragma once
#include "WCore.h"	
#include "WTimer.h"
#include "WInput.h"
class Sample : public WCore
{
public:
	WInput m_Input;
	WTimer m_Timer;
public:

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:

	Sample();
	virtual ~Sample();
};

