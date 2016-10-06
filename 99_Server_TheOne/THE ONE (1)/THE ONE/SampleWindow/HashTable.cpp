#include "HashTable.h"

//////////////  HashCode  //////////////

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

//////////////  Insert  //////////////

template<typename T>
void HashTable<T> ::InsertData(T* pData)
{
	int iIndex = HashCode(pData->m_szName);
	m_Array[iIndex].InsertLink(pData);
}

//////////////  Delete  //////////////

template<typename T>
void HashTable<T> ::DeleteData(const TCHAR* pszStr, int iNum)
{
	int iIndex = HashCode(pszStr);
	Node<T>* pNode = m_Array[iIndex].Find(iNum);
	m_Array[iIndex].DeleteLink(pNode);
}

//////////////  Find  //////////////

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

//////////////  Free  //////////////

template<typename T>
void  HashTable<T>::Free()
{
	for (int iArray = 0; iArray < m_iLenth; iArray++)
	{
		m_Array[iArray].Free();
	}
}

//////////////  ������  //////////////

template<typename T>
HashTable<T>::HashTable(int iTableSize)
{
	m_Array = new LinkedList<T>[iTableSize];
	m_iLenth = iTableSize;
}

//////////////  ������  //////////////

template<typename T>
HashTable<T>::~HashTable()
{
}
