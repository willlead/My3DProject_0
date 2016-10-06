#pragma once

#include <windows.h>

// Window
extern HWND			g_hWnd;					
extern HINSTANCE	g_hInstance;
extern RECT			g_rtClient;

// Core - 더블버퍼링
extern HDC			g_hScreenDC;
extern HDC			g_hOffScreenDC;
