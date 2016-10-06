#pragma once
#include "TBasisStd.h"
#include "TModel.h"
//#include "TMesh.h"
class Parser
{
public:
	Parser();
	virtual ~Parser();

public:
	FILE*	m_pFile;
	TCHAR	m_szBuffer[MAX_PATH]; // ���� , �� 
	TCHAR	m_szWord[MAX_PATH]; // ����
public:
	bool	Init();
	bool	Open(TCHAR* szFileName);
	bool	Close();

	bool	FindWord(TCHAR* szFindWord);
public:
	virtual	bool	Read() = 0;
};
