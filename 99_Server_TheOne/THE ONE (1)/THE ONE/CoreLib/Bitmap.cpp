#include "Bitmap.h"
#include "Sys.h"

bool Bitmap::Load(TCHAR* pszFileName)
{
	m_hScreenDC = g_hScreenDC;
	m_hOffScreenDC = g_hOffScreenDC;
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance, pszFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	m_hMemoryDC = CreateCompatibleDC(m_hScreenDC);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemoryDC, m_hBitmap);

	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	TCHAR SaveName[MAX_PATH] = { 0, };
	_tsplitpath_s(pszFileName, Drive, Dir, Name, Ext);
	_stprintf_s(SaveName, _T("%s%s"), Name, Ext);
	_tcscpy_s(m_szName, SaveName);
	
	return true;
}

bool Bitmap::Init()
{
	return true;
}

bool Bitmap::Frame()
{
	return true;
}

bool Bitmap::Render()
{
	BitBlt(m_hOffScreenDC, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight, m_hMemoryDC, 0, 0, SRCCOPY);
	return true;
}

bool Bitmap::Release()
{
	DeleteObject(SelectObject(m_hMemoryDC, m_hOldBitmap));
	DeleteDC(m_hMemoryDC);
	return true;
}

Bitmap::Bitmap()
{
}


Bitmap::~Bitmap()
{
}
