#include "TBasisLib_2.h"


int TBasisLib_2::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( m_pMainCamera )
	{
		m_pMainCamera->WndProc( hWnd, message, wParam, lParam );
	}
	m_ShaderEditer.MsgProc( hWnd, message, wParam, lParam );

	return WndProc(hWnd, message, wParam, lParam);
}
int TBasisLib_2::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return -1;
}
//--------------------------------------------------------------------------------------
// 카메라 타입 설정	
//--------------------------------------------------------------------------------------
void TBasisLib_2::SetCameraType( CAMERA_TYPE type )
{
	if( type == DEBUG_CAMERA )		
	{
		// 이전 카메라의 타입이 설정되어 있었다면 상태값을 유지 시킨다.
		if( m_pMainCamera )
		{
			m_pDebugCamera->SetViewMatrix( *m_pMainCamera->GetEyePt(), *m_pMainCamera->GetLookAtPt() );
		}
		m_pMainCamera = m_pDebugCamera;
		m_CameraType = DEBUG_CAMERA;
	}
	if( type == MODELVIEW_CAMERA )	
	{
		if( m_pMainCamera )
		{
			m_pModelViewCamera->SetViewMatrix( *m_pMainCamera->GetEyePt(), *m_pMainCamera->GetLookAtPt() );
		}
		m_pMainCamera = m_pModelViewCamera;
		m_CameraType = MODELVIEW_CAMERA;
	}
	if( type == BACKVIEW_CAMERA )
	{
		if( m_pMainCamera )
		{
			m_pBackViewCamera->SetViewMatrix( *m_pMainCamera->GetEyePt(), *m_pMainCamera->GetLookAtPt() );
		}
		m_pMainCamera = m_pBackViewCamera;
		m_CameraType = BACKVIEW_CAMERA;		
	}

}
bool TBasisLib_2::PreInit()
{
	if( FAILED( InitDevice(m_hWnd,this->m_iWindowWidth, this->m_iWindowHeight) ) )
	{
		MessageBox( 0, _T("CreateDevice  실패"), _T("Fatal error"), MB_OK );
		return false;
	}
	if( !I_Timer.Init() )	return false;



	// DirectX Input 생성 
	if( !I_Input.InitDirectInput( m_hInstance , m_hWnd, true, true ) )
	{
		MessageBox( 0, _T("I_Input.InitDirectInput 실패"), _T("Fatal error"), MB_OK );
		return false;
	}
	// DirectX Input 초기화
	if( !I_Input.Init() )
	{
		MessageBox( 0, _T("I_Input.Init() 실패"), _T("Fatal error"), MB_OK );
		return false;
	}
	
	SAFE_NEW( m_pDirectionLine, TDirectionLineShape );

	if( FAILED( m_pDirectionLine->Create(GetDevice(),m_pImmediateContext) ) )
	{
		MessageBox( 0, _T("m_LineShape 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}

	//--------------------------------------------------------------------------------------
	// 상태객체들 생성
	//--------------------------------------------------------------------------------------
	if( FAILED( m_DxState.Create(GetDevice()) ))
	{
		return false;
	}

	//--------------------------------------------------------------------------------------
	// 쉐이더 및 텍스쳐 매니져 디바이스 세팅	
	//--------------------------------------------------------------------------------------
	I_Shader.SetDevice( GetDevice() );
	I_Texture.SetDevice( GetDevice(),m_pImmediateContext );
	//--------------------------------------------------------------------------------------
	// 카메라 타입 설정	
	//--------------------------------------------------------------------------------------
	SAFE_NEW( m_pModelViewCamera, TModelViewCamera );
	m_pModelViewCamera->SetModelCenter(	D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_pModelViewCamera->SetViewMatrix(	D3DXVECTOR3( 0.0f, 0.0f, -100.0f ), 
									D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	float fAspectRatio = m_iWindowWidth / (float)m_iWindowHeight;  
	m_pModelViewCamera->SetProjMatrix( D3DX_PI* 0.25f, fAspectRatio, 0.1f, 3000.0f );
	m_pModelViewCamera->SetWindow( m_iWindowWidth, m_iWindowHeight );

	////////////////////
	SAFE_NEW( m_pBackViewCamera, TBackViewCamera );
	m_pBackViewCamera->SetModelCenter(	D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_pBackViewCamera->SetViewMatrix(	D3DXVECTOR3( 10.0f, 10.0f, -100.0f), 
									D3DXVECTOR3( 0.0f, 0.0f, 0.0f  ) );	
	m_pBackViewCamera->SetProjMatrix( D3DX_PI * 0.25f,fAspectRatio, 0.1f, 3000.0f );
	m_pBackViewCamera->SetWindow( m_iWindowWidth, m_iWindowHeight );

	////////////////////
	SAFE_NEW( m_pDebugCamera, TCamera );
	m_pDebugCamera->SetViewMatrix(	D3DXVECTOR3( 0.0f, 0.0f, -100.0f), 
									D3DXVECTOR3( 0.0f, 0.0f, 0.0f  ) );	
	m_pDebugCamera->SetProjMatrix( D3DX_PI * 0.25f,fAspectRatio, 0.1f, 3000.0f );

	SetCameraType(MODELVIEW_CAMERA);

	m_matProj = *m_pMainCamera->GetProjMatrix();
	//--------------------------------------------------------------------------------------
	// 툴에서 사용된다.
	//--------------------------------------------------------------------------------------
	SetLib();
	return true;
}
void TBasisLib_2::SetLib() {}
bool TBasisLib_2::Init()
{
	return true;
}
bool TBasisLib_2::PostInit()
{	
	return true;
}
bool TBasisLib_2::TInit()
{
	if( !PreInit() ) return false;
	if( !Init() ) return false;
	if( !PostInit() ) return false;
	return true;
}

bool TBasisLib_2::PreFrame()
{
	if( !m_ShaderEditer.Frame() ) return false;
	if( !I_Timer.Frame() ) return false;
	if( !I_Input.Frame() ) return false;
	//--------------------------------------------------------------------------------------
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------
	m_pMainCamera->Update(I_Timer.GetSPF());
	m_matView = *m_pMainCamera->GetViewMatrix();
	return true;
}
bool TBasisLib_2::PostFrame()
{	
	//--------------------------------------------------------------------------------------
	// 디버그 카메라 및 모델 뷰 카메라 전환
	//--------------------------------------------------------------------------------------
	if( g_InputData.bChangeCameraType )
	{
		if( m_CameraType == MODELVIEW_CAMERA )
		{
			SetCameraType(DEBUG_CAMERA);	
		}
		else if( m_CameraType == DEBUG_CAMERA )
		{
			SetCameraType(BACKVIEW_CAMERA);	
		}
		else if( m_CameraType == BACKVIEW_CAMERA )
		{
			SetCameraType(MODELVIEW_CAMERA);	
		}
	}

	if(g_InputData.bChangeFillMode)
	{
		m_bWireFrameRender = !m_bWireFrameRender;
		(m_bWireFrameRender ) ? m_StateBackCullSolid.SetFillMode(m_pd3dDevice, D3D11_FILL_WIREFRAME) :
								m_StateBackCullSolid.SetFillMode(m_pd3dDevice, D3D11_FILL_SOLID);
		//m_pImmediateContext->RSSetState( m_StateBackCullSolid.m_pRasterizerState );
	}	
	return true;
}
bool TBasisLib_2::Frame()
{
	return true;
}
bool TBasisLib_2::Release()
{	
	return true;
}
bool TBasisLib_2::TRelease()
{
	CleanupDevice();
	if( !I_Timer.Release() ) return false;		
	if( !I_Input.Release() ) return false;
	if( !m_pDirectionLine->Release() ) return false;
	if( !m_ShaderEditer.Release() ) return false;

	SAFE_DEL( m_pDirectionLine );
	SAFE_DEL( m_pDebugCamera );
	SAFE_DEL( m_pModelViewCamera );
	SAFE_DEL( m_pBackViewCamera );
	return Release();
}
bool TBasisLib_2::TFrame()
{
	PreFrame();
	Frame();
	PostFrame();
	return true;
}
bool TBasisLib_2::TRender()
{
	PreRender();
	Render();
	DrawDebug();
	PostRender();
	I_Input.PostProcess();
	return true;
}

bool TBasisLib_2::Render()
{
	return true;
}
bool TBasisLib_2::PreRender()
{
    // Just clear the backbuffer
    float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f }; //red,green,blue,alpha
	m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	m_pImmediateContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0, 0 );	
	m_pImmediateContext->RSSetViewports(1, &m_MainVP);	
	m_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	//--------------------------------------------------------------------------------------
	// 깊이 및 스텐실 뷰 리소스 랜더타켓 뷰에 바인딩
	//--------------------------------------------------------------------------------------
	m_pImmediateContext->OMSetRenderTargets( 1,&m_pRenderTargetView, m_pDepthStencilView );	
	m_StateDepthStencil.ApplyDepthStencil( m_pd3dDevice, m_pImmediateContext, 0.0f);
	m_StateBackCullSolid.ApplyRasterizer( m_pd3dDevice, m_pImmediateContext );
	return true;
}
bool TBasisLib_2::DrawDebug(bool bDrawString, bool bDrawLine )
{
	m_pImmediateContext->OMSetRenderTargets( 1,&m_pRenderTargetView, m_pDepthStencilView );	
	m_pImmediateContext->RSSetViewports(1, &m_MainVP);	
	if( bDrawString )
	{
		// FPS 출력
		TCHAR pBuffer[MAX_PATH];
		TCHAR pBufferA[MAX_PATH];
		TCHAR pBufferB[MAX_PATH];
		TCHAR pBufferC[MAX_PATH];
		memset( pBuffer, 0, sizeof( TCHAR ) * MAX_PATH );
		_stprintf_s( pBuffer, _T("FPS:%d"), I_Timer.GetFPS() );	
		RECT rc1 = {0,0, m_iWindowWidth, m_iWindowHeight/2};

		memset( pBufferA, 0, sizeof( TCHAR ) * MAX_PATH );
		_stprintf_s( pBufferA, _T("%10.4f %10.4f "), I_Timer.GetElapsedTime(), g_fSecPerFrame );		
		RECT rc2 = {65,0, m_iWindowWidth, m_iWindowHeight/2};

		memset( pBufferB, 0, sizeof( TCHAR ) * MAX_PATH );
		TCHAR_STRING pViewString[3];
		pViewString[0] = L"DEBUG_VIEW[F8]";pViewString[1] = L"MODEL_VIEW[F8]";pViewString[2] = L"BACK_VIEW[F8]";
		_stprintf_s( pBufferB, _T("CAMERA:%s"), pViewString[(int)m_CameraType].c_str() );		
		RECT rc3 = {220,0, m_iWindowWidth, m_iWindowHeight/2};

		TCHAR_STRING strText;	
		if( m_StateBackCullSolid.m_FillMode == D3D11_FILL_WIREFRAME )
			strText =  L"[FILL[F5]:WIREFRAME] ";
		else
			strText =  L"[FILL[F5]:SOLID] ";

		memset( pBufferC, 0, sizeof( TCHAR ) * MAX_PATH );
		_stprintf_s( pBufferC, _T("%s"), strText.c_str());	
		RECT rc4 = {500,0, m_iWindowWidth, m_iWindowHeight/2};

		I_Write.Begin();
		I_Write.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		I_Write.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		I_Write.DrawText( rc1, pBuffer,  D2D1::ColorF(1.3f, 0.3f, 0.3f,1.0));		
		I_Write.DrawText( rc2, pBufferA,  D2D1::ColorF(1.3f, 0.3f, 0.3f,1.0));		
		I_Write.DrawText( rc3, pBufferB,  D2D1::ColorF(1.3f, 0.3f, 0.3f,1.0));		
		I_Write.DrawText( rc4, pBufferC,  D2D1::ColorF(1.3f, 0.3f, 0.3f,1.0));	
		I_Write.End();
	}
	if( bDrawLine )
	{
		m_pDirectionLine->SetMatrix( NULL, &m_matView, &m_matProj );
		m_pDirectionLine->Render();	
	}

	return true;	
}
bool TBasisLib_2::DrawDebugRect(RECT* rcDest, TCHAR* pString, D3DXCOLOR color )
{
	if( rcDest == NULL ) return false;	

	if( I_Write.m_pTextFormat)
	{
		D2D1_SIZE_F rtSize = I_Write.m_pRT->GetSize ();
        //Draw a grid background.
        int width = static_cast <int> (rtSize.width);
        int height = static_cast <int> (rtSize.height);


		I_Write.Begin();
		I_Write.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		I_Write.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		//RECT rc1 = {0,0, m_iWindowWidth, m_iWindowHeight};
		I_Write.DrawText( *rcDest, pString,  D2D1::ColorF(color.r,color.g,color.b,0.5));		
		I_Write.End();
	}

	return true;
}
bool TBasisLib_2::PostRender()
{	
	GetSwapChain()->Present( 0, 0 );
	ClearD3D11DeviceContext(m_pImmediateContext);
	return true;
}
bool TBasisLib_2::Run()
{
	if( !TInit() ) return false;
	// Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			TFrame();
            TRender();
        }
    }
	if( !TRelease() ) return false;
	return true;
}
TBasisLib_2::TBasisLib_2(void)
{	
	m_pDirectionLine	= NULL;
	m_pMainCamera		= NULL;
	m_pDebugCamera		= NULL;
	m_pModelViewCamera	= NULL;
	m_pBackViewCamera	= NULL;
	m_bWireFrameRender	= false;
}

TBasisLib_2::~TBasisLib_2(void)
{
}

bool TBasisLib_2::CheckWindowSize()
{
	// AdjustWindowRect()함수의 호출이 있기 때문에 
	// 윈도우 작업영역과 백버퍼의 영역이 일치하게된다.
    GetClientRect( m_hWnd, &m_rcWindowClient );
	UINT iWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
	UINT IHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
	if( m_iWindowWidth != iWidth  || m_iWindowHeight != IHeight )
	{
		m_iWindowWidth	= iWidth;
		m_iWindowHeight = IHeight;		
		return false;
	}
	return true;
}
HRESULT TBasisLib_2::CreateDxResource()
{
	HRESULT hr;	
	DXGI_SWAP_CHAIN_DESC CurrentSD;
	GetSwapChain()->GetDesc( &CurrentSD );	

	m_iWindowWidth = CurrentSD.BufferDesc.Width;
	m_iWindowHeight = CurrentSD.BufferDesc.Height;
	
	float fAspectRatio = CurrentSD.BufferDesc.Width / (float)CurrentSD.BufferDesc.Height;  
	if( m_pModelViewCamera )
	{
		m_pModelViewCamera->SetProjMatrix( D3DX_PI/4.0f, fAspectRatio, 0.1f, 1000.0f );
		m_pModelViewCamera->SetWindow( CurrentSD.BufferDesc.Width, CurrentSD.BufferDesc.Height );
	}
	if( m_pBackViewCamera )
	{
		m_pBackViewCamera->SetProjMatrix( D3DX_PI/4.0f, fAspectRatio, 0.1f, 1000.0f );
		m_pBackViewCamera->SetWindow( CurrentSD.BufferDesc.Width, CurrentSD.BufferDesc.Height );
	}

	if( m_pDebugCamera )
	{
		m_pDebugCamera->SetProjMatrix( D3DX_PI * 0.25f,fAspectRatio, 0.1f, 1000.0f );
	}
	if( m_pMainCamera )
	{
		m_matProj = *m_pMainCamera->GetProjMatrix();
	}

	IDXGISurface1*		pBackBuffer=NULL;
	hr = GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);	
	I_Write.Set(m_hWnd, pBackBuffer );		
	if( pBackBuffer )	pBackBuffer->Release();

	// 각종 뷰 리소스 적용
	if( FAILED( hr = CreateResource() ))
	{
		return hr;
	}
	if( m_pDirectionLine && FAILED( hr  = m_pDirectionLine->CreateResource() ))
	{
		return hr;
	}	
	return S_OK;
}
HRESULT TBasisLib_2::DeleteDxResource()
{
	HRESULT hr;	
	if( !I_Write.Release() ) return false;	
	//--------------------------------------------------------------------------------------
	// 랜더타켓과 깊이스텐실 버퍼를 해제한다.
	//--------------------------------------------------------------------------------------	
	if( m_pDirectionLine && FAILED( hr = m_pDirectionLine->DeleteResource() ) )
	{
		return hr;
	}	
	// 하위 클래스들을 위한 뷰 리소스를 해제한다.
	if( FAILED( hr= DeleteResource()))
	{
		return hr;
	}	
	return S_OK;
}
HRESULT TBasisLib_2::CreateResource()
{
	return S_OK;
}
HRESULT TBasisLib_2::DeleteResource()
{
	return S_OK;
}