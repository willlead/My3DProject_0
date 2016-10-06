#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TSprite.h"
class TSpriteMgr : public TSingleton < TSpriteMgr >
{
private:
	friend class TSingleton<TSpriteMgr>;
	ID3D11Device* m_pd3dDevice;
public:
	typedef map<INT, TSprite*>			TemplateMap;
	typedef TemplateMap::iterator		TemplateMapItor;
	TemplateMapItor						TItor;
	TemplateMap							TMap;
	INT									m_iCurIndex;
public:
	void SetDevice(ID3D11Device* pDevice);
	INT  Add(TCHAR* pTextureFileName,
		TCHAR* pShaderFileName,
		ID3D11BlendState* pBlendState = nullptr);
	TSprite* GetPtr(INT iIndex);
	bool Release();
	bool Delete(INT iDelete);
public:
	TSpriteMgr();
	virtual ~TSpriteMgr();
};
#define I_Sprite TSpriteMgr::GetInstance()

