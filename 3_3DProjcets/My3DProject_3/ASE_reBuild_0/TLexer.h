#pragma once
#include <stdio.h>
#include <tchar.h>
class TLexer
{
public:
	int GetToken(TCHAR* TokenString);
	TCHAR GetNextChar();

public:
	TLexer();
	virtual ~TLexer();
};

