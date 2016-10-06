#pragma once
#include "TDefine.h"
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
public:
	friend class TSingleton<TInput>;
public:
	LPDIRECTINPUT8 m_pDI;
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;
	BYTE m_KeyState[KEYSTATECOUNT];
	BYTE m_KeyStateOld[KEYSTATECOUNT];
	DIMOUSESTATE m_DIMouseState;
	bool InitDirectInput();
public:
	vector<wstring>    m_strList;
	TCHAR		m_csBuffer[MAX_PATH];
	//DWORD		m_dwKeyState[256];
	DWORD		m_MouseState[3];
	DWORD		m_dwBeforeMouseState[3];
	POINT		m_MousePos;
public:
	DWORD		KeyCheck(DWORD dwKey);
	// Ŭ���� ��� ���� �ʱ�ȭ
	bool		Init();
	// �� �����ӿ��� ����� ����Ѵ�.
	bool		Frame();
	// �� �����ӿ��� ��ο츦 ����Ѵ�.
	bool		Render();
	// Ŭ���� ��� ���� �Ҹ��� ����Ѵ�.
	bool		Release();
	void		MsgEvent(MSG msg);
public:
	TInput();
	virtual ~TInput();
};
#define I_Input TInput::GetInstance()
