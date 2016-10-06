#include "Sample.h"
struct SimpleVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR4 Color0;
	//D3DXVECTOR4 Color1;
};
struct ConstantBuffer
{
	D3DXMATRIX mWorld; // 월드 행렬
	D3DXMATRIX mView;  // 뷰(카메라)  행렬
	D3DXMATRIX mProj;  // 투영 행렬
};
ConstantBuffer cb, cb_game;
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
bool Sample::Init()
{


	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&m_pBackBufferTex);
	g_pSwapChain->GetDesc(&m_sd);
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.Width = m_sd.BufferDesc.Width;
	loadInfo.Height = m_sd.BufferDesc.Height;
	loadInfo.Depth = D3DX11_DEFAULT;
	loadInfo.FirstMipLevel = 0;
	loadInfo.MipLevels = 1;
	loadInfo.Usage = D3D11_USAGE_STAGING;	
	loadInfo.CpuAccessFlags= D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	loadInfo.BindFlags = 0;
	loadInfo.MiscFlags = 0;
	loadInfo.Format = m_sd.BufferDesc.Format;
	loadInfo.Filter = D3DX11_FILTER_LINEAR;
	loadInfo.MipFilter - D3DX11_FILTER_NONE;
	m_Texture1 = GetTexture2DFromFile(
		L"02.jpg",
		&loadInfo);
	m_Texture1->GetDesc(&m_td1);
	WriteDotPixel(m_Texture1);

	HRESULT hr;
	
	//// font 
	//IDXGISurface1*  pSurface;
	//hr = g_pSwapChain->GetBuffer(0,
	//	__uuidof(IDXGISurface1),
	//	(LPVOID*)&pSurface);
	//m_Font.Set(pSurface);
	//pSurface->Release();

	//// 상수버퍼를 초기화   
	//// 단위행렬로 월드 행렬을 만든다.
	//D3DXMatrixIdentity(&cb.mWorld); //
	//D3DXMatrixIdentity(&cb.mView);
	//D3DXMatrixIdentity(&cb.mProj);

	////D3DXMatrixRotationY(&cb.mWorld, D3DXToRadian(180.0f));// m_Timer.m_fAccumulation);

	//g_Eye.x = 0.0f;
	//g_Eye.y = 0.0f;
	//g_Eye.z = -5.0f;

	//// Compile the vertex shader
	//ID3DBlob* pVSBlob = NULL;
	////hr = CompileShaderFromFile(L"hlsl.fx", "VShader", "vs_4_0", &pVSBlob);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL,
	//		L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
	//	return hr;
	//}

	//// Create the vertex shader
	//hr = g_pd3dDevice->CreateVertexShader(
	//	pVSBlob->GetBufferPointer(),
	//	pVSBlob->GetBufferSize(),
	//	NULL,
	//	&g_pVertexShader);
	//if (FAILED(hr))
	//{
	//	pVSBlob->Release();
	//	return hr;
	//}

	//// Define the input layout
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	//	{ "COLORa",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	//	//{ "COLORb",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0 },
	//};
	//// NUM = sizeof(layout) / sizeof(layout[0])
	//UINT numElements = ARRAYSIZE(layout);

	//// Create the input layout
	//hr = g_pd3dDevice->CreateInputLayout(
	//	layout,
	//	numElements,
	//	pVSBlob->GetBufferPointer(),
	//	pVSBlob->GetBufferSize(),
	//	&g_pVertexLayout);

	//pVSBlob->Release();
	//if (FAILED(hr))
	//	return hr;


	//// Compile the pixel shader
	//ID3DBlob* pPSBlob = NULL;
	////hr = CompileShaderFromFile(L"hlsl.fx", "PS", "ps_4_0", &pPSBlob);
	//if (FAILED(hr))
	//{
	//	MessageBox(NULL,
	//		L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
	//	return hr;
	//}

	//// Create the pixel shader
	//hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	//pPSBlob->Release();
	//if (FAILED(hr))
	//	return hr;

	//SimpleVertex vertices[] =
	//{
	//	/*정면*/
	//	//Front 4 Vertex
	//	D3DXVECTOR3(-0.5f,  0.5f, 0.25f),D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
	//	D3DXVECTOR3(-0.5f, -0.5f, 0.25f),D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),
	//	D3DXVECTOR3(0.5f,  0.5f, 0.25f),D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
	//	D3DXVECTOR3(0.5f, -0.5f, 0.25f),D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),

	//	//Behind 4 Vertex
	//	D3DXVECTOR3(-0.5f,  0.5f, 0.75f),D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
	//	D3DXVECTOR3(-0.5f, -0.5f, 0.75f),D3DXVECTOR4(1.0f,1.0f, 1.0f, 1.0f),
	//	D3DXVECTOR3(0.5f,  0.5f, 0.75f),D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
	//	D3DXVECTOR3(0.5f, -0.5f, 0.75f),D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),


	//};
	//int iNumVertex = sizeof(vertices) / sizeof(vertices[0]);
	//D3D11_BUFFER_DESC bd;
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(SimpleVertex) * iNumVertex;
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	//ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = vertices;
	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData,
	//	&g_pVertexBuffer);
	//if (FAILED(hr))
	//	return hr;

	//WORD indices[] =
	//{
	//	//Front
	//	0, 2, 1,
	//	2, 3, 1,

	//	//Top
	//	4, 6, 0,
	//	6, 2, 0,

	//	//Behind
	//	5, 7, 4,
	//	7, 6, 4,

	//	//Bottom
	//	1, 3, 5,
	//	3, 7, 5,

	//	//Right
	//	2, 6, 3,
	//	6, 7, 3,

	//	//Left
	//	4, 0, 5,
	//	0, 1, 5,
	//};
	//int iNumIndex = sizeof(indices) / sizeof(indices[0]);
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(DWORD) * iNumIndex;
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//InitData.pSysMem = indices;

	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData,
	//	&g_pIndexBuffer);
	//if (FAILED(hr))
	//	return hr;

	//// Create constant buffer
	////D3D11_BUFFER_DESC bd;
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(ConstantBuffer);
	//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = 0;

	////	D3D11_SUBRESOURCE_DATA InitData;
	//ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = &cb;

	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData,
	//	&g_pConstantBuffer);
	//if (FAILED(hr))
	//	return false;
	return true;
}
bool Sample::Frame()
{
	D3DXMatrixIdentity(&cb_game.mWorld);

	if (m_Input.KeyCheck('W') == KEY_HOLD)
		g_Eye.z += 10.0f * m_Timer.m_fSecondPerFrame;
	if (m_Input.KeyCheck('S') == KEY_HOLD)
		g_Eye.z -= 10.0f * m_Timer.m_fSecondPerFrame;
	
	if (m_Input.KeyCheck('0') == KEY_PUSH)
	{
		BOOL bFullScreen;
		g_pSwapChain->GetFullscreenState(&bFullScreen, NULL);
		g_pSwapChain->SetFullscreenState(!bFullScreen, NULL);
	}
	if (m_Input.KeyCheck('1') == KEY_PUSH)
	{
		DXGI_MODE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = 1024;
		desc.Height = 768;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.RefreshRate.Denominator = 0;
		desc.RefreshRate.Numerator = 0;
		desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		g_pSwapChain->ResizeTarget(&desc);
	}
	if (m_Input.KeyCheck('2') == KEY_PUSH)
	{
		DXGI_MODE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = 800;
		desc.Height = 600;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.RefreshRate.Denominator = 0;
		desc.RefreshRate.Numerator = 0;
		desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		g_pSwapChain->ResizeTarget(&desc);
		g_pSwapChain->SetFullscreenState(TRUE, NULL);
	}

	if (m_Input.KeyCheck(VK_ESCAPE))
	{
		SendMessage(g_hWnd, WM_CLOSE, 0, 0);
	}

	//D3DXVECTOR3 Eye = D3DXVECTOR3(0.0f, 0.0f, -5.0f);	 // 카메라의 위치
	D3DXVECTOR3 Eye = g_Eye;
	D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		 // 카메라가 보는 
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cb_game.mView, &Eye, &At, &Up);	 // 뷰 행렬 계산
	
														 // 투영 행렬 계산
														 // (D3DXMATRIX *pOut,  계산된 투영 행렬 반환
	FLOAT fovy = D3DX_PI * 0.5f;					     // 시야각
	FLOAT Aspect = 800.0f / 600.0f;						 // 화면 비율( 1.33f )
	FLOAT zn = 0.1f;									 // 근단면 거리
	FLOAT zf = 1000.0f;									 // 원단면 거리

	D3DXMatrixPerspectiveFovLH(&cb_game.mProj, fovy, Aspect, zn, zf); // ==>> 투영 좌표


	D3DXMatrixTranspose(&cb.mWorld, &cb_game.mWorld);
	D3DXMatrixTranspose(&cb.mView, &cb_game.mView);
	D3DXMatrixTranspose(&cb.mProj, &cb_game.mProj);


	
	// Update constant buffer in every frame
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer,
		0, NULL, &cb, 0, 0);

	return true;
}
bool Sample::Render()
{
	m_Input.Frame();
	// Just clear the backbuffer
	float ClearColor[4] = { 1.0f, 1.0f,1.0f, 1.0f }; //red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(
		g_pRenderTargetView,
		ClearColor);

	//// Update our time
	//static float t = 0.0f;
	//if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	//{
	//   t += (float)D3DX_PI * 0.0125f;
	//}
	//else
	//{
	//   static DWORD dwTimeStart = 0;
	//   DWORD dwTimeCur = GetTickCount();
	//   if (dwTimeStart == 0)
	//      dwTimeStart = dwTimeCur;
	//   t = (dwTimeCur - dwTimeStart) / 1000.0f;
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

	//TCHAR pBuffer[256];
	//memset(pBuffer, 0, sizeof(TCHAR) * 256);
	//_stprintf_s(pBuffer, _T("FPS:%d SecPerFrame:%10.5f Elapsed : %10.2f"), m_Timer.m_iFPS,
	//	m_Timer.m_fSecondPerFrame,
	//	m_Timer.m_fAccumulation);

	//m_Font.Begin();
	//m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	//m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	//RECT rc1 = { 0,0, m_dwWidth, m_dwHeight };
	//m_Font.DrawText(rc1, pBuffer, D2D1::ColorF(1, 1, 1, 0.5));
	//m_Font.end();

	return true;
}
bool Sample::Release()
{
	m_Font.Release();
	if (g_pConstantBuffer) g_pConstantBuffer->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	CleanupDevice();

	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
#ifndef _DEBUG
#define TCORE_RUN(s,x,y) TCORE_START;TCORE_WIN(s,x,y);
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int       nCmdShow)
{
	Sample sample;
	if (sample.SetWindow(hInstance, _T("Sample Win"), 800, 600))
	{
		sample.Run();
	}
}
#endif