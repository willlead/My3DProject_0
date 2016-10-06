#include "Collision.h"

bool RectInPoint(RECT& rt, POINT& pt)
{
	if (pt.x <= rt.right && pt.x >= rt.left)
	{
		if (pt.y <= rt.bottom && pt.y >= rt.top)
		{
			return true;
		}
	}
	return false;
}

bool RectInRect(RECT& r1, RECT& r2)
{
	RECT MinLeft, MaxLeft, MinTop, MaxTop;
	MinLeft = r1.left < r2.left ? r1 : r2;
	MaxLeft = r1.left < r2.left ? r2 : r1;
	MinTop = r1.top < r2.top ? r1 : r2;
	MaxTop = r1.top < r2.top ? r2 : r1;
	if ((MinLeft.right >= MaxLeft.left) && (MaxTop.top <= MinTop.bottom))
	{
		return true;
	}
	return false;
}

bool SphereInSphere(Sphere& s1, Sphere& s2)
{
	float fDistanceRadius = s1.fRadius + s2.fRadius;
	float fX = s1.ptCenter.x - s2.ptCenter.x;
	float fY = s1.ptCenter.y - s2.ptCenter.y;
	float fDistanceCenter = sqrt((fX*fX) + (fY*fY));
	if (fDistanceCenter <= fDistanceRadius)
	{
		return true;
	}
	return false;
}

bool SphereInSphere(RECT& r1, RECT& r2)
{
	Sphere s1, s2;
	float fwRadius, fhRadius;
	s1.ptCenter.x = (r1.right - r1.right) / 2;
	s1.ptCenter.y = (r1.bottom - r1.top) / 2;
	fwRadius = r1.right - s1.ptCenter.x;
	fhRadius = r1.bottom - s1.ptCenter.y;
	s1.fRadius = fwRadius > fhRadius ? fwRadius : fhRadius;
		
	s2.ptCenter.x = (r2.right - r2.right) / 2;
	s2.ptCenter.y = (r2.bottom - r2.top) / 2;
	fwRadius = r2.right - s2.ptCenter.x;
	fhRadius = r2.bottom - s2.ptCenter.y;
	s2.fRadius = fwRadius > fhRadius ? fwRadius : fhRadius;

	return SphereInSphere(s1, s2);
}

POINT CollisionPt(RECT& rt, POINT& pt)
{
	return pt;
}

POINT CollisionPt(RECT& r1, RECT& r2)
{
	POINT r1Center, r2Center;
	POINT pt;
	r1Center.x = (r1.left + r1.right) / 2;
	r1Center.y = (r1.top + r1.bottom) / 2;
	r2Center.x = (r2.left + r2.right) / 2;
	r2Center.y = (r2.top + r2.bottom) / 2;
	pt.x = (r1Center.x + r2Center.x) / 2;
	pt.y = (r1Center.y + r2Center.y) / 2;
	return pt;
}

POINT CollisionPt(Sphere& s1, Sphere&s2)
{
	POINT pt;
	pt.x = (s1.ptCenter.x + s2.ptCenter.x) / 2;
	pt.y = (s1.ptCenter.y + s2.ptCenter.y) / 2;
	return pt;
}

Collision::Collision()
{
}


Collision::~Collision()
{
}
