#pragma once
#include <Windows.h>
#include <tchar.h>

struct WRect
{
	int   x, y;
	int   w, h;
};

struct NodeType
{
	int data;
	WRect rect;
};

template <class W> class WSingleton
{
public:
	static W& GetInstance()
	{
		static W theSingleton;
		return theSingleton;
	}
};