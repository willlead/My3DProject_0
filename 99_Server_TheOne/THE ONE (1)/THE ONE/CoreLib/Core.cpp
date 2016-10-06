#include "Core.h"
HDC g_hScreenDC;
HDC g_hOffScreenDC;

bool Core::Run()
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
			MsgEvent(msg);
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return true;
}

bool Core::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}

bool Core::GameInit()
{
	m_hScreenDC = GetDC(m_hWnd);
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);
	m_hOffScreenBitmap = CreateCompatibleBitmap(m_hScreenDC, m_rtClient.right-m_rtClient.left, m_rtClient.bottom-m_rtClient.top);
	m_hOldBitmap=(HBITMAP)SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);

	g_hScreenDC = m_hScreenDC;
	g_hOffScreenDC = m_hOffScreenDC;

	I_Timer.Init();
	I_Input.Init();
	Init();
	return true;
}

bool Core::GameFrame()
{
	I_Timer.Frame();
	I_Input.Frame();
	Frame();
	return true;
}

bool Core::GameRender()
{
	PreRender();
	Render();
	I_Timer.Render();
	I_Input.Render();
	//DebugString();
	PostRender();
	return true;
}

bool Core::GameRelease()
{
	DeleteObject(SelectObject(m_hOffScreenDC, m_hOldBitmap));
	DeleteDC(m_hOffScreenDC);
	ReleaseDC(m_hWnd, m_hScreenDC);

	I_Timer.Release();
	I_Input.Release();
	Release();
	return true;
}

bool Core::Init()
{
	return true;
}

bool Core::Frame()
{
	return true;
}

bool Core::PreRender()
{
	COLORREF BkColor = RGB(255, 255, 255);
	m_hbrBack = CreateSolidBrush(BkColor);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hOffScreenDC, m_hbrBack);
	PatBlt(m_hOffScreenDC, 0, 0, m_rtClient.right - m_rtClient.left, m_rtClient.bottom - m_rtClient.top, PATCOPY);
	SelectObject(m_hOffScreenDC, hOldBrush);
	return true;
}

bool Core::Render()
{
	return true;
}

bool Core::PostRender()
{
	BitBlt(m_hScreenDC, 0, 0, m_rtClient.right - m_rtClient.left, m_rtClient.bottom - m_rtClient.top, m_hOffScreenDC, 0, 0, SRCCOPY);
	return true;
}

bool Core::Release()
{
	DeleteObject(m_hbrBack);
	return true;
}

void Core::MsgEvent(MSG msg)
{
	I_Input.MsgEvent(msg);		//메세지 핸들링
}

void Core::DebugString()
{
#ifdef _DEBUG
	SetBkMode(m_hOffScreenDC, TRANSPARENT);
	TextOut(m_hOffScreenDC, 0, 60, I_Timer.m_szBuffer, _tcslen(I_Timer.m_szBuffer));
	TextOut(m_hOffScreenDC, 0, 80, I_Input.m_szBuffer, _tcslen(I_Input.m_szBuffer));
#endif
}

Core::Core()
{
}


Core::~Core()
{
}
