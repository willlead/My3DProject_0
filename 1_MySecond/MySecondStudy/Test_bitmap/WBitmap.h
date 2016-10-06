#pragma once
#include "WStd.h"
class WBitmap
{
// sprite ==============================================
public:

	RECT     m_rtDraw;
	POINT    m_Position;

public:

// bitmap ==============================================
public:
	int m_iIndex;
	TCHAR    m_szName[MAX_PATH];
	HDC m_hScreenDC;
	HDC m_hOffScreenDC;
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	BITMAP m_hBitInfo;

public:
	bool Load(TCHAR * pFileName);
	bool Load(HDC hScreenDC,		HDC hOffScreenDC,		TCHAR* pFileName);

// defalut ==============================================
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	WBitmap();
	virtual ~WBitmap();
};

