#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	int			m_iTypeIndex;

public:
	bool		Frame();

public:
	Enemy();
	virtual ~Enemy();
};

