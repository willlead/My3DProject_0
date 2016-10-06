#include "Sample.h"


ID3D11Buffer*           g_pConstantBuffer = NULL;

ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader*                 g_pVertexShader = NULL;
ID3D11PixelShader*                  g_pPixelShader = NULL;
ID3D11InputLayout*                  g_pVertexLayout = NULL;
ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;

ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11SamplerState*                 g_pSamplerLinear = NULL;
ID3D11RenderTargetView*             g_pRenderTargetView = NULL;

D3DXMATRIX                          g_World;
D3DXMATRIX                          g_View;
D3DXMATRIX                          g_Projection;
D3DXVECTOR4                         g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR4 Color;
	D3DXVECTOR4 vMeshColor;
};


struct ConstantBuffer
{
	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProjection;
	D3DXVECTOR4 vMeshColor;
};




bool Sample::Init()
{
	HRESULT hr = S_OK;
	m_pMainCamera = make_shared<TCamera>();
	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, -10.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 10000.0f);
	m_pMainCamera->SetWindow(m_iWindowWidth, m_iWindowHeight);

#ifdef _BOX
	m_ParserASE.Open(L"../../data/obj/box_test.ase");
#else
	m_ParserASE.Open(L"../../data/obj/st02sc00.ase");
#endif
	m_Object = m_ParserASE.m_ASE;



#pragma region Tex
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

#pragma endregion 

	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile(L"Tutorial04.fx", "VS", "vs_5_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(L"Tutorial04.fx", "PS", "ps_5_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	PNCT_VERTEX* vertices;
	vertices = (PNCT_VERTEX *)malloc(sizeof(PNCT_VERTEX) * m_ParserASE.m_ASE.m_VertexList.size());
	//m_Object.ObjectList[0].m_VertexList.size());

	for (int i = 0; i < m_ParserASE.m_ASE.m_VertexList.size(); i++)
	{
		vertices[i] = {
			//m_Object.ObjectList[0].VertexList[i], 
			m_Object.m_VertexList[i].p,
			m_Object.m_VertexList[i].n,//D3DXVECTOR3(1.0f, 0.0f, 1.0f),
			m_Object.m_VertexList[i].c,//D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f),
			m_Object.m_VertexList[i].t,
			//m_Object.Ob
		};
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_Object.ObjectList[0].FaceList.size() * 3; // 개수 변경
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Create index buffer
	int* indices;
	indices = (int *)malloc(sizeof(int) * m_Object.ObjectList[0].FaceList.size() * 3);
	//int iNum = sizeof(indices) / sizeof(indices[0]);

	for (int i = 0; i < m_Object.ObjectList[0].FaceList.size(); i++) {
		indices[3 * i] = m_Object.ObjectList[0].FaceList[i].i[0];
		indices[3 * i + 1] = m_Object.ObjectList[0].FaceList[i].i[1];
		indices[3 * i + 2] = m_Object.ObjectList[0].FaceList[i].i[2];
	}
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * m_Object.ObjectList[0].FaceList.size() * 3;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;

#pragma region Tex

	//// Load the Texture
	hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"flagstone.bmp", NULL, NULL, &g_pTextureRV, NULL);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear); //
	if (FAILED(hr))
		return hr;
#pragma endregion 


	// Initialize the world matrix
	D3DXMatrixIdentity(&g_World);


#ifdef _TEX

	// Initialize the view matrix
	D3DXVECTOR3 Eye = D3DXVECTOR3(0.0f, 1.0f, -20.0f);
	D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up);

	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(&g_Projection, 1.570796327f, m_iWindowWidth / (FLOAT)m_iWindowHeight, 0.01f, 1000.0f);

	g_View = *m_pMainCamera->GetViewMatrix();
	g_Projection = *m_pMainCamera->GetProjMatrix();
#else
	// Initialize the view matrix
	D3DXVECTOR3 Eye = D3DXVECTOR3(0.0f, 3.0f, -6.0f);// , 0.0f);
	D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 1.0f, 0.0f);// , 0.0f);	
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//, 0.0f);												   	
	D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up);//g_View = D3DXMatrixLookAtLH(Eye, At, Up);

	ConstantBuffer cb;
	D3DXMatrixTranspose(&g_View, &cb.mView);//XMMatrixTranspose(g_View);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	// Initialize the projection matrix
	//g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);
	D3DXMatrixPerspectiveFovLH(&g_Projection, 0.785398163f, width / (FLOAT)height, 0.01f, 100.0f);

	ConstantBuffer cb1;
	//cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
	D3DXMatrixTranspose(&cb1.mProjection, &g_Projection);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

#endif

	return true;

}

bool Sample::Frame()
{
	m_pMainCamera->Frame();
	g_View = *m_pMainCamera->GetViewMatrix();
	g_Projection = *m_pMainCamera->GetProjMatrix();
	return true;
}
bool Sample::Render()
{
	// Update our time
	static float t = 0.0f;
	if (m_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)3.141592654f * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	//
	// Animate the cube
	//
	//D3DXMatrixRotationY(&g_World, t);




#pragma region Tex

	// Modify the color
	g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

#pragma endregion 

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(GetRenderTargetView(), ClearColor);


	//
	// Update variables
	//
	ConstantBuffer cb;
	D3DXMatrixTranspose(&cb.mWorld, &g_World);
	//D3DXMatrixIdentity(&cb.mWorld);
	D3DXMatrixTranspose(&cb.mView, &g_View);
	D3DXMatrixTranspose(&cb.mProjection, &g_Projection);
	cb.vMeshColor = g_vMeshColor;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	//
	// Renders a triangle
	//
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

	//g_pImmediateContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list
	g_pImmediateContext->Draw(m_Object.m_VertexList.size(), 0);
	//
	// Present our back buffer to our front buffer
	//
	m_pSwapChain->Present(0, 0);

	return true;
}
bool Sample::Relase()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();

	if (g_pSamplerLinear) g_pSamplerLinear->Release();
	if (g_pTextureRV) g_pTextureRV->Release();
	if (g_pConstantBuffer) g_pConstantBuffer->Release();

	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pDepthStencil) g_pDepthStencil->Release();
	if (g_pDepthStencilView) g_pDepthStencilView->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();

	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
	return true;
}

Sample::Sample()
{
}


Sample::~Sample()
{
}

TBASIS_RUN(L"AseBox_Render_1")