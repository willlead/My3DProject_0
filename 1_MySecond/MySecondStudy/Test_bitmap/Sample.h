#pragma once
#include "WCore.h"
#include "WBitmap.h"
#include "WNode.h"
#include "WCollision.h"
#include <vector>

#define NodeCnt 10

class Sample : public WCore
{

// Node =================================================
public:
	WNode m_Node;


// bitmap =================================================
public:
	WBitmap    m_BackGround;
	WBitmap    m_HeroColorBitmap;
	WBitmap    m_HeroMaskBitmap;
	WBitmap    m_MobColorBitmap;
	WBitmap    m_MobMaskBitmap;

	HBRUSH	   m_back;

	HDC  m_hMaskMemDC;
	HDC  m_hColorMemDC;
	HDC  m_RotationBitmapDC;

public:
	bool DrawBackGround();
	void DrawObject();


// Sprite =================================================
public:
	WBitmap    m_Bitmap;
	typedef std::vector<RECT>  SPRITE_ARRAY;
	//RECT **m_SpriteList;
	std::vector<SPRITE_ARRAY>  m_SpriteList;
	float m_fSecPerRender;
	int   m_iSpriteID;
	int   m_iFrame;
	float m_fTimer;
	float m_fLifeTime;

public:
	bool    LoadSprite(TCHAR* pFileName);
	void	SetSprite(int iIndex, float fLifeTime = 1.0f);

	
// defalut ================================================

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};

