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

