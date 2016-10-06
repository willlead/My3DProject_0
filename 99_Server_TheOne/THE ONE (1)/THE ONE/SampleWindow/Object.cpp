#include "Object.h"
#include "Sys.h"
#include "Timer.h"
#include <math.h>

void Object::SetPosition(int iX, int iY, float fSpeed)
{
	m_fX = iX;
	m_fY = iY;
	m_fSpeed = fSpeed;
}

void Object::SetSpeed(float fSpeed)
{
	m_fSpeed += I_Timer.m_fSecondPerFrame*fSpeed;
	if (m_fSpeed < 30.0f)
	{
		m_fSpeed = 30.0f;
	}
}

void Object::SetRect(int iX, int iY, int iW, int iH)
{
	m_rt.left = iX;
	m_rt.top = iY;
	m_rt.right = iW;
	m_rt.bottom = iH;
}

void Object::SetRect(RECT& rt)
{
	m_rt.left = rt.left;
	m_rt.top = rt.top;
	m_rt.right = rt.right;
	m_rt.bottom = rt.bottom;
}

bool Object::Init()
{
	m_hBrush = CreateSolidBrush(RGB(255, 255, 255));
	return true;
}

bool Object::Frame()
{
	m_rtCollision.left = m_fX - m_rt.right / 2;
	m_rtCollision.top = m_fY - m_rt.bottom / 2;
	m_rtCollision.right = m_rtCollision.left + m_rt.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rt.bottom;
	return true;
}

bool Object::Release()
{
	DeleteObject(m_hBrush);
	m_ColorBitmap->Release();
	if (m_MaskBitmap)
	{
		m_MaskBitmap->Release();
	}
	return true;
}

Object::Object()
{
	m_fSpeed = 0.0;
	m_bSent = false;
}


Object::~Object()
{
}
