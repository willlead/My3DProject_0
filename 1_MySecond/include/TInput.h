#pragma once
#include "tstd.h"
enum KeyState {
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD,
};
class TInput
{
public:
	std::vector<wstring>    m_strList;
	TCHAR		m_csBuffer[MAX_PATH];
	DWORD		m_dwKeyState[256];
	DWORD		m_dwMouseState[3];
	DWORD		m_dwBeforeMouseState[3];
	POINT		m_MousePos;
public:
	DWORD		KeyCheck(DWORD dwKey);
	// 클래스 멤버 변수 초기화
	bool		Init();
	// 매 프레임에서 계산을 담당한다.
	bool		Frame();
	// 매 프레임에서 드로우를 담당한다.
	bool		Render();
	// 클래스 멤버 변수 소멸을 담당한다.
	bool		Release();
	void		MsgEvent(MSG msg);
public:
	TInput();
	virtual ~TInput();
};

