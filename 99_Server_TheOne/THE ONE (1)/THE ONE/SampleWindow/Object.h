#pragma once
#include "Shape.h"

class Object : public Shape
{
public:
	HBRUSH		m_hBrush;
	float		m_fSpeed;
	RECT		m_rtCollision;
	bool		m_bCollide;
	bool		m_bSent;
	int			m_iNum;
	
public:
	void		SetPosition(int, int, float);
	void		SetSpeed(float);
	void		SetRect(int, int, int, int);
	void		SetRect(RECT&);
	
public:
	bool		Init();
	bool		Frame();
	bool		Release();

public:
	Object();
	virtual ~Object();
};



