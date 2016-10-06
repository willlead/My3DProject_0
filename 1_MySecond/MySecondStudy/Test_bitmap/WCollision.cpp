#include "WCollision.h"

bool RectInPt(WRect & rt, POINT & pt)
{
	if (rt.x <= pt.x && rt.x+ rt.w >= pt.x)
	{
		if (rt.y <= pt.y && rt.y + rt.h >= pt.y)
		{
			return true;
		}
	}
	return false;
}

WCollision::WCollision()
{
}


WCollision::~WCollision()
{
}


