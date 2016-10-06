#include "TWindow.h"
#include <windowsx.h>
TWindow* g_Window = NULL;

RECT			g_rtWindow;
HWND			g_hWnd;
HINSTANCE		g_hInstance;
// �ü���� ���� �޼����� �޴� �Լ�
// �޼����� �޼��� ť�� ����ǰ� 
// �ʿ�� �ش� ������ ť���� �����´�.
LRESULT CALLBACK WndProc(HWND hWnd,
				UINT msg, 
				WPARAM wParam, 
				LPARAM lParam)
{
	if(g_Window->MsgProc(hWnd, msg, 
		wParam, lParam)	 > 0)
	{
		return 0;
	}

	switch (msg)
	{		
	case WM_SIZE:
	{
		//if( wParam == SIZE_MAXIMIZED )
		if (wParam != SIZE_MINIMIZED && g_Window != NULL)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			//POINT pos = { width, height };
			//ScreenToClient(hWnd, &pos);
			g_Window->ResizeClient(width, height);
		}
	}break;
		// �����츦 �ٽ� �׷��� �Ҷ� �߻�
		case WM_PAINT://BeginPaint
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}break;
		case WM_DESTROY:
		{
			// �ݱ� �޼����� ������ 
			// ���ν����� ����Ѵ�.
			PostQuitMessage(0);
		}break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT TWindow::MsgProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	return 0;
}
void TWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int x = (iScreenWidth-(m_rtWindow.right-m_rtWindow.left)) / 2;
	int y = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;
	RECT rt = { 0,0,m_rtWindow.right, m_rtWindow.bottom };
	//AdjustWindowRectEx(&rt, 
	//	GetWindowStyle(m_hWnd), 
	//	GetMenu(m_hWnd) != NULL, 
	//	GetWindowExStyle(m_hWnd));
	AdjustWindowRect(&rt, GetWindowStyle(m_hWnd), FALSE);

	MoveWindow(m_hWnd, x, y, 
		rt.right-rt.left,
		rt.bottom-rt.top, true);
}
bool	TWindow::ResizeClient(UINT iWidth, UINT iHeight)
{
	GetClientRect(m_hWnd, &m_rtWindow);
	g_rtWindow = m_rtWindow;
	return ResizeDevice(iWidth, iHeight);
}
// char ��Ƽ����Ʈ => CHAR
//      ��Ƽ�ڵ�   => WCHAR
bool	TWindow::SetWindow(HINSTANCE hInstance,
	TCHAR* pTitleName,
	int iWidth, int iHeight,
	DWORD dwStyle )
{
	m_hInstance = hInstance;
	g_hInstance = m_hInstance;
	//1, ������ Ŭ���� ��ü ���(�ü��)
	WNDCLASSEX wc;
	wc.cbSize	= sizeof(WNDCLASSEX);
	wc.style	= CS_HREDRAW | CS_VREDRAW;
	// ������ ���ν���
	wc.lpfnWndProc = WndProc,//WNDPROC     lpfnWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"KGCA",
	wc.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	if (RegisterClassEx(&wc) == false)
	{
		return false;
	}
	//2, ��ϵ� Ŭ���� ��ü�� ����Ͽ� ������ ����	
	RECT rt = { 0,0,iWidth, iHeight };
	AdjustWindowRect(&rt, dwStyle, FALSE);
	m_rtWindow = rt;
#ifdef _DEBUG
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,//DWORD dwExStyle,
		L"KGCA",// LPCWSTR lpClassName,
		pTitleName,//LPCWSTR lpWindowName,
		dwStyle
		,//DWORD dwStyle,
		0,		0, 
		rt.right-rt.left,
		rt.bottom - rt.top,
		NULL,		NULL,		m_hInstance,		NULL);
	if (m_hWnd == NULL) return false;
	g_hWnd = m_hWnd;
#else
	m_hWnd = CreateWindowEx(
		WS_EX_TOPMOST,//DWORD dwExStyle,
		L"KGCA",// LPCWSTR lpClassName,
		pTitleName,//LPCWSTR lpWindowName,
		WS_POPUPWINDOW,//DWORD dwStyle,
		0, 0, iWidth, iHeight,
		NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL) return false;
#endif
	GetClientRect(m_hWnd, &m_rtWindow);
	g_rtWindow = m_rtWindow;
	CenterWindow();

	ShowWindow(m_hWnd, SW_SHOW );
	return true;
};
bool	TWindow::Run()
{
	GameInit();
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	// �޼��� ť���� �޼��� 1���� �����´�.
	while( true )
	{
		// PM_NOREMOVE, PM_REMOVE
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Ű �ؼ��ض�.
			TranslateMessage(&msg);
			// �޼��� ���ν����� �����ض�.
			DispatchMessage(&msg);			

			if (msg.message == WM_QUIT)
			{
				break;
			}		

			MsgEvent(msg);
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return true;
};
bool	TWindow::Release()
{
	return true;
};

TWindow::TWindow()
{
	g_Window = this;
}


TWindow::~TWindow()
{
}
