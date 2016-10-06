#include "stdafx.h"
#include "Select.h"


bool Select::RemvoeSession(User* pUser) {
	closesocket(pUser->m_Sockek);
	pUser->m_isConnect = FALSE;

}
User* Select::GetSession() {

}
bool Select::Run() {

}
bool Select::InitSelectServert() {

}





Select::Select()
{
}


Select::~Select()
{
}
