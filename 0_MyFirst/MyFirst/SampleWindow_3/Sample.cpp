#include "Sample.h"

void Sample::MsgEvent(MSG msg)
{
	m_Input.MsgEvent(msg);
}

bool Sample::Init()
{
	m_Timer.Init();
	m_Input.Init();
	return true;
}
bool Sample::Frame()
{
	m_Timer.Frame();
	m_Input.Frame();
	if (m_Input.KeyCheck('W') == KEY_PUSH)
	{
		MessageBox(m_hWnd, L"W click", L"KeyState", MB_YESNOCANCEL);
	}
	if (m_Input.KeyCheck('S') == KEY_PUSH)
	{
		//MessageBox(m_hWnd, L"W click", L"KeyState", MB_OK);
	}
	if (m_Input.KeyCheck('A') == KEY_PUSH)
	{

	}
	if (m_Input.KeyCheck('D') == KEY_PUSH)
	{

	}


	return true;
}
bool Sample::Render()
{
	m_Timer.Render();
	m_Input.Render();
	HDC hdc = GetDC(this->m_hWnd);
	//SetBkMode(hdc, TRANSPARENT); 화면을 투명하게한다.
	// 화면에 FPS를 출력
	TextOut(hdc, 0, 0, m_Timer.m_csBuffer, _tcslen(m_Timer.m_csBuffer));
	ReleaseDC(m_hWnd, hdc);
	return true;
}
bool Sample::Release()
{
	m_Timer.Release();
	m_Input.Release();
	return true;
}

int WINAPI wWinMain(
	HINSTANCE hInstatnce,
	HINSTANCE hPrevInstatnce,
	LPWSTR   lpCmdLine,
	int      nCmdShow)
{
	CreateCompatibleDC();

	Sample win;
	//win.SetRect(1024, 768);
	if (win.SetWindow(hInstatnce) == true)
	{
		win.Run();
	}
	return 0;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}