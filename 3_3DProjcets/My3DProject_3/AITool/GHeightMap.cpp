#include "stdafx.h"
#include "GHeightMap.h"


bool GHeightMap::CreateHeightMap(TCHAR * strHeightMap)
{
	HRESULT hr;
	D3DX11_IMAGE_INFO ImageInfo;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.Usage = D3D11_USAGE_STAGING;
	loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;
	loadInfo.pSrcInfo = &ImageInfo;

	ID3D11Resource *pTexture = NULL;
	if (FAILED(hr = D3DX11CreateTextureFromFile(m_pd3dDevice, strHeightMap,
		&loadInfo, NULL, &pTexture, NULL)))
	{
		return false;
	}
	ID3D11Texture2D *pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}
	SAFE_RELEASE(pTexture);

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	m_fHeightList.resize(desc.Height*desc.Width);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					m_fHeightList[row * desc.Width + col] = uRed;	/// DWORDÀÌ¹Ç·Î pitch/4	
				}
			}
			m_pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	m_iNumRows = desc.Height;
	m_iNumCols = desc.Width;

	SAFE_RELEASE(pTexture2D);
	return true;
}

float GHeightMap::GetHeightOfVertex(UINT Index)
{
	return m_fHeightList[Index] * m_MapDesc.fScaleHeight;
}

D3DXVECTOR4 GHeightMap::GetColorOfVertex(UINT Index)
{
	return D3DXVECTOR4(1, 1, 1, 1.0f);
}

float GHeightMap::GetHeight(float fPosX, float fPosZ)
{
	float fCellX = ((float)(m_iNumCols - 1) / 2.0f)*m_fSellDistance + fPosX;
	float fCellZ = ((float)(m_iNumRows - 1) / 2.0f)*m_fSellDistance - fPosZ;

	fCellX /= (float)m_fSellDistance;
	fCellZ /= (float)m_fSellDistance;

	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	if (fVertexCol < 0.0f) fVertexCol = 0.0f;
	if (fVertexRow < 0.0f) fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol) fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertexRow) fVertexRow = (float)(m_iNumRows - 2);

	float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightmap((int)fVertexRow, (int)fVertexCol +1);
	float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;

	float fHeight = 0.0f;

	if (fDeltaZ < (1.0f - fDeltaX))
	{
		float uy = B - A;
		float vy = C - A;

		fHeight = A + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
			
	}

	return fHeight;
}

float GHeightMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - ( fStart*fTangent) + (fEnd*fTangent);
}

float GHeightMap::GetHeightmap(int row, int col)
{
	return 0.0f;
}

GHeightMap::GHeightMap()
{
}


GHeightMap::~GHeightMap()
{
}
