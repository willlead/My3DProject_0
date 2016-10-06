#pragma once

#include "WLinkdedList.h"
#include "WStudent.h"

class WManager
{
public:
	WLinkdedList<WStudent> m_List;
public:
	bool InputData(int StudCnt=10);
	void ShowData();
	void Release();
	void Sort();
	WStudent* Find(int iCnt);
	WNode<WStudent>* Swap(	WNode<WStudent> * a,
							WNode<WStudent> * b);

public:
	WManager();
	virtual ~WManager();
};

