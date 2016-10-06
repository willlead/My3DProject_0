#pragma once
#include "Window.h"
#include "Timer.h"
#include "Input.h"

class Core : public Window
{
public:
	HBITMAP					m_hOffScreenBitmap;
	HBITMAP					m_hOldBitmap;
	HDC						m_hOffScreenDC;
	HDC						m_hScreenDC;
	HBRUSH					m_hbrBack;

public:
	bool					GameRun();
	bool					GameInit();
	bool					GameFrame();
	bool					GameRender();
	bool					GameRelease();

public:
	virtual bool			Run();
	virtual bool			Init();
	virtual bool			Frame();
	virtual bool			PreRender();
	virtual bool			Render();
	virtual bool			PostRender();
	virtual bool			Release();
	virtual void			MsgEvent(MSG);
	virtual void			DebugString();

public:
	Core();
	virtual ~Core();
};

