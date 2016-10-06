#pragma once
#include "WStd.h"

class WWindow
{
public:
	HWND m_hWnd;
	RECT m_rtWindow;
	RECT m_rtClient;
	int  m_iWidth;
	int  m_iHeight;

public:
	void SetRect(int iWidth, int iHeight);
	bool SetWindow(
		HINSTANCE hInstance,
		TCHAR * titleName = L"MySecondStudy",
		int iX = 0,		int iY = 0,
		int iWidth = 1280,		int iHeight = 720);

public:
	WWindow();
	virtual ~WWindow();
};

