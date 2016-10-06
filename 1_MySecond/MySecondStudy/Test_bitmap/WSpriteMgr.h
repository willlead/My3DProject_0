#pragma once
#include "WSprite.h"
#include <map>
class WSpriteMgr : public WSingleton<WSpriteMgr>
{
private:
	INT m_iKey;
	friend class WSingleton<WSpriteMgr>;
	//map<INT, WSprite*> m_mapList;
	WSprite * Load(TCHAR* pszLoadFile);
	WSprite * Find(INT iKey);
public:
	WSpriteMgr();
	virtual ~WSpriteMgr();
};

#define I_Sprite WSpriteMgr::GetInstance()