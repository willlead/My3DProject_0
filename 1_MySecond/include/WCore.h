#pragma once
#include "WWindow.h"
#include "WTimer.h"
#include "WInput.h"
class WCore : public WWindow 
{
public:
	HDC			m_hScreenDC;  // 제 1 DC => 결과
							  // 제 2 DC=> 과정
	HDC			m_hOffScreenDC;
	// 제 2 DC의 비트맵
	HBITMAP		m_hOffScreenBitmap;
public:
	WTimer     m_Timer;
	WInput     m_Input;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool     PreRender();// 드로우
	virtual bool     Render();// 드로우
	virtual bool     PostRender();// 드로우
	virtual bool     Release();// 삭제, 소멸
	virtual void     DebugString();
	virtual void     MsgEvent(MSG msg);
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

