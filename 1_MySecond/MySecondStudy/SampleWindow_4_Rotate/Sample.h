#pragma once
#include "WCore.h"

#define MAX_OBJECT 10
class Sample : public WCore
{
public:
	WEnemey m_BackGround;
	WHero m_Hero;
	WEnemy m_Object[MAX_OBJECT];
	float m_iX;
	float m_iY;
public :
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	Sample();
	virtual ~Sample();
};

