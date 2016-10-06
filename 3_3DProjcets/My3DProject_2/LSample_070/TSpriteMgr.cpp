#include "TSpriteMgr.h"

void TSpriteMgr::SetDevice(ID3D11Device* pDevice)
{
	m_pd3dDevice = pDevice;
}
INT  TSpriteMgr::Add(TCHAR* pTextureFileName,
	TCHAR* pShaderFileName,
	ID3D11BlendState* pBlendState)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pTextureFileName)
	{
		_tsplitpath(pTextureFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR)* MAX_PATH);
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TSprite *pPoint = (TSprite*)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	TSprite *pPoint = NULL;
	SAFE_NEW(pPoint, TSprite);
	assert(pPoint);
	pPoint->m_szPath = Dir;
	if (FAILED(pPoint->Load(m_pd3dDevice, pShaderFileName, pTextureFileName, pBlendState)))
	{
		SAFE_DEL(pPoint);
		return 0;
	}
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	pPoint->m_iIndex = m_iCurIndex;
	return m_iCurIndex;
}
TSprite* TSpriteMgr::GetPtr(INT iIndex)
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end())	return NULL;
	TSprite *pPoint = (*itor).second;
	return pPoint;
}
bool TSpriteMgr::Release()
{
	TSprite* pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TSprite*)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
	return true;
}
bool TSpriteMgr::Delete(INT iDelete) 
{
	return true;
}



TSpriteMgr::TSpriteMgr()
{
	m_pd3dDevice = NULL;
	m_iCurIndex = 0;
	TMap.clear();
}


TSpriteMgr::~TSpriteMgr()
{
	TSprite* pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TSprite*)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;

}
