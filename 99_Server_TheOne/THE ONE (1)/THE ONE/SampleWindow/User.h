#pragma once
#include "Bullet.h"
#include "Character.h"
#include "Protocol.h"
class User
{
public:
	char									m_name[10];
	SOCKET									m_sock;
	bool									m_bIngame;
	int										m_iNum;
	Character								m_Character;

public:			// 캐릭터 위치 보정
	float									m_fCharacterX;
	float									m_fCharacterY;
	float									m_fPositionTimer;
	float									m_fPositionTime;
	bool									m_bPositionrevise;

public:			// 총알
	list<Bullet*>							m_BulletList;
	list<UPACKET_BULLET_CREATE>				m_BulletDataList;
	int										m_iBulletCreate;
	int										m_iBulletNum;
	int										m_iBulletCnt;

public:			// 충돌
	float									m_fCollideX;
	float									m_fCollideY;

public:
	void									Reset();

public:
	User();
	virtual ~User();
};

