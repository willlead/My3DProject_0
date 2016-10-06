#include "Character.h"
#include "Timer.h"
#include "Sys.h"

void Character::Up()
{
	m_fY -= I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fY < m_rt.bottom / 2)
	{
		m_fY = m_rt.bottom / 2;
	}
	if (m_bShow == true && m_fY <= 700)
	{
		m_bShow = false;
	}
}

void Character::Down()
{
	m_fY += I_Timer.m_fSecondPerFrame * m_fSpeed;
	if (m_fY > g_rtClient.bottom - m_rt.bottom / 2)
	{
		m_fY = g_rtClient.bottom - m_rt.bottom / 2;
	}
}

void Character::Left()
{
	m_fX -= I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fX < m_rt.right/2)
	{
		m_fX = m_rt.right / 2;
	}
}

void Character::Right()
{
	m_fX += I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fX > g_rtClient.right - m_rt.right / 2)
	{
		m_fX = g_rtClient.right - m_rt.right / 2;
	}
}
void Character::Reset()
{
	m_bUsed = -1;		// 대기상태

	m_bCollide = false;
	m_bSent = false;

	m_CharacterState = 0;
	m_bShow = false;
	m_bPlaySound = true;
	m_iLife = 3;
	m_iDirection = 0; //stop 상태

	m_bRebornShow = false;
	m_bReborn = false;
	m_fRebornEndTime = 2.0f;
	m_fRebornTimer = 0.0f;
	m_fRebornTime = 0.0f;
	m_fRebornShowTime = 0.3f;
}

Character::Character()
{	
	m_bUsed = -1;		// 대기상태

	m_bCollide = false;
	m_bSent = false;

	m_CharacterState = 0;
	m_bShow = false;
	m_bPlaySound = true;
	m_iLife = 3;
	m_iDirection = 0; //stop 상태

	m_bRebornShow = false;
	m_bReborn = false;
	m_fRebornEndTime = 2.0f;
	m_fRebornTimer = 0.0f;
	m_fRebornTime = 0.0f;
	m_fRebornShowTime = 0.3f;
}


Character::~Character()
{
}
