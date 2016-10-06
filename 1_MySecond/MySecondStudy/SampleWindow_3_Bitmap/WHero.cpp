#include "WHero.h"

float g_fSecondPerFrame = 0.0f;

WHero::WHero()
{
}


WHero::~WHero()
{
}

bool WHero::Init()
{
	return true;
}

bool WHero::Frame()
{
	m_rtCollide.x = m_PosX - (m_rt.w / 2);
	m_rtCollide.y = m_PosY - (m_rt.h / 2);
	m_rtCollide.x = m_rtCollide.x + m_rt.w;
	m_rtCollide.x = m_rtCollide.y = m_rt.h;
	return true;
}

bool WHero::Render()
{
	BitBlt(m_Bitmap.m_hOffScreenDC,
		m_PosX, m_PosY,
		m_rt.w, m_rt.h,
		m_Bitmap.m_hMemDC,
		m_rt.x, m_rt.y, SRCCOPY);
	return true;
}

bool WHero::Release()
{
	m_Bitmap.Release();
	return true;
}

void WHero::Left()
{
	m_PosX -= g_fSecondPerFrame + m_fSpeed;
}

void WHero::Right()
{
	m_PosX += g_fSecondPerFrame + m_fSpeed;
}

void WHero::Up()
{
	m_PosY -= g_fSecondPerFrame + m_fSpeed;
}

void WHero::Down()
{
	m_PosY += g_fSecondPerFrame + m_fSpeed;
}
