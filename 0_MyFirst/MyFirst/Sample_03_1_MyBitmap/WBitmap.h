#pragma once
#include "WStd.h"
class WBitmap
{
public:
	HDC		 m_hScreenDC;
	HDC      m_hMemDC;
	HBITMAP  m_hBitmap;
	BITMAP   m_hBitInfo;
public:
	bool     Load(HDC hScreenDC, TCHAR* pFileName);
public:
	WBitmap();
	virtual ~WBitmap();
};

