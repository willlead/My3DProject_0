#include "TAcceptor.h"
#include "TDebugString.h"
#include "TServer.h"
#include "TSynchronize.h"
#include "TServerIOCP.h"

bool		TAcceptor::Init()
{

}
bool		TAcceptor::Run()
{

}
bool		TAcceptor::Set(int iPort, const char* strAddress )
{

}
int			TAcceptor::GetIPList(vector<string>& ipList)
{
	char szHostName[1024];
	string strHostName;
	string strIPAddr = "";
	int  i;
	WSADATA WsaData;
	if (WSAStartup(0x202, &WsaData) == SOCKET_ERROR)
	{
		return 0;
	}
	if (!::gethostname(szHostName, 1024))
	{
		strHostName = szHostName;
		struct hostent* pHost;
		pHost = gethostbyname(szHostName);

		if (pHost != NULL)
		{

			for (i = 0; i<255; i++)
			{
				if (pHost->h_addr_list[i])
				{
					BYTE    bAddr[4];
					char    strIPAddr[256] = { 0, };
					bAddr[0] = (BYTE)pHost->h_addr_list[i][0];
					bAddr[1] = (BYTE)pHost->h_addr_list[i][1];
					bAddr[2] = (BYTE)pHost->h_addr_list[i][2];
					bAddr[3] = (BYTE)pHost->h_addr_list[i][3];
					sprintf(strIPAddr, "%d.%d.%d.%d", bAddr[0], bAddr[1], bAddr[2], bAddr[3]);
					ipList.push_back(strIPAddr);
				}
				else
				{
					break;
				}
			}
		}
	}
	WSACleanup();
	return ipList.size();

}

TAcceptor::TAcceptor()
{
}


TAcceptor::~TAcceptor()
{
}
