#include "PacketPool.h"
#include "Sample.h"
#include "Sys.h"

void PacketPool::AddPacket(UPACKET packet)
{
	::EnterCriticalSection(&m_cs);
	m_RecvPacketList.push_back(packet);
	::LeaveCriticalSection(&m_cs);
}

void PacketPool::PacketProcess()
{			
	::EnterCriticalSection(&m_cs);
	list<UPACKET>::iterator iter;
	for (iter = m_RecvPacketList.begin(); iter != m_RecvPacketList.end(); iter++)
	{
		UPACKET packet = *iter;
		ProcessWork(&packet);

	}
	m_RecvPacketList.clear();
	::LeaveCriticalSection(&m_cs);
}

User* PacketPool::FindUser(SOCKET sock)
{
	list<User*>::iterator iter;
	for (iter = I_Sample.m_UserList.begin(); iter != I_Sample.m_UserList.end(); iter++)
	{
		User* pUser = (*iter);
		if (pUser->m_sock == sock)
		{
			return pUser;
		}
	}
	return 0;
}

void PacketPool::ProcessWork(UPACKET* pPacket)
{
	User* pUser = 0;
	switch (pPacket->ph.type)
	{
		case PACKET_GAME_JOIN:
		{
			UPACKET_GAME_JOIN* pUser = (UPACKET_GAME_JOIN*)pPacket->msg;
			if (I_Sample.m_Client.m_bIngame == TRUE)
			{
				I_Sample.UserIn(pUser->sock, pUser->UserNum);
			}
			else
			{
				I_Sample.GameStart(pUser->sock, pUser->UserNum);
			}

		}break;
		case PACKET_USER_DROP:
		{
			SOCKET* pSock = (SOCKET*)pPacket->msg;
			I_Sample.m_UserSock = *pSock;
			I_Sample.m_UserDrop = true;
		}break;
		case PACKET_GAME_STARTED:
		{
			MessageBox(g_hWnd, _T("게임이 이미 시작되었습니다."), _T("알림"), MB_OK);
		}break;
		case PACKET_USER_POSITION:
		{
			UPACKET_USER_POSITION* pPosition = (UPACKET_USER_POSITION*)pPacket->msg;
			pUser = FindUser(pPosition->sock);
			pUser->m_bPositionrevise = true;
			pUser->m_fCharacterX = pPosition->fX;
			pUser->m_fCharacterY = pPosition->fY;
		}break;
		case PACKET_USER_MOVE:
		{
			UPACKET_USER_MOVE* move = (UPACKET_USER_MOVE*)pPacket->msg;
			pUser = FindUser(move->sock);
			if (pUser == 0)			// 유저가 나간 이후 메시지 수신 시 발생
			{
				break;
			}
			switch (move->iDirection)
			{
				case STOP:
				{
					pUser->m_Character.m_iDirection = STOP;
				}break;
				case UP:
				{
					pUser->m_Character.m_iDirection = UP;
				}break;
				case DOWN:
				{
					pUser->m_Character.m_iDirection = DOWN;
				}break;
				case LEFT:
				{
					pUser->m_Character.m_iDirection = LEFT;
				}break;
				case RIGHT:
				{
					pUser->m_Character.m_iDirection = RIGHT;
				}break;
			}
			I_Sample.SpriteSetRect(pUser);
		}break;
		case PACKET_USER_COLLIDE:
		{
			UPACKET_USER_COLLIDE* collide = (UPACKET_USER_COLLIDE*)pPacket->msg;
			pUser = FindUser(collide->sock);
			pUser->m_Character.m_bCollide = true;
			pUser->m_fCollideX = collide->CollidefX;
			pUser->m_fCollideY = collide->CollidefY;
		}break;
		case PACKET_BULLET_CREATE:
		{
			UPACKET_BULLET_CREATE* pBullet = (UPACKET_BULLET_CREATE*)pPacket->msg;
			pUser = FindUser(pBullet->sock);
			pUser->m_BulletDataList.push_back(*pBullet);
		}break;
		case PACKET_BULLET_DELETE:
		{
			UPACKET_BULLET_DELETE* pBullet = (UPACKET_BULLET_DELETE*)pPacket->msg;
			pUser = FindUser(pBullet->sock);
			pUser->m_iBulletCreate = false;
			pUser->m_iBulletNum = pBullet->BulletNum;
		}break;
		case PACKET_ENEMY_CREATE:
		{
			UPACKET_ENEMY_CREATE* pEnemy = (UPACKET_ENEMY_CREATE*)pPacket->msg;
			I_Sample.m_EnemyDataList.push_back(*pEnemy);
		}break;
		case PACKET_ENEMY_DELETE:
		{
			UPACKET_ENEMY_DELETE* pEnemy = (UPACKET_ENEMY_DELETE*)pPacket->msg;
			I_Sample.m_iEnemyCreate = false;
			I_Sample.m_iEnemyNum = pEnemy->EnemyNum;
		}break;
		case PACKET_ENEMY_COLLIDE:
		{
			UPACKET_ENEMY_COLLIDE* collide = (UPACKET_ENEMY_COLLIDE*)pPacket->msg;
			I_Sample.m_bCollide = true;
			I_Sample.m_fCollideX = collide->CollidefX;
			I_Sample.m_fCollideY = collide->CollidefY;

		}break;
		case PACKET_ITEM_CREATE:
		{
			UPACKET_ITEM_CREATE* pItem = (UPACKET_ITEM_CREATE*)pPacket->msg;
			I_Sample.m_ItemDataList.push_back(*pItem);
		}break;
		case PACKET_ITEM_DELETE:
		{
			UPACKET_ITEM_DELETE* pItem = (UPACKET_ITEM_DELETE*)pPacket->msg;
			I_Sample.m_iItemCreate = false;
			I_Sample.m_iItemNum = pItem->ItemNum;
		}break;
	}
}

PacketPool::PacketPool()
{
	::InitializeCriticalSection(&m_cs);
}


PacketPool::~PacketPool()
{
	::DeleteCriticalSection(&m_cs);
}
