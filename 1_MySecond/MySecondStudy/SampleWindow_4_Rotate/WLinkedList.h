#pragma once
#include "WNode.h"

template <class W>
class WLinkedList
{
public : 
	int m_iCount;
	WNode<W>* m_pHead;
	WNode<W>* m_pTail;
	WNode<W>* GetHead();
	bool AddLink(W* pLink);
	void Release();
	
	void Delete(int iIndex) {};
	void Delete(W* iData) {};
	void Delete(WNode<W>* pNode) {};

	W* GetData(int iIndex	)
	{
		WNode<W>* pNode = m_pHead;
		while (pNode!=NULL)
		{
			if (pNode->m_pData->m_iIndex == iIndex)
			{
				return pNode->m_pData;
			}
			pNode = pNode->m_pNext;
		}
		return NULL;
	};
	W*				GetData(TCHAR* pszData)
	{
		WNode<W>* pNode = m_pHead;
		while (pNode != NULL)
		{
			if (_tcsicmp(pNode->m_pData->m_szName,
				pszData) == 0)
			{
				return pNode->m_pData;
			}
			pNode = pNode->m_pNext;
		}
		return NULL;
	};

	
public:
	WLinkedList();
	virtual ~WLinkedList();
};

#pragma region func
template <class W>
WNode<W>* WLinkedList<W>::GetHead() {
	return m_pHead;
}
template <class W>
bool WLinkedList<W>::AddLink(W* pLink) {

}
template <class W>
void WLinkedList<W>::Release() {

}


template <class W>
WLinkedList<W>::WLinkedList() {
	m_iCount = 0;
	m_pHead = NULL;
	m_pTail = NULL;
}
template <class W> 
WLinkedList<W>::~WLinkedList() {

}
#pragma endregion