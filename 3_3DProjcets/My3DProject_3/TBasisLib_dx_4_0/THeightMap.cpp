#include "THeightMap.h"
float THeightMap::GetHeight(float fPosX, float fPosZ)
{
	// fPosX/fPosZ�� ��ġ�� �ش��ϴ� ���̸ʼ��� ã�´�.
	// m_iNumCols��m_iNumRows�� ����/������ ���� ũ�Ⱚ��.
	float fCellX = ((float)(m_iNumCols - 1) / 2.0f)*m_fSellDistance + fPosX;
	float fCellZ = ((float)(m_iNumRows - 1) / 2.0f)*m_fSellDistance - fPosZ;

	// ���� ũ��� ������ 1������ ������ �ٲپ� ���̸� �迭�� �����Ѵ�.
	fCellX /= (float)m_fSellDistance;
	fCellZ /= (float)m_fSellDistance;

	// fCellX, fCellZ ������ �۰ų� ���� �ִ� ����( �Ҽ��κ��� �߶󳽴�.)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// ���̸� ������ ����� ������ �ʱ�ȭ �Ѵ�.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2)< fVertexCol)	fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2)< fVertexRow)	fVertexRow = (float)(m_iNumRows - 2);

	// ���� ���� �÷��� �����ϴ� 4�� ������ ���̰��� ã�´�. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A������ ��ġ���� ������ ��(������)�� ����Ѵ�.
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// �����۾��� ���� ���� ���ս��� ã�´�. 
	float fHeight = 0.0f;
	// �����̽��� �������� �����Ѵ�.
	if (fDeltaZ < (1.0f - fDeltaX))  //ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C	
						  // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸX�� ���� ���� �������� ã�´�.		
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// �Ʒ����̽��� �������� �����Ѵ�.
	else // DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
						  // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸZ�� ���� ���� �������� ã�´�.		
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}
float THeightMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}
float THeightMap::GetHeightmap(int row, int col)
{
	return m_fHeightList[row * m_iNumRows + col];
}
bool THeightMap::CreateHeightMap(TCHAR* strHeightMapTex)
{
	HRESULT hr;
	D3DX11_IMAGE_INFO ImageInfo;
	/*if( FAILED( D3DX11GetImageInfoFromFile( strHeightMapTex,NULL, &ImageInfo,&hr ) ) )
	{
	return false;
	}	*/

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.Usage = D3D11_USAGE_STAGING;
	loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;
	loadInfo.pSrcInfo = &ImageInfo;

	ID3D11Resource *pTexture = NULL;
	if (FAILED(hr = D3DX11CreateTextureFromFile(m_pd3dDevice, strHeightMapTex, &loadInfo, NULL, &pTexture, NULL)))
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

	if (pTexture2D )
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
					m_fHeightList[row * desc.Width + col] = uRed;	/// DWORD�̹Ƿ� pitch/4	
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
float THeightMap::GetHeightOfVertex(UINT Index)
{
	return m_fHeightList[Index] * m_MapDesc.fScaleHeight;
};
D3DXVECTOR4 THeightMap::GetColorOfVertex(UINT Index)
{
	return D3DXVECTOR4(1, 1, 1, 1.0f);
};
THeightMap::THeightMap(void)
{
}

THeightMap::~THeightMap(void)
{
}
