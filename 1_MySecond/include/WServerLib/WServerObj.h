#pragma once
#include "WServerStd.h"
class WServerObj
{
public :
	CRITICAL_SECTION m_CS;
public:
	WServerObj();
	virtual ~WServerObj();
};

