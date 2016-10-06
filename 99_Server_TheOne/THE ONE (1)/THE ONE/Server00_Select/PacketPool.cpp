#include "PacketPool.h"
#include "Select.h"
#include "Game.h"

void PacketPool::AddPacket(UPACKET packet)
{
	m_RecvPacketList.push_back(packet);
}

void PacketPool::PacketProcess()
{			
	list<UPACKET>::iterator iter;
	for (iter = m_RecvPacketList.begin(); iter != m_RecvPacketList.end(); iter++)
	{
		UPACKET packet = *iter;
		ProcessWork(&packet);
	}
	m_RecvPacketList.clear();
}

void PacketPool::ProcessWork(UPACKET* pPacket)
{
	switch (pPacket->ph.type)
	{
		case PACKET_USER_DROP:
		{
			SOCKET* pSock = (SOCKET*)pPacket->msg;
			list<User*>::iterator iter;

			I_Select.BroadCast(pPacket);

			for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
			{
				User* pUser = (*iter);
				if (pUser->m_sock == *pSock)
				{
					pUser->m_connect = false;
				}
			}

			printf("PACKET_USER_DROP\n");
		}break;
		case PACKET_USER_POSITION:
		{
			I_Select.BroadCast(pPacket);

		//	printf("PACKET_USER_POSITION\n");
		}break;
		case PACKET_USER_MOVE:
		{
			/*UPACKET_USER_MOVE* move = (UPACKET_USER_MOVE*)pPacket->msg;
			list<User*>::iterator iter;
			for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
			{
				User* pUser = (*iter);
				if (pUser->m_sock == move->sock)
				{
					pUser->m_fX = move->fX;
					pUser->m_fY = move->fY;
					pUser->m_iDirection = move->iDirection;
					break;
				}
			}*/
			UPACKET_USER_MOVE* move = (UPACKET_USER_MOVE*)pPacket->msg;
			I_Select.BroadCast(pPacket);

			printf("PACKET_USER_MOVE\n");			
		}break;
		case PACKET_USER_COLLIDE:
		{
			I_Select.BroadCast(pPacket);

			printf("PACKET_USER_COLLIDE\n");
		}break;
		case PACKET_BULLET_CREATE:
		{
			/*UPACKET_BULLET_CREATE* pBullet = (UPACKET_BULLET_CREATE*)pPacket->msg;
			list<User*>::iterator iter;
			for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
			{
				User* pUser = (*iter);
				if (pUser->m_sock == pBullet->sock)
				{
					pUser->m_BulletCreate = true;
					pUser->m_iBulletNum = pBullet->BulletNum;
					pUser->m_fBulletfX = pBullet->BulletfX;
					pUser->m_fBulletfY = pBullet->BulletfY;
					break;
				}
			}*/
						
			I_Select.BroadCast(pPacket);

			printf("PACKET_BULLET_CREATE\n");
		}break;
		case PACKET_BULLET_DELETE:
		{
			/*UPACKET_BULLET_DELETE* pBullet = (UPACKET_BULLET_DELETE*)pPacket->msg;
			list<User*>::iterator iter;
			for (iter = I_Select.m_UserList.begin();iter != I_Select.m_UserList.end();iter++)
			{
				User* pUser = (*iter);
				if (pUser->m_sock == pBullet->sock)
				{
					pUser->m_BulletCreate = false;
					pUser->m_iBulletNum = pBullet->BulletNum;
					break;
				}
			}*/

			I_Select.BroadCast(pPacket);

			printf("PACKET_BULLET_DELETE\n");
		}break;
		case PACKET_ENEMY_DELETE:
		{
			/*UPACKET_ENEMY_DELETE* pEnemy = (UPACKET_ENEMY_DELETE*)pPacket->msg;
			I_Game.m_bEnemyDelete = true;
			I_Game.m_iEnemyNum = pEnemy->EnemyNum;*/

			I_Select.BroadCast(pPacket);

			printf("PACKET_ENEMY_DELETE\n");
		}break;
		case PACKET_ENEMY_COLLIDE:
		{
			I_Select.BroadCast(pPacket);

			printf("PACKET_ENEMY_COLLIDE\n");
		}break;
		case PACKET_ITEM_DELETE:
		{
			/*UPACKET_ITEM_DELETE* pItem = (UPACKET_ITEM_DELETE*)pPacket->msg;
			I_Game.m_bItemDelete = true;
			I_Game.m_iItemNum = pItem->ItemNum;*/

			I_Select.BroadCast(pPacket);

			printf("PACKET_ITEM_DELETE\n");
		}break;
	}
}

PacketPool::PacketPool()
{

}


PacketPool::~PacketPool()
{

}
