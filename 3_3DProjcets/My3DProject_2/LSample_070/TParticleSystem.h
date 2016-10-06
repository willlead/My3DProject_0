#pragma once
#include "TParticle.h"
class TParticleSystem
{
public:
	ID3D11Device*			m_pd3dDevice;
	TSprite*				m_pSprite;
	TParticle*				m_pParticles;

	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matView;
	D3DXMATRIX				m_matProj;

	int						m_iStartCnt;
	int						m_iEndCnt;
	float					m_fTime;
	float					m_fRadius;
	int						m_iMaxCnt;
	float					m_fAddTime;
	float					m_fAddRadius;
	D3DXVECTOR3				m_vScale;
public:
	void Init();
	bool Create(
		ID3D11Device* pDevice, int iMaxCounter = 100,
		float fAddTime = 10.0f, float fAddRadius = 30.0f,
		D3DXVECTOR3 vScale = D3DXVECTOR3 (1.0f, 1.0f, 1.0f));
	bool Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime);
	void Render(ID3D11DeviceContext* pContext);
	void Release();
public:
	void SetMaxCounter(int iMaxNum);
	int GetMaxCounter() { return m_iMaxCnt; }
	void ResetCounter();
public:
	void SetRadius(float fRadius = 1.0f) { m_fRadius = fRadius; }
	void SetAddTime(float fTime) { m_fAddTime = fTime; }
	void SetAddRadius(float fTime) { m_fAddRadius = fTime; }
	void SetScale(float fX, float fY, float fZ) { m_vScale.x = fX, m_vScale.y = fY, m_vScale.z = fZ; }
public:
	void SetMatrix(D3DXMATRIX* pmatWorld, D3DXMATRIX* pmatView, D3DMATRIX* pmatProj);
	D3DXVECTOR3 Circle(float fRadius, float fRadian);
public:
	TParticleSystem();
	virtual ~TParticleSystem();
};

