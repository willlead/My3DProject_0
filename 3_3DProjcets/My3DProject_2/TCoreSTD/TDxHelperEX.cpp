#include "TDXHelperEx.h"
#include "TInput.h"


HRESULT DX::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
ID3D11VertexShader*   DX::LoadVertexShaderFile(
	ID3D11Device*  pd3dDevice,
	TCHAR* pLoadShaderFile,
	ID3DBlob** ppVSBlob)
{
	HRESULT hr = S_OK;
	ID3D11VertexShader* pVertexShader = NULL;
	hr = CompileShaderFromFile(
		pLoadShaderFile,
		"VS",
		"vs_5_0",
		ppVSBlob);

	if (FAILED(hr))
	{
		H(hr);
		return NULL;
	}
	ID3DBlob* pVSBlob = *ppVSBlob;
	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		NULL,
		&pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		H(hr);
		return NULL;
	}
	return pVertexShader;
};
ID3D11PixelShader*   DX::LoadPixelShaderFile(
	ID3D11Device*  pd3dDevice,
	TCHAR* pLoadShaderFile, 
	char* csEntry)
{
	HRESULT hr = S_OK;

	ID3D11PixelShader* pPixelShader = NULL;
	ID3DBlob* pPSBlob = NULL;
	
	if (csEntry != 0)
	{
		hr = CompileShaderFromFile(
			pLoadShaderFile,
			csEntry,
			"ps_5_0",
			&pPSBlob);
	}
	else
	{
		hr = CompileShaderFromFile(
			pLoadShaderFile,
			"PS",
			"ps_5_0",
			&pPSBlob);
	}
	
	
	if (FAILED(hr))
	{
		H(hr);
		return NULL;
	}

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(
		pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(), NULL,
		&pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
	{
		H(hr);
		return NULL;
	}
	return pPixelShader;
}
ID3D11GeometryShader*   DX::LoadGeometryShaderFile(
	ID3D11Device*  pd3dDevice,
	TCHAR* pLoadShaderFile)
{
	HRESULT hr = S_OK;
	ID3D11GeometryShader* pShader = NULL;
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(
		pLoadShaderFile,
		"GS",
		"gs_5_0",
		&pPSBlob);
	if (FAILED(hr))
	{
		H(hr);
		return NULL;
	}

	// Create the pixel shader
	hr = pd3dDevice->CreateGeometryShader(
		pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(), NULL,
		&pShader);
	pPSBlob->Release();
	if (FAILED(hr))
	{
		H(hr);
		return NULL;
	}
	return pShader;
}
ID3D11InputLayout* DX::CreateInputlayout(
	ID3D11Device*  pd3dDevice,
	D3D11_INPUT_ELEMENT_DESC layout[],
	UINT numElements,
	ID3DBlob* pVSBlob)
{
	HRESULT hr = S_OK;
	ID3D11InputLayout* pInputLayout = NULL;
	// Create the input layout
	hr = pd3dDevice->CreateInputLayout(layout,
		numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&pInputLayout);
	if (FAILED(hr))
	{
		H(hr);
		return NULL;
	}		
	return pInputLayout;
}
ID3D11Buffer* DX::CreateBuffer(
	ID3D11Device*  pd3dDevice,
	void *data,
	UINT iNumCount,
	UINT iSize,
	D3D11_BIND_FLAG bindFlag,
	bool bDynamic)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = NULL;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	if (bDynamic)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = iSize * iNumCount;
	bd.BindFlags = bindFlag;
	InitData.pSysMem = data;

	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData,
		&pBuffer)))
	{
		H(hr);
		return NULL;
	}
		
	return pBuffer;
}

ID3D11ShaderResourceView* DX::CreateShaderResourceView(
	ID3D11Device*  pd3dDevice,
	TCHAR* szFileName)
{
	HRESULT hr;
	ID3D11ShaderResourceView* srv = nullptr;
	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(
		pd3dDevice,
		szFileName,
		NULL, NULL, &srv,
		NULL)))
	{
		H(hr);
		return nullptr;
	}
	return srv;
}
namespace DX
{
	ID3D11RasterizerState*   TDxState::g_pWireFrameRS = 0;
	ID3D11RasterizerState*   TDxState::g_pBackSolidRS = 0;
	ID3D11RasterizerState*   TDxState::g_pFrontSolidRS = 0;
	ID3D11RasterizerState*   TDxState::g_pNoneSolidRS = 0;
	ID3D11RasterizerState*   TDxState::g_pCullSolidRS[3] = { 0, };
	ID3D11DepthStencilState* TDxState::g_pDepthStencilAddDSS = 0; // ���� ���۸� �Ҽ�ȭ
	ID3D11DepthStencilState* TDxState::g_pDepthStencilDisableDSS = 0;// ��Ȱ��ȭ
	ID3D11SamplerState*		 TDxState::g_pSamplerState=0;

	HRESULT TDxState::SetState(ID3D11Device* pDevice)
	{
		HRESULT hr = S_OK;
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_BACK;
		rd.DepthClipEnable = TRUE;
		if (FAILED(hr = pDevice->CreateRasterizerState(&rd,
			&g_pWireFrameRS)))
		{
			H(hr);
			return false;
		};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		if (FAILED(hr = pDevice->CreateRasterizerState(&rd,
			&g_pBackSolidRS)))
		{
			H(hr);
			return false;
		};
		g_pCullSolidRS[2] = g_pBackSolidRS;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_FRONT;
		if (FAILED(hr = pDevice->CreateRasterizerState(&rd,
			&g_pFrontSolidRS)))
		{
			H(hr);
			return false;
		};
		g_pCullSolidRS[1] = g_pFrontSolidRS;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_NONE;
		if (FAILED(hr = pDevice->CreateRasterizerState(&rd,
			&g_pNoneSolidRS)))
		{
			H(hr);
			return false;
		};
		g_pCullSolidRS[0] = g_pNoneSolidRS;


		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		dsd.StencilEnable = TRUE;
		dsd.StencilReadMask = 0xffffffff;
		dsd.StencilWriteMask = 0xffffffff;

		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// ���ٽ� ������ ���� ���� ����� ó��
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		// ���� ������  ���� ���� ����� ó��
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		// ���ٽ� ������ ���� ���� ���
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// ���ٽ� ������ ���� ���� ����� ó��
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		// ���� ������  ���� ���� ����� ó��
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		// ���ٽ� ������ ���� ���� ���
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

		if (FAILED(hr = pDevice->CreateDepthStencilState(
			&dsd, &g_pDepthStencilAddDSS)))
		{
			return true;
		}

		dsd.DepthEnable = FALSE;
		dsd.StencilEnable = FALSE;
		if (FAILED(hr = pDevice->CreateDepthStencilState(
			&dsd, &g_pDepthStencilDisableDSS)))
		{
			return true;
		}

		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
		sd.Filter = D3D11_FILTER_ANISOTROPIC;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.BorderColor[0] = 0.0f;
		sd.BorderColor[1] = 0.0f;
		sd.BorderColor[2] = 0.0f;
		sd.BorderColor[3] = 1.0f;
		sd.MaxAnisotropy = 16;
		sd.MinLOD = 0;
		sd.MaxLOD = 0;
		if (FAILED(hr = pDevice->CreateSamplerState(&sd,
			&g_pSamplerState)))
		{
			H(hr);
			return false;
		}
		return hr;
	}
	bool TDxState::Release()
	{
		if (g_pWireFrameRS) g_pWireFrameRS->Release();
		if (g_pBackSolidRS) g_pBackSolidRS->Release();
		if (g_pFrontSolidRS) g_pFrontSolidRS->Release();
		if (g_pNoneSolidRS) g_pNoneSolidRS->Release();
		if (g_pDepthStencilAddDSS) g_pDepthStencilAddDSS->Release();
		if (g_pDepthStencilDisableDSS) g_pDepthStencilDisableDSS->Release();
		
		if (g_pSamplerState) g_pSamplerState->Release();

		return true;
	}


	TDXObject::TDXObject()
	{
		m_bWireFrameRender = false;
		m_iPrimitiveType = 4;
		m_iCullMode = 0;
	}
	TDXObject::~TDXObject()
	{
		Release();
	}
	bool TDXObject::Frame()
	{
		if (I_Input.KeyCheck(DIK_1) == KEY_UP)
		{
			m_bWireFrameRender = !m_bWireFrameRender;
		}
		if (I_Input.KeyCheck(DIK_2) == KEY_UP)
		{
			++m_iPrimitiveType;
			if (m_iPrimitiveType > 5)
			{
				m_iPrimitiveType = 1;
			}
			m_iPrimitiveType = min(m_iPrimitiveType, 5);
		}
		if (I_Input.KeyCheck(DIK_3) == KEY_UP)
		{
			++m_iCullMode;
			if (m_iCullMode > 2)
			{
				m_iCullMode = 0;
			}
			m_iCullMode = min(m_iCullMode, 2);
		}
		return true;
	}
	bool TDXObject::Render(
		ID3D11DeviceContext*    pContext,
		UINT VertexSize, UINT Count)
	{

		ApplyRS(pContext, TDxState::g_pCullSolidRS[m_iCullMode]);
		if( m_bWireFrameRender)
			ApplyRS(pContext, TDxState::g_pWireFrameRS);


		pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType);
		pContext->IASetInputLayout(g_pInputlayout.Get());
		UINT stride = VertexSize;
		UINT offset = 0;
		pContext->IASetVertexBuffers(0, 1, g_pVertexBuffer.GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer(g_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->VSSetConstantBuffers(0, 1, g_pConstantBuffer.GetAddressOf());
		pContext->VSSetShader(g_pVertexShader.Get(), NULL, 0);
		pContext->GSSetShader(g_pGeometryShader.Get(), NULL, 0);
		pContext->PSSetShader(g_pPixelShader.Get(), NULL, 0);
		pContext->PSSetShaderResources(0, 1, g_pTexSRV.GetAddressOf());
		pContext->DrawIndexed(Count, 0, 0);
		return true;
	}
	void  TDXObject::Release()
	{
		if (g_pVertexBuffer != nullptr)
		{
			//g_pVertexBuffer.Get()->Release();
			g_pVertexBuffer.ReleaseAndGetAddressOf();

		}
		/*if (g_pIndexBuffer) g_pIndexBuffer->Release();
		if (g_pConstantBuffer) g_pConstantBuffer->Release();
		if (g_pVertexShader) g_pVertexShader->Release();
		if (g_pPixelShader) g_pPixelShader->Release();
		if (g_pInputlayout) g_pInputlayout->Release();
		if (g_pVSBlob) g_pVSBlob->Release();
		if (g_pTexSRV)g_pTexSRV->Release();
		g_pVertexBuffer = NULL;
		g_pIndexBuffer = NULL;
		g_pConstantBuffer = NULL;
		g_pVertexShader = NULL;
		g_pPixelShader = NULL;
		g_pInputlayout = NULL;
		g_pVSBlob = NULL;*/
	}



	HRESULT TDxRT::CreateDSV(
		ID3D11Device*  pd3dDevice,
		UINT iWidth, UINT iHeight)
	{
		HRESULT hr;
		ID3D11Texture2D* pTex = NULL;
		D3D11_TEXTURE2D_DESC  td;
		td.Width = iWidth;
		td.Height = iHeight;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		if (FAILED(hr = pd3dDevice->CreateTexture2D(&td,
			NULL, &pTex)))
		{
			H(hr);
			return hr;
		}


		D3D11_DEPTH_STENCIL_VIEW_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		if (FAILED(hr = pd3dDevice->CreateDepthStencilView(
			pTex, &dsd, &m_pDSV)))
		{
			return hr;
		}

		pTex->Release();
		return hr;
	}
}
