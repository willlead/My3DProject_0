#pragma once
#include "Std.h"

class Bitmap
{
public:
	HBITMAP			m_hBitmap;
	HBITMAP			m_hOldBitmap;
	BITMAP			m_BitmapInfo;
	HDC				m_hScreenDC;
	HDC				m_hMemoryDC;
	HDC				m_hOffScreenDC;
	TCHAR			m_szName[MAX_PATH];

public:
	bool			Load(TCHAR*);

public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();

public:
	Bitmap();
	virtual ~Bitmap();
};

