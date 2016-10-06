#pragma once
#include "WLinkedList.h"
template <class W>
class WHashTable
{
public:
	WLinkedList<W>* m_pArray;
	int m_iLength;
	int Hash(TCHAR* pName);
	void Insert(W* newItem);
	W* Find(TCHAR* pszName);
public:
	WHashTable(int iLength = 20);
	virtual ~WHashTable();
};


template <class W>
int WHashTable<W>::Hash(TCHAR* pName) {
	int iKey = 5381;
	int c;
	size_t hash = 0x811c9dc5;
	while (c = pName++)
	{
		hash ^= c;
		hash *= 0x01000193;
		/*int c;
		while (c = *pName++)
		{
		iKey = ((iKey << 5) + iKey) + c;
		}*/
		/*int iValue = 0;
		int iLen = _tcslen(pName);
		for (int i = 0; i < iLen; i++)
		{
		iValue += pName[i];
		}
		int iKey = (iValue * iValue) % m_iLength;*/
		return hash % m_iLength;
	}
}

template <class W>
void WHashTable<W>::Insert(W* newItem) {
	int iIndex = Hash(newItem->pszName);
	m_pArray[iIndex].AddLink(newItem);
}

template <class W>
W* WHashTable<W>::Find(TCHAR* pszName) {
	int iIndex = Hash(pszName);
	return m_pArray[iIndex].GetData(pszName);
}


template <class W>
WHashTable<W>::WHashTable(int iLength = 20) {
	m_pArray = new WLinkedList<W>[iLength];
	m_iLength = iLength;
}

template <class W>
WHashTable<W>::~WHashTable() {
	delete m_pArray;
}