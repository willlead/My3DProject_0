#include "WWindow.h"

// �ü���� �޼��� ����ϴ� �Լ��� �ȴ�.
LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,// �ΰ����� ������
	LPARAM lParam)// �ΰ����� ������
{
	switch (msg)
	{
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			INT KKK = 0;
		}
		if (wParam == SIZE_MAXIMIZED)
		{
			INT KKK = 0;
		}
		UINT iWidth = LOWORD(lParam);
		UINT iHeight = HIWORD(lParam);
		TCHAR strBuffer[MAX_PATH] = { 0, };
		_stprintf_s(strBuffer, L"%ld %ld\n", iWidth, iHeight);
		OutputDebugString(strBuffer);
		//MessageBox(hWnd, strBuffer, L"WM_SIZE", MB_OK);

	}break;
	case WM_DESTROY:
	{
		// �ݱ� �޼����� ������ ���ν����� ����Ѵ�.
		// �޼��� ť�� ���� �ڿ� WM_QUIT �� �߰�
		PostQuitMessage(0);
	}
	}
	// ������ �޼����� �ü���� �˾Ƽ� ó���ض�.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
bool WWindow::SetWindow(HINSTANCE hInstatnce,
	TCHAR* titleName,
	int iX, int iY, int iWidth,
	int iHeight)
{
	// 1, ������ Ŭ���� ��ü �ü���� ���
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.lpszClassName = L"KGCA";
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstatnce;
	// ��Ʈ��( ����Ǿ� �ִ�. )
	wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.cbClsExtra = 0;// ������ Ŭ�������� ����Ϸ��� ������ �޸� ũ��
	wc.cbWndExtra = 0;// �����쿡�� ����Ϸ��� ������ �޸� ũ��
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	if (RegisterClassEx(&wc) == false)
	{
		return false;
	}

	// 2, ��ϵ� Ŭ���� ��ü�� ����Ͽ� ������ ����
	m_hWnd = CreateWindowEx(
		// �۾�ǥ���� ��ܿ� ��ġ�ض�.
		WS_EX_APPWINDOW, //DWORD dwExStyle,
		L"KGCA",
		titleName,
		WS_OVERLAPPEDWINDOW,//WS_OVERLAPPEDWINDOW, //WS_POPUPWINDOW, //DWORD dwStyle,
		iX, //int X,
		iY,//int Y,
		iWidth,//int nWidth,
		iHeight,//int nHeight,
		NULL, //HWND hWndParent,
		NULL,//HMENU hMenu,
		hInstatnce,//HINSTANCE hInstance,
		NULL);  //LPVOID lpParam);
	if (m_hWnd != NULL)
	{
		// 3, ������ ��Ÿ����, ��ħ, ������ �ٿ��
		ShowWindow(m_hWnd, SW_SHOW);
	}

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);
	return true;
}

WWindow::WWindow()
{
}


WWindow::~WWindow()
{
}
