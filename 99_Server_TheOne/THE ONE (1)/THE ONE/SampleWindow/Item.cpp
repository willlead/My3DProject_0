#include "Item.h"
#include "Timer.h"
#include "Sys.h"

bool Item::Frame()
{
	m_fY += I_Timer.m_fSecondPerFrame*m_fSpeed;
	if (m_fY >= g_rtClient.bottom + m_rt.bottom / 2)
	{
		m_bUsed = false;
	}
	Object::Frame();

	return true;
}

Item::Item()
{
	m_bUsed = true;
}


Item::~Item()
{
}
