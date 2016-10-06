#include "TParticleSystem.h"
#include "TTimer.h"
//#include "TSpriteMgr.h"

void TParticleSystem::Init()
{
	m_pd3dDevice = NULL;
	m_pSprite = NULL;
	m_pParticles = NULL;
	m_fRadius = 1.0f;
	m_iStartCnt = 0;
	m_iEndCnt = 1;
	m_fTime = 0.0f;
	m_iMaxCnt = MAX_PARTICLE;
	m_fAddTime = 0.1f;
	m_fAddRadius = 30.0f;
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;
}
bool TParticleSystem::Create(
	ID3D11Device* pDevice, int iMaxCounter,
	float fAddTime, float fAddRadius,
	D3DXVECTOR3 vScale)
{
	m_pd3dDevice = pDevice;
	assert(m_pd3dDevice);
	SetMaxCounter(iMaxCounter);
	SetAddTime(fAddTime / iMaxCounter);
	SetRadius(fAddRadius);
	SetScale(vScale.x, vScale.y, vScale.z);
	D3DXMatrixIdentity(&m_matWorld);
	return true;
}
bool TParticleSystem::Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime)
{
	D3DXVECTOR3 vReturnValue = Circle(m_fRadius, fGlobalTime);
	vReturnValue *= m_fAddRadius;

	if ((fGlobalTime - m_fTime) > m_fAddTime)
	{
		if (m_iEndCnt + 1 > m_iMaxCnt)
		{
			m_iEndCnt = m_iMaxCnt;
			m_iStartCnt++;
			if (m_iStartCnt >= m_iEndCnt)
			{
				m_iStartCnt = 0, m_iEndCnt = 1;
			}
		}
		else
		{
			m_pParticles[m_iEndCnt].m_vPos.x = vReturnValue.x;
			m_pParticles[m_iEndCnt].m_vPos.y = vReturnValue.y;
			m_pParticles[m_iEndCnt].m_vPos.z =
				sqrt(vReturnValue.x * vReturnValue.x + vReturnValue.y* vReturnValue.y);
			m_iEndCnt += 1;
		}
		m_fTime = fGlobalTime;
	}

	return true;
}
void TParticleSystem::Render(ID3D11DeviceContext* pContext)
{
	D3DXMATRIX matTransposed, matTrans, matScale;
	D3DXMATRIX matY;

	D3DXMatrixInverse(&matTransposed, NULL, &m_matView);

	matTransposed._41 = 0.0f; matTransposed._42 = 0.0f; matTransposed._43 = 0.0f; 
	matTransposed._44 = 1.0f;

	for (int iParticle = m_iStartCnt; iParticle < m_iEndCnt; iParticle++)
	{
		D3DXMatrixTranslation(&matTrans,
			m_pParticles[iParticle].m_vPos.x,
			m_pParticles[iParticle].m_vPos.y,
			m_pParticles[iParticle].m_vPos.z);
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixMultiply(&m_matWorld, &matScale, &matTransposed);
		D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matTrans);

		m_pSprite = I_Sprite.GetPtr(m_pParticles[iParticle].m_iSpriteIndex);
		m_pSprite->m_cbData.Color = m_pParticles[iParticle].m_vColor;
		m_pSprite->Updata(pContext,
			m_pParticles[iParticle].m_fTimer,
			m_pParticles[iParticle].m_iApplyIndex,
			g_fDurationTime, g_fSecPerFrame);
	}


}
void TParticleSystem::Release()
{
	SAFE_DELETE_ARRAY(m_pParticles);
}
void TParticleSystem::SetMaxCounter(int iMaxNum)
{
	m_iMaxCnt = iMaxNum;
	SAFE_DELETE_ARRAY(m_pParticles);
	SAFE_NEW_ARRAY(m_pParticles, TParticle, m_iMaxCnt);
	assert(m_pParticles);
}
void TParticleSystem::ResetCounter()
{
	m_iStartCnt = 0;
	m_iEndCnt = 1;
	m_fTime = 0.0f;
}
void TParticleSystem::SetMatrix(D3DXMATRIX* pmatWorld, D3DXMATRIX* pmatView, D3DMATRIX* pmatProj)
{
	assert(pmatView && pmatProj);
	if (pmatWorld != NULL)
		m_matWorld = *pmatWorld;
	m_matView = *pmatView;
	m_matProj = *pmatProj;
}
D3DXVECTOR3 TParticleSystem::Circle(float fRadius, float fRadian)
{
	D3DXVECTOR3 vFinally;
	vFinally.x = fRadius * cosf(fRadian);
	vFinally.y = fRadius * sinf(fRadian);
	vFinally.z = 0.0f;
	return vFinally;
}
TParticleSystem::TParticleSystem()
{
	Init();
}
TParticleSystem::~TParticleSystem()
{
}
