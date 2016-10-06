#include "Bullet.h"
#include "Timer.h"

bool Bullet::Frame()
{
	m_fY -= I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fY < - m_iBulletHigh /2)
	{
		m_bUsed = false;
	}
	return true;
}

Bullet::Bullet()
{
	m_iBulletHigh = 23;
}


Bullet::~Bullet()
{
}
