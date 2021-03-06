#pragma once
#include "TBasisStd.h"
#include "fmod.hpp" // c언어
#include "fmod_errors.h"

#if defined(_WIN64)
#pragma comment (lib, "fmodex64_vc.lib")
#else
#pragma comment (lib, "fmodex_vc.lib")
#endif

using namespace FMOD;

class TSound
{
public:
	FMOD::Sound*	m_pSound; // 사운드파일 로더.
	FMOD::Channel*	m_pChannel;// 사운드의 실행 통로
	TCHAR			m_strBuffer[MAX_PATH];
	float			m_fVolume;
	TCHAR_STRING	m_strName;
	int				m_iIndex;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Load(TCHAR* pFileName);
	void Play(System*	pSystem);
	void Paused();
	void Stop();
	void VolumeUp();
	void VolumeDown();
	void SetMode(DWORD dwMode = FMOD_LOOP_NORMAL);
	TSound(void);
	~TSound(void);
};
class TSoundMgr : public TSingleton<TSoundMgr>
{
private:
	friend class TSingleton<TSoundMgr>;
public:
	typedef std::map<int, TSound*>::iterator TItor;
	FMOD::System*			m_pSystem;// Fmod core
	std::map<int, TSound*>	m_Map;
	int						m_iCurIndex;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	int			Load(const char* pFileName); // 사운드 추가
	TSound*		GetPtr(int iIndex);
public:
	TSoundMgr();
	~TSoundMgr();
};
#define I_SoundMgr TSoundMgr::GetInstance()

