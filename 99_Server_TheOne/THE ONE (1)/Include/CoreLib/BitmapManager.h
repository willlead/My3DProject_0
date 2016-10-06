#pragma once
#include "Bitmap.h"
#include "HashTable.h"
#include "Std.h"

class BitmapManager : public Singleton<BitmapManager>
{
public:
	LinkedList<Bitmap>		m_List;
	HashTable<Bitmap>		m_Hash;

public:
	Bitmap*					AddData(TCHAR*);

public:
	bool					Init();
	bool					Frame();
	bool					Render();
	bool					Release();

public:
	BitmapManager();
	virtual ~BitmapManager();
};

#define I_BitmapManager BitmapManager::GetInstance()