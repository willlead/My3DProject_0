#include "Enemy.h"
#include "Timer.h"
#include "Sys.h"

bool Enemy::Frame()
{
	/*if (m_fX < 0)
	{
		m_iDirection[0] *= (-1);
		m_fX = 0.0;
	}
	if (m_fX>800 - m_rt.right)
	{
		m_iDirection[0] *= (-1);
	}
	if (m_fY < 0)
	{
		m_iDirection[1] *= (-1);
		m_fY = 0;
	}
	if (m_fY < 0 || m_fY>600 - m_rt.bottom)
	{
		m_iDirection[1] *= (-1);
		m_fY = 600 - m_rt.bottom;
	}

	m_fX += I_Timer.m_fSecondPerFrame*m_fSpeed*m_iDirection[0];
	m_fY += I_Timer.m_fSecondPerFrame*m_fSpeed*m_iDirection[1];
	Object::Frame();*/

	m_fY += I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fY > g_rtClient.bottom + m_rt.bottom / 2)
	{
		m_bUsed = false;
		m_bCollide = false;
	}
	Object::Frame();

	return true;
}

Enemy::Enemy()
{
	m_bUsed = true;
	m_iTypeIndex = 0;
}


Enemy::~Enemy()
{
}
