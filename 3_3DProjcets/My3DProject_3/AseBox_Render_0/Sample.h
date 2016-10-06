#pragma once

#include "TBasisLib_0.h"
#include "ParserASE.h"

class Sample : public TBasisLib_0
{
public:
	ParserASE    m_ParserASE;
	TAseObject m_Object;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	Sample();
	virtual ~Sample();

};


