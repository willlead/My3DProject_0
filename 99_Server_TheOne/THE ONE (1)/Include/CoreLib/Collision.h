#pragma once
#include "Std.h"

struct Sphere
{
	POINT				ptCenter;
	float				fRadius;
};

class Collision
{
public:
	friend bool			RectInPoint(RECT&, POINT&);
	friend bool			RectInRect(RECT&, RECT&);
	friend bool			SphereInSphere(Sphere&, Sphere&);
	friend bool			SphereInSphere(RECT&, RECT&);
	friend POINT		CollisionPt(RECT&, POINT&);
	friend POINT		CollisionPt(RECT&, RECT&);
	friend POINT		CollisionPt(Sphere&, Sphere&);
	
public:
	Collision();
	virtual ~Collision();
};

