#include "Bullet.h"
#include "Timer.h"

bool Bullet::Frame()
{
	m_fY -= I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fY < - m_rt.bottom/2)
	{
		m_bUsed = false;
		m_bCollide = false;
	}
	Object::Frame();

	return true;
}

Bullet::Bullet()
{
	m_bUsed = true;
	m_bStart = false;
	m_iNumSpriter = 0;
}


Bullet::~Bullet()
{
}
