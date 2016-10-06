#pragma once
#include "WStd.h"
#include "Tsys.h"
enum KeyState {
	KEY_FREE = 0,
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
public:
	bool    Init(); // �ʱ�ȭ
	bool    Frame();// ���
	bool    Render();// ��ο�
	bool    Release();// ����, �Ҹ�
	DWORD	KeyCheck(DWORD dwKey);
	void MsgEvent(MSG msg);
public:
	WInput();
	virtual ~WInput();
};

