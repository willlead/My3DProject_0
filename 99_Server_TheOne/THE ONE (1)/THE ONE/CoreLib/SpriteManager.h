#pragma once
#include "Sprite.h"
#include "Std.h"

class SpriteManager : public Singleton<SpriteManager>
{
public:
	int						m_iKey;
	map<int, Sprite*>		m_MapList;

public:
	int						Load(TCHAR*, TCHAR*, TCHAR*);
	Sprite*					Find(int);

public:
	bool					Init();
	bool					Frame();
	bool					Render();
	bool					Release();

public:
	SpriteManager();
	virtual ~SpriteManager();
};

#define I_SpriteManager SpriteManager::GetInstance()

