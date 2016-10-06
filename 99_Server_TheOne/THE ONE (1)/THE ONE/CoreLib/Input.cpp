#include "Input.h"
#include "Sys.h"

DWORD Input::KeyCheck(DWORD dwKey)
{
	SHORT sKey = GetAsyncKeyState(dwKey);
	if (sKey & 0x8000)
	{
		if (m_dwKeyState[dwKey]==KEY_FREE)
		{
			m_dwKeyState[dwKey] = KEY_DOWN;
		}
		else
		{
			m_dwKeyState[dwKey] = KEY_HOLD;
		}
	}
	else
	{
		if (m_dwKeyState[dwKey] == KEY_HOLD || m_dwKeyState[dwKey] == KEY_DOWN)
		{
			m_dwKeyState[dwKey] = KEY_UP;
		}
		else
		{
			m_dwKeyState[dwKey] = KEY_FREE;
		}
	}
	return m_dwKeyState[dwKey];
}

void Input::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
	{
		m_dwMouseState[0] = true;
	}break;
	case WM_LBUTTONUP:
	{
		m_dwMouseState[0] = false;
	}break;
	case WM_RBUTTONDOWN:
	{
		m_dwMouseState[1] = true;
	}break;
	case WM_RBUTTONUP:
	{
		m_dwMouseState[1] = false;
	}break;case WM_MBUTTONDOWN:
	{
		m_dwMouseState[2] = true;
	}break;
	case WM_MBUTTONUP:
	{
		m_dwMouseState[2] = false;
	}break;
	}
}

bool Input::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(m_dwMouseState, sizeof(DWORD) * 3);
	return true;
}

bool Input::Frame()
{
	GetCursorPos(&m_MousePoint);
	ScreenToClient(g_hWnd, &m_MousePoint);
	return true;
}

bool Input::Render()
{
#ifdef _DEBUG
	_stprintf_s(m_szBuffer, _T("x=[%d], y=[%d]"), m_MousePoint.x, m_MousePoint.y);
#endif
	return true;
}

bool Input::Release()
{
	return true;
}

Input::Input()
{
}


Input::~Input()
{
}
