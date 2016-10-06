#include "Sample.h"
#include "TDxState.h"
//#include <time.h>
bool Sample::Init()
{
	//srand(timeGetTime());

	if (GetDevice() == NULL) return true;

	I_Sprite.SetDevice(GetDevice());
	I_Sprite.Add(L"../../data/DeferredParticle.DDS",
		L"../../data/shader/plane.hlsl",
		TDxState::g_pAlphaBlend);

	INT iIndex = I_Sprite.Add(L"../../data/lighting.dds",
		L"../../data/shader/plane.hlsl",
		TDxState::g_pColorBlend);
	TSprite* pSprite = I_Sprite.GetPtr(iIndex);
	pSprite->SetAttribute(3.0f, 4, 128, 4, 128);

	iIndex = I_Sprite.Add(0,
		L"../../data/shader/plane.hlsl",
		TDxState::g_pColorBlend);
	pSprite = I_Sprite.GetPtr(iIndex);

	T_STR_VECTOR FileList;
	for (int iTex = 0; iTex < 10; iTex++)
	{
		TCHAR pBuffer[50];
		if (iTex < 10)
			_stprintf_s(pBuffer,
				L"../../data/shine%d.bmp", iTex);
		else
			_stprintf_s(pBuffer,
				L"../../data/shine0%d.bmp", iTex);
		FileList.push_back(pBuffer);
	}
	pSprite->SetTextureArray(FileList);
	pSprite->SetAttribute(2.0f);

	if (!m_ParticleSystem.Create(GetDevice(),
		100, D3DX_PI*2.0f))
	{
		return false;
	}

	m_ParticleSystem.SetScale(10.0f, 10.f, 10.f);
	m_ParticleSystem.SetAddRadius(50.f);

	//SAFE_NEW_ARRAY(m_pParticles, TParticle, MAX_PARTICLE);
	////if (!m_pParticles && MAX_PARTICLE) m_pParticles = new TParticle[MAX_PARTICLE];

	//SAFE_NEW(m_pSprite, TSprite);
	//if (m_pSprite->Create(GetDevice(),
	//	L"../../data/shader/plane.hlsl",
	//	L"../../data/particle.bmp") == false)
	//{
	//	MessageBox(0, _T("m_pSprite->Create"), _T("Fatal error"), MB_OK);
	//	return 0;
	//}
	//D3DXMatrixIdentity(&m_matWorld);

	m_pMainCamera = make_shared<TModelViewCamera>();
	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 0.0f, -5.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(m_iWindowWidth, m_iWindowHeight);
	return true;
}
bool Sample::Frame()
{

	//float fElapsedTime = m_Timer.GetSPF();


	m_fSumTimer += m_Timer.GetSPF();
	float fRadius = 1.0f;

	if (I_Input.KeyCheck(DIK_HOME) == KEY_UP)
	{
		m_dwEffectType++;
		if (m_dwEffectType > 13) m_dwEffectType = 0;
		m_ParticleSystem.ResetCounter();
		m_fSumTimer = 0.0f;
	}
	if (I_Input.KeyCheck(DIK_END) == KEY_UP)
	{
		m_dwEffectType--;
		if (m_dwEffectType < 0) m_dwEffectType = 12;
		m_ParticleSystem.ResetCounter();
		m_fSumTimer = 0.0f;
	}

	switch (m_dwEffectType)
	{
	default:
		break;
	case 1: // 세잎 장미
		fRadius = sinf(m_fSumTimer / 2); break;
	case 2: // 네잎장미
		fRadius = sinf(2 * m_fSumTimer); break;
	case 3: // 별
		fRadius = sinf(4 * m_fSumTimer); break;

	case 4:  // Cardioid( 심장형 곡선 좌 )
		fRadius = 1 + cosf(m_fSumTimer); break;
	case 5:  // Cardioid( 심장형 곡선 우)
		fRadius = 1 - cosf(m_fSumTimer); break;
	case 6:  // Cardioid( 심장형 곡선 상)
		fRadius = 1 - sinf(m_fSumTimer); break;
	case 7:  // Cardioid( 심장형 곡선 상)
		fRadius = 1 + sinf(m_fSumTimer); break;

	case 8:  // 파형 가로
		fRadius = 1 + cosf(2 * m_fSumTimer); break;
	case 9:  // 파형 세로
		fRadius = 1 - cosf(2 * m_fSumTimer); break;
	case 10: // 트랙 ( 가로 )
		fRadius = 2 + cosf(2 * m_fSumTimer); break;
	case 11: // 트랙( 세로 ) 
		fRadius = 2 - cosf(2 * m_fSumTimer); break;
	case 12:// 별
		fRadius = tanf(m_fSumTimer); break;
	}
	m_ParticleSystem.SetRadius(fRadius);
	m_ParticleSystem.Frame(m_pImmediateContext, m_fSumTimer, g_fSecPerFrame);


	//for (int iParticle = 0; iParticle < MAX_PARTICLE; iParticle++)
	//{
	//	m_pParticles[iParticle].m_vPos += (fElapsedTime * m_pParticles[iParticle].m_vAdd);
	//	m_pParticles[iParticle].m_fTimer += fElapsedTime;
	//	float fTime = m_pParticles[iParticle].m_fTimer / m_pParticles[iParticle].m_fLifeTime;
	//	m_pParticles[iParticle].m_vColor.w = 0.1f;

	//	if (m_pParticles[iParticle].m_fTimer >= m_pParticles[iParticle].m_fLifeTime)
	//	{
	//		//m_pParticles[iParticle].m_bRender = false;
	//		m_pParticles[iParticle].m_fTimer = 0;
	//		m_pParticles[iParticle].m_vAdd *= -1.0f;
	//	}
	//}
	//// 빌보드 행렬
	//FLOAT fDeterminant;
	//D3DXMatrixInverse(&m_matBillboard, &fDeterminant, m_pMainCamera->GetViewMatrix());
	//m_matBillboard._41 = 0.0f;
	//m_matBillboard._42 = 0.0f;
	//m_matBillboard._43 = 0.0f;
	//m_matBillboard._44 = 1.0f;
	//m_pSprite->Frame(m_pImmediateContext, m_Timer.GetElapsedTime(), g_fSecPerFrame);

	m_pMainCamera->Update(g_fSecPerFrame);
	m_matWorld = *m_pMainCamera->GetWorldMatrix();
	return true;
}
bool Sample::Render()
{
	D3DXMATRIX matSpriteWorld, matTrans;
	ApplyBS(m_pImmediateContext, TDxState::g_pColorBlend);
	ApplyRS(m_pImmediateContext, TDxState::g_pRSNoneCullSolid);
	ApplyDSS(m_pImmediateContext, TDxState::g_pDSSDepthDisable);

	m_ParticleSystem.SetMatrix(NULL, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	m_ParticleSystem.Render(m_pImmediateContext);

	//for (int iParticle = 0; iParticle < MAX_PARTICLE; iParticle++)
	//{
	//	//if( !m_pParticles[iParticle].m_bRender ) continue;
	//	m_pSprite->m_cbData.Color = m_pParticles[iParticle].m_vColor;
	//	D3DXMatrixTranslation(&matTrans, m_pParticles[iParticle].m_vPos.x,
	//		m_pParticles[iParticle].m_vPos.y,
	//		m_pParticles[iParticle].m_vPos.z);

	//	matSpriteWorld = m_matBillboard *  matTrans;
	//	m_pSprite->SetMatrix(&matSpriteWorld, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	//	m_pSprite->Render(m_pImmediateContext);
	//}
	return true;
}
bool Sample::Release()
{
	m_ParticleSystem.Release();
	//SAFE_DEL(m_pSprite);
	//SAFE_DELETE_ARRAY(m_pParticles);
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
	m_dwEffectType = 0;
	m_fSumTimer = 0.0f;
	//SAFE_ZERO(m_pSprite);
	//SAFE_ZERO(m_pParticles);
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
#ifndef _DEBUG

TBASIS_RUN(_T("LSample_070_Partticle_1"));
#else

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	Sample gSample;
	if (!gSample.InitWindow(hInstance, nCmdShow, _T("LSample_070_Partticle_1")))	
	{		return 0;	}	ShowCursor(TRUE);
	gSample.Run();
	return 1;
}
#endif