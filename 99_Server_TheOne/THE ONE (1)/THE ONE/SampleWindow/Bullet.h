#pragma once
#include "Object.h"

class Bullet : public Object
{
public:
	bool		m_bStart;
	int			m_iNumSpriter;

public:
	bool		Frame();

public:
	Bullet();
	virtual ~Bullet();
};

