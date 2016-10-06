#pragma once
#include "BitmapManager.h"


class Shape
{
public:
	Bitmap*				m_ColorBitmap;
	Bitmap*				m_MaskBitmap;
	float				m_fX;
	float				m_fY;
	RECT				m_rt;		// x, y, w, h
	int					m_bUsed;

public:
	bool				Load(TCHAR* pszFileName1, TCHAR* pszFileName2 = 0);
	void				SetPosition(int, int);
	void				DrawBackGround();

public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();

public:
	Shape();
	virtual ~Shape();
};

