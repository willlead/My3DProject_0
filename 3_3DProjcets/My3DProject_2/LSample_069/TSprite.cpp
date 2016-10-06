#include "TSprite.h"

HRESULT TSprite::Load(ID3D11Device* pd3dDevice, TCHAR* pLoadTextureString,
	TCHAR* pLoadShaderString, ID3D11BlendState* pBlendState)
{
	if (!Create(pd3dDevice, pLoadTextureString, pLoadShaderString))
	{
		MessageBox(0, _T("Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pBlendState = pBlendState;
	return S_OK;
}
bool TSprite::Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime)
{
	Updata(pContext, m_fTime, m_iApplyIndex, fGlobalTime, fElapsedTime);
	return true;
}
void TSprite::Updata(ID3D11DeviceContext* pContext, float& pfCurrentTimer,
	int& iApplyIndex, float fGlobalTime, float fElapsedTime)
{
	pfCurrentTimer += fElapsedTime;
	if (pfCurrentTimer >= m_fSecPerRender)
	{
		if (++iApplyIndex >= m_iNumTexture)
			iApplyIndex = 0;
		pfCurrentTimer = 0.0f;

		//Texture Animation
		if (m_TextureIndex.size())
		{
			m_pAnimSRV = I_Texture.GetPtr(m_TextureIndex[iApplyIndex])->m_pTextureRV;
		}
		else
		{
			if (m_Object.g_pTextureSRV != nullptr && m_RectList.size())
			{
				D3DXVECTOR4 vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				D3DXVECTOR4 vUV = m_RectList[iApplyIndex].vUV;

				PCT_VERTEX vertices[] =
				{
					{ D3DXVECTOR3(-1.0f, 1.0f, 0.0f),  vColor, D3DXVECTOR2(vUV.x, vUV.y)},
					{ D3DXVECTOR3(1.0f, 1.0f,  0.0f),  vColor, D3DXVECTOR2(vUV.z, vUV.w) },
					{ D3DXVECTOR3(1.0f, -1.0f, 0.0f),  vColor, D3DXVECTOR2(vUV.x, vUV.y) },
					{ D3DXVECTOR3(-1.0f, -1.0f, 0.0f),  vColor, D3DXVECTOR2(vUV.z, vUV.w) },
				};
			}
		}
	}
}
bool TSprite::Render(ID3D11DeviceContext* pContext)
{
	if (m_pBlendState != nullptr) DX::ApplyBS(pContext, m_pBlendState);
	PreRender(pContext);
	if (m_pAnimSRV != nullptr)
	{
		pContext->PSSetShaderResources(0, 1, &m_pAnimSRV);
	}
	PostRender(pContext);
	return true;
}
void TSprite::SetTextureArray(T_STR_VECTOR FileList)
{
	HRESULT hr = S_OK;
	for (int iList = 0; iList < FileList.size(); iList++)
	{
		INT iIndex = I_Texture.Add(m_pd3dDevice, FileList[iList].c_str());
		if (iIndex <= 0)
		{
			MessageBox(0, _T("m_Texture.Load 실패"), _T("Fatal error"), MB_OK);
			return;
		}
		m_TextureIndex.push_back(iIndex);
	}
	m_iNumTexture = m_TextureIndex.size();
}
D3DXVECTOR4 TSprite::SetUV(RECT& Rect)
{
	D3DXVECTOR4 vUV;
	float fOffSetX = 0.0f;
	if (Rect.left > 0)
	{
		fOffSetX = (float)Rect.right / (float)(RectSet.left * RectSet.right);
	}
	float fOffSetY = 0.0f;
	if (Rect.top > 0)
	{
		fOffSetY = (float)Rect.top / (float)(RectSet.top * RectSet.right);
	}

	vUV.x = fOffSetX;
	vUV.y = fOffSetY;

	fOffSetX = 1.0f;
	if (Rect.right > 0)
	{
		fOffSetX = (float)Rect.right / (float)(RectSet.left * RectSet.right);
	}
	fOffSetY = 1.0f;
	if (Rect.bottom > 0)
	{
		fOffSetY = (float)Rect.bottom / (float)(RectSet.top * RectSet.bottom);
	}

	vUV.z = fOffSetX;
	vUV.w = fOffSetY;
	return vUV;
}
void TSprite::SetAttribute(float fAnimTime, int iWidth, int iWidthSize,
	int iHeight, int iHeightSize)
{
	RectSet.left = iWidth;
	RectSet.right = iWidthSize;
	RectSet.top = iHeight; 
	RectSet.bottom = iHeightSize;

	if (m_iNumTexture > 0)
	{
		m_fSecPerRender = fAnimTime / m_iNumTexture;
		return;
	}
	else
	{
		m_fSecPerRender = fAnimTime / (iWidth* iHeight);
		m_iNumTexture = iWidth* iHeight;
	}

	for (int iHeight = 0; iHeight < RectSet.left; iHeight++)
	{
		for (int iWidth = 0; iWidth < RectSet.top; iWidth++)
		{
			TRectUV tRect;
			tRect.Rect.left = iWidth			* RectSet.right;
			tRect.Rect.right = (iWidth + 1)		* RectSet.right;
			tRect.Rect.top = iHeight			* RectSet.bottom;
			tRect.Rect.bottom = (iHeight + 1)	* RectSet.bottom;

			tRect.vUV = SetUV(tRect.Rect);
			m_RectList.push_back(tRect);
		}
	}

}


TSprite::TSprite()
{
	m_BlendType = 0;
	m_fTime = 0.0f;
	m_iApplyIndex = 0;
	m_fSecPerRender = 0.2f;
	m_iNumTexture = 0;
	RectSet.left = 4;
	RectSet.right = 64; // 텍스쳐 가로 셋 갯수 및 크기	
	RectSet.top = 4;
	RectSet.bottom = 64; // 텍스쳐 세로 셋 갯수 및 크기
	m_pAnimSRV = nullptr;
	m_pBlendState = nullptr;
	m_ParticleColor = D3DXVECTOR4(1, 1, 1, 1);
}


TSprite::~TSprite()
{
}
