#pragma once
#include "TObjMgr.h"

class TObject : public TModel
{
public:
	vector<int>			m_pModelList;
public:
	bool		Init();
	bool		Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString=0);
	bool		Load(ID3D11Device* pd3dDevice,	const TCHAR* strFileName,	const TCHAR* strShaderName,	bool bThread = false);
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Release();
	bool		ResetResource();	
	void		SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd);
	TModel* const SetActionFrame(DWORD dwIndex, DWORD dwStrat, DWORD dwEnd);
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
public:
	TObject(void);
	virtual ~TObject(void);
};
