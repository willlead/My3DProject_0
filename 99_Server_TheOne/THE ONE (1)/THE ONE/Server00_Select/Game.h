#pragma once
#include "Select.h"
#include "Thread.h"
#include "Timer.h"
#include <time.h>
#include "Enemy.h"
#include "Item.h"

enum Direction { STOP = 0, LEFT, UP, RIGHT, DOWN };

class Game : public Singleton<Game>, public Thread
{
public:
	int								m_iClientWidth;
	int								m_iClientHigh;

public:			// 유저
	float							m_fUserSpeed;
	int								m_iUserWidth;
	int								m_iUserHigh;
	float							m_fBulletSpeed;

public:			// 적기
//	list<Enemy*>					m_EnemyList;
	float							m_fEnemyTimer0;
	float							m_fEnemyTimer1;
	int								m_iEnemyCnt;
	float							m_fEnemyShowTime0;
	float							m_fEnemyShowTime1;
	float							m_fEnemySpeed;
	int								m_iEnemyWidth0;
	int								m_iEnemyHigh0;
	int								m_iEnemyWidth1;
	int								m_iEnemyHigh1;
	int								m_EnemyiX;			// 이전에 생성된 적기의 X좌표
	bool							m_bEnemyDelete;
	int								m_iEnemyNum;

public:			//아이템
//	list<Item*>						m_ItemList;
	float							m_fItemTimer;
	int								m_iItemCnt;
	float							m_fItemShowTime;
	int								m_iItemWidth;
	float							m_fItemSpeed;
	bool							m_bItemDelete;
	int								m_iItemNum;

public:
	bool							Init();
	bool							Run();

public:
	Game();
	virtual ~Game();
};

#define I_Game Game::GetInstance()

