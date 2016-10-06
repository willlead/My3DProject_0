#include "Sample.h"
#include "Collision.h"
#include "Timer.h"
#include <time.h>
#include "Sys.h"
#include "AsyncSelect.h"

template <class T>
void ERASE(T* DataList)
{
	T::iterator Dataiter = DataList->begin();
	T::iterator DelDataiter;
	while (Dataiter != DataList->end())
	{
		delete (*Dataiter);
		DelDataiter = Dataiter;
		Dataiter++;
		DataList->erase(DelDataiter);
	}
	DataList->clear();
}

bool Sample::LoadData(TCHAR* pszFileName, vector<DATA_ARRAY>& List, int& iNumData)
{
	FILE* fp;
	TCHAR Buffer[MAX_PATH];
	TCHAR TmpStr[MAX_PATH];
	int iTmp = 0;
	int iNum = 0;
	RECT rt;

	_wfopen_s(&fp, pszFileName, _T("r"));
	if (fp == 0)
	{
		return 0;
	}
	_fgetts(Buffer, _countof(Buffer), fp);
	_stscanf_s(Buffer, _T("%s%d"), TmpStr, _countof(TmpStr), &iNumData);

	List.resize(iNumData);

	for (int iData = 0; iData < iNumData; iData++)
	{
		_fgetts(Buffer, _countof(Buffer), fp);
		_stscanf_s(Buffer, _T("%s%d"), TmpStr, _countof(TmpStr), &iNum);

		List[iData].resize(iNum);

		for (int jData = 0;jData < iNum; jData++)
		{
			_fgetts(Buffer, _countof(Buffer), fp);
			_stscanf_s(Buffer, _T("%d %d%d%d%d"), &iTmp, &rt.left, &rt.top, &rt.right, &rt.bottom);
			List[iData][jData] = rt;
		}
	}
	fclose(fp);
	return true;
}

void Sample::ReadScore(TCHAR* pszFileName)
{
	TCHAR Buffer[MAX_PATH];
	FILE* fp;
	_wfopen_s(&fp, _T("text.txt"), _T("r"));
	if (fp == 0)
	{
		return;
	}
	fread(m_szRecordScore, 1, sizeof(m_szRecordScore), fp);
	_stscanf_s(m_szRecordScore, _T("%s %d"), Buffer, _countof(Buffer), &m_iRecordScore);
	fclose(fp);
}

void Sample::WriteScore(TCHAR* pszFileName)
{
	FILE* fp;
	_wfopen_s(&fp, _T("text.txt"),_T("w"));
	fwrite(m_szRecordScore, 1, sizeof(m_szRecordScore), fp);

	fclose(fp);
}

void Sample::MenuOption()
{	
	m_Sound.Play(6, false);
	if (m_iMenuState[0] == 2)
	{
		Sleep(250);
		m_Sound.Stop(6);
		m_iMenu = 0;
	}

	if (m_iMenuState[1] == 2)
	{
		Sleep(250);
		m_Sound.Stop(6);
		DestroyWindow(m_hWnd);
	}
}

void Sample::Reset()
{
	//////////////// game
	m_Client.Reset();
	m_Client.m_Character.SetRect(90, 1, 42, 60);
	ERASE(&m_Client.m_BulletList);
	m_GameStart = false;

	//score
	m_iScore = 0;
	ReadScore(m_pszFileName);

	//item
	ERASE(&m_ItemList);
	m_iItemCreate = -1;

	//enemy
	ERASE(&m_EnemyList);
	m_iEnemyCreate = -1;		// 대기 상태

	//collision
	m_bCollide = false;

	//sprite
	m_iNumSprite = 0;
	ERASE(&m_CollisionSpriterList);
	ERASE(&m_BulletSpriterList);
	m_iNumBulletSpriter = 0;

	// game end
	m_fEndTime = 0.0f;
	m_fEndTimer = 0.0f;
	m_bEnd = false;
	m_bEndShow = false;
	m_bGameOver = false;

	//////////// network
	m_PacketPool.m_RecvPacketList.clear();

	list<User*>::iterator iter = m_UserList.begin();
	list<User*>::iterator Deliter;
	while (iter != m_UserList.end())
	{
		User* pUser = (*iter);
		if (pUser != &m_Client)
		{
			delete (*iter);
		}
		Deliter = iter;
		iter++;
		m_UserList.erase(Deliter);
	}

	m_UserDrop = false;
}

void Sample::EndTimeOver()
{
	m_bEnd = true;
	m_fEndTime = 0.0f;
	m_fEndTimer = 0.0f;
	m_bEndShow = false;
	m_bGameOver = false;
}

void Sample::ShowTimeOver()
{
	if (m_bEndShow)
	{
		m_bEndShow = false;
	}
	else
	{
		m_bEndShow = true;
	}
	m_fEndTimer = 0.0;
}

void Sample::SpriteSetRect(User* pUser)
{
	Sprite* pSprite = 0;
	switch (pUser->m_Character.m_iDirection)
	{
		case UP:
		{
			pSprite = I_SpriteManager.Find(1 + 6 * (pUser->m_iNum - 1));
			pSprite->SetRectArray(m_SpriteList[0], 1.0f);
		}break;
		case DOWN:
		{
			pSprite = I_SpriteManager.Find(1 + 6 * (pUser->m_iNum - 1));
			pSprite->SetRectArray(m_SpriteList[0], 1.0f);
		}break;
		case LEFT:
		{
			pSprite = I_SpriteManager.Find(2 + 6 * (pUser->m_iNum - 1));
			pSprite->SetRectArray(m_SpriteList[1], 0.3f);
		}break;
		case RIGHT:
		{
			pSprite = I_SpriteManager.Find(3 + 6 * (pUser->m_iNum - 1));
			pSprite->SetRectArray(m_SpriteList[2], 0.3f);
		}break;
	}
}

void Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SOCKET:
		{
			if (I_AsyncSelect.NetworkProc(wParam, lParam) == false)
			{
				//MessageBox(g_hWnd, _T("접속 종료"), _T("알림"), MB_OK);
				//DWORD ret = GetLastError();
				if (m_bGameOver == true)
				{
					break;
				}
				I_AsyncSelect.m_netConnect = false;
				m_iMenu = -1;
			}
		}break;
	}
}

bool Sample::Run()
{
	GameInit();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			MsgEvent(msg);
		}
		else
		{
			m_PacketPool.PacketProcess();
			GameRun();
		}
	}
	GameRelease();
	return true;
}

void Sample::GameStart(SOCKET sock, int iNum)			// 서버 접속 후 소켓 정보 저장, 캐릭터 등장
{
	m_Client.m_bIngame = true;
	m_Client.m_sock = sock;
	m_Client.m_iNum = iNum;
	m_Client.m_Character.SetRect(90, 1, 42, 60);
	m_Client.m_Character.SetPosition(183 * iNum, 750, 350.0f);
	if (iNum == 1)
	{
		m_Client.m_Character.Load(_T("Data/bitmap1.bmp"), _T("Data/bitmap2.bmp"));
	}
	else if(iNum == 2)
	{
		m_Client.m_Character.Load(_T("Data/bitmap3.bmp"), _T("Data/bitmap2.bmp"));
	}
	m_UserList.push_back(&m_Client);
}				

void Sample::UserIn(SOCKET sock, int iNum)
{
	m_Client.m_Character.m_bShow = true;
	m_Client.m_Character.m_bUsed = true;
	User* pUser = new User;
	pUser->m_sock = sock;
	pUser->m_iNum = iNum;
	pUser->m_Character.m_bUsed = true;
	pUser->m_Character.m_bShow = true;
	pUser->m_Character.SetRect(90, 1, 42, 60);
	pUser->m_Character.SetPosition(183 * iNum, 750, 350.0f);
	if (iNum == 1)
	{
		pUser->m_Character.Load(_T("Data/bitmap1.bmp"), _T("Data/bitmap2.bmp"));
	}
	else if (iNum == 2)
	{
		pUser->m_Character.Load(_T("Data/bitmap3.bmp"), _T("Data/bitmap2.bmp"));
	}
	m_UserList.push_back(pUser);
	m_GameStart = true;
}

bool Sample::Init()
{
	Sprite* pSprite = 0;
	int iIndex;

	//sound
	m_Sound.Init();
	m_Sound.LoadBGM("Data/BGM.mid");
	m_Sound.Load("Data/Attack.wav");
	m_Sound.Load("Data/Item.wav");
	m_Sound.Load("Data/Erase.wav");
	m_Sound.Load("Data/End.wav");
	m_Sound.Load("Data/Show.wav");
	m_Sound.LoadBGM("Data/Main.mid");
	m_Sound.Load("Data/Button.wav");
	float i = I_Timer.m_fAccumulation;
	

	// object
	m_Main.Load(_T("Data/Main.bmp"));

	m_Menu[0][0].Load(_T("Data/Menu0-0.bmp"), _T("Data/Menu.bmp"));
	m_Menu[0][1].Load(_T("Data/Menu0-1.bmp"), _T("Data/Menu.bmp"));
	m_Menu[0][2].Load(_T("Data/Menu0-2.bmp"), _T("Data/Menu.bmp"));
	for (int i = 0; i < 3;i++)
	{
		m_Menu[0][i].SetPosition(275, 475, 0.0f);
		m_Menu[0][i].SetRect(0, 0, 288, 65);
	}

	m_Menu[1][0].Load(_T("Data/Menu1-0.bmp"), _T("Data/Menu.bmp"));
	m_Menu[1][1].Load(_T("Data/Menu1-1.bmp"), _T("Data/Menu.bmp"));
	m_Menu[1][2].Load(_T("Data/Menu1-2.bmp"), _T("Data/Menu.bmp"));
	for (int i = 0; i < 3;i++)
	{
		m_Menu[1][i].SetPosition(275, 560, 0.0f);
		m_Menu[1][i].SetRect(0, 0, 288, 65);
	}
	
	m_BackGround.Load(_T("Data/BackGround.bmp"));

	m_Client.m_Character.Load(_T("Data/bitmap1.bmp"), _T("Data/bitmap2.bmp"));
	I_BitmapManager.AddData(_T("Data/bitmap3.bmp"));
	
	m_Enemyrt[0] = { 1, 62, 44, 75 };
	m_Enemyrt[1] = { 0, 0, 50, 58 };
	I_BitmapManager.AddData(_T("Data/Enemy10.bmp"));
	I_BitmapManager.AddData(_T("Data/Enemy11.bmp"));

	I_BitmapManager.AddData(_T("Data/Item1.bmp"));
	I_BitmapManager.AddData(_T("Data/Item2.bmp"));

	m_End.Load(_T("Data/gameover0.bmp"), _T("Data/gameover1.bmp"));
	m_End.SetRect(0, 0, 296, 71);
	m_End.SetPosition(m_rtClient.right / 2, m_rtClient.bottom / 2, 0.0f);
	

	m_CharacterLife.Load(_T("Data/Life0.bmp"), _T("Data/Life1.bmp"));
	m_CharacterLife.SetRect(0, 0, 21, 30);
	
	//sprite
	LoadData(_T("RectBitmap1.txt"), m_SpriteList, m_iNumSprite);
	
	I_SpriteManager.Load(_T("Up/Down1"), _T("Data/Bitmap1.bmp"), _T("Data/Bitmap2.bmp"));
	iIndex = I_SpriteManager.Load(_T("Left1"), _T("Data/Bitmap1.bmp"), _T("Data/Bitmap2.bmp"));
	pSprite = I_SpriteManager.Find(iIndex);
	pSprite->SetRectArray(m_SpriteList[1], 0.4f);
	iIndex = I_SpriteManager.Load(_T("Right1"), _T("Data/Bitmap1.bmp"), _T("Data/Bitmap2.bmp"));
	pSprite = I_SpriteManager.Find(iIndex);
	pSprite->SetRectArray(m_SpriteList[2], 0.3f);

	iIndex = I_SpriteManager.Load(_T("Bullet"), _T("Data/Bitmap1.bmp"), _T("Data/Bitmap2.bmp"));
	pSprite = I_SpriteManager.Find(iIndex);
	pSprite->SetRectArray(m_SpriteList[3], 0.5f);

	I_SpriteManager.Load(_T("Type A"), _T("Data/Bitmap1.bmp"), _T("Data/Bitmap2.bmp"));
	I_SpriteManager.Load(_T("Type B"), _T("Data/Bitmap1.bmp"), _T("Data/Bitmap2.bmp"));

	I_SpriteManager.Load(_T("Up/Down2"), _T("Data/Bitmap3.bmp"), _T("Data/Bitmap2.bmp"));
	iIndex = I_SpriteManager.Load(_T("Left2"), _T("Data/Bitmap3.bmp"), _T("Data/Bitmap2.bmp"));
	pSprite = I_SpriteManager.Find(iIndex);
	pSprite->SetRectArray(m_SpriteList[1], 0.4f);
	iIndex = I_SpriteManager.Load(_T("Right2"), _T("Data/Bitmap3.bmp"), _T("Data/Bitmap2.bmp"));
	pSprite = I_SpriteManager.Find(iIndex);
	pSprite->SetRectArray(m_SpriteList[2], 0.3f);
	
	//score
	m_iScore = 0;
	ReadScore(m_pszFileName);
	LoadData(_T("RectLetter.txt"), m_LetterList, m_iNumWord);
	m_Letter.Load(_T("Data/Bitmap1.bmp"), _T("Data/Bitmap2.bmp"));
	
	return true;
}

bool Sample::Frame()
{
	m_Sound.Frame();

	if(m_iMenu == -1)
	{
		
		MenuOption();

		//reset
		Reset();
		if (m_UserList.size() > 0)
		{
			int i = 0;
		}
		//menuselect
		POINT pt;
		RECT rt = { 0, };
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		for (int iMenu = 0;iMenu < 2;iMenu++)
		{
			rt.left = m_Menu[iMenu][0].m_fX - m_Menu[iMenu][0].m_rt.right / 2;
			rt.top = m_Menu[iMenu][0].m_fY - m_Menu[iMenu][0].m_rt.bottom / 2;
			rt.right = rt.left + m_Menu[iMenu][0].m_rt.right;
			rt.bottom = rt.top + m_Menu[iMenu][0].m_rt.bottom;
			if (RectInPoint(rt, pt))
			{
				m_iMenuState[iMenu] = 1;
			}
			else
			{
				m_iMenuState[iMenu] = 0;
			}
		}

		if (I_Input.m_dwMouseState[0] == true)
		{
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);
			for (int iMenu = 0;iMenu < 2;iMenu++)
			{
				rt.left = m_Menu[iMenu][0].m_fX - m_Menu[iMenu][0].m_rt.right / 2;
				rt.top = m_Menu[iMenu][0].m_fY - m_Menu[iMenu][0].m_rt.bottom / 2;
				rt.right = rt.left + m_Menu[iMenu][0].m_rt.right;
				rt.bottom = rt.top + m_Menu[iMenu][0].m_rt.bottom;
				if (RectInPoint(rt, pt))
				{
					m_Sound.Play(7, false);
					m_iMenuState[iMenu] = 2;
				}
			}
		}
	}
	
	if (m_iMenu == 0)
	{
		m_iMenuState[0] = 0;
		m_iMenuState[1] = 0;

		//서버 접속
		if (I_AsyncSelect.m_netConnect == false)
		{
			char szIP[16] = "192.168.0.6";
			int iPort = 10000;
			if (I_AsyncSelect.Init() == false)
			{
				//MessageBox(g_hWnd, _T("접속 종료"), _T("알림"), MB_OK);
				I_Input.m_dwMouseState[0] = false;
				m_iMenu = -1;
				return false;
			}
			if (I_AsyncSelect.NetConnect(iPort, szIP) == false)
			{
				//int i = MessageBox(g_hWnd, _T("접속 종료"), _T("알림"), MB_OK);
				//DWORD ret = GetLastError();
				I_Input.m_dwMouseState[0] = false;
				m_iMenu = -1;
				return false;
			}
			if (I_AsyncSelect.NetAsyncSelect(m_hWnd) == false)
			{
				//MessageBox(g_hWnd, _T("접속 종료"), _T("알림"), MB_OK);
				I_Input.m_dwMouseState[0] = false;
				m_iMenu = -1;
				return false;
			}
		}

		Sprite* pSprite = 0;

		pSprite = I_SpriteManager.Find(1);
		int  i = 0;

		// user delete
		if (m_UserDrop == true)
		{
			list<User*>::iterator iter;
			for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
			{
				if (m_UserSock == m_Client.m_sock)
				{
					break;
				}
				if ((*iter)->m_sock == m_UserSock)
				{
					delete *iter;
					m_UserList.erase(iter);
					break;
				}
			}
		}

		//end blink
		if (m_Client.m_Character.m_bUsed == false && m_bEnd == false)
		{
			m_fEndTime += I_Timer.m_fSecondPerFrame;
			m_fEndTimer += I_Timer.m_fSecondPerFrame;
			if (m_fEndTime >= m_fEndEndTime)
			{
				EndTimeOver();
				return true;
			}
			if (m_fEndTimer >= m_fEndShowTime)
			{
				ShowTimeOver();
				return true;
			}
		}

		//sound
		m_Sound.Play(0, false);

		//bullet, bulletsprite create
		
		if (I_Input.KeyCheck('0') == KEY_DOWN)
		{
			if (m_GameStart == true)
			{
				m_Sound.Play(1, true);
				UPACKET_BULLET_CREATE bullet;
				bullet.sock = m_Client.m_sock;
				bullet.BulletNum = m_Client.m_iBulletCnt++;
				bullet.BulletfX = m_Client.m_Character.m_fX;
				bullet.BulletfY = m_Client.m_Character.m_fY - m_Client.m_Character.m_rt.bottom / 2;
				I_AsyncSelect.NetSend((char*)&bullet, PACKET_BULLET_CREATE, sizeof(bullet));
			}
		}

		//bullet create
		list<User*>::iterator iter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);

			if (pUser->m_BulletDataList.size() > 0)
			{
				list<UPACKET_BULLET_CREATE>::iterator Bulletiter = pUser->m_BulletDataList.begin();
				list<UPACKET_BULLET_CREATE>::iterator DelBulletiter;
				while (Bulletiter != pUser->m_BulletDataList.end())
				{
					UPACKET_BULLET_CREATE pBulletData = (*Bulletiter);
					Bullet* pBullet = new Bullet;
					pBullet->Load(_T("Data/bitmap1.bmp"), _T("Data/bitmap2.bmp"));
					pBullet->SetRect(276, 1, 8, 23);
					pBullet->SetPosition(pBulletData.BulletfX, pBulletData.BulletfY, 400.0f);
					pBullet->m_iNumSpriter = m_iNumBulletSpriter++;
					pBullet->m_bStart = true;
					pBullet->m_iNum = pBulletData.BulletNum;
					pUser->m_BulletList.push_back(pBullet);

					Spriter* pSpriter = new Spriter;
					pSpriter->m_pSprite = I_SpriteManager.Find(4);
					pSpriter->m_pSprite->SetRectArray(m_SpriteList[3], 0.3f);
					pSpriter->m_iNumber = pBullet->m_iNumSpriter;
					m_BulletSpriterList.push_back(pSpriter);
					DelBulletiter = Bulletiter++;
					pUser->m_BulletDataList.erase(DelBulletiter);
				}
			}
		}
		
		//enemy create
		if (m_EnemyDataList.size() > 0)
		{
			list<UPACKET_ENEMY_CREATE>::iterator Enemyiter = m_EnemyDataList.begin();
			list<UPACKET_ENEMY_CREATE>::iterator DelEnemyiter;
			while (Enemyiter != m_EnemyDataList.end())
			{
				UPACKET_ENEMY_CREATE pEnemyData = (*Enemyiter);
				Enemy* pEnemy = new Enemy;
				pEnemy->m_iTypeIndex = pEnemyData.Enemytype;
				if (pEnemy->m_iTypeIndex == 0)
				{
					pEnemy->Load(_T("Data/bitmap1.bmp"), _T("Data/bitmap2.bmp"));
				}
				else
				{
					pEnemy->Load(_T("Data/Enemy10.bmp"), _T("Data/Enemy11.bmp"));
				}
				pEnemy->SetRect(m_Enemyrt[pEnemy->m_iTypeIndex]);
				pEnemy->SetPosition(pEnemyData.EnemyfX, pEnemyData.EnemyfY, pEnemyData.EnemySpeed);
				pEnemy->m_iNum = pEnemyData.EnemyNum;
				m_EnemyList.push_back(pEnemy);
				DelEnemyiter = Enemyiter++;
				m_EnemyDataList.erase(DelEnemyiter);
			}
		}

		//item create
		if (m_ItemDataList.size() > 0)
		{
			list<UPACKET_ITEM_CREATE>::iterator Itemiter = m_ItemDataList.begin();
			list<UPACKET_ITEM_CREATE>::iterator DelItemiter;
			while (Itemiter != m_ItemDataList.end())
			{
				UPACKET_ITEM_CREATE pItemData = (*Itemiter);
				Item* pItem = new Item;
				pItem->Load(_T("Data/Item1.bmp"), _T("Data/Item2.bmp"));
				pItem->SetRect(0, 0, 26, 28);
				pItem->SetPosition(pItemData.ItemfX, pItemData.ItemfY, pItemData.ItemSpeed);
				pItem->m_iNum = pItemData.ItemNum;
				m_ItemList.push_back(pItem);
				DelItemiter = Itemiter++;
				m_ItemDataList.erase(DelItemiter);
			}
		}

		//character move, frame
		int iKeyState[4] = { 0, };
		for (int iKey = 0; iKey < 4; iKey++)
		{
			iKeyState[iKey] = I_Input.KeyCheck('1' + iKey);
		}
		if (m_GameStart == true)
		{
			for (int iKey = 0; iKey < 4; iKey++)
			{
				if (iKeyState[iKey] == KEY_UP)
				{
					if (m_Client.m_Character.m_iDirection == iKey + 1)
					{
						UPACKET_USER_MOVE move;
						move.sock = m_Client.m_sock;
						move.iDirection = STOP;
						int iLen = sizeof(move);
						I_AsyncSelect.NetSend((char*)&move, PACKET_USER_MOVE, iLen);
					}
				}
				else if (iKeyState[iKey] == KEY_DOWN)
				{
					UPACKET_USER_MOVE move;
					move.sock = m_Client.m_sock;
					move.iDirection = iKey + 1;
					move.fX = m_Client.m_Character.m_fX;
					move.fY = m_Client.m_Character.m_fY;
					int iLen = sizeof(move);
					I_AsyncSelect.NetSend((char*)&move, PACKET_USER_MOVE, iLen);
				}
			}
		}
		
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);
			switch (pUser->m_Character.m_iDirection)
			{
				case STOP:
				{
					pUser->m_Character.m_CharacterState = 0;
				}break;
				case UP:
				{
					pUser->m_Character.Up();
					pSprite = I_SpriteManager.Find(1 + 6 * (pUser->m_iNum - 1));
					pSprite->SetPosition(pUser->m_Character.m_fX, pUser->m_Character.m_fY);
					pUser->m_Character.m_CharacterState = 1;
				}break;
				case DOWN:
				{
					pUser->m_Character.Down();
					pSprite = I_SpriteManager.Find(1 + 6 * (pUser->m_iNum - 1));
					pSprite->SetPosition(pUser->m_Character.m_fX, pUser->m_Character.m_fY);
					pUser->m_Character.m_CharacterState = 1;
				}break;
				case LEFT:
				{
					pUser->m_Character.Left();
					pSprite = I_SpriteManager.Find(2 + 6 * (pUser->m_iNum - 1));
					pSprite->SetPosition(pUser->m_Character.m_fX, pUser->m_Character.m_fY);
					pUser->m_Character.m_CharacterState = 2;
				}break;
				case RIGHT:
				{
					pUser->m_Character.Right();
					pSprite = I_SpriteManager.Find(3 + 6 * (pUser->m_iNum - 1));
					pSprite->SetPosition(pUser->m_Character.m_fX, pUser->m_Character.m_fY);
					pUser->m_Character.m_CharacterState = 3;
				}break;
			}
		}


		if (m_Client.m_Character.m_bPlaySound == true)
		{
			m_Sound.Play(5, true);
			m_Client.m_Character.m_bPlaySound = false;
		}

		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);
			pUser->m_Character.Frame();
		}

		// character position revise
		
		if (m_Client.m_Character.m_bUsed == true)
		{
			m_Client.m_fPositionTimer += I_Timer.m_fFrameTime;
			if (m_Client.m_fPositionTimer >= m_Client.m_fPositionTime)
			{
				m_Client.m_fPositionTimer = 0.0f;

				UPACKET_USER_POSITION position;
				position.sock = m_Client.m_sock;
				position.fX = m_Client.m_Character.m_fX;
				position.fY = m_Client.m_Character.m_fY;
				I_AsyncSelect.NetSend((char*)&position, PACKET_USER_POSITION, sizeof(position));
			}
		}
		if (m_UserList.size() > 1)
		{
			for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
			{
				User* pUser = (*iter);
				if (pUser == &m_Client)
				{
					continue;
				}
				if (pUser->m_bPositionrevise == true)
				{
					pUser->m_bPositionrevise = false;
					pUser->m_Character.m_fX = pUser->m_fCharacterX;
					pUser->m_Character.m_fY = pUser->m_fCharacterY;
				}
			}
		}

		//character reborn
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);
			if (pUser->m_Character.m_bReborn == true)
			{
				pUser->m_Character.m_fRebornTime += I_Timer.m_fSecondPerFrame;
				pUser->m_Character.m_fRebornTimer += I_Timer.m_fSecondPerFrame;
				if (pUser->m_Character.m_fRebornTime >= pUser->m_Character.m_fRebornEndTime)
				{
					pUser->m_Character.m_bReborn = false;
					pUser->m_Character.m_bRebornShow = false;
					pUser->m_Character.m_fRebornTime = 0.0f;
					pUser->m_Character.m_fRebornTimer = 0.0f;
					return true;
				}
				if (pUser->m_Character.m_fRebornTimer >= pUser->m_Character.m_fRebornShowTime)
				{
					if (pUser->m_Character.m_bRebornShow == true)
					{
						pUser->m_Character.m_bRebornShow = false;
					}
					else
					{
						pUser->m_Character.m_bRebornShow = true;
					}
					pUser->m_Character.m_fRebornTimer = 0.0;
					return true;
				}
			}
		}

		// bullet erase
		list<Bullet*>::iterator Bulletiter;
		list<Bullet*>::iterator DelBulletiter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);
			Bulletiter = pUser->m_BulletList.begin();
			if (pUser->m_iBulletCreate == false)
			{
				pUser->m_iBulletCreate = -1;
				while (Bulletiter != pUser->m_BulletList.end())
				{
					Bullet* pBullet = *Bulletiter;
					if ((*Bulletiter)->m_iNum == pUser->m_iBulletNum)
					{
						delete (*Bulletiter);
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
			while (Bulletiter != pUser->m_BulletList.end())
			{
				if ((*Bulletiter)->m_bUsed == false)
				{
					if ((*Bulletiter)->m_bCollide == true)
					{
						if ((*Bulletiter)->m_bSent == false)
						{
							(*Bulletiter)->m_bSent = true;
							UPACKET_BULLET_DELETE bullet;
							bullet.sock = pUser->m_sock;
							bullet.BulletNum = (*Bulletiter)->m_iNum;
							I_AsyncSelect.NetSend((char*)&bullet, PACKET_BULLET_DELETE, sizeof(bullet));
						}
						Bulletiter++;
					}
					else
					{
						delete(*Bulletiter);
						DelBulletiter = Bulletiter;
						Bulletiter++;
						pUser->m_BulletList.erase(DelBulletiter);
					}
				}
				else
				{
					Bulletiter++;
				}
			}
		}

		//enemy erase
		list<Enemy*>::iterator Enemyiter = m_EnemyList.begin();
		list<Enemy*>::iterator DelEnemyiter;
		if (m_iEnemyCreate == false)
		{
			while (Enemyiter != m_EnemyList.end())
			{
				if ((*Enemyiter)->m_iNum == m_iEnemyNum)
				{
					delete (*Enemyiter);
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
			if ((*Enemyiter)->m_bUsed  == false)
			{
				if ((*Enemyiter)->m_bCollide == true)
				{
					if ((*Enemyiter)->m_bSent == false)
					{
						(*Enemyiter)->m_bSent = true;
						UPACKET_ENEMY_DELETE enemy;
						enemy.EnemyNum = (*Enemyiter)->m_iNum;
   						I_AsyncSelect.NetSend((char*)&enemy, PACKET_ENEMY_DELETE, sizeof(enemy));
					}
					Enemyiter++;
				}
				else
				{
					delete (*Enemyiter);
					DelEnemyiter = Enemyiter;
					Enemyiter++;
					m_EnemyList.erase(DelEnemyiter);
				}
			}
			else
			{
				Enemyiter++;
			}
		}

		//Item erase
		list<Item*>::iterator Itemiter = m_ItemList.begin();
		list<Item*>::iterator DelItemiter;
		if (m_iItemCreate == false)
		{
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
				if ((*Itemiter)->m_bCollide == true)
				{
					if ((*Itemiter)->m_bSent == false)
					{
						(*Itemiter)->m_bSent = true;
						UPACKET_ITEM_DELETE Item;
						Item.ItemNum = (*Itemiter)->m_iNum;
						I_AsyncSelect.NetSend((char*)&Item, PACKET_ITEM_DELETE, sizeof(Item));
					}
					Itemiter++;
				}
				else
				{
					delete (*Itemiter);
					DelItemiter = Itemiter;
					Itemiter++;
					m_ItemList.erase(DelItemiter);
				}
			}
			else
			{
				Itemiter++;
			}
		}

		//sprite erase
		list<Spriter*>::iterator Spriteriter = m_CollisionSpriterList.begin();
		list<Spriter*>::iterator DelSpriteriter;
		while (Spriteriter != m_CollisionSpriterList.end())
		{
			if (!(*Spriteriter)->m_bUsed)
			{
				delete (*Spriteriter);
				DelSpriteriter = Spriteriter;
				Spriteriter++;
				m_CollisionSpriterList.erase(DelSpriteriter);
			}
			else
			{
				Spriteriter++;
			}
		}

		Spriteriter = m_BulletSpriterList.begin();
		while (Spriteriter != m_BulletSpriterList.end())
		{
			if (!(*Spriteriter)->m_bUsed)
			{
				delete (*Spriteriter);
				DelSpriteriter = Spriteriter;
				Spriteriter++;
				m_BulletSpriterList.erase(DelSpriteriter);
			}
			else
			{
				Spriteriter++;
			}
		}

		//bullet, bulletsprite frame
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);
			Bulletiter = pUser->m_BulletList.begin();
			while (Bulletiter != pUser->m_BulletList.end())
			{
				(*Bulletiter)->Frame();
				if ((*Bulletiter)->m_bStart)
				{
					list<Spriter*>::iterator Spriteriter = m_BulletSpriterList.begin();
					while (Spriteriter != m_BulletSpriterList.end())
					{
						if ((*Spriteriter)->m_iNumber == (*Bulletiter)->m_iNumSpriter)
						{
							break;
						}
						Spriteriter++;
					}
					(*Spriteriter)->SetPosition((*Bulletiter)->m_fX, (*Bulletiter)->m_fY);
				}
				Bulletiter++;
			}
		}

		//enemy frame
		Enemyiter = m_EnemyList.begin();
		while (Enemyiter != m_EnemyList.end())
		{
			(*Enemyiter)->Frame();
			Enemyiter++;
		}

		//Item frame
		Itemiter = m_ItemList.begin();
		while (Itemiter != m_ItemList.end())
		{
			(*Itemiter)->Frame();
			Itemiter++;
		}

		//character, enemy collision
		if (m_Client.m_Character.m_bReborn == false)
		{
			if (m_Client.m_Character.m_bUsed == true)
			{
				Enemyiter = m_EnemyList.begin();
				while (Enemyiter != m_EnemyList.end())
				{
					if ((*Enemyiter)->m_bUsed)
					{
						if (RectInRect((*Enemyiter)->m_rtCollision, m_Client.m_Character.m_rtCollision))
						{
							m_Sound.Play(4, true);
							Spriter* pSpriter = new Spriter;
							pSpriter->m_pSprite = I_SpriteManager.Find(6);
							pSpriter->m_pSprite->SetRectArray(m_SpriteList[4], 0.3f);
							POINT pt = CollisionPt((*Enemyiter)->m_rtCollision, m_Client.m_Character.m_rtCollision);
							pSpriter->SetPosition(pt.x, pt.y);
							m_CollisionSpriterList.push_back(pSpriter);
							m_Client.m_Character.m_iLife--;
							(*Enemyiter)->m_bUsed = false;
							(*Enemyiter)->m_bCollide = true;
							m_Client.m_Character.m_bReborn = true;
							m_Client.m_Character.m_bRebornShow = true;
							m_Client.m_Character.m_fRebornTime = 0.0f;
							m_Client.m_Character.m_fRebornTimer = 0.0f;

							UPACKET_USER_COLLIDE collide;
							collide.sock = m_Client.m_sock;
							collide.CollidefX = pt.x;
							collide.CollidefY = pt.y;
							I_AsyncSelect.NetSend((char*)&collide, PACKET_USER_COLLIDE, sizeof(collide));

							if (m_Client.m_Character.m_iLife == 0)
							{
								m_Client.m_Character.m_bUsed = false;
								m_bEndShow = true;
								m_Client.m_Character.m_rt = { 0, };
								m_Client.m_Character.m_fX = 0;
								m_Client.m_Character.m_fY = 0;
								m_bGameOver = true;

								UPACKET_ENEMY_DELETE enemy;
								enemy.EnemyNum = (*Enemyiter)->m_iNum;
								I_AsyncSelect.NetSend((char*)&enemy, PACKET_ENEMY_DELETE, sizeof(enemy));
								I_AsyncSelect.NetSend((char*)&m_Client.m_sock, PACKET_USER_DROP, sizeof(m_Client.m_sock));
							}
						}
					}
					Enemyiter++;
				}
			}
		}
		if (m_UserList.size() > 1)
		{
			for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
			{
				User* pUser = (*iter);
				if (pUser == &m_Client)
				{
					continue;
				}
				if (pUser->m_Character.m_bCollide == true)
				{
					pUser->m_Character.m_bCollide = false;
					Spriter* pSpriter = new Spriter;
					pSpriter->m_pSprite = I_SpriteManager.Find(6);
					pSpriter->m_pSprite->SetRectArray(m_SpriteList[4], 0.3f);
					pSpriter->SetPosition(pUser->m_fCollideX, pUser->m_fCollideY);
					m_CollisionSpriterList.push_back(pSpriter);
					pUser->m_Character.m_bReborn = true;
					pUser->m_Character.m_bRebornShow = true;
					pUser->m_Character.m_fRebornTime = 0.0f;
					pUser->m_Character.m_fRebornTimer = 0.0f;
				}
			}
		}
		
		//character, item collision
		Itemiter = m_ItemList.begin();
		while (Itemiter != m_ItemList.end())
		{
			if ((*Itemiter)->m_bUsed)
			{
				if (RectInRect((*Itemiter)->m_rtCollision, m_Client.m_Character.m_rtCollision))
				{
					m_Sound.Play(2, true);
					(*Itemiter)->m_bUsed = false;
					(*Itemiter)->m_bCollide = true;
					m_iScore += 20;
				}
			}
			Itemiter++;
		}

		//enemy, bullet collision
		Enemyiter = m_EnemyList.begin();
		while (Enemyiter != m_EnemyList.end())
		{
			if ((*Enemyiter)->m_bUsed)
			{
				Bulletiter = m_Client.m_BulletList.begin();
				while (Bulletiter != m_Client.m_BulletList.end())
				{
					if ((*Bulletiter)->m_bUsed)
					{
						if (RectInRect((*Enemyiter)->m_rtCollision, (*Bulletiter)->m_rtCollision))
						{
							if ((*Enemyiter)->m_iTypeIndex == 0)
							{
								m_Sound.Play(3, true);
								Spriter* pSpriter = new Spriter;
								pSpriter->m_pSprite = I_SpriteManager.Find(5);
								pSpriter->m_pSprite->SetRectArray(m_SpriteList[5], 0.3f);
								POINT pt = CollisionPt((*Enemyiter)->m_rtCollision, (*Bulletiter)->m_rtCollision);
								pSpriter->SetPosition(pt.x, pt.y);
								m_CollisionSpriterList.push_back(pSpriter);
								(*Enemyiter)->m_bUsed = false;
								(*Enemyiter)->m_bCollide = true;
								(*Bulletiter)->m_bUsed = false;
								(*Bulletiter)->m_bCollide = true;
								m_iScore += 10;

								UPACKET_ENEMY_COLLIDE collide;
								collide.CollidefX = pt.x;
								collide.CollidefY = pt.y;
								I_AsyncSelect.NetSend((char*)&collide, PACKET_ENEMY_COLLIDE, sizeof(collide));
							}
							else
							{
								(*Bulletiter)->m_bUsed = false;
								(*Bulletiter)->m_bCollide = true;
							}
						}
					}
					Bulletiter++;
				}
			}
			Enemyiter++;
		}
		if (m_bCollide == true)
		{
			m_bCollide = false;
			Spriter* pSpriter = new Spriter;
			pSpriter->m_pSprite = I_SpriteManager.Find(5);
			pSpriter->m_pSprite->SetRectArray(m_SpriteList[5], 0.3f);
			pSpriter->SetPosition(m_fCollideX, m_fCollideY);
			m_CollisionSpriterList.push_back(pSpriter);
		}
	}

	if (m_iMenu == 1)
	{
		DestroyWindow(m_hWnd);
	}
	
	return true;
}

bool Sample::Render()
{
	if (m_iMenu == -1)
	{
		m_Main.DrawBackGround();
		for (int iMenu = 0;iMenu < m_iMenuNum;iMenu++)
		{
			m_Menu[iMenu][m_iMenuState[iMenu]].Render();
		}
	}

	if (m_iMenu == 0)
	{
		m_BackGround.DrawBackGround();

		//Item
		list<Item*>::iterator Itemiter = m_ItemList.begin();
		while (Itemiter != m_ItemList.end())
		{
			if ((*Itemiter)->m_bUsed)
			{
				(*Itemiter)->Render();
			}
			Itemiter++;
		}

		//bullet, bulletsprite
		list<User*>::iterator iter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);
			list<Bullet*>::iterator Bulletiter = pUser->m_BulletList.begin();
			while (Bulletiter != pUser->m_BulletList.end())
			{
				if ((*Bulletiter)->m_bUsed)
				{
					if ((*Bulletiter)->m_bStart)
					{
						list<Spriter*>::iterator Spriteriter = m_BulletSpriterList.begin();
						while (Spriteriter != m_BulletSpriterList.end())
						{
							if ((*Spriteriter)->m_iNumber == (*Bulletiter)->m_iNumSpriter)
							{
								break;
							}
							Spriteriter++;
						}
						(*Spriteriter)->SetSprite();
						if ((*Spriteriter)->m_pSprite->Frame())
						{
							(*Spriteriter)->Set();
							(*Spriteriter)->m_pSprite->Render();
						}
						else
						{
							(*Bulletiter)->m_bStart = false;
							(*Spriteriter)->m_bUsed = false;
						}
					}
					else
					{
						(*Bulletiter)->Render();
					}
				}
				Bulletiter++;
			}
		}

		//enemy
		list<Enemy*>::iterator Enemyiter = m_EnemyList.begin();
		while (Enemyiter != m_EnemyList.end())
		{
			if ((*Enemyiter)->m_bUsed)
			{
				(*Enemyiter)->Render();
			}
			Enemyiter++;
		}

		//character
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			User* pUser = (*iter);
			if (pUser->m_Character.m_bUsed == true)
			{
				if (pUser->m_Character.m_bRebornShow == false)
				{
					if (pUser->m_Character.m_CharacterState == 0)
					{
						pUser->m_Character.Render();
					}
					else
					{
						I_SpriteManager.Find(pUser->m_Character.m_CharacterState + 6 * (pUser->m_iNum - 1))->Frame();
						I_SpriteManager.Find(pUser->m_Character.m_CharacterState + 6 * (pUser->m_iNum - 1))->Render();
					}
				}
			}
			else
			{
				if (m_bEnd == true)
				{
					m_Sound.Stop(0);
					m_iMenu = -1;

					//score
					if (m_iScore > m_iRecordScore)
					{
						_stprintf_s(m_szRecordScore, _T("최고점수= %d"), m_iScore);
						WriteScore(m_pszFileName);
					}

					I_AsyncSelect.m_netConnect = false;
					I_AsyncSelect.Release();
					return true;
				}
				else
				{
					if (m_bEndShow == true)
					{
						m_End.Render();
					}
				}
			}
		}

		//sprite
		list<Spriter*>::iterator Spriteriter = m_CollisionSpriterList.begin();
		while (Spriteriter != m_CollisionSpriterList.end())
		{
			(*Spriteriter)->SetSprite();
			if ((*Spriteriter)->m_pSprite->Frame())
			{
				(*Spriteriter)->Set();
				(*Spriteriter)->m_pSprite->Render();
			}
			else
			{
				(*Spriteriter)->m_bUsed = false;
			}
			Spriteriter++;
		}

		//score
		for (int iLife = 0;iLife < m_Client.m_Character.m_iLife; iLife++)
		{
			m_CharacterLife.SetPosition(470 + 30 * iLife, 20, 0.0f);
			m_CharacterLife.Render();
		}
		_stprintf_s(m_szRecordScore, _T(": %d"), m_iRecordScore);
		_stprintf_s(m_szCurrentScore, _T(": %d"), m_iScore);
		SetBkMode(m_hOffScreenDC, TRANSPARENT);
		for (int iWord = 0;iWord < m_iNumWord-1;iWord++)
		{
			for (int iLetter = 0; iLetter < m_LetterList[iWord].size(); iLetter++)
			{
				m_Letter.SetRect(m_LetterList[iWord][iLetter]);
				m_Letter.SetPosition(6 + 8 * iLetter, 10 + 20 * iWord, 0.0f);
				m_Letter.Render();
			}
		}
		SetTextColor(g_hOffScreenDC, RGB(255, 255, 255));
		TextOut(g_hOffScreenDC, 55, 0, m_szRecordScore, _tcslen(m_szRecordScore));
		TextOut(g_hOffScreenDC, 50, 20, m_szCurrentScore, _tcslen(m_szCurrentScore));
	}

	return true;
}

bool Sample::Release()
{
	m_BackGround.Release();
	m_Client.m_Character.Release();

	//score
	if (m_iScore > m_iRecordScore)
	{
		_stprintf_s(m_szRecordScore, _T(": %d"), m_iScore);
		WriteScore(m_pszFileName);
	}
	return true;

	//sprite
	for (int iSprite = 0;iSprite < m_iNumSprite; iSprite++)
	{
		m_SpriteList[iSprite].clear();
	}
	m_SpriteList.clear();

	list<Spriter*>::iterator Spriteriter = m_CollisionSpriterList.begin();
	list<Spriter*>::iterator DelSpriteriter;
	while (Spriteriter != m_CollisionSpriterList.end())
	{
		delete (*Spriteriter);
		DelSpriteriter = Spriteriter;
		Spriteriter++;
		m_CollisionSpriterList.erase(DelSpriteriter);
	}
	m_CollisionSpriterList.clear();

	Spriteriter = m_BulletSpriterList.begin();
	while (Spriteriter != m_BulletSpriterList.end())
	{
		delete (*Spriteriter);
		DelSpriteriter = Spriteriter;
		Spriteriter++;
		m_BulletSpriterList.erase(DelSpriteriter);
	}
	m_BulletSpriterList.clear();

	//bullet
	list<Bullet*>::iterator Bulletiter = m_Client.m_BulletList.begin();
	list<Bullet*>::iterator DelBulletiter;
	while (Bulletiter != m_Client.m_BulletList.end())
	{
		delete (*Bulletiter);
		DelBulletiter = Bulletiter;
		Bulletiter++;
		m_Client.m_BulletList.erase(DelBulletiter);
	}
	m_Client.m_BulletList.clear();
	
	//enemy
	list<Enemy*>::iterator Enemyiter = m_EnemyList.begin();
	list<Enemy*>::iterator DelEnemyiter;
	while (Enemyiter != m_EnemyList.end())
	{
		delete (*Enemyiter);
		DelEnemyiter = Enemyiter;
		Enemyiter++;
		m_EnemyList.erase(DelEnemyiter);
	}
	m_EnemyList.clear();

	//Item
	list<Item*>::iterator Itemiter = m_ItemList.begin();
	list<Item*>::iterator DelItemiter;
	while (Itemiter != m_ItemList.end())
	{
		delete (*Itemiter);
		DelItemiter = Itemiter;
		Itemiter++;
		m_ItemList.erase(DelItemiter);
	}
	m_ItemList.clear();

	return true;
}

Sample::Sample()
{	
	m_iMenu = -1;
	m_iMenuNum = 2;	

	m_bEnd = false;
	m_fEndEndTime = 2.5f;
	m_fEndShowTime = 0.4f;
	m_bGameOver = false;
}

Sample::~Sample()
{
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow)
{
	if (I_Sample.SetWindow(hInstance, _T("THE ONE"), 0, 0, 550, 800) == true)
	{
		I_Sample.Run();
	}
}	