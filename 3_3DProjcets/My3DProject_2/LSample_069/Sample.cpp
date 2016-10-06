#include "Sample.h"
#include <time.h>
bool Sample::Init()
{
	srand(timeGetTime());
	
	SAFE_NEW_ARRAY(m_pParticles, TParticle, MAX_PARTICLE);
	//if (!m_pParticles && MAX_PARTICLE) m_pParticles = new TParticle[MAX_PARTICLE];

	SAFE_NEW(m_pSprite, TSprite);
	if (m_pSprite->Create(GetDevice(),
		L"../../data/shader/plane.hlsl",
		L"../../data/particle.bmp") == false)
	{
		MessageBox(0, _T("m_pSprite->Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
	
	D3DXMatrixIdentity(&m_matWorld);

	m_pMainCamera = make_shared<TModelViewCamera>();
	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 0.0f, -5.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(m_iWindowWidth, m_iWindowHeight);
	return true;
}
bool Sample::Frame()
{
	float fElapsedTime = m_Timer.GetSPF();
	
	m_pMainCamera->Update(g_fSecPerFrame);
	m_matWorld = *m_pMainCamera->GetWorldMatrix();

	for (int iParticle = 0; iParticle < MAX_PARTICLE; iParticle++)
	{
		m_pParticles[iParticle].m_vPos += (fElapsedTime * m_pParticles[iParticle].m_vAdd);
		m_pParticles[iParticle].m_fTimer += fElapsedTime;
		float fTime = m_pParticles[iParticle].m_fTimer / m_pParticles[iParticle].m_fLifeTime;
		m_pParticles[iParticle].m_vColor.w = 0.1f;

		if (m_pParticles[iParticle].m_fTimer >= m_pParticles[iParticle].m_fLifeTime)
		{
			m_pParticles[iParticle].m_bRender = false;
			m_pParticles[iParticle].m_fTimer = 0;
			m_pParticles[iParticle].m_vAdd *= -1.0f;
		}
	}
	// ºôº¸µå Çà·Ä
	FLOAT fDeterminant;
	D3DXMatrixInverse(&m_matBillboard, &fDeterminant, m_pMainCamera->GetViewMatrix());
	m_matBillboard._41 = 0.0f;
	m_matBillboard._42 = 0.0f;
	m_matBillboard._43 = 0.0f;
	m_matBillboard._44 = 1.0f;
	m_pSprite->Frame(m_pImmediateContext, m_Timer.GetElapsedTime(), g_fSecPerFrame);
	return true;
}
bool Sample::Render()
{
	D3DXMATRIX matSpriteWorld, matTrans;
	ApplyBS(m_pImmediateContext, TDxState::g_pColorBlend);
	ApplyRS(m_pImmediateContext, TDxState::g_pRSNoneCullSolid);
	ApplyDSS(m_pImmediateContext, TDxState::g_pDSSDepthDisable);

	for (int iParticle = 0; iParticle < MAX_PARTICLE; iParticle++)
	{
		//if( !m_pParticles[iParticle].m_bRender ) continue;
		m_pSprite->m_cbData.Color = m_pParticles[iParticle].m_vColor;
		D3DXMatrixTranslation(&matTrans, m_pParticles[iParticle].m_vPos.x,
			m_pParticles[iParticle].m_vPos.y,
			m_pParticles[iParticle].m_vPos.z);

		matSpriteWorld = m_matBillboard *  matTrans;
		m_pSprite->SetMatrix(&matSpriteWorld, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
		m_pSprite->Render(m_pImmediateContext);
	}
	return true;
}
bool Sample::Release()
{
	SAFE_DEL(m_pSprite);
	SAFE_DELETE_ARRAY(m_pParticles);
	return true;
}

HRESULT Sample::CreateResource()
{
	HRESULT hr;
	return S_OK;
}
HRESULT Sample::DeleteResource()
{
	HRESULT hr = S_OK;
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}
Sample::Sample()
{
	SAFE_ZERO(m_pSprite);
	SAFE_ZERO(m_pParticles);
}
Sample::~Sample()
{
	
}
int Sample::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
	}
	return -1;
}
TBASIS_RUN(L"TBasisSample Partticle_0");