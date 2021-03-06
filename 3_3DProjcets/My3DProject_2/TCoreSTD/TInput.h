#pragma once
#include "tstd.h"
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")

enum KeyState {
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD,
};


#define KEYSTATECOUNT 256
//#define WIN_KEY_USE

class TInput : public TSingleton<TInput>
{
private:
	friend class TSingleton<TInput>;
public:
	//  인풋 객체
	LPDIRECTINPUT8		m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // 키보드
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// 마우스
	BYTE				  m_KeyState[KEYSTATECOUNT];
	BYTE				  m_KeyStateOld[KEYSTATECOUNT];
	DIMOUSESTATE		  m_DIMouseState;
	bool   InitDirectInput();
public:
	std::vector<wstring>    m_strList;
	TCHAR		m_csBuffer[MAX_PATH];
	BYTE		m_MouseState[3];
	BYTE		m_BeforeMouseState[3];
	POINT		m_MousePos;
public:
	BYTE		KeyCheck(BYTE dwKey);
	// 클래스 멤버 변수 초기화
	bool		Init();
	// 매 프레임에서 계산을 담당한다.
	bool		Frame();
	// 매 프레임에서 드로우를 담당한다.
	bool		Render();
	// 클래스 멤버 변수 소멸을 담당한다.
	bool		Release();
	void		MsgEvent(MSG msg);
private:
	TInput();
public:
	virtual ~TInput();
};
#define I_Input TInput::GetInstance()
