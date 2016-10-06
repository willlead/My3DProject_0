#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TBasisLib_0.h"
#include "FileOpen.h"
class Sample : public TBasisLib_0
{
public:
	FILE* m_pFile;
	TCHAR m_szBuffer[MAX_PATH];
	TCHAR	m_szWord[MAX_PATH];
public:
	bool Open(char* file);

	bool Open(TCHAR* file);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};

