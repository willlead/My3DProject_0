#pragma once

#include "TBasisLib_0.h"
#include "ParserASE.h"

class Sample : public TBasisLib_0
{
public:
	ParserASE    m_ParserASE;
public:
	bool Init();

public:
	Sample();
	virtual ~Sample();

};


