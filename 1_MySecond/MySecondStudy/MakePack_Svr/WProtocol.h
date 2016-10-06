#pragma once

#include <Windows.h>

#define PACKET_HEADER_SIZE 4

#pragma pack (push, 1)
typedef struct
{
	WORD len;
	WORD type;
} PACKET_HEADER;
typedef struct
{
	PACKET_HEADER ph;
	char msg[2048];
} UPACKET, *P_UPACKET;
typedef struct
{
	byte szName[13];
	int iIndex;
	int iData[50];
} CHARACTER_INFO;

typedef struct
{
	byte szName[13];
	int iIndex;
	int iData[10];
} ITEM_INFO;
#pragma pack(pop)	

#define PACKET_CHAT_MSG 1000
#define PACKET_GAME_START 2000
#define PACKET_GAME_END 3000
#define PACKET_CREATE_CHARACTER 4000
#define PACKET_MOVE_CHARACTER 5000


