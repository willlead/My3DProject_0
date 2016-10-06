#pragma once
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <process.h>
using namespace std;
#pragma comment (lib, "ws2_32.lib")
template<class W> class WSingleton
{
public:
	static W& GetInstance()
	{
		static W theSingleInstance;
		return theSingleInstance;
	}
};