#pragma once
#include "Protocol.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1000

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
