#pragma once
#include "GObjMgr.h"
#include "GAnimation.h"

class GObject : public GModel
{
public:	
	vector<shared_ptr<TObjWM>>			m_pModelList;
	vector<shared_ptr<GObject>>			m_pSubObjList;	
	bool			m_bDefferedMultiThread;
	GAnimation		m_Animation;
public:
	void		Add(ID3D11Device* pDevice, shared_ptr<GObject> pObj);
	bool		Init();
	bool		Load(ID3D11Device* pd3dDevice,	const TCHAR* strFileName,	const TCHAR* strShaderName,	bool bThread = false);
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Draw(ID3D11DeviceContext* pContext, TObjWM* pUint, bool bCommand);
	virtual bool		PreDraw(ID3D11DeviceContext*    pContext, TObjWM* pUnit, bool bCommand);
	virtual bool		PostDraw(ID3D11DeviceContext*    pContext, TObjWM* pUnit, bool bCommand);
	bool		Release();
	bool		ResetResource();	
	void		SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd);
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	// 1���� �޽��� �ټ��� ������Ʈ�� ����� ��쿡 ���ȴ�.
	bool		ObjectFrame();
	bool		ObjectRender(ID3D11DeviceContext*    pContext, bool bCommand=false);
	bool		Set(ID3D11Device* pd3dDevice, int iIndex);
	void		SetConstantBuffers(ID3D11DeviceContext* pContext, TObjWM* pUint, int iMesh);
	bool		SetCommandRender(ID3D11DeviceContext* pContext);
	bool		CommandRender(ID3D11DeviceContext* pContext);
	bool		CommandDraw(ID3D11DeviceContext* pContext, TObjWM* pUnit);
	virtual HRESULT	SetCommmandList(ID3D11DeviceContext* pContext, GMesh* pSubMesh, bool bSave = false);
	virtual void	ExecuteCommandList(ID3D11DeviceContext* pContext, GMesh* pSubMesh, bool bClear = true);
public:
	GObject(void);
	GObject(int iIndex);
	explicit GObject(std::shared_ptr<GObject> myStuff)
	{
		m_pSubObjList.push_back(move(myStuff));
	}
	explicit GObject(GObject* const myStuff)
	{
		shared_ptr<GObject> pObj(myStuff);
		m_pSubObjList.push_back(move(pObj));
	}
	GObject(GObject &obj);
	virtual ~GObject(void);
};
