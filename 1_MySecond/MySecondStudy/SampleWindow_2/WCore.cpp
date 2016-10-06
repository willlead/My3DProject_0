#include "WCore.h"

bool WCore::Init() { return true; };
bool WCore::Frame() { return true; };
bool WCore::Render() { return true; };
bool WCore::Release() { return true; };


bool WCore::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}

bool WCore::GameInit()
{
	Init();
	return true;
}

bool WCore::GameFrame()
{
	m_Timer.Frame();
	m_Input.Frame();
	Frame();
	return true;
}

bool WCore::GameRender()
{
	Render();
	return true;
}

bool WCore::GameRelease()
{
	Release();
	return false;
}

bool WCore::Run()
{
	GameInit();
	MSG msg;
	
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	
	return true;
}

WCore::WCore()
{
}


WCore::~WCore()
{
}