#pragma once
#include "TWindow.h"
#include "TTimer.h"
#include "TInput.h"
class TCore : public TWindow
{
public:
	TTimer		m_Timer;
	TInput		m_Input;

	HFONT		m_fontDefault;
	HFONT		m_fontDefaultOld;
public:
	bool	GameRun();
	bool	GameInit();
	bool	GameFrame();
	virtual bool	GameRender();
	bool    GameRelease();
	void	MsgEvent(MSG msg);


public:
	////////// 가상 함수들 ///////////
	virtual bool		Init() { return true; }
	virtual bool		PreFrame() { return true; }
	virtual bool		PostFrame() { return true; }
	virtual bool		Frame() { return true; }
	virtual bool		Render() { return true; }
	virtual bool		Release() { return true; };
public:
	TCore();
	virtual ~TCore();
};

