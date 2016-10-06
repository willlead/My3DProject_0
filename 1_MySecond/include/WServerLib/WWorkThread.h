#pragma once
#include "WThread.h"
class WWorkThread : public WThread
{
public:
	bool bLoop;
public:
	bool Run();
public:
	WWorkThread();
	virtual ~WWorkThread();
};

