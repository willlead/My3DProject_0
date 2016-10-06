#include "TSound.h"

#pragma region  TSOUND
bool TSound::Init()
{

}
bool TSound::Frame()
{

}
bool TSound::Render()
{

}
bool TSound::Release()
{

}
bool TSound::Load(TCHAR* pFileName)
{

}
void TSound::Play(System*	pSystem)
{

}
void TSound::Paused()
{

}
void TSound::Stop()
{

}
void TSound::VolumeUp()
{

}
void TSound::VolumeDown()
{

}
void TSound::SetMode(DWORD dwMode = FMOD_LOOP_NORMAL)
{

}

TSound::TSound()
{
}


TSound::~TSound()
{
}
#pragma endregion

#pragma region TSoundMgr
bool		TSoundMgr::Init()
{
	return true;
}
bool		TSoundMgr::Frame()
{
	return true;
}
bool		TSoundMgr::Render()
{
	return true;
}
bool		TSoundMgr::Release()
{
	return true;
}
int			TSoundMgr::Load(const char* pFileName) // 사운드 추가
{

}
TSound*		TSoundMgr::GetPtr(int iIndex)
{

}
TSoundMgr::TSoundMgr()
{
	m_iCurIndex = 0;
}
TSoundMgr::~TSoundMgr()
{
	Release();
}
#pragma endregion