#pragma once
#include "Core.h"
#include "Enemy.h"
#include "SpriteManager.h"
#include "FSound.h"
#include "Bullet.h"
#include "Item.h"
#include "Spriter.h"
#include "PacketPool.h"
#include "User.h"

enum Direction { STOP =0, LEFT, UP , RIGHT, DOWN };

#define MAX_OBJECT 10
typedef	vector<RECT> DATA_ARRAY;

class Sample : public Core, public Singleton<Sample>
{
	//////////////// 게임
public:
	FSound									m_Sound;
	Object									m_Main;
	Object									m_Menu[2][3];
	int										m_iMenuNum;
	int										m_iMenuState[2];
	Object									m_BackGround;
	Object									m_CharacterLife;
	User									m_Client;
	TCHAR									m_szLife[MAX_PATH];
	int										m_iMenu;	
	float									m_fUserX;
	bool									m_GameStart;

public:			// 점수 
	TCHAR*									m_pszFileName = _T("text.txt");
	int										m_iScore;
	int										m_iRecordScore;
	TCHAR									m_szCurrentScore[MAX_PATH];
	TCHAR									m_szRecordScore[MAX_PATH];
	vector<DATA_ARRAY>						m_LetterList;
	int										m_iNumWord;
	Object									m_Letter;

public:			// 아이템
	list<Item*>								m_ItemList;
	list<UPACKET_ITEM_CREATE>				m_ItemDataList;
	int										m_iItemCreate;
	int										m_iItemNum;

public:			// 적기
	list<Enemy*>							m_EnemyList;
	list<UPACKET_ENEMY_CREATE>				m_EnemyDataList;
	RECT									m_Enemyrt[2];		// x, y, h, w
	int										m_iEnemyNum;
	int										m_iEnemyCreate;

public:			// 충돌
	bool									m_bCollide;
	float									m_fCollideX;
	float									m_fCollideY;

public:			// 스프라이트
	vector<DATA_ARRAY>						m_SpriteList;
	int										m_iNumSprite;
	list<Spriter*>							m_CollisionSpriterList;
	list<Spriter*>							m_BulletSpriterList;
	int										m_iNumBulletSpriter;

public:			// 게임 종료
	float									m_fEndEndTime;
	float									m_fEndTime;
	float									m_fEndTimer;
	float									m_fEndShowTime;
	Object									m_End;
	bool									m_bEnd;
	bool									m_bEndShow;
	bool									m_bGameOver;

public:
	bool									LoadData(TCHAR*, vector<DATA_ARRAY>&, int&);
	void									ReadScore(TCHAR*);
	void									WriteScore(TCHAR*);
	void									MenuOption();
	void									Reset();
	void									EndTimeOver();
	void									ShowTimeOver();
	void									SpriteSetRect(User*);


	///////////////// 네크워크
public:
	PacketPool								m_PacketPool;
	list<User*>								m_UserList;
	bool									m_UserDrop;
	SOCKET									m_UserSock;

public:
	void									MsgProc(HWND, UINT, WPARAM, LPARAM);
	void									GameStart(SOCKET, int);
	void									UserIn(SOCKET, int);

public:
	bool									Run();
	bool									Init();
	bool									Frame();
	bool									Render();
	bool									Release();
	
public:
	Sample();
	virtual ~Sample();
};

#define I_Sample Sample::GetInstance()

