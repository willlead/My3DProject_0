#pragma once
#include "Std.h"
#include "fmod.hpp"
#include "fmod_errors.h"

using namespace FMOD;

#define MAX_SOUND 100

class FSound
{
public:
	int				m_iNumSound;
	System*			m_pSystem;
	Sound*			m_pSound[MAX_SOUND];
	Channel*		m_pChannel[MAX_SOUND];

public:
	int				Load(char*);
	int				LoadBGM(char*);
	void			Play(int, bool);
	void			Stop(int);
	void			Paused(int);
	void			Volume(int, float, bool);

public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();

public:
	FSound();
	virtual ~FSound();
};

