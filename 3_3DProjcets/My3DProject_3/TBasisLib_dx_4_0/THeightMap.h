#pragma once
#include "TMap.h"

class THeightMap : public TMap
{
public:
	vector<float>		m_fHeightList;
public:
	bool		CreateHeightMap(TCHAR* strHeightMap);
	float		GetHeightOfVertex(UINT Index);
	D3DXVECTOR4 GetColorOfVertex(UINT Index);
	float		GetHeight(float fPosX, float fPosZ);
	float		Lerp(float fStart, float fEnd, float fTangent);
	float		GetHeightmap(int row, int col);
public:
	THeightMap(void);
	virtual ~THeightMap(void);
};
