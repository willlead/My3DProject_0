#include "Sample_2.h"


D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;
ID3D11Buffer*           g_pIndexBuffer = NULL;
ID3D11Buffer*           g_pConstantBuffer = NULL;
D3DXVECTOR3            g_Eye;

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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

void CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	if (g_pConstantBuffer) g_pConstantBuffer->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;
#pragma region Device
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes;
	driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL,
			g_driverType, NULL, createDeviceFlags,
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd,
			&g_pSwapChain,
			&g_pd3dDevice, &g_featureLevel,
			&g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);
#pragma endregion 

	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile(L"hlsl.fx", "VShader", "vs_4_0", &pVSBlob);

	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		NULL,
		&g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLORa",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		//{ "COLORb",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	// NUM = sizeof(layout) / sizeof(layout[0])
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&g_pVertexLayout);

	pVSBlob->Release();
	if (FAILED(hr))
		return hr;


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(L"hlsl.fx", "PS", "ps_4_0", &pPSBlob);
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
	//  투영좌표계
	//      0,0                 800,0
	//   -1.0f, 1.0f,       1.0f,1.0f
	//
	//           0.0f, 0.0f
	//
	//   0, 600                 800, 600
	//  -1.0f, -1.0f           1.0f, -1.0f
	// 로칼 좌표계
	SimpleVertex vertices[] =
	{
		/*정면*/
		//Front 4 Vertex
		D3DXVECTOR3(-0.5f,  0.5f, 0.25f),D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(-0.5f, -0.5f, 0.25f),D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(0.5f,  0.5f, 0.25f),D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(0.5f, -0.5f, 0.25f),D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),

		//Behind 4 Vertex
		D3DXVECTOR3(-0.5f,  0.5f, 0.75f),D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXVECTOR3(-0.5f, -0.5f, 0.75f),D3DXVECTOR4(1.0f,1.0f, 1.0f, 1.0f),
		D3DXVECTOR3(0.5f,  0.5f, 0.75f),D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXVECTOR3(0.5f, -0.5f, 0.75f),D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),

		//앞면
		//// 0,상,좌
		//D3DXVECTOR3(-10.5f, 10.5f, 0.0f),	D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		//// 1,상,우
		//D3DXVECTOR3(10.5f, 10.5f, .0f),	D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		//// 2,하,우
		//D3DXVECTOR3(10.5f, -10.5f, 5.0f),	D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		//// 3,하,좌
		//D3DXVECTOR3(-10.5f, -10.5f, 5.0f),D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		//
		////뒷면
		//// 4,
		//D3DXVECTOR3(-10.5f, 10.5f, 15.0f),	D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		//// 5,
		//D3DXVECTOR3(10.5f, 10.5f, 15.0f),	D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		//// 6,
		//D3DXVECTOR3(10.5f, -10.5f, 15.0f),	D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
		//// 7,
		//D3DXVECTOR3(-10.5f, -10.5f, 15.0f),D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f),
		//D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),

		//// tri 0
		//XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		//XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		//XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		//XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		//XMFLOAT3(0.5f, -0.5f, 0.5f),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		//XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),		
		//// tri 1
		//XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		//XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		//XMFLOAT3(0.5f, -0.5f, 0.5f),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		//XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		//XMFLOAT3(-0.5f, -0.5f, 0.5f),XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		//XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	};
	int iNumVertex = sizeof(vertices) / sizeof(vertices[0]);
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * iNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData,
		&g_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	WORD indices[] =
	{
		//Front
		0, 2, 1,
		2, 3, 1,

		//Top
		4, 6, 0,
		6, 2, 0,

		//Behind
		5, 7, 4,
		7, 6, 4,

		//Bottom
		1, 3, 5,
		3, 7, 5,

		//Right
		2, 6, 3,
		6, 7, 3,

		//Left
		4, 0, 5,
		0, 1, 5,

		/*0,1,2,
		0,2,3,

		5,4,7,
		5,7,6,

		1,5,6,
		1,6,2,

		4,0,3,
		4,3,7,

		4,5,1,
		4,1,0,

		3,2,6,
		3,6,7,*/

	};
	int iNumIndex = sizeof(indices) / sizeof(indices[0]);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * iNumIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	InitData.pSysMem = indices;

	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData,
		&g_pIndexBuffer);
	if (FAILED(hr))
		return hr;



	return S_OK;
}
//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------

bool Sample::Init()
{
	// 상수버퍼를 초기화	
	// 단위행렬로 월드 행렬을 만든다.
	D3DXMatrixIdentity(&cb.mWorld); //
	D3DXMatrixIdentity(&cb.mView);
	D3DXMatrixIdentity(&cb.mProj);
	//D3DXMatrixRotationY(&cb.mWorld, D3DXToRadian(180.0f));// m_Timer.m_fAccumulation);
	g_Eye.x = 0.0f;
	g_Eye.y = 0.0f;
	g_Eye.z = -5.0f;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}
	// 상수버퍼를 생성 // Create constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &cb;

	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &InitData,
		&g_pConstantBuffer);
	if (FAILED(hr))
		return false;
	return true;
}

bool Sample::Frame()
{
	//Clear constant buffer
	//단위행렬로 월드 행렬을 만듦.
	D3DXMatrixIdentity(&cb_game.mWorld);

	if (m_Input.KeyCheck('W') == KEY_HOLD)
		g_Eye.z += 10.0f * m_Timer.m_fSecondPerFrame;
	if (m_Input.KeyCheck('S') == KEY_HOLD)
		g_Eye.z -= 10.0f * m_Timer.m_fSecondPerFrame;

	//D3DXVECTOR3 Eye = D3DXVECTOR3(0.0f, 0.0f, -5.0f); // 카메라의 위치
	D3DXVECTOR3 Eye = g_Eye;
	D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 카메라가 보는 
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cb.mView, &Eye, &At, &Up);// 뷰 행렬 계산





												  // 투영 행렬 계산
												  // (D3DXMATRIX *pOut,  계산된 투영 행렬 반환
	FLOAT fovy = D3DX_PI * 0.5f;  // 시야각
	FLOAT Aspect = 800.0f / 600.0f;// 화면 비율( 1.33f )
	FLOAT zn = 0.1f; // 근단면 거리
	FLOAT zf = 1000.0f;   // 원단면 거리

	D3DXMatrixPerspectiveFovLH(&cb.mProj, fovy, Aspect, zn, zf); // ==>> 투영 좌표


																 // 전치 행렬 변환
																 // Update variables
	D3DXMatrixTranspose(&cb.mWorld, &cb_game.mWorld);
	D3DXMatrixTranspose(&cb.mView, &cb_game.mView);
	D3DXMatrixTranspose(&cb.mProj, &cb_game.mProj);

	// Update constant buffer in every frame
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	return true;
}


bool Sample::Render()
{
	// Just clear the backbuffer
	float ClearColor[4] = { 1.0f, 1.0f,1.0f, 1.0f }; //red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(
		g_pRenderTargetView,
		ClearColor);

	//// Update our time
	//static float t = 0.0f;
	//if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	//{
	//	t += (float)D3DX_PI * 0.0125f;
	//}
	//else
	//{
	//	static DWORD dwTimeStart = 0;
	//	DWORD dwTimeCur = GetTickCount();
	//	if (dwTimeStart == 0)
	//		dwTimeStart = dwTimeCur;
	//	t = (dwTimeCur - dwTimeStart) / 1000.0f;
	//}
	//
	// Animate the cube
	//
	//cb.mWorld = D3DXMatrixRotationY(t);
	//D3DXMatrixRotationY(&cb.mWorld, D3DXToRadian(t));// m_Timer.m_fAccumulation);


	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 인덱스 버퍼
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer,
		DXGI_FORMAT_R16_UINT, 0);

	// 상수 버퍼
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// Set primitive topology g_pImmediateContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	// 정점버퍼만 있을 경우
	//g_pImmediateContext->Draw(6, 0);	
	// 인덱스 버퍼가 사용될 경우
	g_pImmediateContext->DrawIndexed(36, 0, 0);

	g_pSwapChain->Present(0, 0);
	return true;
}
bool Sample::Release()
{
	CleanupDevice();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}

TCORE_RUN(_T("Sample Win"), 800, 600)