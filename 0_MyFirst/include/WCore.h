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
	virtual bool     Init(); // �ʱ�ȭ
	virtual bool     Frame();// ���
	virtual bool     Render();// ��ο�
	virtual bool     Release();// ����, �Ҹ�
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

