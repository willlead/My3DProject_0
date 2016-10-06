#include "TPointParticle.h"
#include "TTimer.h"
HRESULT			TPointParticle::SetInputLayout()
{

}
HRESULT			TPointParticle::CreateVertexBuffer()
{

}
HRESULT			TPointParticle::CreateConstantBuffer()
{

}
HRESULT			TPointParticle::LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pShaderFile)
{
	HRESULT hr = S_OK;
	TShape::LoadShaderFile(pDevice, pShaderFile);
	m_pGSAdvanceParticlesMain.Attach(DX::LoadGeometryShaderFile(pDevice, pShaderFile,
		m_Object.g_pGSBlob.GetAddressOf(),
		"GSAdvanceParticlesMain"));

	D3D11_SO_DECLARATION_ENTRY pDecl[] =
	{
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "NORMAL", 0, 0, 3, 0 },
		{ 0, "TIMER", 0, 0, 1, 0 },
		{ 0, "TYPE", 0, 0, 1, 0 },
	};
	//float3 vPos : POSITION;         //position of the particle
	//float3 vVel : NORMAL;           //velocity of the particle
	//float  fTimer : TIMER;            //timer for the particle
	//uint   Type : TYPE;             //particle type
	int iEntry = sizeof(pDecl);
	int size = ARRAYSIZE(pDecl);
	UINT stride = 8 * sizeof(float);
	UINT elems = sizeof(pDecl) / sizeof(D3D11_SO_DECLARATION_ENTRY);

	UINT strides[] = { sizeof(PARTICLE_VERTEX) };
	if (FAILED(hr = m_pd3dDevice->CreateGeometryShaderWithStreamOutput(
		(void*)m_Object.g_pGSBlob.Get()->GetBufferPointer(),
		m_Object.g_pGSBlob.Get()->GetBufferSize(),
		pDecl,
		4,  
		strides,
		1,
		D3D11_SO_NO_RASTERIZED_STREAM,
		NULL,
		m_pGSStreamOutput.GetAddressOf())))
	{
		H(hr);
		return hr;
	}
	m_pVSPassThroughMain.Attach(DX::LoadVertexShaderFile(pDevice, pShaderFile,
		m_Object.g_pVSBlob.GetAddressOf(), "VSPassThroughmain"));
	return S_OK;


}
bool			TPointParticle::Init()
{

}
bool			TPointParticle::Frame(ID3D11DeviceContext*    pContext,
	float fElapsedTime, float fSecPerFrame)
{

}
bool			TPointParticle::Render(ID3D11DeviceContext*    pImmediateContext)
{

}
HRESULT			TPointParticle::CreateResource()
{

}

TPointParticle::TPointParticle()
{
}


TPointParticle::~TPointParticle()
{
}
