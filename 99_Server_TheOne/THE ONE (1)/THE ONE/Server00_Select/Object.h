#pragma once
#include "Std.h"
#include "Timer.h"

class Object
{
public:
	float				m_fX;
	float				m_fY;
	float				m_fSpeed;
	bool				m_bUsed;
	int					m_iNum;

public:
	Object();
	virtual ~Object();
};



