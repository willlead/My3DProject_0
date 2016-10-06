#pragma once
#include "WWindow.h"
#include "WTimer.h"
#include "WWindow.h"
class WCore : public WWindow
{
public:
	WTImer m_Timer;
	WInput m_Input;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	bool Run();
	bool GameRun();
	bool GameInit();
	bool GameFrame();
	bool GameRender();
	bool GameRelease();

public:
	WCore();
	virtual ~WCore();
};

