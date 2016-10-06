#pragma once
#include "WSubject.h"

class WStudent
{
public:
	int m_iIndex;
	char m_szName[5];
	WSubject m_Subject;
	int m_iTotal;
	float m_fAverage;

public:
	int			GetIndex();
	
public:
	WStudent();
	virtual ~WStudent();
};

