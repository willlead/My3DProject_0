#include "Protocol.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1000

#ifndef singleton
#define singleton

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

#endif

static char* GetWtoM(WCHAR* data)
{
	char szBuffer[1024];
	int iLen = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, data, -1, szBuffer, iLen, NULL, NULL);
	return szBuffer;
}

static WCHAR* GetMtoW(char* data)
{
	WCHAR szBuffer[1024];
	int iLen = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, data, iLen, szBuffer, iLen);
	return szBuffer;
}

