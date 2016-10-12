
// MapTool.h : MapTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "TBasisLib_0.h"

#include "TCamera.h"
#include "GMapTileRender.h"
#include "THeightMap.h"
#include "TMap.h"

// CMapToolApp:
// �� Ŭ������ ������ ���ؼ��� MapTool.cpp�� �����Ͻʽÿ�.
//

class CMapToolApp : public CWinAppEx, public TBasisLib_0
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
	THeightMap		m_Map;
	
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


public:
	CMapToolApp();


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

extern CMapToolApp theApp;
