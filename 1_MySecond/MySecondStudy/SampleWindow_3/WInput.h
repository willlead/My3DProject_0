#pragma once
#include "WBase.h"
enum KeyState {
	KEY_FREE =0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD,
};
class WInput
{
public:
	DWORD m_dwKeyState[256];
	DWORD m_dwMouseState[3];
	POINT m_MovePt;
	TCHAR m_csBuffer[MAX_PATH];
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	DWORD KeyCheck(DWORD dwKey);
	void MsgEvent(MSG msg);

public:
	WInput();
	virtual ~WInput();
};

