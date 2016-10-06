#include "TClient.h"


bool		TClient::Init()
{
	return true;
}
bool		TClient::Frame()
{
	return true;
}
bool		TClient::Release()
{
	return true;
}
int			TClient::CreateConnectSocket(int iPort)
{
	return;
}
int			TClient::SendMsg(char* pMsg, int iSize, WORD code)
{
	return;
}
int			TClient::ProcessPacket()
{
	return;
}

TClient::TClient()
{
}


TClient::~TClient()
{
}
