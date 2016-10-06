#pragma once
#include "Std.h"

enum KeyState { KEY_FREE = 0, KEY_DOWN, KEY_HOLD, KEY_UP };

class Input : public Singleton<Input>
{
public:
	DWORD		m_dwKeyState[256];
	DWORD		m_dwMouseState[3];
	POINT		m_MousePoint;
	TCHAR		m_szBuffer[MAX_PATH];		

public:
	DWORD		KeyCheck(DWORD);
	void		MsgEvent(MSG);		// Window 메세지를 받아서 마우스 상태 처리

public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

public:
	Input();
	virtual ~Input();
};

#define I_Input Input::GetInstance()