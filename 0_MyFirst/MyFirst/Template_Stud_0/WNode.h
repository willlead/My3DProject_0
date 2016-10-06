#pragma once
#include <iostream>

template < class Tpl >
class WNode
{
public:
	Tpl* m_pData;
	WNode* m_pNext;
	WNode* m_pPrev;
	void Delete()
	{
		delete m_pData;
	}
public:
	WNode();
	~WNode();
};


template <class Tpl>
WNode<Tpl>::WNode()
{
	m_pData = NULL;
	m_pNext = m_pPrev = 0;
}

template <class Tpl>
WNode<Tpl>::~WNode()
{
	delete m_pData;
}
