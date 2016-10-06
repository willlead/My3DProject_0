
// AITool.h : AITool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "TBasisLib_0.h"

#include "TCamera.h"
#include "GHeightMap.h"
#include "GMapTileRender.h"
#include "TMiniMap.h"


// CAIToolApp:
// 이 클래스의 구현에 대해서는 AITool.cpp을 참조하십시오.
//

class CAIToolApp : public CWinAppEx, public TBasisLib_0
{
public:
	shared_ptr<TCamera >	m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// 쿼드트리
	//--------------------------------------------------------------------------------------
	GMapTileRender	m_QuadTree;
	//--------------------------------------------------------------------------------------
	// 지형
	//--------------------------------------------------------------------------------------
	GHeightMap		m_Map;
	//--------------------------------------------------------------------------------------
	// 미니맵
	//--------------------------------------------------------------------------------------
	TMiniMap		m_MiniMap;
	int							m_iDrawDepth;
	ComPtr<ID3D11PixelShader>   m_pPixelShader;// 프로스텀 전용 픽쉘쉐이더
	ComPtr<ID3D11ShaderResourceView> m_pTexture[4];
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();

	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT			CreateResource();
	HRESULT			DeleteResource();
	bool			DrawDebug();
	//--------------------------------------------------------------------------------------
	// 디버깅 용도
	//--------------------------------------------------------------------------------------
	TShape*			m_pLine;
	bool			m_bDebugRender;
	bool			DrawQuadLine(GNode* pNode);
	void			DrawMiniMap();

public:
	CAIToolApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CAIToolApp theApp;
