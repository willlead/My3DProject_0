#include "TParticle.h"



TParticle::TParticle()
{	
	m_iSpriteIndex = (rand() % I_Sprite.TMap.size()) + 1;

	int iRand = rand();
	//m_bRender = true;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLifeTime = (float)(rand() % 5) + 5.0f;
	m_fTimer = 0.0f;
	m_vAdd = D3DXVECTOR3(cosf(iRand), sinf(iRand), 0);
	m_vColor.x = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.y = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.z = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.w = 1.0f;
	m_iApplyIndex = 0;
}


TParticle::~TParticle()
{
}
