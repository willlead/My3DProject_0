#include "bgCollider.h"

bgCollider::bgCollider()
{
}

bgCollider::~bgCollider()
{
}

bool bgCollider::RectInPoint(RECT & rectDest, POINT & posSrc)
{
	if (posSrc.x >= rectDest.left && posSrc.x <= rectDest.right)
	{
		if (posSrc.y >= rectDest.top && posSrc.x <= rectDest.bottom)
		{
			return true;
		}
	}
	return false;
}

bool bgCollider::RectInPointWH(RECT& rectDest, POINT& posSrc)
{
	if (posSrc.x >= rectDest.left && posSrc.x <= rectDest.left + rectDest.right)
	{
		if (posSrc.y >= rectDest.top && posSrc.x <= rectDest.top + rectDest.bottom)
		{
			return true;
		}
	}
	return false;
}

bool bgCollider::RectInRect(RECT& rectDest, RECT& rectSrc)
{
	if (rectDest.right < rectSrc.left) return false;
	if (rectSrc.right < rectDest.left) return false;
	if (rectDest.bottom < rectSrc.top) return false;
	if (rectSrc.bottom < rectDest.top) return false;

	return true;
}

bool bgCollider::RectInRectWH(RECT& rectDest, RECT& rectSrc)
{
	RECT rtDest, rtSrc;

	rtDest.left = rectDest.left;
	rtDest.right = rectDest.left + rectDest.right;
	rtDest.top = rectDest.top;
	rtDest.bottom = rectDest.top + rectDest.bottom;

	rtSrc.left = rectSrc.left;
	rtSrc.right = rectSrc.left + rectSrc.right;
	rtSrc.top = rectSrc.top;
	rtSrc.bottom = rectSrc.top + rectSrc.bottom;

	if (rtDest.right < rtSrc.left) return false;
	if (rtSrc.right < rtDest.left) return false;
	if (rtDest.bottom < rtSrc.top) return false;
	if (rtSrc.bottom < rtDest.top) return false;

	return true;
}

bool bgCollider::SphereInPoint(SPHERE& sphDest, POINT& posSrc)
{
	int iDisX = abs(sphDest.pos.x - posSrc.x);
	int iDisY = abs(sphDest.pos.y - posSrc.y);
	double iDistance = sqrt((iDisX*iDisX) + (iDisY*iDisY));

	if (iDistance <= (double)sphDest.rad)
		return true;

	return false;
}

bool bgCollider::SphereInVLine(SPHERE& sphDest, POINT& posSrc, int iEndY)
{
	int iDisX, iDisY;
	double iDistance;
	// ������ X��ǥ�� ���� ���� ���� ������
	if ((posSrc.x <= sphDest.pos.x + sphDest.rad) && (posSrc.x >= sphDest.pos.x - sphDest.rad))
	{
		iDisX = abs(sphDest.pos.x - posSrc.x);

		// ������ �� Y��ǥ�� ���� �߽������� �Ʒ��� ������
		if (posSrc.y > sphDest.pos.y)
		{
			iDisY = abs(sphDest.pos.y - posSrc.y);
		}
		// ������ �� Y��ǥ�� ���� �߽������� ���� ������
		else if (iEndY < sphDest.pos.y)
		{
			iDisY = abs(sphDest.pos.y - iEndY);
		}
		// ������ �� Y��ǥ �ȿ� ���� �߽����� ������
		else
		{
			iDisY = 0; // ���� �߽����� Y��ǥ�̹Ƿ� Y�Ÿ��� 0
		}

		iDistance = sqrt((iDisX*iDisX) + (iDisY*iDisY));

		if (iDistance <= (double)sphDest.rad)
			return true;
	}
	return false;
}

bool bgCollider::SphereInHLine(SPHERE& sphDest, POINT& posSrc, int iEndX)
{
	return false;
}

bool bgCollider::SphereInSphere(SPHERE& sphDest, SPHERE& sphSrc)
{
	int iDisX = abs(sphDest.pos.x - sphSrc.pos.x);
	int iDisY = abs(sphDest.pos.y - sphSrc.pos.y);
	double iDistance = sqrt((iDisX*iDisX) + (iDisY*iDisY));

	if (iDistance <= (double)(sphDest.rad + sphSrc.rad))
		return true;

	return false;
}

bool bgCollider::SphereInSphere(RECT& rectDest, RECT& rectSrc)
{
	return false;
}

bool bgCollider::SphereInSphereWH(RECT& rectDest, RECT& rectSrc)
{
	return false;
}
