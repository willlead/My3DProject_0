#include "bgShapeGuideAxis.h"

bgShapeGuideAxis::bgShapeGuideAxis()
{
}

bgShapeGuideAxis::~bgShapeGuideAxis()
{
}

bool bgShapeGuideAxis::Init()
{
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	return true;
}

bool bgShapeGuideAxis::Frame()
{
	return true;
}

bool bgShapeGuideAxis::Render()
{
	UINT iStride = sizeof(VertexPC);
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

bool bgShapeGuideAxis::Release()
{
	return true;
}

HRESULT bgShapeGuideAxis::CreateBuffer(float fLength)
{
	HRESULT hr = S_OK;

	// ���ؽ� ����
	VertexPC Vertices[] =
	{
		{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } }, // ���� ����
		{ { fLength, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } }, // x�� ����
		{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } }, // ���� �ʷ�
		{ { 0.0f, fLength, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } }, // y�� �ʷ�
		{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }, // ���� �Ķ�
		{ { 0.0f, 0.0f, fLength },{ 0.0f, 0.0f, 1.0f, 1.0f } }, // z�� �Ķ�
	};
	m_iNumVertex = COUNTOF(Vertices);

	// �ε��� ����
	UINT iIndices[] =
	{
		0, 1, // x�� ����
		2, 3, // y�� �ʷ�
		4, 5, // z�� �Ķ�
	};
	m_iNumIndex = COUNTOF(iIndices);

	// ���ؽ����� ����
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = sizeof(VertexPC) * m_iNumVertex;
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

HRESULT bgShapeGuideAxis::LoadShader()
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	// �������̴� �ε�
	ID3DBlob* pVSB = NULL;
	HR_RETURN(D3DX11CompileFromFile(L"PC.hlsl", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, NULL, NULL, &pVSB, NULL, NULL));
	HR_RETURN(m_pDevice->CreateVertexShader((DWORD*)pVSB->GetBufferPointer(), pVSB->GetBufferSize(), NULL, &m_pVS));

	// �ȼ����̴� �ε�
	ID3DBlob* pPSB = NULL;
	HR_RETURN(D3DX11CompileFromFile(L"PC.hlsl", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pPSB, NULL, NULL));
	HR_RETURN(m_pDevice->CreatePixelShader((DWORD*)pPSB->GetBufferPointer(), pPSB->GetBufferSize(), NULL, &m_pPS));

	// ���̾ƿ� ����
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR_RETURN(m_pDevice->CreateInputLayout(layout, 2, pVSB->GetBufferPointer(), pVSB->GetBufferSize(), &m_pInputLayout));

	return hr;
}
