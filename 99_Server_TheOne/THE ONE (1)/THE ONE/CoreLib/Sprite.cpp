#include "Sprite.h"
#include "Timer.h"
#include "Sys.h"

void Sprite::SetRectArray(vector<RECT> rtList, float fLifeTime)
{
	m_iNumFrame = rtList.size();
	for (int iList = 0;iList < m_iNumFrame; iList++)
	{
		m_rtList.push_back(rtList[iList]);
	}
	m_fLifeTime = fLifeTime;
	m_fSecPerRender = m_fLifeTime / m_iNumFrame;
	m_iCurrentFrame = 0;
	m_fTimer = 0.0;
}

bool Sprite::Init()
{
	m_iCurrentFrame = m_iNumFrame = 0;
	m_fLifeTime = m_fSecPerRender = m_fTimer = 0.0;
	return true;
}

bool Sprite::Frame()
{
	m_fTimer += I_Timer.m_fSecondPerFrame;
	if (m_fTimer > m_fSecPerRender)
	{
		if (++m_iCurrentFrame >= m_iNumFrame)
		{
			return false;
		}
		m_fTimer = 0.0;
	}
	m_rt = m_rtList[m_iCurrentFrame];
	return true;
}

bool Sprite::Release()
{
	return true;
}

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}
