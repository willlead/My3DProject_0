#pragma once
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <set>
#include <list>
#include <string>
#include <map>
#include <vector>
#include <wrl.h>

#include <d3dx11.h>
#include <d3dcompiler.h>

//#include <xnamath.h>
#include <d3dx10math.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <dxgi.h> 
//#include <dxerr.h>


using namespace std;
using namespace Microsoft::WRL;

#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx10d.lib")
#pragma comment( lib, "d3dx11d.lib")
#else
#pragma comment( lib, "d3dx10.lib")
#pragma comment( lib, "d3dx11.lib")
#endif

#pragma comment( lib, "dwrite.lib")
#pragma comment	(lib, "d2d1.lib")
#pragma comment( lib, "ws2_32.lib")
#pragma comment( lib, "winmm.lib" )



typedef basic_string<TCHAR>				T_STR;
typedef basic_string<TCHAR>::iterator	T_ITOR;

typedef vector<basic_string<TCHAR>>	T_STR_VECTOR;
typedef vector<string>				C_STR_VECTOR;

typedef list<basic_string<TCHAR>>	T_STR_LIST;
typedef string						CHAR_STRING;

typedef vector<DWORD>				DWORD_VECTOR;
typedef	vector< DWORD >::iterator	DWORD_VECTOR_ITOR;

typedef list<DWORD>					DWORD_LIST;
typedef list<DWORD>::iterator		DWORD_LIST_ITOR;

typedef list< HANDLE >				HANDLE_LIST;
typedef	list< HANDLE >::iterator	HANDLE_LIST_ITOR;

//////////////////////////////////////////////
// DX 매소드 호출에 대한 반환값 처리 매크로
//////////////////////////////////////////////
#ifndef V
#define V(x)       { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};
#define TCORE_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR    lpCmdLine,int       nCmdShow){Sample sample;
#define TCORE_WIN(s,x,y) if (sample.SetWindow(hInstance, L#s, x, y))	{	sample.Run();	} }
#define TCORE_RUN(s,x,y) TCORE_START;TCORE_WIN(s,x,y);
#define TCORE_END }

#define TBASIS_EPSILON		((FLOAT)  0.001f)
#define TBASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))
#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;
//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// 디버그 메세지( 오류가 예상되는 곳에서 사용함 ) : 파일명과 라인 및 문자열이 출력되며 강제 종료됨.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define	DEBUGMSG(lpText)															\
	{																						\
		TCHAR szBuffer[256];															\
		_stprintf_s(szBuffer, _T("[File: %s][Line: %d]\n[Note : %s]"), __FILE__, __LINE__, lpText);	\
		MessageBox(NULL, szBuffer, _T("ERROR"), MB_ICONERROR);									\
		_ASSERT(1);																			\
		_asm {																				\
		int 3																			\
		}																					\
	}
#else
#define DEBUGMSG(lpText)
#endif

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern RECT			g_rtWindow;
extern int			g_iFPS;// 1초당 프레임 개수
extern float		g_fSecPerFrame; // 1프레임의 경과시간
extern float		g_fAccumulation;// 실행된 경과 시간