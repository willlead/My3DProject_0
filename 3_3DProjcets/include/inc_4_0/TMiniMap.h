#pragma once
#include "TDxHelper.h"
#include "TCamera.h"
using namespace DX;

class TMiniMap : public TCamera
{
public:
	shared_ptr<TShape>		m_pPlaneShape;
	shared_ptr<TDxRT>		m_pMiniMapRT;
	D3D11_VIEWPORT			m_vp;
public:
	void		Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height,
		FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
	bool								Create(ID3D11Device* pd3dDevice,
		TCHAR* pLoadShaderFile = 0,
		TCHAR* pLoadTexture = 0);
	bool								Frame();
	bool								Render(ID3D11DeviceContext*    pImmediateContext);
	bool								Release();
	bool								BeginRender(ID3D11DeviceContext*    pImmediateContext);
	bool								EndRender(ID3D11DeviceContext*    pImmediateContext);
	HRESULT								CreateResource(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pImmediateContext, UINT iWidth, UINT iHeight);
	HRESULT								DeleteResource();
public:
	TMiniMap(void);
	~TMiniMap(void);
};
