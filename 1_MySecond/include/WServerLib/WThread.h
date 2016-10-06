#pragma once
#include "WServerObj.h"
class WThread : public WServerObj
{
public:
	UINT m_hThread;
	UINT m_iThreadID;
	bool m_bStarted;
public:
	void CreateThread();
	static UINT WINAPI HandleRunner(LPVOID parameter);
	virtual bool Run();
public:
	WThread();
	virtual ~WThread();
};

