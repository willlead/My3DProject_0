#pragma once
#include "WCore.h"	
#include "WBitmap.h"
class Sample : public WCore
{
public:
	WBitmap m_BackGround;
	WBitmap m_Hero;
	float m_iX;
	float m_iY;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:

	Sample();
	virtual ~Sample();
};

