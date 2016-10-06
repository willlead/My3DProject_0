#include "TDebugString.h"

void		TDebugString::DisplayText(char* fmt, ...)
{

}
TCHAR*		TDebugString::GetMbtoWcs(const char* srcmsg)
{

}
char*		TDebugString::GetWcstoMbs(WCHAR* srcmsg)
{

}
HRESULT		TDebugString::ConvertAnsiStringToWideCch(WCHAR* wstrDestination,
	const CHAR* strSource,
	int cchDestChar)
{

}
HRESULT		TDebugString::ConvertWideStringToAnsiCch(CHAR* strDestination, const WCHAR* wstrSource,
	int cchDestChar)
{

}

bool		TDebugString::Init()
{
	return true;
}
bool		TDebugString::Frame()
{
	return true;
}
bool		TDebugString::Release()
{
	return true;
}

void		TDebugString::T_ERROR(bool bPrint)
{

}

TDebugString::TDebugString()
{
}


TDebugString::~TDebugString()
{
}
