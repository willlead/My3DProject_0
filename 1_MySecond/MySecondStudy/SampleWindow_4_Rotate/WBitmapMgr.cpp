#include "WBitmapMgr.h"

template <typename W>
void SwapPoint(W* a, W*b)
{
	a->m_pNext = b;
	b->m_pPrve = a;
}

WBitmapMgr::WBitmapMgr()
{
}


WBitmapMgr::~WBitmapMgr()
{
}

WNode<WBitmap>* WBitmapMgr::Swap(
	WNode<WBitmap>* a,
	WNode<WBitmap>* b)
{
	WNode<WBitmap> * aPrev = a->m_pPrev;
	WNode<WBitmap> * aNext = a->m_pNext;
	WNode<WBitmap> * bPrev = b->m_pPrev;
	WNode<WBitmap> * bNext = b->m_pNext;

	// 1¹ø Haed <-> b
	if (aPrev != NULL) {
		SwapPoint(aPrev, b);
	}
	else {
		m_List.m_pHead = b;
		b->m_pPrev = NULL;
	}
	// b <-> a
	if (aNext != b)
	{
		SwapPoint(b, aNext);
		// a -> c -> b
		if (aNext->m_pNext == b)	SwapPoint(aNext, a);
		else						SwapPoint(bPrev, a);
	}
	else // b <-> a
	{
		SwapPoint(b, a);
		a->m_pNext = bNext;
		if (bNext != NULL) bNext->m_pPrev = a;
	}
	// a-> c -> b ->d
	if (bNext != NULL)  SwapPoint(a, bNext);
	else				a->m_pNext = NULL;

	if (aNext == b) return a;
	return aNext;
}

WBitmap * WBitmapMgr::Load(TCHAR* pszLoadFile) {
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	TCHAR SaveName[MAX_PATH] = { 0, };
	_tsplitpath_s(pszLoadFile, Drive, Dir, Name, Ext);
	_stprintf_s(SaveName, _T("%s%s"), Name, Ext);
	WBitmap* pBitmap = NULL;
	pBitmap = m_List.GetData(SaveName);
	if (pBitmap)
	{
		return pBitmap;
	}

	WBitmap* pData = new WBitmap;
	if (pData->Load(pszLoadFile))
	{
		m_Hash.Insert(pData);
		m_List.AddLink(pData);
		//return m_List.GetData(pData->m_szName);
		return m_Hash.Find(SaveName);
	}
	return NULL;
}



WBitmap* WBitmapMgr::Find(int iIndex) {
	for (WNode<WBitmap>* pLink = m_List.m_pHead;
	pLink!=NULL; pLink = pLink->m_pNext	)
	{
		WBitmap* pData = pLink->m_pData;
		if (pData->m_iIndex == iIndex)
		{
			return pData;
		}
	}
	return NULL;
}


WBitmap* WBitmapMgr::Find(TCHAR* pszName) {
	int iIndex = this->m_Hash.Hash(pszName);
	if (iIndex >= m_Hash.m_iLength)return NULL;
	return m_Hash.m_pArray[iIndex].GetData(pszName);
}


void WBitmapMgr::Sort(bool bUp = true) {

}
void WBitmapMgr::Release() {
	m_List.Release();
}
void WBitmapMgr::ShowData() {

}
void WBitmapMgr::ShowData(WBitmap * pStd) {

}

