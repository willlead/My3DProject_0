#pragma once
#include "WBitmap.h"
class WObject
{
public:


public:	
	bool     Load(HDC hScreenDC,
		HDC hOffScreenDC,
		TCHAR* pFileName);

	virtual bool     Init();
	virtual bool     Frame();
	virtual bool     Render();
	virtual bool     Release();

public:
	//bool Draw();
	//void	 SetPos(int iX, int iY, float fSpeed);
	//void	 SetSpeed(float fSpeed = 1.0f);
	//void	 SetRect(int x, int y, int w, int h);

public:
	WObject();
	virtual ~WObject();
};

