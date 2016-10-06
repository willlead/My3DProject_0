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
	//  ��ǲ ��ü
	LPDIRECTINPUT8		m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // Ű����
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// ���콺
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
	// Ŭ���� ��� ���� �ʱ�ȭ
	bool		Init();
	// �� �����ӿ��� ����� ����Ѵ�.
	bool		Frame();
	// �� �����ӿ��� ��ο츦 ����Ѵ�.
	bool		Render();
	// Ŭ���� ��� ���� �Ҹ��� ����Ѵ�.
	bool		Release();
	void		MsgEvent(MSG msg);
private:
	TInput();
public:
	virtual ~TInput();
};
#define I_Input TInput::GetInstance()
