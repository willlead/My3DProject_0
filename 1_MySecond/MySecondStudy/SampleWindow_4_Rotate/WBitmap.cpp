#include "WBitmap.h"
#include "WSys.h"
HDC g_hScreenDC = NULL;
HDC g_hOffScreenDC = NULL;

bool WBitmap::Load(TCHAR * pFileName) {
	m_hScreenDC = g_hScreenDC;
	m_hOffScreenDC = g_hOffScreenDC;

	m_hBitmap = (HBITMAP)LoadImage(g_hInstance, pFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	GetObject(m_hBitmap, sizeof(HBITMAP), &m_hBitInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	TCHAR SaveName[MAX_PATH] = { 0, };

	_tsplitpath_s(pFileName, Drive, Dir, Name, Ext);
	_stprintf_s(SaveName, _T("%s%s"),Name, Ext);
	return true;

}
bool WBitmap::Load(HDC hScreenDC, HDC hOffScreenDC, TCHAR* pFileName) {
	m_hScreenDC = hScreenDC;
	m_hOffScreenDC = hOffScreenDC;
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance, pFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	GetObject(m_hBitmap, sizeof(BITMAP), &m_hBitInfo);

	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}
bool WBitmap::Init() {

	return true;
}
bool WBitmap::Frame() {
	return true;
}
bool WBitmap::Render() {
	BitBlt(m_hOffScreenDC, 0, 0, m_hBitInfo.bmWidth, m_hBitInfo.bmHeight, m_hMemDC, 0, 0, SRCCOPY);
	return true;
}
bool WBitmap::Release() {
	SelectObject(m_hMemDC, m_hOldBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
}

WBitmap::WBitmap()
{
	m_iIndex = -1;
}


WBitmap::~WBitmap()
{
}
