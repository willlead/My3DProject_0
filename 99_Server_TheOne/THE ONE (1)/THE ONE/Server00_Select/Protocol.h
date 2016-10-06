#pragma once
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <list>

using namespace std;

#define PACKET_HEADER_SIZE 4

#pragma pack(push, 1)

typedef struct {
	WORD len;
	WORD type;
} PACKET_HEADER;

typedef struct {
	PACKET_HEADER ph;
	char		  msg[2048];
} UPACKET;

typedef struct {
	SOCKET		sock;
	int			UserNum;
} UPACKET_GAME_JOIN;

typedef struct {
	SOCKET		sock;
	float		fX;
	float		fY;
} UPACKET_USER_POSITION;

typedef struct {
	SOCKET		sock;
	int			iDirection;
	float		fX;
	float		fY;
} UPACKET_USER_MOVE;

typedef struct {
	SOCKET		sock;
	float		CollidefX;
	float		CollidefY;
} UPACKET_USER_COLLIDE;

typedef struct {
	SOCKET		sock;
	int			BulletNum;
	float		BulletfX;
	float		BulletfY;
} UPACKET_BULLET_CREATE;

typedef struct {
	SOCKET		sock;
	int			BulletNum;
} UPACKET_BULLET_DELETE;

typedef struct {
	int			EnemyNum;
	int			Enemytype;
	float		EnemyfX;
	float		EnemyfY;
	float		EnemySpeed;
} UPACKET_ENEMY_CREATE;

typedef struct {
	int			EnemyNum;
} UPACKET_ENEMY_DELETE;

typedef struct {
	float		CollidefX;
	float		CollidefY;
} UPACKET_ENEMY_COLLIDE;

typedef struct {
	int			ItemNum;
	float		ItemfX;
	float		ItemfY;
	float		ItemSpeed;
} UPACKET_ITEM_CREATE;

typedef struct {
	int			ItemNum;
} UPACKET_ITEM_DELETE;

#pragma pack(pop)

#define PACKET_GAME_JOIN			0x0001
#define PACKET_USER_DROP			0x0002
#define PACKET_GAME_STARTED			0x0004
#define PACKET_USER_POSITION		0x0008

#define PACKET_USER_MOVE			0x0011
#define PACKET_USER_COLLIDE			0x0012

#define PACKET_BULLET_CREATE		0x0021
#define PACKET_BULLET_DELETE		0X0022

#define PACKET_ENEMY_CREATE			0x0041
#define PACKET_ENEMY_DELETE			0x0042
#define PACKET_ENEMY_COLLIDE		0x0044

#define PACKET_ITEM_CREATE			0x0081
#define PACKET_ITEM_DELETE			0x0082