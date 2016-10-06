#include "Sample.h"

//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
//ID3D11Device*           g_pd3dDevice = NULL;      //�տ� I�� �ٴ°�� �������̽���
//ID3D11DeviceContext*    g_pImmediateContext = NULL;   //�̰͵��� ����ؾ߸� ���̷�ƮX�� �̿��� �� �ִ�.
//IDXGISwapChain*         g_pSwapChain = NULL;
//ID3D11RenderTargetView* g_pRenderTargetView = NULL;
//ID3D11VertexShader*     g_pVertexShader = NULL;
//ID3D11PixelShader*      g_pPixelShader = NULL;
//ID3D11InputLayout*      g_pVertexLayout = NULL;
//ID3D11Buffer*           g_pVertexBuffer = NULL;
//ID3D11Buffer*           g_pIndexBuffer = NULL;
//ID3D11Buffer*           g_pConstantBuffer = NULL;
D3DXVECTOR3            g_Eye;


struct SimpleVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR4 Colora;
	//D3DXVECTOR4 Color1;
};

struct ConstantBuffer
{
	D3DXMATRIX mWorld; // ���� ���
	D3DXMATRIX mView;  // ��(ī�޶�)  ���
	D3DXMATRIX mProj;  // ���� ���
};
ConstantBuffer cb, cb_game;

HRESULT Sample::CompileShaderFromFile(
	WCHAR* szFileName, 
	LPCSTR szEntryPoint, 
	LPCSTR szShaderModel, 
	ID3DBlob** ppBlobOut)
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

void Sample::CleanupDevice()
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();

	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}
HRESULT Sample::InitDevice()
{

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	HRESULT hr = S_OK;
	if (FAILED(hr = CreateDevice()))
	{
		MessageBox(0, _T("CreateDevice  ����"), _T("Fatal error"), MB_OK);
		return hr;
	}
	if (FAILED(hr = CreateSwapChain(m_hWnd, width, height)))
	{
		MessageBox(0, _T("CreateSwapChain  ����"), _T("Fatal error"), MB_OK);
		return hr;
	}
	if (FAILED(hr = SetRenderTargetView()))
	{
		MessageBox(0, _T("SetRenderTargetView  ����"), _T("Fatal error"), MB_OK);
		return hr;
	}
	if (FAILED(hr = SetViewPort()))
	{
		MessageBox(0, _T("SetViewPort  ����"), _T("Fatal error"), MB_OK);
		return hr;
	}

	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile(L"hlsl.fx", "VS", "vs_4_0", &pVSBlob);

	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = m_pd3dDevice->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		NULL,
		&m_pVertexShader);
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
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&m_pVertexLayout);

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
	hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	//  ������ǥ��
	//      0,0                 800,0
	//   -1.0f, 1.0f,       1.0f,1.0f
	//
	//           0.0f, 0.0f
	//
	//   0, 600                 800, 600
	//  -1.0f, -1.0f           1.0f, -1.0f
	// ��Į ��ǥ��
	SimpleVertex vertices[] =
	{
		/*����*/
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
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData,
		&m_pVertexBuffer);
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

	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData,
		&m_pIndexBuffer);
	if (FAILED(hr))
		return hr;



	return S_OK;
}
bool Sample::Init()
{

	// ������۸� �ʱ�ȭ   
	// ������ķ� ���� ����� �����.
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
	// ������۸� ���� // Create constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &cb;

	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &InitData,
		&m_pConstantBuffer);
	if (FAILED(hr))
		return false;
	return true;
}
bool Sample::Frame()
{
	//Clear constant buffer
	//������ķ� ���� ����� ����.
	D3DXMatrixIdentity(&cb_game.mWorld);

	if (m_Input.KeyCheck('W') == KEY_HOLD)
		g_Eye.z += 10.0f * m_Timer.m_fSecondPerFrame;
	if (m_Input.KeyCheck('S') == KEY_HOLD)
		g_Eye.z -= 10.0f * m_Timer.m_fSecondPerFrame;

	//D3DXVECTOR3 Eye = D3DXVECTOR3(0.0f, 0.0f, -5.0f); // ī�޶��� ��ġ
	D3DXVECTOR3 Eye = g_Eye;
	D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ī�޶� ���� 
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cb_game.mView, &Eye, &At, &Up);// �� ��� ���





													   // ���� ��� ���
													   // (D3DXMATRIX *pOut,  ���� ���� ��� ��ȯ
	FLOAT fovy = D3DX_PI * 0.5f;  // �þ߰�
	FLOAT Aspect = 800.0f / 600.0f;// ȭ�� ����( 1.33f )
	FLOAT zn = 0.1f; // �ٴܸ� �Ÿ�
	FLOAT zf = 1000.0f;   // ���ܸ� �Ÿ�

	D3DXMatrixPerspectiveFovLH(&cb_game.mProj, fovy, Aspect, zn, zf); // ==>> ���� ��ǥ


																	  // ��ġ ��� ��ȯ
																	  // Update variables
	D3DXMatrixTranspose(&cb.mWorld, &cb_game.mWorld);
	D3DXMatrixTranspose(&cb.mView, &cb_game.mView);
	D3DXMatrixTranspose(&cb.mProj, &cb_game.mProj);

	// Update constant buffer in every frame
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	return true;
}
bool Sample::Render()
{
	// Just clear the backbuffer
	float ClearColor[4] = { 1.0f, 1.0f,1.0f, 1.0f }; //red,green,blue,alpha
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView,
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
	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// �ε��� ����
	m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R16_UINT, 0);

	// ��� ����
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// Set primitive topology g_pImmediateContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set primitive topology
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);

	// �������۸� ���� ���
	//g_pImmediateContext->Draw(6, 0);   
	// �ε��� ���۰� ���� ���
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	m_pSwapChain->Present(0, 0);
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
#ifndef _DEBUG
TCORE_RUN(_T("Sample Cube 1"), 800, 600)
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int       nCmdShow)
{
	Sample sample;
	if (sample.SetWindow(hInstance, _T("Sample Cube 1"), 800, 600))
	{
		sample.Run();
	}
}
#endif