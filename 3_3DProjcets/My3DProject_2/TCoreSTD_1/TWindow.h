#pragma once
#include "TDevice.h"
class TWindow : public TDevice
{
public:
	HWND		m_hWnd; // 윈도우 핸들
	HINSTANCE	m_hInstance; // 실행객체 핸들
	RECT		m_rtWindow;
public:
	void	CenterWindow();

	bool	SetWindow(HINSTANCE hInstance,
		TCHAR* pTitleName = L"Sample",
		int iWidth = 800, int iHeight = 600,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW);
	bool	Run();
	bool	Release();
	virtual  bool   ResizeClient(UINT iWidth, UINT iHeight);
	virtual  bool	GameRun() { return true; };
	virtual  void   MsgEvent(MSG msg) {};
	virtual  bool	GameInit() { return true; };
	virtual  bool   GameRelease() { return true; };
	virtual LRESULT MsgProc(HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);
public:
	TWindow();
	virtual ~TWindow();
};


