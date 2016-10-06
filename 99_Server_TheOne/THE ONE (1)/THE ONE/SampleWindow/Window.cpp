#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		UINT iWidth = LOWORD(lParam);
		UINT iHeight = HIWORD(lParam);
		TCHAR StrBuffer[MAX_PATH] = { 0, };
		_stprintf_s(StrBuffer, _T("%d, %d\n"), iWidth, iHeight);
		OutputDebugString(StrBuffer);
	}break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool Window::SetWindow(HINSTANCE hInstance, TCHAR* pszStr, int iX, int iY, int iWidth, int iHeight)
{
	m_hInstance = hInstance;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = m_hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = _T("Class_0");
	wc.style = CS_HREDRAW || CS_VREDRAW;
	if (RegisterClassEx(&wc) == false)
	{
		return false;
	}
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, _T("Class_0"), pszStr,
		WS_OVERLAPPEDWINDOW, iX, iY, iWidth, iHeight, NULL, NULL, hInstance, NULL);
	if (m_hWnd == NULL)
	{
		return false;
	}
	ShowWindow(m_hWnd, SW_SHOW);
	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);
	return true;
}

bool Window::Run()
{
	GameInit();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return true;
}

Window::Window()
{
}


Window::~Window()
{
}
