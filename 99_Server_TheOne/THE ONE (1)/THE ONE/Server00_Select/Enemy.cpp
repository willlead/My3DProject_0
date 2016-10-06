#include "Enemy.h"
#include "Timer.h"

bool Enemy::Frame()
{
	m_fY += I_Timer.m_fSecondPerFrame*m_fSpeed;
	//if (m_fY > g_rtClient.bottom + m_rt.bottom / 2)
	{
		m_bUsed = false;
	}

	return true;
}

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}
