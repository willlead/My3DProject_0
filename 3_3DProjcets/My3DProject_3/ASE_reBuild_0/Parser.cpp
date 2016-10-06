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

	Read(); //  다음 문자을 읽어라.

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
		// 한줄 읽어 오기 
		//TCHAR	m_szBuffer[MAX_PATH];

		_stscanf(m_szBuffer, _T("%s"), m_szWord);
		// 그 줄에서 첫번째 문자를 읽어서 szWord에 저장


		//현재 파일 포인터의 위치를 저장하는 puase
		if (!_tcsicmp(m_szWord, szFindWord))
		{
			return true;
		}
	}
	return false;
}

INT Parser::FindWord2(TCHAR* szFindWord, INT index)
{
	FILE* fpTemp;
	fpTemp = m_pFile;
	_fgetts(m_szBuffer, MAX_PATH, m_pFile);
	// 한줄 읽어 오기 
	_stscanf(m_szBuffer, _T("%s"), m_szWord);
	// 그 줄에서 첫번째 문자를 읽어서 szWord에 저장

	//현재 파일 포인터의 위치를 저장하는 puase
	if (!_tcsicmp(m_szWord, szFindWord))
	{
		return index;
	}
	return 
}


Parser::Parser()
{
	Init();
}


Parser::~Parser()
{
	Close();
}
