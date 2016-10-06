#include "User.h"

void User::Reset()
{
	m_bIngame = false;
	m_fPositionTimer = 0.0f;
	m_fPositionTime = 1.0f;
	m_bPositionrevise = false;
	m_iBulletCreate = -1;		//대기 상태
	m_iBulletNum = 0;
	m_iBulletCnt = 0;
	m_Character.Reset();
}

User::User()
{
}


User::~User()
{
}
