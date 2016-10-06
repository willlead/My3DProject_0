#include "Game.h"
#include "Item.h"

bool Item::Frame()
{
	m_fY += I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fY >= I_Game.m_iClientHigh +m_iItemHigh / 2)
	{
		m_bUsed = false;
	}
	return true;
}

Item::Item()
{
	m_iItemHigh = 28;
}


Item::~Item()
{
}
