#include "WServerObj.h"



WServerObj::WServerObj()
{
	EnterCriticalSection(&m_CS);
}


WServerObj::~WServerObj()
{
	LeaveCriticalSection(&m_CS);
}
