#ifndef TBASIS_TPROTOCOL
#define TBASIS_TPROTOCOL
#include <windows.h>
#pragma  pack(push, 1 )

typedef struct {
	WORD	len;	// msg 바이트 크기
	WORD	type;	// 어떤 패킷이다.
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;

// 서버
#define PACKET_CHAT_NAME_REQ 1000 // 대화명 입력해라.
// 클라이언트
#define PACKET_CHAT_NAME_ACK 1001 // 대화명 입력 전송.
// 보통 채팅 메세지
#define PACKET_CHAT_MSG 1002 // 대화명 입력 전송.

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8방향
}TPACKET_USER_POSITION;
#define PACKET_USER_POSITION	3000

#pragma pack(pop)

#endif