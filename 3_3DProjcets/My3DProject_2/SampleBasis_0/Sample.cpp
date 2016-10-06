#include "Sample.h"



Sample::Sample()
{
}


Sample::~Sample()
{
}
#ifdef _DEBUG
TBASIS_RUN(_T"Sample");
#else
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){ Sample gSample;
if( !gSample.InitWindow( hInstance, nCmdShow,  L#x ) )	{	return 0;	}	ShowCursor ( TRUE );gSample.Run();	return 1; }
#endif