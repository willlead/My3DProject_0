#include "WInput.h"
#include "WSys.h"	
bool WInput::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(m_dwMouseState, sizeof(DWORD) * 3);
	return true;
}

bool WInput::Frame()
{
	GetCursorPos(&m_MovePt);
	ScreenToClient(g_hWnd, &m_MovePt);
	return true;
}

bool WInput::Render()
{
#ifdef _DEBUG
	_stprintf_s(
		m_csBuffer,
		L"x = [%d], y = [%d]\n",
		m_MovePt.x, m_MovePt.y);
	//OutputDebugString(m_csBuffer);
	HDC hdc = GetDC(g_hWnd);
	//SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 0, 25, m_csBuffer,
		_tcslen(m_csBuffer));
	ReleaseDC(g_hWnd, hdc);

#endif // _DEBUG

	return true;
}

bool WInput::Release()
{
	return true;
}

DWORD WInput::KeyCheck(DWORD dwKey)
{
	SHORT sKey = GetAsyncKeyState(dwKey);
	if (sKey & 0x8000)
	{
		if (m_dwKeyState[dwKey] == KEY_FREE)
			m_dwKeyState[dwKey] = KEY_PUSH;
		else
			m_dwKeyState[dwKey] = KEY_HOLD;
	}
	else
	{
		if (m_dwKeyState[dwKey] == KEY_PUSH ||
			m_dwKeyState[dwKey] == KEY_HOLD)
			m_dwKeyState[dwKey] = KEY_UP;
		else
			m_dwKeyState[dwKey] = KEY_FREE;
	}

	return m_dwKeyState[dwKey];
}

void WInput::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	default:
		break;
	case WM_LBUTTONDOWN:
	{
		GetCursorPos(&m_MovePt);
		ScreenToClient(g_hWnd, &m_MovePt);
		m_dwMouseState[0] = TRUE;
	}break;
	case WM_LBUTTONUP:
	{
		m_dwMouseState[0] = FALSE;
	}break;
	case WM_RBUTTONDOWN:
	{
		m_dwMouseState[1] = TRUE;
	}break;
	case WM_RBUTTONUP:
	{
		m_dwMouseState[1] = FALSE;
	}break;
	case WM_MBUTTONDOWN:
	{
		m_dwMouseState[2] = TRUE;
	}break;
	case WM_MBUTTONUP:
	{
		m_dwMouseState[2] = FALSE;
	}break;
	}
}

WInput::WInput()
{
}


WInput::~WInput()
{
}
