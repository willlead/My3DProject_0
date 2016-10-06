#pragma once
#include "LinkedList.h"

template<typename T>
class HashTable
{
private:
	LinkedList<T>*		m_Array;
	int					m_iLenth;		// LinkedListÀÇ °³¼ö

private:
	int					HashCode(const TCHAR*);

public:
	void				InsertData(T*);
	T*					Find(TCHAR*);
	void				Free();

public:
	HashTable(int iTableSize = 10);
	~HashTable();
};

template<typename T>
int HashTable<T> ::HashCode(const TCHAR* pszStr)
{
	int iSum = 0;
	for (int iChar = 0; iChar < _tcslen(pszStr); iChar++)
	{
		iSum += pszStr[iChar];
	}
	return (_tcslen(pszStr) * iSum) % m_iLenth;
}

template<typename T>
void HashTable<T> ::InsertData(T* pData)
{
	int iIndex = HashCode(pData->m_szName);
	m_Array[iIndex].InsertLink(pData);
}

template<typename T>
T* HashTable<T>::Find(TCHAR* pszStr)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	TCHAR SaveName[MAX_PATH] = { 0, };
	_tsplitpath_s(pszStr, Drive, Dir, Name, Ext);
	_stprintf_s(SaveName, _T("%s%s"), Name, Ext);


	int iKey = HashCode(SaveName);
	return m_Array[iKey].Find(SaveName);
}

template<typename T>
void  HashTable<T>::Free()
{
	for (int iArray = 0; iArray < m_iLenth; iArray++)
	{
		m_Array[iArray].Free();
	}
}

template<typename T>
HashTable<T>::HashTable(int iTableSize)
{
	m_Array = new LinkedList<T>[iTableSize];
	m_iLenth = iTableSize;
}

template<typename T>
HashTable<T>::~HashTable()
{
}
