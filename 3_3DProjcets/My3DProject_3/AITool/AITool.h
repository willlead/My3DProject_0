
// AITool.h : AITool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "TBasisLib_0.h"

#include "TCamera.h"
#include "GHeightMap.h"
#include "GMapTileRender.h"
#include "TMiniMap.h"


// CAIToolApp:
// �� Ŭ������ ������ ���ؼ��� AITool.cpp�� �����Ͻʽÿ�.
//

class CAIToolApp : public CWinAppEx, public TBasisLib_0
{
public:
	shared_ptr<TCamera >	m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// ����Ʈ��
	//--------------------------------------------------------------------------------------
	GMapTileRender	m_QuadTree;
	//--------------------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------------------
	GHeightMap		m_Map;
	//--------------------------------------------------------------------------------------
	// �̴ϸ�
	//--------------------------------------------------------------------------------------
	TMiniMap		m_MiniMap;
	int							m_iDrawDepth;
	ComPtr<ID3D11PixelShader>   m_pPixelShader;// ���ν��� ���� �Ƚ����̴�
	ComPtr<ID3D11ShaderResourceView> m_pTexture[4];
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();

	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT			CreateResource();
	HRESULT			DeleteResource();
	bool			DrawDebug();
	//--------------------------------------------------------------------------------------
	// ����� �뵵
	//--------------------------------------------------------------------------------------
	TShape*			m_pLine;
	bool			m_bDebugRender;
	bool			DrawQuadLine(GNode* pNode);
	void			DrawMiniMap();

public:
	CAIToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
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
