#pragma once
#include <Windows.h>
#include <tchar.h>
using namespace std;

struct WRect
{
	int   x, y;
	int   w, h;
};

struct NodeType
{
	int index;
	int data;
	WRect rect;
	NodeType* pHead;
	NodeType* pTail;

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