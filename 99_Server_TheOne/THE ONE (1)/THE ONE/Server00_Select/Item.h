#pragma once
#include "Object.h"

class Item : public Object
{
public:
	int				m_iItemHigh;

public:
	bool			Frame();

public:
	Item();
	virtual ~Item();
};

