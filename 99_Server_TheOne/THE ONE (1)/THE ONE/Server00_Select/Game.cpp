#include "Game.h"

bool Game::Init()
{
	m_iEnemyCnt = 0;
	m_fEnemyTimer0 = m_fEnemyTimer1 = 0.0f;
	m_fEnemyShowTime0 = 2.0f;
	m_fEnemyShowTime1 = 5.0f;
	m_iClientWidth = 550;
	m_iClientHigh = 800;
	m_iEnemyWidth0 = 44;
	m_iEnemyHigh0 = 75;
	m_iEnemyWidth1 = 50;
	m_iEnemyHigh1 = 58;
	m_EnemyiX = 0;
	m_fEnemySpeed = 100.0f;
	m_fItemTimer = 0.0f;
	m_fItemShowTime = 5.0f;
	m_iItemWidth = 26;
	m_iItemCnt = 0;
	m_fItemSpeed = 100.0f;
	m_fUserSpeed = 350.0f;
	m_iUserWidth = 42;
	m_iUserHigh = 60;
	m_bEnemyDelete = false;
	m_fBulletSpeed = 400.0f;
	m_bItemDelete = false;

	Create();
	return true;
}

bool Game::Run()
{
	I_Timer.Init();

	while (1)
	{
		if (I_Select.m_UserList.size() == 0)
		{
			m_bStart = false;
			break;
		}

		I_Timer.Frame();

		//enemy create	
		srand((unsigned)time(NULL));
		m_fEnemyTimer0 += I_Timer.m_fSecondPerFrame;
		m_fEnemyTimer1 += I_Timer.m_fSecondPerFrame;

		if (m_fEnemyTimer0 >= m_fEnemyShowTime0 || m_fEnemyTimer1 >= m_fEnemyShowTime1)
		{
			UPACKET_ENEMY_CREATE enemy;
			enemy.EnemySpeed = m_fEnemySpeed;
			int iX = 0;
			if (m_fEnemyTimer0 >= m_fEnemyShowTime0)
			{				
				enemy.EnemyNum = m_iEnemyCnt++;
				enemy.Enemytype = 0;
				while (1)
				{
					iX = rand()*rand() % m_iClientWidth;
					if (iX > m_iClientWidth - m_iEnemyWidth0 / 2)
					{
						iX = m_iClientWidth - m_iEnemyWidth0 / 2;
					}
					if (iX < m_iEnemyWidth0 / 2)
					{
						iX = m_iEnemyWidth0 / 2;
					}
					if (-50 > iX - m_EnemyiX || iX - m_EnemyiX > 50)
					{
						break;
					}
				}
				m_fEnemyTimer0 = 0.0;
				m_EnemyiX = iX;
				enemy.EnemyfX = iX;
				enemy.EnemyfY = -m_iEnemyHigh0 / 2;
				UPACKET packet;
				ZeroMemory(&packet, sizeof(packet));
				packet.ph.type = PACKET_ENEMY_CREATE;
				memcpy(packet.msg, (char*)&enemy, sizeof(enemy));
				packet.ph.len = PACKET_HEADER_SIZE + sizeof(enemy);
				I_Select.BroadCast(&packet);

				/*Enemy* pEnemy = new Enemy;
				pEnemy->m_fX = iX;
				pEnemy->m_fY = -m_iEnemyHigh0 / 2;
				pEnemy->m_iNum = enemy.EnemyNum;
				pEnemy->m_iTypeIndex = 0;
				pEnemy->m_fSpeed = m_fEnemySpeed;
				m_EnemyList.push_back(pEnemy);*/
			}
			if (m_fEnemyTimer1 >= m_fEnemyShowTime1)
			{
				enemy.EnemyNum = m_iEnemyCnt++;
				enemy.Enemytype = 1;
				while (1)
				{
					iX = rand()*rand() % m_iClientWidth;
					if (iX > m_iClientWidth - m_iEnemyWidth1 / 2)
					{
						iX = m_iClientWidth - m_iEnemyWidth1 / 2;
					}
					if (iX < m_iEnemyWidth1 / 2)
					{
						iX = m_iEnemyWidth1 / 2;
					}
					if (-50 > iX - m_EnemyiX || iX - m_EnemyiX > 50)
					{
						break;
					}
				}
				m_fEnemyTimer1 = 0.0;
				m_EnemyiX = iX;
				enemy.EnemyfX = iX;
				enemy.EnemyfY = -m_iEnemyHigh1 / 2;
				UPACKET packet;
				ZeroMemory(&packet, sizeof(packet));
				packet.ph.type = PACKET_ENEMY_CREATE;
				memcpy(packet.msg, (char*)&enemy, sizeof(enemy));
				packet.ph.len = PACKET_HEADER_SIZE + sizeof(enemy);
				I_Select.BroadCast(&packet);

				/*Enemy* pEnemy = new Enemy;
				pEnemy->m_fX = iX;
				pEnemy->m_fY = -m_iEnemyHigh1 / 2;
				pEnemy->m_iNum = enemy.EnemyNum;
				pEnemy->m_iTypeIndex = 1;
				pEnemy->m_fSpeed = m_fEnemySpeed;
				m_EnemyList.push_back(pEnemy);*/
			}
		}

		//item create
		m_fItemTimer += I_Timer.m_fSecondPerFrame;
		if (m_fItemTimer >= m_fItemShowTime)
		{
			UPACKET_ITEM_CREATE item;
			item.ItemNum = m_iItemCnt++;
			int iX = rand()*rand() % m_iClientWidth;
			if (iX > m_iClientWidth - m_iItemWidth / 2)
			{
				iX = m_iClientWidth - m_iItemWidth / 2;
			}
			if (iX < m_iItemWidth / 2)
			{
				iX = m_iItemWidth / 2;
			}
			m_fItemTimer = 0.0;
			item.ItemfX = iX;
			item.ItemfY = -m_iEnemyHigh1 / 2;
			item.ItemSpeed = m_fItemSpeed;
			UPACKET packet;
			ZeroMemory(&packet, sizeof(packet));
			packet.ph.type = PACKET_ITEM_CREATE;
			memcpy(packet.msg, (char*)&item, sizeof(item));
			packet.ph.len = PACKET_HEADER_SIZE + sizeof(item);
			I_Select.BroadCast(&packet);

			/*Item* pItem = new Item;
			pItem->m_fX = iX;
			pItem->m_fY = -m_iEnemyHigh1 / 2;
			pItem->m_iNum = item.ItemNum;
			pItem->m_fSpeed = m_fItemSpeed;
			m_ItemList.push_back(pItem);*/
		}

		//bullet create
		//list<User*>::iterator iter;
		//for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
		//{
		//	User* pUser = (*iter);
		//	if (pUser->m_BulletCreate == true)
		//	{
		//		pUser->m_BulletCreate = -1;
		//		Bullet* pBullet = new Bullet;
		//		pBullet->m_iNum = pUser->m_iBulletNum;
		//		pBullet->m_fX = pUser->m_fBulletfX;
		//		pBullet->m_fY = pUser->m_fBulletfY;
		//		pBullet->m_fSpeed = m_fBulletSpeed;
		//		pUser->m_BulletList.push_back(pBullet);
		//	}
		//}
		
		//user frame
	/*	for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
		{
			User* pUser = (*iter);
			switch (pUser->m_iDirection)
			{
				case LEFT:
				{
					pUser->m_fX -= I_Timer.m_fSecondPerFrame*m_fUserSpeed;
					if (pUser->m_fX < m_iUserWidth / 2)
					{
						pUser->m_fX = m_iUserWidth / 2;
					}
				}break;
				case UP:
				{
					pUser->m_fY -= I_Timer.m_fSecondPerFrame*m_fUserSpeed;
					if (pUser->m_fY < m_iUserHigh / 2)
					{
						pUser->m_fY = m_iUserHigh / 2;
					}
				}break;
				case RIGHT:
				{
					pUser->m_fX += I_Timer.m_fSecondPerFrame*m_fUserSpeed;
					if (pUser->m_fX > m_iClientWidth - m_iUserWidth / 2)
					{
						pUser->m_fX = m_iClientWidth - m_iUserWidth / 2;
					}
				}break;
				case DOWN:
				{
					pUser->m_fY += I_Timer.m_fSecondPerFrame*m_fUserSpeed;
					if (pUser->m_fY > m_iClientHigh - m_iUserHigh / 2)
					{
						pUser->m_fY = m_iClientHigh - m_iUserHigh / 2;
					}
				}break;
			}
		}*/

		//enemy frame
	/*	list<Enemy*>::iterator Enemyiter;
		for (Enemyiter = m_EnemyList.begin();Enemyiter != m_EnemyList.end();Enemyiter++)
		{
			Enemy* pEnemy = (*Enemyiter);
			pEnemy->m_fY += I_Timer.m_fSecondPerFrame*(pEnemy->m_fSpeed);
			if (pEnemy->m_iTypeIndex == 0)
			{
				if (pEnemy->m_fY > m_iClientHigh + m_iEnemyHigh0 / 2)
				{
					pEnemy->m_bUsed = false;
				}
			}
			else
			{
				if (pEnemy->m_fY > m_iClientHigh + m_iEnemyHigh1 / 2)
				{
					pEnemy->m_bUsed = false;
				}
			}
		}*/
		m_fEnemySpeed += I_Timer.m_fSecondPerFrame * 2;
		m_fEnemyShowTime0 -= I_Timer.m_fSecondPerFrame / 30;
		m_fEnemyShowTime1 -= I_Timer.m_fSecondPerFrame / 30;
		if (m_fEnemyShowTime0 < 0.5f)
		{
			m_fEnemyShowTime0 = 0.5f;
		}
		if (m_fEnemyShowTime1 < 0.5f)
		{
			m_fEnemyShowTime1 = 0.5f;
		}

		// bullet frame
		/*for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
		{
			User* pUser = (*iter);
			if (pUser->m_BulletList.size() != 0)
			{
				list<Bullet*>::iterator Bulletiter;
				for (Bulletiter = pUser->m_BulletList.begin();Bulletiter != pUser->m_BulletList.end();Bulletiter++)
				{
					(*Bulletiter)->Frame();
				}
			}
		}*/

		//item frame
	/*	list<Item*>::iterator Itemiter;
		for (Itemiter = m_ItemList.begin();Itemiter != m_ItemList.end();Itemiter++)
		{
			(*Itemiter)->Frame();
		}*/
		m_fItemSpeed += I_Timer.m_fSecondPerFrame * 1.5;
		m_fItemShowTime -= I_Timer.m_fSecondPerFrame / 20;
		if (m_fItemShowTime < 1.5f)
		{
			m_fItemShowTime = 1.5f;
		}


		// bullet erase
	/*	for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
		{
			User* pUser = (*iter);
			list<Bullet*>::iterator Bulletiter = pUser->m_BulletList.begin();
			list<Bullet*>::iterator DelBulletiter;
			if (pUser->m_BulletCreate == false)
			{
				pUser->m_BulletCreate = -1;				
				while(Bulletiter != pUser->m_BulletList.end())
				{
					if ((*Bulletiter)->m_iNum == pUser->m_iBulletNum)
					{
						delete *Bulletiter;
						DelBulletiter = Bulletiter;
						Bulletiter++;
						pUser->m_BulletList.erase(DelBulletiter);
					}
					else
					{
						Bulletiter++;
					}
				}
			}
			Bulletiter = pUser->m_BulletList.begin();
			while(Bulletiter != pUser->m_BulletList.end())
			{
				if ((*Bulletiter)->m_bUsed == false)
				{
					delete *Bulletiter;
					DelBulletiter = Bulletiter;
					Bulletiter++;
					pUser->m_BulletList.erase(DelBulletiter);
				}
				else
				{
					Bulletiter++;
				}
			}
		}*/

		// enemy erase
	/*	list<Enemy*>::iterator DelEnemyiter;
		if (m_bEnemyDelete == true)
		{
			m_bEnemyDelete = false;
			Enemyiter = m_EnemyList.begin();
			while (Enemyiter != m_EnemyList.end())
			{
				if ((*Enemyiter)->m_iNum == m_iEnemyNum)
				{
					delete *Enemyiter;
					DelEnemyiter = Enemyiter;
					Enemyiter++;
					m_EnemyList.erase(DelEnemyiter);
				}
				else
				{
					Enemyiter++;
				}
			}
		}
		Enemyiter = m_EnemyList.begin();
		while (Enemyiter != m_EnemyList.end())
		{
			if ((*Enemyiter)->m_bUsed == false)
			{
				delete *Enemyiter;
				DelEnemyiter = Enemyiter;
				Enemyiter++;
				m_EnemyList.erase(DelEnemyiter);
			}
			else
			{
				Enemyiter++;
			}
		}*/

		//Item erase
	/*	list<Item*>::iterator DelItemiter;
		if (m_bItemDelete == true)
		{
			m_bItemDelete = false;
			Itemiter = m_ItemList.begin();
			while (Itemiter != m_ItemList.end())
			{
				if ((*Itemiter)->m_iNum == m_iItemNum)
				{
					delete (*Itemiter);
					DelItemiter = Itemiter;
					Itemiter++;
					m_ItemList.erase(DelItemiter);
				}
				else
				{
					Itemiter++;
				}
			}
		}
		Itemiter = m_ItemList.begin();
		while (Itemiter != m_ItemList.end())
		{
			if ((*Itemiter)->m_bUsed == false)
			{
				delete (*Itemiter);
				DelItemiter = Itemiter;
				Itemiter++;
				m_ItemList.erase(DelItemiter);
			}
			else
			{
				Itemiter++;
			}
		}*/
				
		Sleep(100);
	}
	return true;
}

Game::Game()
{
}


Game::~Game()
{
}
