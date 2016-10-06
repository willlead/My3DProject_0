#include "Window.h"
#include "Sys.h"

Window* g_pWindow = 0;

HINSTANCE g_hInstance = 0;
HWND g_hWnd;
RECT g_rtClient;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_pWindow->MsgProc(hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_SIZE:
	{
#ifdef _DEBUG
		UINT iWidth = LOWORD(lParam);
		UINT iHeight = HIWORD(lParam);
		TCHAR StrBuffer[MAX_PATH] = { 0, };
		_stprintf_s(StrBuffer, _T("%d, %d\n"), iWidth, iHeight);
		OutputDebugString(StrBuffer);
#endif
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
	g_hInstance = hInstance;
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

	RECT rt = { 0, 0, iWidth, iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, _T("Class_0"), pszStr,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, iX, iY, rt.right - rt.left, rt.bottom - rt.top, NULL, NULL, hInstance, NULL);
	g_hWnd = m_hWnd;
	if (m_hWnd == NULL)
	{
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);

	GetWindowRect(m_hWnd, &m_rtWindow);

	CenterWindow();

	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
	return true;
}

void Window::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;

	MoveWindow(g_hWnd, x, y, m_rtWindow.right - m_rtWindow.left, m_rtWindow.bottom - m_rtWindow.top, true);
}

void Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}

Window::Window()
{
	g_pWindow = this;
}


Window::~Window()
{
}
