#include "WInput.h"

float g_fSeconePerFrame = 0.0f;

void WInput::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
	{
	
		m_dwMouseState[0] = TRUE; break;
	}
	case WM_LBUTTONUP:
	{
		m_dwMouseState[0] = FALSE; break;
	}
	case WM_RBUTTONDOWN:
	{
		m_dwMouseState[1] = TRUE; break;
	}
	case WM_RBUTTONUP:
	{
		m_dwMouseState[1] = FALSE; break;
	}
	case WM_MBUTTONDOWN:
	{
		m_dwMouseState[2] = TRUE; break;
	}
	case WM_MBUTTONUP:
	{
		m_dwMouseState[2] = FALSE; break;
	}

	
	default:
		break;
	}
}


bool WInput::Init()
{
	
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(m_dwMouseState, sizeof(DWORD) * 3);
	return true;
}

bool WInput::Frame()
{
	//GetCursorPos(&m_MovePt);	
	ClientToScreen(g_hWnd, &m_MovePt);
	ScreenToClient(g_hWnd, &m_MovePt);
	return true;
}

DWORD WInput::KeyCheck(DWORD dwKey)
{
	// 키상태를 비동기로 체크한다.
	SHORT sKey = GetAsyncKeyState(dwKey);

	if (sKey & 0x8000)
	{// 0x8000 = 1000 0000 0000 0000 최상위 비트가 1이냐 따짐
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



WInput::WInput()
{
}


WInput::~WInput()
{
}
