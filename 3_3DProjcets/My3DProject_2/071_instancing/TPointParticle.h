#pragma once
#include "TShape.h"
#define MAX_PARTICLES 30000

struct PARTICLE_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vVel;
	float fTimer;
	UINT Type;	
};
struct PARTICLE_CONSTANT_BUFFER
{
	D3DXMATRIX m_pmWorldViewProj;
	D3DXMATRIX m_pmInvView;

	D3DXVECTOR4 m_pvFrameGravity;

	float			m_pfGlobalTime;
	float			m_pfElapsedTime;
	float			m_pSecondsPerFirework;
	float			m_pNumEmber1s;

	float			m_pMaxEmber2s;
	float			m_py;
	float			m_pz;
	float			m_pw;
};
class TPointParticle : public TShape
{
public:
	ID3D11Buffer*                       m_pParticleStreamTo;
	ID3D11Buffer*                       m_pParticleDrawFrom;
	ID3D11Texture1D*                    m_pRandomTexture;
	ComPtr<ID3D11ShaderResourceView>    m_pRandomTexRV;
	ID3D11VertexShader*					m_pRenderParticles;
	ID3D11VertexShader*					m_pAdvanceParticles;
	ComPtr<ID3D11Buffer>				m_pConstantBuffer;
	PARTICLE_CONSTANT_BUFFER			m_cbParticleData;
	bool                                m_bFirst;
	ComPtr<ID3D11GeometryShader>		m_pGSAdvanceParticlesMain;
	ComPtr<ID3D11VertexShader>			m_pVSPassThroughMain;
	ComPtr<ID3D11GeometryShader>	    m_pGSStreamOutput;
public:
	HRESULT			SetInputLayout();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateConstantBuffer();
	HRESULT			LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pShaderFile);
	bool			Init();
	bool			Frame(ID3D11DeviceContext*    pContext, float fElapsedTime, float fSecPerFrame);
	bool			Render(ID3D11DeviceContext*    pImmediateContext);
	HRESULT			CreateResource();
public:
	TPointParticle();
	virtual ~TPointParticle();
};

