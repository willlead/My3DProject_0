#include "bgShapeBox.h"

bgShapeBox::bgShapeBox()
{
}

bgShapeBox::~bgShapeBox()
{
}

bool bgShapeBox::Init()
{
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return true;
}

bool bgShapeBox::Frame()
{
	return true;
}

bool bgShapeBox::Render()
{
	UINT iStride = sizeof(VertexPNCT);
	UINT iOffset = 0;
	m_pDContext->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
	m_pDContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	m_pDContext->IASetInputLayout(m_pInputLayout);
	m_pDContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	m_pDContext->VSSetShader(m_pVS, NULL, 0);
	m_pDContext->VSSetConstantBuffers(0, 1, &m_pCB);
	m_pDContext->HSSetShader(NULL, NULL, 0);
	m_pDContext->DSSetShader(NULL, NULL, 0);
	m_pDContext->GSSetShader(NULL, NULL, 0);
	//m_pDContext->GSSetConstantBuffers(0, 1, &m_pCB);
	m_pDContext->RSSetState(m_pRasterizerState);
	m_pDContext->PSSetShader(m_pPS, NULL, 0);
	//m_pDContext->PSSetConstantBuffers(0, 1, &m_pCB);

	//m_pDContext->OMSetRenderTargets(1, &, &);
	//m_pDContext->OMSetBlendState(1, &, &);
	//m_pDContext->OMSetDepthStencilState(&, 0);

	m_pDContext->DrawIndexed(m_iNumIndex, 0, 0);
	return true;
}

bool bgShapeBox::Release()
{
	return true;
}

HRESULT bgShapeBox::CreateBuffer(float fSize)
{
	HRESULT hr = S_OK;

	// ���ؽ� ����
	VertexPNCT Vertices[] =
	{
		{ { -fSize, +fSize, -fSize },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } }, // 0
		{ { +fSize, +fSize, -fSize },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } }, // 1
		{ { +fSize, -fSize, -fSize },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } }, // 2
		{ { -fSize, -fSize, -fSize },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } }, // 3
		{ { -fSize, +fSize, +fSize },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } }, // 4
		{ { +fSize, +fSize, +fSize },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } }, // 5
		{ { +fSize, -fSize, +fSize },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } }, // 6
		{ { -fSize, -fSize, +fSize },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }, // 7
	};
	m_iNumVertex = COUNTOF(Vertices);

	// �ε��� ����
	UINT iIndices[] =
	{
		0, 1, 2, 0, 2, 3, // �ո�      4 ======= 5
		5, 4, 7, 5, 7, 6, // �޸�    //|      //||
		4, 0, 3, 4, 3, 7, // ����   0 ======= 1 ||
		1, 5, 6, 1, 6, 2, // ����   || 7 ----||- 6
		4, 5, 1, 4, 1, 0, // ���   ||/      ||//
		3, 2, 6, 3, 6, 7, // �ϴ�   3 ======= 2
	};
	m_iNumIndex = COUNTOF(iIndices);

	// ���ؽ����� ����
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = sizeof(VertexPNCT) * m_iNumVertex;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VData;
	VData.pSysMem = Vertices;
	VData.SysMemPitch = 0;
	VData.SysMemSlicePitch = 0;

	HR_RETURN(m_pDevice->CreateBuffer(&VBDesc, &VData, &m_pVB));

	// �ε������� ����
	D3D11_BUFFER_DESC IBDesc;
	IBDesc.ByteWidth = sizeof(UINT) * m_iNumIndex;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	IBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IData;
	IData.pSysMem = iIndices;
	IData.SysMemPitch = 0;
	IData.SysMemSlicePitch = 0;

	HR_RETURN(m_pDevice->CreateBuffer(&IBDesc, &IData, &m_pIB));

	return hr;
}

HRESULT bgShapeBox::LoadShader()
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	// �������̴� �ε�
	ID3DBlob* pVSB = NULL;
	HR_RETURN(D3DX11CompileFromFile(L"PNCT.hlsl", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, NULL, NULL, &pVSB, NULL, NULL));
	HR_RETURN(m_pDevice->CreateVertexShader((DWORD*)pVSB->GetBufferPointer(), pVSB->GetBufferSize(), NULL, &m_pVS));

	// �ȼ����̴� �ε�
	ID3DBlob* pPSB = NULL;
	HR_RETURN(D3DX11CompileFromFile(L"PNCT.hlsl", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pPSB, NULL, NULL));
	HR_RETURN(m_pDevice->CreatePixelShader((DWORD*)pPSB->GetBufferPointer(), pPSB->GetBufferSize(), NULL, &m_pPS));

	// ���̾ƿ� ����
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR_RETURN(m_pDevice->CreateInputLayout(layout, 4, pVSB->GetBufferPointer(), pVSB->GetBufferSize(), &m_pInputLayout));

	return hr;
}
