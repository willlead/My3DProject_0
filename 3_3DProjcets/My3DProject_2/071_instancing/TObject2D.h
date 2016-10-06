#pragma once
#include "TSprite.h"
#include "TParticle.h"

#pragma pack (push, 1)	
struct TInstance
{
	D3DXMATRIX	mMatWorld;
	D3DXVECTOR4 vColor;
};
#pragma pack(pop)

class TObject2D : public TSprite
{
public:
	vector<TParticle> m_ParticleList;
	ComPtr<ID3D11Buffer> m_BufferInstance;

	TInstance* m_pInstanceList;
	int m_iCounter;
	void SetCounter(int iCnt) { m_iCounter = iCnt;  }
	D3DXMATRIX m_matBillboard;
public:
	bool Create(ID3D11Device* pDevice,
		TCHAR* pLoadShaderString,
		TCHAR* pLoadTextureString,
		ID3D11BlendState* pBlendState = nullptr);
	void SetMatrix(const D3DXMATRIX* pWorld, const D3DXMATRIX* pView,
		const D3DXMATRIX* pProj);
	bool Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime);
	bool Render(ID3D11DeviceContext* pContext);
	HRESULT SetInputLayout();
	HRESULT InitBufferData(ID3D11Device* pd3dDevice);

public:
	TObject2D();
	virtual ~TObject2D();
};

