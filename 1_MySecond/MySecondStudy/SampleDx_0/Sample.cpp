#include "Sample.h"

HRESULT		hr;

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


HRESULT InitDevice()
{

	HRESULT hr = S_OK;

	//RECT rc;
	//GetClientRect(g_hWnd, &rc);
	//UINT width = rc.right - rc.left;
	//UINT height = rc.bottom - rc.top;

	
	// Compile the vertex shader
	/*ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile(L"hlsl.fx", "VS", "vs_4_0", &pVSBlob);

	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}*/

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

		m_Device
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




	// ������۸� �ʱ�ȭ	
	// ������ķ� ���� ����� �����.
	D3DXMatrixIdentity(&cb.mWorld); //
	D3DXMatrixIdentity(&cb.mView);
	D3DXMatrixIdentity(&cb.mProj);
	//D3DXMatrixRotationY(&cb.mWorld, D3DXToRadian(180.0f));// m_Timer.m_fAccumulation);
	g_Eye.x = 0.0f;
	g_Eye.y = 0.0f;
	g_Eye.z = -5.0f;

	if (FAILED(hr = CreateDevice()))
	{
		return false;
	}
	if (FAILED(hr = CreateSwapChain(m_hWnd, this->m_iWindowWidth, this->m_iWindowHeight)))
	{
		return false;
	}

	if (FAILED(hr = SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(hr = SetViewPort()))
	{
		return false;
	}



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

	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData,
		&g_pConstantBuffer);
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

	// �ε��� ����
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer,
		DXGI_FORMAT_R16_UINT, 0);

	// ��� ����
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// Set primitive topology g_pImmediateContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	// �������۸� ���� ���
	//g_pImmediateContext->Draw(6, 0);	
	// �ε��� ���۰� ���� ���
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