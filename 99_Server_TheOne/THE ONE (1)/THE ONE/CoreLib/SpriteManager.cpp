#include "SpriteManager.h"

int SpriteManager::Load(TCHAR* pszName, TCHAR* pszColorFile, TCHAR* pszMaskFile)
{
	Sprite* pSprite = new Sprite;
	map<int, Sprite*>::iterator itor;
	for (int iKey = 1; iKey <= m_iKey; iKey++)
	{ 
		itor = m_MapList.find(iKey);
		if ((*itor).second->m_szName == pszName)
		{ 
			delete pSprite;
			return iKey;
		}
	}
	pSprite->Load(pszColorFile, pszMaskFile);
	pSprite->m_szName = pszName;
	m_MapList.insert(make_pair(++m_iKey, pSprite));
	return m_iKey;
}

Sprite* SpriteManager::Find(int iKey)
{
	map<int, Sprite*>::iterator itor = m_MapList.find(iKey);
	if (itor == m_MapList.end())
	{
		return 0;
	}
	Sprite* pSprite = (*itor).second;
	return pSprite;
}

bool SpriteManager::Init()
{
	return true;
}

bool SpriteManager::Frame()
{
	return true;
}

bool SpriteManager::Render()
{
	return true;
}

bool SpriteManager::Release()
{
	Sprite* pSprite = 0;
	for (int iSprite = 1;iSprite <= m_iKey;iSprite++)
	{
		pSprite = (*m_MapList.find(iSprite)).second;
		delete pSprite;
	}
	m_MapList.clear();
	return true;
}

SpriteManager::SpriteManager()
{
	m_iKey = 0;
}


SpriteManager::~SpriteManager()
{
}
