#include "TDevice.h"

ID3D11Device*	g_pd3dDevice = nullptr;

HRESULT  TDevice::DeleteDXResouce()
{
	g_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	m_DefaultRT.Release();	
	return S_OK;
}
HRESULT  TDevice::CreateDXResouce()
{
	return S_OK;
}
bool   TDevice::ResizeDevice(UINT iWidth, UINT iHeight)
{
	HRESULT hr;
	if (m_pd3dDevice == NULL || g_pSwapChain == NULL) return true;
	// ������ ������ ��ü���� ����
	DeleteDXResouce();
	m_dwWidth = iWidth;
	m_dwHeight = iHeight;
	// �ٽ� ũ�⸦ �����ؼ� ��ü ����	
	g_pSwapChain->GetDesc(&m_sd);
	if (FAILED(hr = g_pSwapChain->ResizeBuffers(
		m_sd.BufferCount,
		iWidth, iHeight,
		m_sd.BufferDesc.Format,
		m_sd.Flags)))
	{
		return false;
	}
	if (FAILED(hr = SetRenderTarget()))
	{
		return false;
	}
	if (FAILED(hr = SetViewPort()))
	{
		return false;
	}

	DXGI_SWAP_CHAIN_DESC sd;
	g_pSwapChain->GetDesc(&sd);
	return true;
}
HRESULT TDevice::SetRenderTarget()
{
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
	{
		if (FAILED(hr))
			return hr;

		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer,
			NULL, &m_DefaultRT.m_pRenderTargetView);
	}
	pBackBuffer->Release();

	g_pSwapChain->GetDesc(&m_sd);

	hr = m_DefaultRT.CreateDSV(
		m_pd3dDevice,
		m_sd.BufferDesc.Width,
		m_sd.BufferDesc.Height);	

	if (FAILED(hr))
		return hr;

	return hr;
}
HRESULT TDevice::SetViewPort()
{
	HRESULT hr = S_OK;
	// Setup the viewport
	
	m_vp.Width = (FLOAT)m_dwWidth;
	m_vp.Height = (FLOAT)m_dwHeight;
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
	
	return hr;
}
HRESULT TDevice::CreateGIFactory()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&g_pGIFactory)))
	{
		return  hr;
	}
	//UINT i = 0;
	//IDXGIAdapter * pAdapter;
	//std::vector <IDXGIAdapter*> vAdapters;
	//while (g_pGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	//{
	//	vAdapters.push_back(pAdapter);
	//	++i;
	//}
	//DXGI_ADAPTER_DESC desc;
	//vAdapters[0]->GetDesc(&desc);

	//i = 0;
	//IDXGIOutput * pOutput;
	//std::vector<IDXGIOutput*> vOutputs;
	//while (vAdapters[0]->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND)
	//{
	//	vOutputs.push_back(pOutput);
	//	++i;
	//}
	//DXGI_OUTPUT_DESC od;
	//vOutputs[0]->GetDesc(&od);
	//vOutputs[1]->GetDesc(&od);
	return hr;
}
HRESULT TDevice::InitSwapChain()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	m_dwWidth = rc.right - rc.left;
	m_dwHeight = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_dwWidth;
	sd.BufferDesc.Height = m_dwHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(hr = g_pGIFactory->CreateSwapChain(m_pd3dDevice, &sd, &g_pSwapChain)))
	{
		return hr;
	}
	return hr;
}
HRESULT TDevice::InitDevice()
{
	HRESULT hr = S_OK;
#pragma region Device
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_UNKNOWN,
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

	IDXGIAdapter* pAdapter = NULL;
	g_pGIFactory->EnumAdapters(0, &pAdapter);

	ComPtr<ID3D11Device> pd3dDevice;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes;
	driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(pAdapter,
			g_driverType, NULL, createDeviceFlags,
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION,
			pd3dDevice.GetAddressOf(), &g_featureLevel,
			&g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if( pAdapter ) pAdapter->Release();
	if (FAILED(hr))
		return hr;
	g_pd3dDevice = pd3dDevice.Detach();
#pragma endregion 
	return S_OK;
}
//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void TDevice::CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();	
	m_DefaultRT.Release();

	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (g_pGIFactory) g_pGIFactory->Release();
}

TDevice::TDevice()
{
}


TDevice::~TDevice()
{
}
