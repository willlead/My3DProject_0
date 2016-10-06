#pragma once
#include "TCore.h"
#include "TModelViewCamera.h"

#include "TParticleSystem.h"
#include "TSprite.h"
#include "TParticle.h"

class Sample : public TCore
{
public:
	TParticleSystem m_ParticleSystem;
	DWORD			m_dwEffectType;
	float			m_fSumTimer;
public:
	shared_ptr<TModelViewCamera>	m_pMainCamera;
	TSprite*						m_pSprite;
	TParticle*						m_pParticles;
	D3DXMATRIX						m_matWorld;
	D3DXMATRIX						m_matBillboard;
public:
	bool							Init();
	bool							Frame();
	bool							Render();
	bool							Release();
	int								WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HRESULT							CreateResource();
	HRESULT							DeleteResource();
public:
	Sample();
	~Sample();
};

