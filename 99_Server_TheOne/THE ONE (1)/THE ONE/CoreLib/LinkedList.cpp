#include "LinkedList.h"

template <typename T>
void LinkedList<T>::InsertLink(T* pData)
{
	Node<T>* pNode = new Node<T>;
	pNode->m_pData = pData;
	if (m_pHead == 0)
	{
		m_pHead = pNode;
		m_pTail = pNode;
	}
	else
	{
		m_pTail->m_pNext = pNode;
		pNode->m_pPrev = m_pTail;
		m_pTail = m_pTail->m_pNext;
	}
}

template <typename T>
T* LinkedList<T>::Find(TCHAR* pszStr)
{
	Node<T>* pNode = m_pHead;
	if (pNode == 0)
	{
		return 0;
	}
	while (_tcsicmp(pNode->m_pData->m_szName, pszStr) != 0)
	{
		pNode = pNode->m_pNext;
		if (pNode == 0)
		{
			return 0;
		}
	}
	return pNode->m_pData;
}

template <typename T>
void  LinkedList<T>::Free()
{
	Node<T>* pNode = m_pHead;
	Node<T>* pDelLink = 0;
	while (pNode != 0)
	{
		pDelLink = pNode;
		pNode = pNode->m_pNext;
		delete pDelLink;
	}
}

template <typename T>
LinkedList<T>::LinkedList()
{
	m_pHead = m_pTail = 0;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
	Free();
}
