#include "bgWindow.h"
#include "bgDInput.h"

bgWindow*	g_pWindow;
HINSTANCE	g_hInstance;
HWND		g_hWnd;

bgWindow::bgWindow()
{
	g_pWindow = this;
	return;
}

bgWindow::~bgWindow()
{
	return;
}

bool bgWindow::AppRun()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	GameInit();

	// ���α׷� ������ ����
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

bool bgWindow::InitWindow(HINSTANCE hInstance, TCHAR* pszTitle, int iWidth, int iHeight, BOOL bFullScreen, bool bCenter)
{
	m_bFullScreen = bFullScreen;
	if (iWidth < 1024)
		m_bSizeLarge = false;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = pszTitle;
	wc.hIconSm = wc.hIcon;
	RegisterClassEx(&wc);

	m_dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; // | WS_MAXIMIZEBOX; // | WS_THICKFRAME;
	m_dwStyleEx = WS_EX_APPWINDOW;
	m_hWnd = CreateWindowEx(m_dwStyleEx, pszTitle, pszTitle, m_dwStyle, 0, 0, iWidth, iHeight, NULL, NULL, hInstance, NULL);
	if (m_hWnd == NULL)
		return false;

	SetWindowSize(m_hWnd, iWidth, iHeight);
	ShowWindow(m_hWnd, SW_SHOW);

	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	//ShowCursor(false);

	g_hInstance = m_hInstance = hInstance;
	g_hWnd = m_hWnd;

	return true;
}

bool bgWindow::SetWindowSize(HWND hWnd, int iWidth, int iHeight)
{
	if (hWnd == NULL)
		return false;

	// ������ ��Ÿ�� ���
	bool bMenu = (GetMenu(hWnd) != NULL);
	m_dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	m_dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);

	// ��ũ�� ũ�� ���
	m_iScreenW = GetSystemMetrics(SM_CXSCREEN);
	m_iScreenH = GetSystemMetrics(SM_CYSCREEN);

	// Ŭ���̾�Ʈ ���� ũ��(���ڰ�) ����
	m_iClientW = iWidth;
	m_iClientH = iHeight;

	m_rtWindow = { 0, 0, iWidth, iHeight };
	AdjustWindowRectEx(&m_rtWindow, m_dwStyle, bMenu, m_dwStyleEx);

	// �������� ũ��(W, H) ���� ���
	m_rtWindow.bottom += (m_dwStyle & WS_CAPTION) ? GetSystemMetrics(SM_CYCAPTION) : 0;
	m_rtWindow.right += (m_dwStyle & WS_BORDER) ? GetSystemMetrics(SM_CXBORDER) : 0;
	m_rtWindow.bottom += (m_dwStyle & WS_BORDER) ? GetSystemMetrics(SM_CYBORDER) : 0;
	m_rtWindow.right += (m_dwStyle & WS_DLGFRAME) ? GetSystemMetrics(SM_CXDLGFRAME) : 0;
	m_rtWindow.bottom += (m_dwStyle & WS_DLGFRAME) ? GetSystemMetrics(SM_CYDLGFRAME) : 0;

	///////////////////////////////////////
	// -1 �ʿ俩��...? �ڡڡڡڡڡڡڡ� ������ ��Ÿ�Ͽ��� �߸� ���Ǿ����� Ȯ���ʿ�
	////////////////////////
	m_iWindowW = m_rtWindow.right - 1;
	m_iWindowH = m_rtWindow.bottom - 1;

	// ���� ũ��� ��ũ���� ��µ� ��ġ(X, Y) ���
	m_rtWindow.left = (m_iScreenW - m_iWindowW) / 2;
	m_rtWindow.top = (m_iScreenH - m_iWindowH) / 2;
	m_iWindowX = m_rtWindow.left;
	m_iWindowY = m_rtWindow.top;

	SetWindowPos(hWnd, NULL, m_iWindowX, m_iWindowY, m_iWindowW, m_iWindowH, NULL);
	return true;
}

// virtual �Լ�
LRESULT CALLBACK bgWindow::AppProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
	{
		m_bActive = (WA_INACTIVE == wParam) ? FALSE : TRUE;
		I_DInput.SetAcquire(m_bActive);
	}
	default:
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	break;
	}
}

// ���� ���ν��� �Լ�
LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;

	default:
	{
		return g_pWindow->AppProc(hWnd, uMsg, wParam, lParam);
	}
	break;
	}
}
