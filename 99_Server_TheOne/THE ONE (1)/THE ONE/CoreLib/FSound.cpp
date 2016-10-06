#include "FSound.h"

int FSound::Load(char* pszName)
{
	m_pSystem->createSound(pszName, FMOD_HARDWARE, 0, &m_pSound[m_iNumSound++]);
	return m_iNumSound - 1;
}

int FSound::LoadBGM(char* pszName)
{
	m_pSystem->createSound(pszName, FMOD_HARDWARE | FMOD_LOOP_NORMAL, 0, &m_pSound[m_iNumSound++]);
	return m_iNumSound - 1;
}

void FSound::Play(int iIndex, bool bPlay)
{
	bool iPlaying = false;
	if (bPlay == false)
	{
		if (m_pChannel[iIndex] != 0)
		{
			m_pChannel[iIndex]->isPlaying(&iPlaying);
		}
	}
	if (iPlaying == false)
	{
		m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pSound[iIndex], false, &m_pChannel[iIndex]);
	}
}

void FSound::Stop(int iIndex)
{
	m_pChannel[iIndex]->stop();
}

void FSound::Paused(int iIndex)
{
	bool bPaused;
	m_pChannel[iIndex]->getPaused(&bPaused);
	m_pChannel[iIndex]->setPaused(!bPaused);
}

void FSound::Volume(int iIndex, float fVolume, bool bMode)
{
	float fCurrentVolume;
	m_pChannel[iIndex]->getVolume(&fCurrentVolume);
	if (bMode)
	{
		fCurrentVolume += fVolume;
	}
	else
	{
		fCurrentVolume -= fVolume;
	}
	m_pChannel[iIndex]->setVolume(fCurrentVolume);
}

bool FSound::Init()
{
	System_Create(&m_pSystem);
	m_pSystem->init(MAX_SOUND, FMOD_INIT_NORMAL, 0);
	return true;
}

bool FSound::Frame()
{
	m_pSystem->update();
	return true;
}

bool FSound::Render()
{
	return true;
}

bool FSound::Release()
{
	for (int iSound = 0;iSound < MAX_SOUND; iSound++)
	{
		m_pSound[iSound]->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
}

FSound::FSound()
{
	m_iNumSound = 0;
}


FSound::~FSound()
{
}
