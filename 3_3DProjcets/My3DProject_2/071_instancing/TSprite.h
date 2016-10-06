#pragma once
#include "TShape.h"

struct TRectUV
{
	D3DXVECTOR4		vUV;
	RECT			Rect;
};

class TSprite : public TPlaneShape
{
public:
	T_STR					m_szName;
	T_STR					m_szPath;
	UINT					m_iIndex;
	UINT					m_BlendType;
	ID3D11BlendState*		m_pBlendState;
	vector<int>				m_TextureIndex;
	int						m_iApplyIndex;
	int						m_iNumTexture;
	float					m_fSecPerRender;
	float					m_fTime;
	RECT					RectSet;
	vector<TRectUV>			m_RectList;
	D3DXVECTOR4				m_ParticleColor;
	ID3D11ShaderResourceView*	m_pAnimSRV;
public:
	HRESULT		Load(ID3D11Device* pd3dDevice, TCHAR* pLoadTextureString,
		TCHAR* pLoadShaderString, ID3D11BlendState* pBlendState = nullptr);
	bool		Frame(ID3D11DeviceContext*    pContext, float fGlobalTime, float fElapsedTime);
	void		Updata(ID3D11DeviceContext*    pContext, float& pfCurrentTimer,
		int& iApplyIndex, float fGlobalTime, float fElapsedTime);
	bool		Render(ID3D11DeviceContext* pContext);
	void		SetTextureArray(T_STR_VECTOR FileList);
	D3DXVECTOR4	SetUV(RECT& Rect);
	void		SetAttribute(float fAnimTime = 1.0f, int iWidth = 1, int iWidthSize = 0,
		int iHeight = 1, int iHeightSize = 0);
public:
	TSprite();
	virtual ~TSprite();
};

