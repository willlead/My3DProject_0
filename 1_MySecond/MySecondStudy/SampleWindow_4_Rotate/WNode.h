#pragma once
#include "WStd.h"

template <class W>
class WNode
{
public:
	W* m_pData;
	WNode* m_pNext;
	WNode * m_pPrev;
public:
	WNode();
	virtual ~WNode();
};

template <class W>
WNode<W>::WNode() {
	W* m_pData = NULL;
	WNode* m_pNext = NULL;
	WNode * m_pPrev = NULL;
}
template <class W>
WNode<W>::~WNode() {
	delete m_pData;
}
