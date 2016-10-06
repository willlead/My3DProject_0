#pragma once
#include "WNode.h"

template <class Tpl> 
class WLinkdedList
{
public:
	int m_iCount;
	WNode<Tpl>* m_pHead;
	WNode<Tpl>* m_pTail;
public:
	bool AddLink(Tpl* pStud);
	bool DelLink(Tpl* pStud);
	void Delete();
	Tpl* GetData(int index);	
	WNode<Tpl>* SetHead(WNode<Tpl>* pNode) { return m_pHead = pNode; }
	WNode<Tpl>* SetTail(WNode<Tpl>* pNode) { return m_pTail = pNode; }
	WNode<Tpl>* GetHead() { return m_pHead; }
	WNode<Tpl>* GetTail() { return m_pTail; }
public:
	WLinkdedList();
	virtual ~WLinkdedList();
};

template<class Tpl>
bool WLinkdedList<Tpl>::AddLink(Tpl * pStud)
{
	WNode<Tpl>* pNode = new WNode<Tpl>;
	pNode->m_pData = pStud;

	if (m_pHead == NULL)
	{
		m_pHead = pNode;
		m_pTail = pNode;
	}
	else
	{		
		m_pTail->m_pNext = pNode;
		pNode->m_pPrev = m_pTail;
		m_pTail = pNode;		
	}
	m_iCount++;
	return true;
}

template<class Tpl>
bool WLinkdedList<Tpl>::DelLink(Tpl * pStud)
{
	WNode<Tpl>* pDelNode = new WNode<Tpl>;
	pDelNode->m_pData = pStud;
	
	if (pDelNode->m_pPrev = NULL)
	{
		m_pHead = m_pHead->m_pNext;
		m_pHead->m_pPrev = NULL;
	}
	else if(pDelNode->m_pNext = NULL)
	{
		m_pTail = m_pTail->m_pPrev;
		m_pTail->m_pNext = NULL;

	}
	else
	{
		pDelNode->m_pNext->m_pPrev = pDelNode->m_pPrev;
		pDelNode->m_pPrev->m_pNext = pDelNode->m_pNext;
	}	
	delete pStud;
	m_iCount--;
}

template<class Tpl>
void WLinkdedList<Tpl>::Delete()
{
	WNode<Tpl>* pDelNode = 0;
	WNode<Tpl>* pNode = m_pHead;
	while (pNode != NULL)
	{
		pDelNode = pNode;
		pNode = pNode->m_pNext;
		delete pDelNode;
	}
	m_pHead = NULL;
	m_pTail = NULL;
}

template<class Tpl>
Tpl * WLinkdedList<Tpl>::GetData(int index)
{
	WNode<Tpl>* pNode = m_pHead;
	while (pNode != NULL)
	{
		if (pNode->m_pData->GetIndex() == index)
		{
			Tpl* pData = pNode->m_pData;
			return pData;
		}
		pNode = pNode->m_pNext;
	}
	return NULL;
	
}

template <class Tpl>
WLinkdedList<Tpl>::WLinkdedList()
{
	m_pHead = NULL;
	m_pTail = NULL;
	m_iCount = 0;
}

template <class Tpl>
WLinkdedList<Tpl>::~WLinkdedList()
{
	Delete();
}
