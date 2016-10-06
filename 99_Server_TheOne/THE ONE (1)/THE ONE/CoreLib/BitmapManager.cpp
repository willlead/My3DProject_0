#include "BitmapManager.h"

Bitmap* BitmapManager::AddData(TCHAR* pszName)
{
	Bitmap* pBitmap = 0;
	pBitmap=m_Hash.Find(pszName);
	if (pBitmap == 0)
	{
		pBitmap = new Bitmap;
		pBitmap->Load(pszName);
		m_List.InsertLink(pBitmap);
		m_Hash.InsertData(pBitmap);
	}
	return pBitmap;
}

bool BitmapManager::Init()
{
	return true;
}

bool BitmapManager::Frame()
{
	return true;
}

bool BitmapManager::Render()
{
	return true;
}

bool BitmapManager::Release()
{
	m_List.Free();
	m_Hash.Free();
	return true;
}

BitmapManager::BitmapManager()
{
}

BitmapManager::~BitmapManager()
{
}
