#pragma once
#include "TBasisStd.h"

#define MAX_PARTICLE 1000

class TParticle
{
public:
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vAdd;
	D3DXVECTOR4 m_vColor;
	float		m_fTimer;
	float		m_fLifeTime;	
public:
	TParticle();
	virtual ~TParticle();
};

