#include "TParticle.h"



TParticle::TParticle()
{
	int iRand = rand();
	m_bRender = true;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLifeTime = (float)(rand() % 5) + 5.0f;
	m_fTimer = 0.0f;
	m_vAdd = D3DXVECTOR3(cosf(iRand), sinf(iRand), 0);
	m_vColor.x = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.y = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.z = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.w = (float)(rand() % RAND_MAX) / RAND_MAX;
}


TParticle::~TParticle()
{
}
