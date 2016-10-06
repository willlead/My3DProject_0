#pragma once
#include "WBitmap.h"
struct WRect
{
	int x, y;
	int w, h;
};
class WObject
{
public :
	WBitmap			 m_Bitmap;
	float			 m_fSpeed;
	float			 m_fPosX;
	float			 m_fPosY;
	float			 m_fDirection[2];
	WRect			 m_rt;
	WRect			 m_rtCollide;
public:
	bool Load(HDC hScreenDC, HDC hOffScreenDC, TCHAR * pFileName);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	bool Draw();
	void SetPos(int iX, int iY, float fSpeed);
	void SetSpeed(float fSpeed = 1.0f);
	void SetRect(int x, int y, int w, int h);
public:
	WObject();
	virtual ~WObject();
};

