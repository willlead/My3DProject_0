#include "TShape.h"

bool TShape::Create(ID3D11Device* pDevice,
	const TCHAR* pLoadShaderFile,
	const TCHAR* pLoadTextureString)
{
	m_pd3dDevice = pDevice;

	if (FAILED(LoadShaderFile(pDevice, pLoadShaderFile)))
	{
		MessageBox(0, _T("LoadShaderFile 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(SetInputLayout()))
	{
		MessageBox(0, _T("SetInputLayout 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(0, _T("CreateVertexBuffer 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateIndexBuffer()))
	{
		MessageBox(0, _T("CreateVertexBuffer 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateConstantBuffer()))
	{
		MessageBox(0, _T("CreateConstantBuffer 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(LoadTextures(pDevice, pLoadTextureString)))
	{
		MessageBox(0, _T("CreateResource 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateResource()))
	{
		MessageBox(0, _T("CreateResource 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	return Init();
}
HRESULT TShape::LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;
	m_Object.g_pTextureSRV.Attach(DX::CreateShaderResourceView(pDevice, pLoadTextureString));
	return hr;
}
HRESULT TShape::LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pShaderFile)
{
	m_Object.g_pVertexShader.Attach(DX::LoadVertexShaderFile(pDevice, pShaderFile, m_Object.g_pVSBlob.GetAddressOf()));
	m_Object.g_pPixelShader.Attach(DX::LoadPixelShaderFile(pDevice, pShaderFile));
	m_Object.g_pGeometryShader.Attach(DX::LoadGeometryShaderFile(pDevice, pShaderFile));
	return S_OK;
}
bool TShape::Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor) { return true; }
HRESULT TShape::SetInputLayout() { return S_OK; }
HRESULT TShape::CreateVertexBuffer() { return S_OK; }
HRESULT TShape::CreateIndexBuffer() { return S_OK; }
HRESULT TShape::CreateConstantBuffer() {
	m_cbData.Color = D3DXVECTOR4(1, 1, 1, 1);
	m_Object.g_pConstantBuffer.Attach(DX::CreateConstantBuffer(m_pd3dDevice, &m_cbData, 1, sizeof(VS_CONSTANT_BUFFER)));
	return S_OK;
}
bool TShape::Init() { return true; }
bool TShape::Frame() { return true; }
bool TShape::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_Object.m_iPrimitiveType);
	m_Object.PreRender(pContext, m_Object.m_iVertexSize);
	return true;
}
bool TShape::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_Object.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	m_Object.PostRender(pContext, m_Object.m_iNumIndex);	
	return true;
}
bool TShape::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	//m_Object.Render(pContext, m_Object.m_iVertexSize, m_Object.m_iNumIndex);
	return true;
}
bool TShape::Release()
{
	return true;
}
void TShape::SetAmbientColor(float fR, float fG, float fB, float fA)
{
	m_cbData.Color = D3DXVECTOR4(fR, fG, fB, fA);
}
void TShape::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenterPos.x = pWorld->_41;
		m_vCenterPos.y = pWorld->_42;
		m_vCenterPos.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
}
ID3D11Buffer* TShape::GetVB()
{
	return m_Object.g_pVertexBuffer.Get();
}
ID3D11Buffer* TShape::GetIB()
{
	return m_Object.g_pIndexBuffer.Get();
}
HRESULT TShape::CreateResource()
{
	m_Object.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
HRESULT TShape::DeleteResource()
{
	return S_OK;
}
TShape::TShape(void)
{
	m_pd3dDevice = NULL;
	m_cbData.Color = D3DXVECTOR4(1, 1, 1, 1);
	m_vCenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);	
}

TShape::~TShape(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TLineShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TLineShape::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_Object.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_Object.g_pVSBlob.Get()->GetBufferSize(),
		m_Object.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
HRESULT TLineShape::CreateVertexBuffer()
{
	HRESULT hr=S_OK;
	m_VertexList.resize(2);
	m_VertexList[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].p = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_VertexList[1].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	UINT iNumVertex = m_VertexList.size();
	m_Object.m_iVertexSize = sizeof(PC_VERTEX);
	m_Object.m_iNumVertex = iNumVertex;
	m_Object.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_VertexList.at(0), m_Object.m_iNumVertex, m_Object.m_iVertexSize));
	return hr;
}
HRESULT TLineShape::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(2);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	UINT iNumIndex = m_IndexList.size();
	m_Object.m_iNumIndex = iNumIndex;
	m_Object.g_pIndexBuffer.Attach(DX::CreateIndexBuffer(m_pd3dDevice, &m_IndexList.at(0), m_Object.m_iNumIndex, sizeof(DWORD)));
	return hr;
}

bool TLineShape::Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor)
{
	PC_VERTEX vertices[2];
	vertices[0].p = vStart;
	vertices[0].c = vColor;
	vertices[1].p = vEnd;
	vertices[1].c = vColor;
	// 동적 리소스 갱신 방법 1 : D3D11_USAGE_DEFAULT 사용
	pContext->UpdateSubresource(GetVB(), 0, NULL, &vertices, 0, 0);
	return Render(pContext);
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TLineShape::CreateResource()
{
	m_Object.m_iPrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	return S_OK;
}
TLineShape::TLineShape(void)
{
}

TLineShape::~TLineShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TBoxShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TBoxShape::SetInputLayout()
{
	HRESULT hr=S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_Object.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_Object.g_pVSBlob.Get()->GetBufferSize(),
		m_Object.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
HRESULT TBoxShape::CreateVertexBuffer()
{
	HRESULT hr=S_OK;
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞면
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	// 뒷면
	m_VertexList[4] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 오른쪽
	m_VertexList[8] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 왼쪽
	m_VertexList[12] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 윗면
	m_VertexList[16] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 아랫면
	m_VertexList[20] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));


	UINT iNumVertex = m_VertexList.size();
	m_Object.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_Object.m_iNumVertex = iNumVertex;
	m_Object.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_VertexList.at(0), m_Object.m_iNumVertex, m_Object.m_iVertexSize));
	return hr;
}
HRESULT TBoxShape::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	UINT iNumIndex = m_IndexList.size();
	m_Object.m_iNumIndex = iNumIndex;
	m_Object.g_pIndexBuffer.Attach(DX::CreateIndexBuffer(m_pd3dDevice, &m_IndexList.at(0), m_Object.m_iNumIndex, sizeof(DWORD)));
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TBoxShape::CreateResource()
{
	m_Object.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
TBoxShape::TBoxShape(void)
{
}

TBoxShape::~TBoxShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TPlaneShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TPlaneShape::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_Object.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_Object.g_pVSBlob.Get()->GetBufferSize(),
		m_Object.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
HRESULT TPlaneShape::CreateVertexBuffer()
{
	HRESULT hr=S_OK;
	m_VertexList.resize(4);
	m_VertexList[0] = PCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[3] = PCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	UINT iNumVertex = m_VertexList.size();
	m_Object.m_iVertexSize = sizeof(PCT_VERTEX);
	m_Object.m_iNumVertex = iNumVertex;
	m_Object.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_VertexList.at(0), m_Object.m_iNumVertex, m_Object.m_iVertexSize));
	return hr;
}
HRESULT TPlaneShape::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 0;
	m_IndexList[4] = 2;
	m_IndexList[5] = 3;

	UINT iNumIndex = m_IndexList.size();
	m_Object.m_iNumIndex = iNumIndex;
	m_Object.g_pIndexBuffer.Attach(DX::CreateIndexBuffer(m_pd3dDevice, &m_IndexList.at(0), m_Object.m_iNumIndex, sizeof(DWORD)));
	return hr;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TPlaneShape::CreateResource()
{
	m_Object.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
TPlaneShape::TPlaneShape(void)
{
}

TPlaneShape::~TPlaneShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TLineShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TDirectionLineShape::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_Object.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_Object.g_pVSBlob.Get()->GetBufferSize(),
		m_Object.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
HRESULT TDirectionLineShape::CreateVertexBuffer()
{
	HRESULT hr=S_OK;
	m_VertexList.resize(6);
	m_VertexList[0] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	m_VertexList[1] = PC_VERTEX(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	m_VertexList[2] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_VertexList[3] = PC_VERTEX(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_VertexList[4] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
	m_VertexList[5] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));

	UINT iNumVertex = m_VertexList.size();
	m_Object.m_iVertexSize = sizeof(PC_VERTEX);
	m_Object.m_iNumVertex = iNumVertex;
	m_Object.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_VertexList.at(0), m_Object.m_iNumVertex, m_Object.m_iVertexSize));
	return hr;
}
HRESULT TDirectionLineShape::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 3;	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5;

	UINT iNumIndex = m_IndexList.size();
	m_Object.m_iNumIndex = iNumIndex;
	m_Object.g_pIndexBuffer.Attach(DX::CreateIndexBuffer(m_pd3dDevice, &m_IndexList.at(0), m_Object.m_iNumIndex, sizeof(DWORD)));
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TDirectionLineShape::CreateResource()
{
	m_Object.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	return S_OK;
}
TDirectionLineShape::TDirectionLineShape(void)
{
}

TDirectionLineShape::~TDirectionLineShape(void)
{
}