#pragma once
#include "WHashTable.h"
#include "WBitmap.h"

class WBitmapMgr : public WSingleton<WBitmapMgr>
{
public:
	friend class WSingleton<WBitmapMgr>;
public:
	WLinkedList<WBitmap> m_List;
	WHashTable<WBitmap> m_Hash;

private:
	WLinkedList<WBitmap> m_InitList;
public:
	WBitmap * Load(TCHAR* pszLoadFile);
	WBitmap* Find(int iIndex);
	void Sort(bool bUp = true);
	void Release();
	void ShowData();
	void ShowData(WBitmap * pStd);

	WNode<WBitmap>* Swap(
		WNode<WBitmap>* a,
		WNode<WBitmap>* b);
	WBitmap* Find(TCHAR* pszName);


	void Delete(int iIndex) {};
	void Delete(WBitmap* iData) {  };
public:
	WBitmapMgr();
	virtual ~WBitmapMgr();
};
#define I_BimapMgr WBitmapMgr::GetInstance()
