#include "Spriter.h"

void Spriter::Set()
{
	m_fTimer = m_pSprite->m_fTimer;
	m_iCurrentFrame = m_pSprite->m_iCurrentFrame;
}

void Spriter::SetSprite()
{
	m_pSprite->m_fTimer = m_fTimer;
	m_pSprite->m_iCurrentFrame = m_iCurrentFrame;
	m_pSprite->m_fX = m_fX;
	m_pSprite->m_fY = m_fY;
}

bool Spriter::Init()
{
	return true;
}

bool Spriter::Frame()
{
	return true;
}

bool Spriter::Render()
{
	return true;
}

bool Spriter::Release()
{
	return true;
}

Spriter::Spriter()
{
	m_iCurrentFrame = 0;
	m_fTimer = 0.0;
	m_bUsed = true;
}


Spriter::~Spriter()
{
}
