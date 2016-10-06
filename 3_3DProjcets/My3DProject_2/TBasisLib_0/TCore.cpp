#include "TCore.h"

T_STR RasterizerState[] =
{
	_T("g_pRSBackCullSolid"),
	_T("g_pRSNoneCullSolid"),
	_T("g_pRSFrontCullSolid"),
};

T_STR SamplerState[] =
{
	_T("g_pSSWrapLinear"),
	_T("g_pSSWrapPoint"),
	_T("g_pSSMirrorLinear"),
	_T("g_pSSMirrorPoint"),
	_T("g_pSSClampLinear"),
	_T("g_pSSClampPoint"),
};

int TCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return -1;
}
bool TCore::PreInit()
{
	if (FAILED(InitDevice(m_hWnd, this->m_iWindowWidth, this->m_iWindowHeight)))
	{
		MessageBox(0, _T("CreateDevice  ����"), _T("Fatal error"), MB_OK);
		return false;
	}
	if (!m_Timer.Init())	return false;

	// DirectX Input �ʱ�ȭ
	if (!I_Input.Init())
	{
		return false;
	}
	return true;
}
bool TCore::Init()
{
	return true;
}
bool TCore::PostInit()
{
	return true;
}
bool TCore::TInit()
{
	if (!PreInit()) return false;
	if (!Init()) return false;
	if (!PostInit()) return false;
	return true;
}

bool TCore::PreFrame()
{
	if (!m_Timer.Frame()) return false;
	if (!I_Input.Frame()) return false;
	if (!Update(m_pImmediateContext)) return false;
	return true;
}
bool TCore::Update(ID3D11DeviceContext*    pContext)
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
		if (m_iCullMode > 3)
		{
			m_iCullMode = 1;
		}
		m_iCullMode = min(m_iCullMode, 3);
		m_bWireFrameRender = false;
	}

	if (I_Input.KeyCheck(DIK_4) == KEY_UP)
	{
		++m_iSamplerMode;
		if (m_iSamplerMode >= 6)
		{
			m_iSamplerMode = 0;
		}
	}

	if (m_bWireFrameRender)
	{
		ApplyRS(pContext, TDxState::g_pRSWireFrame);
	}
	else
	{
		ApplyRS(pContext, TDxState::g_pRS[m_iCullMode - 1]);
	}
	ApplySS(pContext, TDxState::g_pSS[m_iSamplerMode]);
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType);
	return true;
}
bool TCore::PostFrame()
{
	return true;
}
bool TCore::Frame()
{
	return true;
}
bool TCore::Release()
{
	return true;
}
bool TCore::TRelease()
{
	CleanupDevice();
	if (!m_Timer.Release()) return false;
	//if( !m_Font.Release() ) return false;	
	if (!m_Font.Release()) return false;
	if (!I_Input.Release()) return false;
	return Release();
}
bool TCore::TFrame()
{
	PreFrame();
	Frame();
	PostFrame();
	return true;
}
bool TCore::TRender()
{
	I_Input.Render();
	m_Timer.Render();

	PreRender();
	Render();
	if (I_Input.KeyCheck(DIK_V))
	{
		DrawDebug();
	}
	PostRender();
	return true;
}

bool TCore::Render()
{
	return true;
}
bool TCore::PreRender()
{
	// Just clear the backbuffer
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	m_pImmediateContext->ClearRenderTargetView(GetRenderTargetView(), ClearColor);
	m_pImmediateContext->ClearDepthStencilView(m_DefaultRT.m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->OMSetRenderTargets(1, GetRenderTargetViewAddress(), m_DefaultRT.m_pDepthStencilView.Get());
	m_pImmediateContext->RSSetViewports(1, &m_DefaultRT.m_vp);
	ApplyDSS(m_pImmediateContext, TDxState::g_pDSSDepthEnable);
	return true;
}
bool TCore::DrawDebug()
{
	// FPS ���
	TCHAR pBuffer[256];
	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("FPS:%d"), m_Timer.GetFPS());

	m_Font.Begin();
	m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	RECT rc1 = { 0,0, m_iWindowWidth, m_iWindowHeight };
	m_Font.DrawText(rc1, pBuffer, D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5));
	m_Font.End();

	//-----------------------------------------------------------------------
	// ����Ǿ� RasterizerState Ÿ�� ǥ��
	//-----------------------------------------------------------------------	
	RECT rc;
	rc.top = 30;
	rc.bottom = 600;
	rc.left = 0;
	rc.right = 800;
	T_STR str = RasterizerState[m_iCullMode];
	DrawDebugRect(&rc, const_cast<TCHAR*>(str.c_str()), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	//-----------------------------------------------------------------------
	// ����Ǿ� SamplerState Ÿ�� ǥ��
	//-----------------------------------------------------------------------	
	rc.top = 50;
	rc.bottom = 600;
	rc.left = 0;
	rc.right = 800;
	str = SamplerState[m_iSamplerMode];
	DrawDebugRect(&rc, const_cast<TCHAR*>(str.c_str()), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	return true;
}

bool TCore::DrawDebugRect(RECT* rcDest, TCHAR* pString, D3DXCOLOR color)
{
	if (rcDest == NULL) return false;

	if (m_Font.m_pTextFormat)
	{
		D2D1_SIZE_F rtSize = m_Font.m_pRT->GetSize();
		//Draw a grid background.
		int width = static_cast <int> (rtSize.width);
		int height = static_cast <int> (rtSize.height);

		m_Font.Begin();
		m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		//RECT rc1 = {0,0, m_iWindowWidth, m_iWindowHeight};
		m_Font.DrawText(*rcDest, pString, D2D1::ColorF(color.r, color.g, color.b, 0.5f));
		m_Font.End();
	}

	return true;
}
bool TCore::PostRender()
{
	HRESULT hr;
	if (FAILED(hr = GetSwapChain()->Present(0, 0)))
	{
		H(hr);
	}
	return true;
}
bool TCore::ToolRun()
{
	if (!TInit()) return false;

	//TFrame();
	//TRender();        

	return true;
}
bool TCore::Run()
{
	if (!TInit()) return false;
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			TFrame();
			TRender();
		}
	}
	if (!TRelease()) return false;
	return true;
}
HRESULT TCore::CreateDxResource()
{
	IDXGISurface1*		pBackBuffer = NULL;
	HRESULT hr = GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);
	m_Font.Set(m_hWnd, m_iWindowWidth, m_iWindowHeight, pBackBuffer);
	if (pBackBuffer)	pBackBuffer->Release();

	if (FAILED(hr = GetSwapChain()->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}

	if (FAILED(hr = m_DefaultRT.UpdateDepthStencilView(m_pd3dDevice, m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height)))
	{
		return hr;
	}
	return CreateResource();
}
HRESULT TCore::DeleteDxResource()
{
	if (!m_Font.Release()) return false;
	return DeleteResource();
}
HRESULT TCore::CreateResource()
{
	return S_OK;
}
HRESULT TCore::DeleteResource()
{
	return S_OK;
}
TCore::TCore(void)
{
	m_iPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iCullMode = 1;
	m_iSamplerMode = 0;
	m_bWireFrameRender = false;
}

TCore::~TCore(void)
{
}
