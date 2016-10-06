#pragma once
#include "WStd.h"
class WCollision
{
public:
	friend bool RectInPt(WRect& rt, POINT& pt);
public:
	WCollision();
	virtual ~WCollision();
};

