#include "WBitmap.h"
#include "WSys.h"
HINSTANCE g_hInstance;

bool WBitmap::Load(HDC hScreenDC, HDC hOffScreenDC, TCHAR * pFileName)
{
	m_hScreenDC = hScreenDC;
	m_hOffScreenDC = hOffScreenDC;

	BITMAP info;
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		pFileName, IMAGE_BITMAP, 0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);

	GetObject(m_hBitmap, sizeof(BITMAP), &info);

	m_hMemDC = CreateCompatibleDC(hScreenDC);

	SelectObject(m_hMemDC, m_hBitmap);
	return true;
}

bool WBitmap::Init() 
{
	//열심히 해라..    !!!!!!!!!!!!!  졸지마  죽는다.181818181818
	
	return true;
}
bool WBitmap::Frame() 
{
	return true;
}
bool WBitmap::Render() 
{
	BitBlt(m_hOffScreenDC, 0, 0,
		m_hBitInfo.bmWidth,
		m_hBitInfo.bmHeight,
		m_hMemDC, 0, 0, SRCCOPY);
	return true;
}

bool WBitmap::Release() 
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
}


WBitmap::WBitmap()
{
}


WBitmap::~WBitmap()
{
}
