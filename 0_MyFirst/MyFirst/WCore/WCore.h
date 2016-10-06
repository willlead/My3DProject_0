#pragma once
#include "WWindow.h"
#include "WTimer.h"
#include "WInput.h"

class WCore : public WWindow
{
public:
	WTimer     m_Timer;
	WInput     m_Input;
public:
	virtual bool     Init(); // 초기화
	virtual bool     Frame();// 계산
	virtual bool     Render();// 드로우
	virtual bool     Release();// 삭제, 소멸
	virtual void MsgEvent(MSG msg);
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

