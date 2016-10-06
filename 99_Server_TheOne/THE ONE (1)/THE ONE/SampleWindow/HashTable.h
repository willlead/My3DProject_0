#pragma once
#include "LinkedList.cpp"

template<typename T>
class HashTable
{
private:
	LinkedList<T>*		m_Array;
	int					m_iLenth;

private:
	int					HashCode(const TCHAR*);

public:
	void				InsertData(T*);
	void				DeleteData(const TCHAR*, int);
	T*					Find(TCHAR*);
	void				Free();

public:
	HashTable(int iTableSize = 10);
	~HashTable();
};
