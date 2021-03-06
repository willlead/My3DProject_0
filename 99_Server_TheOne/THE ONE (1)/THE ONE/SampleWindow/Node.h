#pragma once
#include "Std.h"

template <typename T>
class Node
{
public:
	T*			m_pData;
	Node*		m_pPrev;
	Node*		m_pNext;

public:
	Node();
	~Node();
};

////////////  생성자  //////////////

template <typename T>
Node<T>::Node()
{
	m_pData = 0;
	m_pPrev = m_pNext = 0;
}

////////////  해제자  //////////////

template <typename T>
Node<T>::~Node()
{
	SAFE_DEL(m_pData);
}