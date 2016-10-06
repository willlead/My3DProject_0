#pragma once
#include "WStd.h"
class WWindow
{
public:
	HWND m_hWnd;
	RECT m_rtWindow;
	RECT m_rtClient;

public :
	bool SetWindow(
		HINSTANCE hInstance,
		TCHAR * titleName = L"MySecondStudy",
		int iX = 0,
		int iY = 0,
		int iWidth = 800,
		int iHeight = 600);

public:
	WWindow();
	virtual ~WWindow();
};

