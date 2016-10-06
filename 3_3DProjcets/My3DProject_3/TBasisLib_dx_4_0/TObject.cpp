#include "TObject.h"
#include "TObjMgr.h"

void TObject::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	for (int iObj = 0; iObj < m_pModelList.size(); iObj++)
	{
		 TModel* const pObj = I_ObjMgr.GetPtr(m_pModelList[iObj]);
		_ASSERT(pObj);
		pObj->SetMatrix(pWorld, pView, pProj);
	}
}
bool TObject::Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString )
{
	for (int iObj = 0; iObj < m_pModelList.size(); iObj++)
	{
		TModel* const pObj = I_ObjMgr.GetPtr(m_pModelList[iObj]);
		_ASSERT(pObj);
		pObj->Create(pDevice, pLoadShaderFile,pLoadTextureString);
	}
	return true;
}
bool TObject::Init()
{
	return true;
}
bool TObject::Load(ID3D11Device* pd3dDevice,
	const TCHAR* strFileName,
	const TCHAR* strShaderName, bool bThread)
{
	int iIndex = I_ObjMgr.Load(pd3dDevice, strFileName, strShaderName, bThread);
	if (iIndex < 0)
	{
		I_ObjMgr.Delete(iIndex);
		return -1;
	}
	m_pModelList.push_back(iIndex);
	return true;
}
bool TObject::Frame()
{
	for (int iObj = 0; iObj < m_pModelList.size(); iObj++)
	{
		TModel* const pObj = I_ObjMgr.GetPtr(m_pModelList[iObj]);
		_ASSERT(pObj);
		pObj->Frame();
	}
	return true;
}
bool TObject::Render(ID3D11DeviceContext*    pContext)
{
	for (int iObj = 0; iObj < m_pModelList.size(); iObj++)
	{
		TModel* const pModel = I_ObjMgr.GetPtr(m_pModelList[iObj]);
		_ASSERT(pModel);
		pModel->Render(pContext);	
	}
	return true;
}
bool TObject::Release()
{
	m_pModelList.clear();
	return true;
}
bool TObject::ResetResource()
{
	return true;
}
void TObject::SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd)
{	
	for (int iObj = 0; iObj < m_pModelList.size(); iObj++)
	{
		TModel* const pObj = I_ObjMgr.GetPtr(m_pModelList[iObj]);
		_ASSERT(pObj);
		if (!_tcsicmp(pObj->m_szName.c_str(), pStrModel))
		{
			pObj->m_iStartFrame = dwStrat;
			pObj->m_iLastFrame = dwEnd;
			pObj->m_fElapseTime = (float)dwStrat;
		}
	}	
}
TModel* const TObject::SetActionFrame(DWORD dwIndex, DWORD dwStrat, DWORD dwEnd)
{
	TModel* const pObj = I_ObjMgr.GetPtr(m_pModelList[dwIndex]);
	_ASSERT(pObj);
	if (pObj)
	{
		pObj->m_iStartFrame = dwStrat;
		pObj->m_iLastFrame = dwEnd;
		pObj->m_fElapseTime = (float)dwStrat;
	}
	return pObj;
}
TObject::TObject(void)
{
}
TObject::~TObject(void)
{
}