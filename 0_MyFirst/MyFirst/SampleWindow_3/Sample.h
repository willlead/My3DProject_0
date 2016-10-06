#pragma once
#include "WCore.h"


class Sample : public WCore
{
public:
	
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

