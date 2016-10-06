#pragma once
#pragma warning( disable:4005)
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <d3dx11.h>
//#include <dxerr.h> 
//#include <wincodec.h>
#include <string.h>
#include "TBasisStd.h"

namespace TBASIS_BASE { 
class TDirectWrite: public TSingleton < TDirectWrite >
{
private:
	friend class TSingleton<TDirectWrite>;
public:
	float					m_fDPIScaleX;
    float					m_fDPIScaleY;	
	FLOAT					m_fdpiX;
    FLOAT					m_fdpiY;
	HWND					m_hWnd;
	ID2D1RenderTarget*		m_pRT;
    ID2D1Factory*			m_pD2DFactory;
    ID2D1SolidColorBrush*	m_pBlackBrush;
    IDWriteFactory*			m_pDWriteFactory;
    IDWriteTextFormat*		m_pTextFormat;   
public:
	// �ʱ�ȭ
	bool				Set(HWND hWnd, IDXGISurface1*	m_pSurface);
    bool				Init();
	// ������
	bool				Begin();
	HRESULT				DrawText(RECT rc, TCHAR* pText, D2D1::ColorF Color= D2D1::ColorF( 1,0,0,1 ) );
	bool				End();
	HRESULT				Draw( TCHAR* pText, RECT rc,   D2D1::ColorF Color= D2D1::ColorF( 1,0,0,1 ) );
	RECT				GetFontRectangle(LPCWSTR text);
	// ��ü �Ҹ�
	bool				Release();	
	// ��ü ���� �� �Ҹ�
    HRESULT				CreateDeviceIndependentResources();
    void				DiscardDeviceIndependentResources();    
	HRESULT				CreateDeviceResources(IDXGISurface1*	m_pSurface);
    void				DiscardDeviceResources();
	// ȭ�� ������ ����
	void				OnResize(UINT width, UINT height, IDXGISurface1*pSurface   );
public:
    TDirectWrite();
    ~TDirectWrite();
    
};
#define I_Write TDirectWrite::GetInstance()
}