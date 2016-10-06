#include "WManager.h"

bool WManager::InputData(int StudCnt)
{
	for (int i = 0; i < StudCnt; i++)
	{
		WStudent * pStudent = new WStudent;
		pStudent->m_iIndex = i;
		
		for (int iName = 0; iName < 4; iName++)
		{			
			pStudent->m_szName[iName] = 65 + rand() % 26;
		}
		pStudent->m_szName[4] = 0;
		pStudent->m_Subject.m_iKor = rand() % 101;
		pStudent->m_Subject.m_iEng = rand() % 101;
		pStudent->m_Subject.m_iMat = rand() % 101;
		pStudent->m_iTotal =
			pStudent->m_Subject.m_iKor +
			pStudent->m_Subject.m_iEng +
			pStudent->m_Subject.m_iMat;
		pStudent->m_fAverage = pStudent->m_iTotal / 3.0f;
		m_List.AddLink(pStudent);
	}
	return true;
}


void WManager::ShowData()
{
	for (WNode<WStudent>*	pNode = m_List.GetHead();
							pNode != NULL;
							pNode = pNode = pNode->m_pNext)
	{
		WStudent * pData = pNode->m_pData;
		wcout << pData->m_iIndex << " "
			<< pData->m_szName << " "
			<< pData->m_Subject.m_iKor << " "
			<< pData->m_Subject.m_iEng << " "
			<< pData->m_Subject.m_iMat << " "
			<< pData->m_iTotal << " "
			<< endl;
	}
}


void WManager::Release()
{
	m_List.Delete();
}

void WManager::Sort()
{
	WNode<WStudent>* pNodeSrc = m_List.GetHead();
	while (pNodeSrc && pNodeSrc->m_pNext)
	{
		WNode<WStudent>* pNodeSwap = pNodeSrc;
		WStudent* pSrcStudent = pNodeSwap->m_pData;
		for (WNode<WStudent>* pNodeDesk = pNodeSrc->m_pNext;
		pNodeDesk != NULL;
			pNodeDesk = pNodeDesk->m_pNext)
		{
			WStudent* pDeskStudent = pNodeDesk->m_pData;
			if (pNodeSwap->m_pData->m_iTotal < pDeskStudent->m_iTotal)
			{
				pNodeSwap = pNodeDesk;
			}
		}
		if (pNodeSrc != pNodeSwap)
		{
			pNodeSrc = Swap(pNodeSrc, pNodeSwap);
		}
		else
		{
			pNodeSrc = pNodeSrc->m_pNext;
		}
	}
	/*WNode<WStudent> * pSwap = m_List.GetHead();
	WNode<WStudent> * pMax;
	WNode<WStudent> * pMaxNext;
	while (pSwap!=0 && pSwap->m_pNext!=0)
	{
		pMax = pSwap;		
		pMaxNext = pMax->m_pNext;
		while (pMaxNext != 0)
		{
			if (pMax->m_pData->m_iTotal < pMaxNext->m_pData->m_iTotal)
			{
				pMax = pMaxNext;
			}
			pMaxNext = pMaxNext->m_pNext;
		}
		if (pMax != pSwap)
		{
			pSwap = Swap(pSwap, pMax);
		}
		pSwap = pSwap->m_pNext;
	
	}*/
}

WStudent * WManager::Find(int iCnt)
{
	WStudent * pData = m_List.GetData(iCnt);

	return pData;
}

template<class Tpl>
void SwapPos(Tpl* pA, Tpl* pB)
{
	pA->m_pNext = pB;
	pB->m_pPrev = pA;
}

WNode<WStudent>* WManager::Swap(WNode<WStudent>* a, WNode<WStudent>* b)
{
	WNode<WStudent>* apPrev = a->m_pPrev;
	WNode<WStudent>* apNext = a->m_pNext;
	WNode<WStudent>* bpPrev = b->m_pPrev;
	WNode<WStudent>* bpNext = b->m_pNext;
	// 1번 Haed <-> b
	if (apPrev != NULL) {
		SwapPos(apPrev, b);
	}
	else {
		m_List.m_pHead = b;
		b->m_pPrev = NULL;
	}

	// b <-> a
	if (apNext != b)
	{
		SwapPos(b, apNext);
		// a -> c -> b
		if (apNext->m_pNext == b)	
			SwapPos(apNext, a);
		else						
			SwapPos(bpPrev, a);
	}
	else // b <-> a
	{
		SwapPos(b, a);
		a->m_pNext = bpNext;
		if (bpNext != NULL) 
			bpNext->m_pPrev = a;
	}
	// a-> c -> b ->d
	if (bpNext != NULL)  
		SwapPos(a, bpNext);
	else				
		a->m_pNext = NULL;

	if (apNext == b) return a;
	return apNext;

	//// a가 헤드인경우
	//if (apPrev == NULL)
	//{
	//	m_List.m_pHead = b;
	//	b->m_pPrev = NULL;
	//}
	//// a가 헤드가 아닌경우
	//else
	//{
	//	apPrev->m_pNext = b;
	//	b->m_pPrev = apPrev;
	//}
	//// a,b가 연속적인 경우
	//if (apNext == b)
	//{
	//	SwapPos(b, apNext);
	//	SwapPos(bpPrev, a);
	//}
	//// a,b가 연속적이지 않은 경우
	//else
	//{
	//	SwapPos(b, a);
	//}
	//// b가 테일인 경우
	//if (bpNext == NULL)
	//{
	//	a->m_pNext = NULL;		
	//	m_List.SetTail(a);
	//}
	//// b가 테일이 아닌경우
	//else
	//{
	//	SwapPos(a, b);		
	//}
	//if (apNext == b) return a;
	//return b;
}

WManager::WManager()
{
}


WManager::~WManager()
{
}
