#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <list>
#include <tchar.h>
#include "Protocol.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1000

template <class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleton;
		return theSingleton;
	}
};