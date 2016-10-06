#include "TWindow.h"

TWindow* g_pWindow = NULL;
//--------------------------------------------------------------------------------------
// 메인 윈도우 프로시져
//--------------------------------------------------------------------------------------
LRESULT WINAPI StaticWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{		
	assert(g_pWindow);
	return g_pWindow->WindowMsgProc(  hWnd,  msg,  wParam,  lParam );   
}
//--------------------------------------------------------------------------------------
// 가상함수 윈도우 프로시져
//--------------------------------------------------------------------------------------
int TWindow::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return -1;
}
//--------------------------------------------------------------------------------------
// 윈도우 프로시져
//--------------------------------------------------------------------------------------
LRESULT TWindow::WindowMsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int iReturn = MsgProc(  hWnd,  message,  wParam,  lParam );
	if( iReturn >= 0 )
	{
		return iReturn;
	}

    switch( message )
    {

		case WM_KEYDOWN:
        {
			switch( wParam )
			{
				case '0':
				{
					if( GetSwapChain() )
					{		
						BOOL IsScreenMode = FALSE;
						GetSwapChain()->GetFullscreenState( &IsScreenMode, NULL );
						GetSwapChain()->SetFullscreenState( !IsScreenMode, NULL );
						SetFullScreenFlag( IsScreenMode );
						assert( GetFullScreenFlag()  == IsScreenMode );
						if( IsScreenMode )
						{
							ShowWindow( hWnd, SW_SHOW );
						}
					}
				}break;
			}
		}break;
		case WM_EXITSIZEMOVE:			
			//if( SIZE_MAXHIDE == wParam ) // 다른 윈도우가 최대화되어 현재 윈도우가 가려지면
			//if( SIZE_MAXIMIZED == wParam ) // 최대화
			//if( SIZE_MAXSHOW == wParam ) // 최대화 되어 현 윈도우를 가려진 후 다시 원래 크리로 복구되어 이 윈도우가 보일때
			//if( SIZE_RESTORED == wParam )// 크기가 변경되어었을 때 
			if( SIZE_MINIMIZED != wParam ) // 최소화
            {			
				UINT width	= LOWORD(lParam);
				UINT height = HIWORD(lParam);
				ResizeDevice(width, height);
			}
			break;
		case WM_CLOSE:
        {
            HMENU hMenu;
            hMenu = GetMenu( hWnd );
            if( hMenu != NULL )      DestroyMenu( hMenu );
            DestroyWindow( hWnd );
            UnregisterClass( L"TBasis3D11_Sample", NULL );            
            return 0;
        }
		case WM_DESTROY:
            PostQuitMessage( 0 );
            break;        
    }
	
    return DefWindowProc( hWnd, message, wParam, lParam );
}
void TWindow::CenterWindow(HWND hwnd)
{
	// get the width and height of the screen
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight= GetSystemMetrics(SM_CYFULLSCREEN);

	// do the centering math
	int iDestX = (iScreenWidth-(m_rcWindowBounds.right-m_rcWindowBounds.left)) / 2;
	int iDestY = (iScreenHeight-(m_rcWindowBounds.bottom-m_rcWindowBounds.top)) / 2;

	// center the window
	MoveWindow( hwnd, iDestX, iDestY, 
				m_rcWindowBounds.right-m_rcWindowBounds.left,
				m_rcWindowBounds.bottom-m_rcWindowBounds.top,
				true);
}
bool TWindow::InitWindow(HINSTANCE hInstance, int nCmdShow, TCHAR* strWindowTitle, BOOL IsFullScreen,
						 int iWindowWidth, int iWindowHeight )
{
	  // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &StaticWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TBasis3D11_Sample";
    wcex.hIconSm = LoadIcon( wcex.hInstance,MAKEINTRESOURCE(IDI_APPLICATION) );
    if( !RegisterClassEx( &wcex ) )
        return false;

    // Create window
    m_hInstance = hInstance;
    RECT rc = { 0, 0, iWindowWidth, iWindowHeight };
	// 작업영역(  타이틀 바/경계선/메뉴/스크롤 바 등의 영역을 제외한 영역), 윈도우 스타일, 메뉴여부
	// AdjustWindowRect()함수의 호출이 있기 때문에 윈도우 작업영역과 D3D 디바이스의 백버퍼의 영역이 일치하게된다.
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    m_hWnd = CreateWindow( L"TBasis3D11_Sample",strWindowTitle, WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !m_hWnd )
        return false;

	// Save window properties
	m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	GetWindowRect( m_hWnd, &m_rcWindowBounds );
	GetClientRect( m_hWnd, &m_rcWindowClient );

	CenterWindow(m_hWnd);
	UpdateWindow(m_hWnd);	

	m_iWindowWidth		= 	m_rcWindowClient.right - m_rcWindowClient.left;
	m_iWindowHeight		=	m_rcWindowClient.bottom-m_rcWindowClient.top;

	ShowWindow( m_hWnd, nCmdShow );
	return true;
}

TWindow::TWindow(void)
{
	m_hInstance			= NULL;
	m_hWnd				= NULL;
	g_pWindow			= this;
}

TWindow::~TWindow(void)
{
}
