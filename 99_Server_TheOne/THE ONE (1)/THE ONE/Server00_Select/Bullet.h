#pragma once
#include "Object.h"

class Bullet : public Object
{
public:
	int			m_iBulletHigh;

public:
	bool		Frame();

public:
	Bullet();
	virtual ~Bullet();
};

