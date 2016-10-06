#pragma once
#include "GMap.h"
class GHeightMap : public GMap
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
	GHeightMap();
	virtual ~GHeightMap();
};

