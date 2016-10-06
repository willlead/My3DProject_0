#include "TSkinObj.h"
#include "TTextureMgr.h"
#include "TTimer.h"
void TSkinObj::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenterPos.x = pWorld->_41;
		m_vCenterPos.y = pWorld->_42;
		m_vCenterPos.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
}
bool TSkinObj::Convert(const TCHAR* strPathName )
{
	return true;
}
bool TSkinObj::Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	if (szLoadName)
	{
		_tsplitpath(szLoadName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
	}
	memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
	_stprintf_s(szFileName, _T("%s%s"), Drive, Dir);

	// ������������������������������������������������������������������
	// 
	// ������������������������������������������������������������������
	FILE	*fp;
	_tfopen_s(&fp, szLoadName, _T("rb"));
	_ASSERT(fp != NULL);

	struct tm newtime;
	fread(&newtime, sizeof(tm), 1, fp);
	T_STR today = _tasctime(&newtime);	// The date string has a \n appended.
	today[today.size() - 1] = 0;
	fread(&m_Scene, sizeof(TScene), 1, fp);

	TCHAR szBuffer[128] = { 0, };
	size_t convertedChars = 0;

	for (int iter = 0; iter < m_Scene.iNumMesh; iter++)
	{
		shared_ptr<tSkmMesh> pMesh = make_shared<tSkmMesh>();

		TCHAR szName[256] = { 0,0 };
		TCHAR szParentName[256] = { 0,0 };

		// ��� �̸�
		int iLength = 0;
		fread(&iLength, sizeof(int), 1, fp);
		fread(szName, sizeof(TCHAR)*iLength, 1, fp);

		pMesh->m_strNodeName = szName;
		// ��� �θ� �̸�
		fread(&iLength, sizeof(int), 1, fp);
		if (iLength > 0)
		{
			fread(szParentName, sizeof(TCHAR)*iLength, 1, fp);
			pMesh->m_strParentName = szParentName;
		}
		// ���� ���
		fread(&pMesh->m_matWorld, sizeof(D3DXMATRIX), 1, fp);
		//m_matWorld = pMesh->m_matWorld;

		fread(&pMesh->m_iNumMtrl, sizeof(int), 1, fp);
		if (pMesh->m_iNumMtrl <= 0)
		{
			LoadMesh(fp, pMesh.get(), szFileName);
			//SAFE_NEW_ARRAY(pMesh->m_pDrawVertex, PNCT5_VERTEX, pMesh->m_iNumVertexs );
		}
		else
		{
			for (int iter = 0; iter < pMesh->m_iNumMtrl; iter++)
			{
				shared_ptr<tSkmMesh>  pSubMesh = make_shared<tSkmMesh>();
				if (LoadMesh(fp, pSubMesh.get(), szFileName))
				{
					SAFE_NEW_ARRAY(pSubMesh->m_pDrawVertex, PNCT5_VERTEX, pSubMesh->m_iNumVertexs);
					pMesh->m_pSubMesh.push_back(pSubMesh);
				}
			}
		}
		m_pMesh.push_back(pMesh);
	}

	// �巹�� ������ �� �����е� ��� �ε�
	int iNumMatrix;
	fread(&iNumMatrix, sizeof(int), 1, fp);
	m_matBipedList.resize(iNumMatrix);
	for (int iMat = 0; iMat < iNumMatrix; iMat++)
	{
		fread(&m_matBipedList[iMat], sizeof(D3DXMATRIX), 1, fp);
	}

	fclose(fp);
	
	m_Object.m_iNumVertex = m_iMaxVertex;
	m_Object.m_iNumIndex = m_iMaxIndex;
	m_Object.m_iVertexSize = sizeof(PNCT5_VERTEX);

	if (!bThread && !Create(pDevice, pLoadShaderFile))
	{
		return false;
	}
	return true;
}
bool TSkinObj::LoadMesh(FILE	*fp, tSkmMesh* pMesh, const TCHAR* szLoadFileName)
{
	char szBuffer[128] = { 0, };
	size_t convertedChars = 0;
	
	fread(&pMesh->m_iNumVertexs, sizeof(int), 1, fp);
	fread(&pMesh->m_iNumFace, sizeof(int), 1, fp);
	fread(&pMesh->m_iNumTex, sizeof(int), 1, fp);

	pMesh->m_VertexArray.resize(pMesh->m_iNumVertexs);
	pMesh->m_IndexArray.resize(pMesh->m_iNumFace * 3);

	fread(&pMesh->m_VertexArray.at(0), sizeof(PNCT5_VERTEX) * pMesh->m_iNumVertexs, 1, fp);
	fread(&pMesh->m_IndexArray.at(0), sizeof(unsigned int) * pMesh->m_iNumFace * 3, 1, fp);

	m_iMaxVertex += pMesh->m_VertexArray.size();
	m_iMaxIndex += pMesh->m_IndexArray.size();
	
	for (int itex = 0; itex < pMesh->m_iNumTex; itex++)
	{
		int iLen = _tcslen(pMesh->m_szTexName[itex]);
		fread(&iLen, sizeof(int), 1, fp);
		fread(&pMesh->m_iTexType[itex], sizeof(int), 1, fp);
		ZeroMemory(pMesh->m_szTexName[itex], sizeof(TCHAR) * 30);
		fread(pMesh->m_szTexName[itex], sizeof(TCHAR) * iLen, 1, fp);

		TCHAR szFileName[MAX_PATH];
		TCHAR Drive[MAX_PATH];
		TCHAR Dir[MAX_PATH];
		TCHAR FName[MAX_PATH];
		TCHAR Ext[MAX_PATH];
		if (szLoadFileName)
		{
			_tsplitpath(szLoadFileName, Drive, Dir, FName, Ext);
			Ext[4] = 0;
			memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		}
		T_STR loadFile = Drive;
		loadFile += Dir;
		loadFile += pMesh->m_szTexName[itex];
		// ��ǻ�� �ؽ���
		if (pMesh->m_iTexType[itex] == ID_TBASIS_DI)
		{
			pMesh->m_iDiffuseTex = I_Texture.Add(g_pd3dDevice, loadFile.c_str());
		}
	}
	return true;
}
//void	TSkinObj::AnimationMatrix(TMesh<PNCT5_VERTEX>* pMesh )
//{	
//	PNCT5_VERTEX*	 pDrawVertex = pMesh->m_pDrawVertex;
//	D3DXVECTOR3 vPos;	
//			
//	D3DXMATRIX		matRotate, matInverse;
//	D3DXVECTOR3		vJointPos, vWeightPos;
//
//	TAniMatrix*	 pAnimMatrix = I_AniMatrixMgr.GetPtr(m_iMatrixIndex);
//
//	int iWeightCnt = 0;
//	for( int iVertex = 0; iVertex < pMesh->m_iNumVertexs; iVertex++ )
//	{			
//		int iBipCount = pMesh->m_VertexArray[iVertex].w2[3];				
//		// �ִ� �����е� �ε��� �������� ũ�� ��� ������.
//		if( iBipCount > 7 ) continue;
//
//		pDrawVertex[iVertex]    = pMesh->m_VertexArray[iVertex];
//		pDrawVertex[iVertex].p  = D3DXVECTOR3( 0.0f,0.0f,0.0f );			
//	
//		D3DXMATRIX matWorld;
//		int iBipedIndex;
//		for( DWORD iIndex = 0; iIndex < iBipCount; iIndex++ )
//		{				
//			if(iIndex < 4)
//			{
//				iBipedIndex = pMesh->m_VertexArray[iVertex].i1[iIndex];
//				vPos = pMesh->m_WeightArray[iWeightCnt++];				
//				D3DXVec3TransformCoord( &vPos,	&vPos, &pAnimMatrix->m_BoneMesh.m_BipedMeshList[iBipedIndex].m_matWorld );	
//				//D3DXVec3TransformCoord( &vPos,	&vPos, &pAnimMatrix->m_BoneMesh.m_pMatrix[iBipedIndex] );
//				pDrawVertex[iVertex].p +=	vPos * pMesh->m_VertexArray[iVertex].w1[iIndex];
//			}
//			else
//			{
//				iBipedIndex = pMesh->m_VertexArray[iVertex].i2[iIndex-4];
//				vPos = pMesh->m_WeightArray[iWeightCnt++];		
//				D3DXVec3TransformCoord( &vPos,	&vPos, 	&pAnimMatrix->m_BoneMesh.m_BipedMeshList[iBipedIndex].m_matWorld);	
//				//D3DXVec3TransformCoord( &vPos,	&vPos, &pAnimMatrix->m_BoneMesh.m_pMatrix[iBipedIndex] );
//				pDrawVertex[iVertex].p +=  vPos * pMesh->m_VertexArray[iVertex].w2[iIndex-4];						
//			}
//		}	
//	}		
//}
bool TSkinObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}
bool TSkinObj::Release()
{
	return true;
}
HRESULT TSkinObj::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 96, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_Object.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice,
		m_Object.g_pVSBlob.Get()->GetBufferSize(),
		m_Object.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
bool TSkinObj::UpdateBuffer()
{
	if (!CombineBuffer(m_Object.g_pVertexBuffer.Get(), m_Object.g_pIndexBuffer.Get()))
	{
		return false;
	}
	return true;
}
bool TSkinObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
{
	size_t dstOffset = 0;
	size_t vbOffset = 0;
	size_t dstibOffset = 0;
	size_t ibOffset = 0;
	{
		for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
		{
			auto pMesh = m_pMesh[dwObject].get();
			if (pMesh->m_pSubMesh.size() > 0)
			{
				for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
				{
					auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
					if (pSubMesh->m_VertexArray.size() < 3) continue;

					pSubMesh->m_Object.m_iNumVertex = pSubMesh->m_VertexArray.size();
					pSubMesh->m_Object.m_iVertexSize = m_Object.m_iVertexSize;

					pSubMesh->m_Object.m_BoxVB.left = dstOffset;
					pSubMesh->m_Object.m_BoxVB.right = dstOffset + pSubMesh->m_Object.m_iNumVertex*m_Object.m_iVertexSize;
					pSubMesh->m_Object.m_BoxVB.top = 0; pSubMesh->m_Object.m_BoxVB.bottom = 1;
					pSubMesh->m_Object.m_BoxVB.front = 0; pSubMesh->m_Object.m_BoxVB.back = 1;

					g_pImmediateContext->UpdateSubresource(
						pVB, 0,
						&pSubMesh->m_Object.m_BoxVB,
						(uint8_t*)&pSubMesh->m_VertexArray.at(0),
						0, 0);


					/*g_pImmediateContext->CopySubresourceRegion(
					m_Object.g_pVertexBuffer.Get(), 0, iBeginPos, 0, 0,
					(void*)&pSubMesh->m_VertexArray.at(0),
					0, &pSubMesh->m_Object.m_BoxVB);*/

					pSubMesh->m_Object.m_iBeginVB = vbOffset;
					vbOffset += pSubMesh->m_Object.m_iNumVertex;
					dstOffset = pSubMesh->m_Object.m_BoxVB.right;

					pSubMesh->m_Object.m_iNumIndex = pSubMesh->m_IndexArray.size();
					pSubMesh->m_Object.m_BoxIB.left = dstibOffset;
					pSubMesh->m_Object.m_BoxIB.right = dstibOffset + pSubMesh->m_Object.m_iNumIndex*sizeof(DWORD);
					pSubMesh->m_Object.m_BoxIB.top = 0;		pSubMesh->m_Object.m_BoxIB.bottom = 1;
					pSubMesh->m_Object.m_BoxIB.front = 0;	pSubMesh->m_Object.m_BoxIB.back = 1;

					g_pImmediateContext->UpdateSubresource(pIB, 0,
						&pSubMesh->m_Object.m_BoxIB, (void*)&pSubMesh->m_IndexArray.at(0), 0, 0);

					pSubMesh->m_Object.m_iBeginIB = ibOffset;
					ibOffset += pSubMesh->m_Object.m_iNumIndex;
					dstibOffset = pSubMesh->m_Object.m_BoxIB.right;

					//texture
					if (pSubMesh->m_iDiffuseTex < 0) continue;
					pSubMesh->m_Object.g_pTextureSRV = I_Texture.GetPtr(pSubMesh->m_iDiffuseTex)->m_pTextureRV;
				}
			}
			else
			{
				if (pMesh->m_VertexArray.size() < 3) continue;

				pMesh->m_Object.m_iNumVertex = pMesh->m_VertexArray.size();
				pMesh->m_Object.m_iVertexSize = m_Object.m_iVertexSize;

				pMesh->m_Object.m_BoxVB.left = dstOffset;
				pMesh->m_Object.m_BoxVB.right = dstOffset + pMesh->m_Object.m_iNumVertex*m_Object.m_iVertexSize;
				pMesh->m_Object.m_BoxVB.top = 0; pMesh->m_Object.m_BoxVB.bottom = 1;
				pMesh->m_Object.m_BoxVB.front = 0; pMesh->m_Object.m_BoxVB.back = 1;

				g_pImmediateContext->UpdateSubresource(pVB, 0,	&pMesh->m_Object.m_BoxVB, (void*)&pMesh->m_VertexArray.at(0), 0, 0);

				pMesh->m_Object.m_iBeginVB = vbOffset;
				vbOffset += pMesh->m_Object.m_iNumVertex;
				dstOffset = pMesh->m_Object.m_BoxVB.right;

				pMesh->m_Object.m_iNumIndex = pMesh->m_IndexArray.size();
				pMesh->m_Object.m_BoxIB.left = dstibOffset;
				pMesh->m_Object.m_BoxIB.right = dstibOffset + pMesh->m_Object.m_iNumIndex*sizeof(DWORD);
				pMesh->m_Object.m_BoxIB.top = 0; pMesh->m_Object.m_BoxIB.bottom = 1;
				pMesh->m_Object.m_BoxIB.front = 0; pMesh->m_Object.m_BoxIB.back = 1;

				g_pImmediateContext->UpdateSubresource(pIB, 0,	&pMesh->m_Object.m_BoxIB, (void*)&pMesh->m_IndexArray.at(0), 0, 0);
				pMesh->m_Object.m_iBeginIB = ibOffset;
				ibOffset += pMesh->m_Object.m_iNumIndex;
				dstibOffset = pMesh->m_Object.m_BoxIB.right;			

				//texture
				if (pMesh->m_iDiffuseTex <= 0) continue;
				pMesh->m_Object.g_pTextureSRV = I_Texture.GetPtr(pMesh->m_iDiffuseTex)->m_pTextureRV;
			}
		}
	}
	return true;
}
bool TSkinObj::Draw(ID3D11DeviceContext*    pContext, TModel* pParent)
{
	//CStopwatch stopwatch;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		pParent->SetMatrix(&pParent->m_matWorld, &pParent->m_matView, &pParent->m_matProj);
		pContext->UpdateSubresource(pParent->m_Object.g_pConstantBuffer.Get(), 0, NULL, &pParent->m_cbData, 0, 0);
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_VertexArray.size() < 3) continue;
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_Object.g_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_Object.m_iNumIndex,
					pSubMesh->m_Object.m_iBeginIB,
					pSubMesh->m_Object.m_iBeginVB);
			}
		}
		else
		{
			if (pMesh->m_VertexArray.size() < 3) continue;
			pContext->PSSetShaderResources(0, 1, pMesh->m_Object.g_pTextureSRV.GetAddressOf());
			pContext->DrawIndexed(pMesh->m_Object.m_iNumIndex,
				pMesh->m_Object.m_iBeginIB,
				pMesh->m_Object.m_iBeginVB);
		}
	}
	//stopwatch.Output(L"\nTTbsObj::Draw");
	return true;
}
TSkinObj::TSkinObj(void)
{
	memset( &m_VersionMark, 0, sizeof(VersionMark));
	m_iMaxWeight		= 0;
	m_Scene.iBindPose	= 0;
}

TSkinObj::~TSkinObj(void)
{
}
