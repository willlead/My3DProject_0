#include "WCore.h"

//==========================================================================
HDC		g_hScreenDC = NULL;
HDC		g_hOffScreenDC = NULL;
//==========================================================================
bool WCore::Init() 
{ 
	return true;
};
bool WCore::Frame()
{
	return true;
};
bool WCore::Render() 
{
	return true; 
};
bool WCore::Release()
{ 
	return true;
};
//==========================================================================
bool WCore::GameInit()
{
	m_hScreenDC = GetDC(m_hWnd);
	// 제 2의 버퍼 생성
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);
	//m_hOffScreenBitmap = CreateCompatibleBitmap(m_hScreenDC, 800, 600);
	m_hOffScreenBitmap = CreateCompatibleBitmap(m_hScreenDC, 1280, 720);
	SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);

	g_hScreenDC = m_hScreenDC;
	g_hOffScreenDC = m_hOffScreenDC;

	m_Timer.Init();
	m_Input.Init();
	Init();
	return true;
}

bool WCore::GameRun()
{

	GameFrame();
	GameRender();
	return true;
}

bool WCore::GameFrame()
{
	m_Timer.Frame();
	m_Input.Frame();
	Frame();
	return true;
}

bool WCore::GameRender()
{
	PreRender();
		Render();
		m_Timer.Render();
		m_Input.Render();
		DebugString();
	PostRender();
	return true;
}

bool WCore::PreRender() {

	// 제 2의 버퍼 초기화, 클리어, 지우기
	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH  hbrBack = CreateSolidBrush(bkColor);
	HBRUSH old = (HBRUSH)SelectObject(m_hOffScreenDC, hbrBack);

	//???
	//PatBlt(m_hOffScreenDC, 0, 0, 800, 600, PATCOPY);
	PatBlt(m_hOffScreenDC, 0, 0, 1280, 720, PATCOPY);
	//???
	DeleteObject(SelectObject(m_hOffScreenDC, old));
		
	return true;
}

void    WCore::DebugString()
{
	SetBkMode(m_hOffScreenDC, TRANSPARENT);
	TextOut(m_hOffScreenDC,
		0, 0, m_Timer.m_csBuffer,
		_tcslen(m_Timer.m_csBuffer));

	TextOut(m_hOffScreenDC,
		0, 25, m_Input.m_csBuffer,
		_tcslen(m_Input.m_csBuffer));
}


bool WCore::PostRender() {
	//BitBlt(m_hScreenDC, 0, 0, 800, 600,  // 대상 위치
	//	m_hOffScreenDC, 0, 0, SRCCOPY);//  원본

	BitBlt(m_hScreenDC, 0, 0, 1280, 720,  // 대상 위치
		m_hOffScreenDC, 0, 0, SRCCOPY);//  원본
	return true;
}

bool WCore::GameRelease()
{
	ReleaseDC(m_hWnd, m_hOffScreenDC);
	ReleaseDC(m_hWnd, m_hScreenDC);

	m_Timer.Release();
	m_Input.Release();

	Release();
	
	return false;
}

void     WCore::MsgEvent(MSG msg)
{
	m_Input.MsgEvent(msg);
};


bool WCore::Run()
{
	GameInit();
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));
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

WCore::WCore()
{
}


WCore::~WCore()
{
}