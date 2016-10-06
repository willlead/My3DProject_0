#include "Parser.h"
bool Parser::Init()
{
	m_pFile = NULL;
	return true;
}

bool Parser::Open(TCHAR* szFileName)
{
	m_pFile = _tfopen(szFileName, _T("rt")); 
	//rt ReadText

	if (m_pFile == NULL)
		return false;

	Read(); //  ���� ������ �о��.

	return true;
}

bool Parser::Close()
{
	if (m_pFile)
		fclose(m_pFile);

	return true;
}

bool Parser::FindWord(TCHAR* szFindWord)
{
	while (!feof(m_pFile)) // 
	{
		_fgetts(m_szBuffer, MAX_PATH, m_pFile); 
		// ���� �о� ���� 
		//TCHAR	m_szBuffer[MAX_PATH];

		_stscanf(m_szBuffer, _T("%s"), m_szWord);
		// �� �ٿ��� ù��° ���ڸ� �о szWord�� ����

		if (!_tcsicmp(m_szWord, szFindWord))
		{
			return true;
		}
	}
	return false;
}

Parser::Parser()
{
	Init();
}


Parser::~Parser()
{
	Close();
}
