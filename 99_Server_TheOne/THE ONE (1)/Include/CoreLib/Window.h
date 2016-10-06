#pragma once
#include <Windows.h>
#include <tchar.h>

class Window
{
public:
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;
	RECT				m_rtWindow;
	RECT				m_rtClient;

public:
	bool				SetWindow(HINSTANCE hInstance, TCHAR* pszStr = _T("Sample"), int iX = 0, int iY = 0, int iWidth = 800, int iHeight = 600);
	void				CenterWindow();

public:
	virtual void		MsgProc(HWND, UINT, WPARAM, LPARAM);


public:
	Window();
	virtual ~Window();
};

