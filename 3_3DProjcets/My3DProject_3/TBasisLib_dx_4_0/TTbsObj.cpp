#include "TTbsObj.h"
#include "TTimer.h"
void TTbsObj::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		/*m_vCenterPos.x = pWorld->_41;
		m_vCenterPos.y = pWorld->_42;
		m_vCenterPos.z = pWorld->_43;*/
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
bool TTbsObj::Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread )
{
	m_Object.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_Object.m_iIndexSize = sizeof(DWORD);

	if( LoadScene(szLoadName) )
	{
		if( !LoadMaterial() ) return false;	
		if( !LoadObject() ) return false;	
		
		CStopwatch stopwatch;
		if (!Convert(pd3dDevice))
		{
			return false;
		}
		m_Object.m_iNumVertex = m_iMaxVertex;
		m_Object.m_iNumIndex = m_iMaxIndex;

		if (!bThread && !Create(pd3dDevice, pLoadShaderFile))
		{
			m_Parser.CloseStream();
			return false;
		}
		m_Parser.CloseStream();
		stopwatch.Output(L"TTbsObj::Load After");
		return true;
	}	
	return false;
}
bool TTbsObj::LoadScene(const TCHAR* strFileName)
{
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];		
	_tsplitpath( strFileName, Drive, Dir, FName, Ext );			
	m_szDirName = Drive;
	m_szDirName+= Dir;	
	m_szName = FName;
	m_szName+= Ext;

	TCHAR strDate[256] = {0,};
	if( m_Parser.OpenStream( strFileName ) )
	{
		if( !m_Parser.GetDataFromFileLoop( _T("#TBF_FILE_EXPORT"), strDate, STRING_DATA ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NOT FIND! [ #SME_FILE_EXPORT ]."));		
		}		
	}
	else{
		return false;
	}
	if( !m_Parser.GetDataFromFileLoop( _T("#SCENE" )) )
	{
		return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #SCENE ]."));
	}
	
	// Scene
	_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
	_stscanf(m_Parser.m_pBuffer, _T("%d %d%d%d%d%d %d%d"),
											&m_Scene.iVersion,
											&m_Scene.iFirstFrame,
											&m_Scene.iLastFrame,
											&m_Scene.iFrameSpeed,
											&m_Scene.iTickPerFrame,
											&m_Scene.iNumMesh,										
											&m_Scene.iMaxWeight,
											&m_Scene.iBindPose);
	return true;
}
bool TTbsObj::LoadMaterial()
{
	// Material		
	int iNumRootMtl = 0;
	if( !m_Parser.GetDataFromFileLoop( _T("#ROOT_MATERIAL" ), &iNumRootMtl, INT_DATA ) )
	{
		return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #SCENE ]."));
	}
	m_Material.resize( iNumRootMtl );
	for( DWORD dwMtl = 0; dwMtl < iNumRootMtl; dwMtl++ )
	{	
		if( !m_Parser.GetDataFromFileLoop( _T("#MATERIAL")) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #MATERIAL ]."));
		}		

		LoadMaterialHeader(&m_Material[dwMtl]);

		if(m_Material[dwMtl].m_dwSubCount > 0)
		{
			// Read ��SubMaterial Counter
			m_Material[dwMtl].m_SubMaterial.resize(m_Material[dwMtl].m_dwSubCount);
			for( DWORD dwSub = 0; dwSub < m_Material[dwMtl].m_dwSubCount; dwSub++ )
			{
				// SubMaterial Index
				if( !m_Parser.GetDataFromFileLoop( _T("#SUBMATERIAL"), &m_Material[dwMtl].m_SubMaterial[dwSub].m_dwIndex, INT_DATA ) )
				{
					return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #SCENE ]."));
				}				
				LoadMaterialHeader(&m_Material[dwMtl].m_SubMaterial[dwSub]);
				LoadTexMap(&m_Material[dwMtl].m_SubMaterial[dwSub], m_szDirName);				
			}
		}
		else
		{		
			// �ؽ�ó �� �ε�
			LoadTexMap(&m_Material[dwMtl], m_szDirName);							
		}		
	}
	return true;
}
void TTbsObj::LoadMaterialHeader(TMtrl* pMtrl)
{
	TCHAR strName[256] = _T("");
	TCHAR strClassName[256] = _T("");
	_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
	_stscanf(m_Parser.m_pBuffer, _T("%s%s%d%d"),strName, 
												strClassName,
												&pMtrl->m_dwSubCount,
												&pMtrl->m_dwTexMapCount );
	m_Parser.ConvertString( pMtrl->m_strName	, strName );
	m_Parser.ConvertString( pMtrl->m_strClassName, strClassName );
}
void TTbsObj::LoadTexMap(TMtrl* pMtrl, T_STR szDirName )
{
	for( DWORD dwTex = 0; dwTex < pMtrl->m_dwTexMapCount; dwTex++ )
	{
		TCHAR strName[256] = _T("");
		TTextexMap TexMap;
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
		_stscanf(m_Parser.m_pBuffer, _T("%d%s"),&TexMap.m_dwType,strName );
		m_Parser.ConvertString( TexMap.m_strTextureName, strName );
		// �ؽ��� �Ŵ��� ��� �� �迭 ����.
		TexMap.m_dwIndex = I_Texture.Add(
			g_pd3dDevice, TexMap.m_strTextureName.c_str(), szDirName.c_str());
		
		pMtrl->m_TexMaps.push_back(TexMap);
	}
}
bool TTbsObj::LoadObject()
{
	// Object	
	DWORD dwCount;
	for( DWORD dwMesh = 0; dwMesh < m_Scene.iNumMesh; dwMesh++ )
	{	
		shared_ptr<tTbsMesh > pMesh = make_shared<tTbsMesh>();
		
		if( !m_Parser.GetDataFromFileLoop( _T("#OBJECTS"), &dwCount, INT_DATA  ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #OBJECTS ]."));
		}
	
		TCHAR strName[256] = _T("");		
		TCHAR strParentName[256] = _T("");
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf(m_Parser.m_pBuffer, _T(" %s %s"),	strName, strParentName );
		m_Parser.ConvertString( pMesh->m_strNodeName, strName );
		m_Parser.ConvertString( pMesh->m_strParentName, strParentName );
		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
		_stscanf(m_Parser.m_pBuffer, _T("%d%d%d%d%d%d %d"),
				&pMesh->m_ClassType,
				&pMesh->m_iNumFace, 
				&pMesh->m_iNumTrack[0],
				&pMesh->m_iNumTrack[1],
				&pMesh->m_iNumTrack[2],
				&pMesh->m_iNumTrack[3],
				&pMesh->m_iMtrlRef);
		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f%f%f"),	&pMesh->m_BBox.vMax.x, &pMesh->m_BBox.vMax.y, &pMesh->m_BBox.vMax.z,
															&pMesh->m_BBox.vMin.x, &pMesh->m_BBox.vMin.y, &pMesh->m_BBox.vMin.z );	

		if( !m_Parser.GetDataFromFileLoop( _T("#WORLD_MATRIX") ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #WORLD_MATRIX ]."));
		}

		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._11,&pMesh->m_matWorld._12,&pMesh->m_matWorld._13,&pMesh->m_matWorld._14 );		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._21,&pMesh->m_matWorld._22,&pMesh->m_matWorld._23,&pMesh->m_matWorld._24);			
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._31,&pMesh->m_matWorld._32,&pMesh->m_matWorld._33,&pMesh->m_matWorld._34);			
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._41,&pMesh->m_matWorld._42,&pMesh->m_matWorld._43,&pMesh->m_matWorld._44 );

		if( !m_Parser.GetDataFromFileLoop( _T("#TRAINGLES") ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #TRAINGLES ]."));
		}			
		
		D3DXMatrixInverse( &pMesh->m_matInverse, NULL, &pMesh->m_matWorld );

		// ����޽��� ���̽��� �����ϸ�
		if( pMesh->m_iMtrlRef >= 0 && m_Material[pMesh->m_iMtrlRef].m_dwSubCount > 0)
		{
			for( int iMtrl = 0; iMtrl < m_Material[pMesh->m_iMtrlRef].m_dwSubCount; iMtrl++ )
			{
				auto pSubMesh = make_shared<tTbsMesh>();			

				pSubMesh->m_iNumFace	= LoadVertexIndex(pSubMesh.get());
				pSubMesh->m_iMtrlRef	= iMtrl;
			
				if (pSubMesh->m_iNumFace <= 0)
				{
					continue;
				}
				// ���������Ͱ� ����ִ� ���� ������Ʈ ����( ���ó�� ���� ������͸����� ���ܵȴ�. )				
				pMesh->m_pSubMesh.push_back( pSubMesh );				
			}
		}
		else
		{
			// ���� �� �ε��� ������ �ε�
			LoadVertexIndex(pMesh.get());			
		}

		// ���ϸ��̼� Ʈ�� �ε�
		LoadAnimation(pMesh.get());

		m_pMesh.push_back( pMesh );	
	}
	return true;
}
void TTbsObj::LoadAnimation(tTbsMesh* pMesh)
{
	if( pMesh->m_iNumTrack[0] && SUCCEEDED(m_Parser. GetDataFromFile(_T("#POSITION_TRACK")) ) )
	{		
		LoadAnimationTrack(pMesh->m_iNumTrack[0], pMesh->m_pPosTrack );
	}
	if( pMesh->m_iNumTrack[1] &&  m_Parser.GetDataFromFile( _T("#ROTATION_TRACK") ) )  
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[1], pMesh->m_pRotTrack );			
	}
	if( pMesh->m_iNumTrack[2] && m_Parser.GetDataFromFile( _T("#SCALE_TRACK") ) )
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[2], pMesh->m_pSclTrack );	
	}					
	if( pMesh->m_iNumTrack[3] && m_Parser.GetDataFromFile( _T("#VISIVILITY_TRACK") ) )
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[3], pMesh->m_pVisTrack );			
	}				
}
int TTbsObj::LoadVertexIndex(tTbsMesh* pMesh)
{
	int iMtrlIndex = 0;
	int iNumSubFaces = 0;
	int iVertexCounter = 0;

	_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
	_stscanf( m_Parser.m_pBuffer, _T("%d %d %d"),  &iMtrlIndex, &iNumSubFaces, &iVertexCounter );
	if (iVertexCounter <= 0) return 0;

	if( iNumSubFaces > 0 )
	{
		pMesh->m_TriList.resize(iNumSubFaces);
	}
	else
	{
		pMesh->m_TriList.resize(pMesh->m_iNumFace);		
	}
	pMesh->m_IndexArray.resize( iNumSubFaces* 3 );
	pMesh->m_VertexArray.resize( iVertexCounter );

	for( int iCnt = 0; iCnt < iVertexCounter; iCnt++ )
	{
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf( m_Parser.m_pBuffer, _T("%f %f %f  %f %f %f  %f %f %f %f  %f %f"), 
					&pMesh->m_VertexArray[iCnt].p.x,	&pMesh->m_VertexArray[iCnt].p.y,	&pMesh->m_VertexArray[iCnt].p.z,
					&pMesh->m_VertexArray[iCnt].n.x,	&pMesh->m_VertexArray[iCnt].n.y,	&pMesh->m_VertexArray[iCnt].n.z,
					&pMesh->m_VertexArray[iCnt].c.x,	&pMesh->m_VertexArray[iCnt].c.y,	&pMesh->m_VertexArray[iCnt].c.z,		&pMesh->m_VertexArray[iCnt].c.w,
					&pMesh->m_VertexArray[iCnt].t.x,	&pMesh->m_VertexArray[iCnt].t.y );								
	}

	for( int iIndex = 0; iIndex < iNumSubFaces; iIndex++ )
	{			
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf( m_Parser.m_pBuffer, _T("%d %d %d"),	&pMesh->m_IndexArray[iIndex*3+0], 
														&pMesh->m_IndexArray[iIndex*3+1], 
														&pMesh->m_IndexArray[iIndex*3+2] );	
	}
	m_iMaxVertex += pMesh->m_VertexArray.size();
	m_iMaxIndex += pMesh->m_IndexArray.size();
	
	// Ʈ���̾ޱ� ����Ʈ ����
	for( DWORD dwFace = 0; dwFace < iNumSubFaces; dwFace++ )
	{
		for( int iCnt = 0; iCnt < 3; iCnt++ )
		{				
			pMesh->m_TriList[dwFace].vVertex[iCnt] = pMesh->m_VertexArray[pMesh->m_IndexArray[dwFace*3+iCnt]];				
		}	
	}	
	return iNumSubFaces;
}
bool TTbsObj::LoadAnimationTrack( int iNumTrack, vector<shared_ptr<TAnimTrack>>& pTrackList )
{
	TAnimTrack* pPrevTrack = NULL;		
	for( int iTrack = 0; iTrack < iNumTrack; iTrack++ )
	{				
		auto pTrack = make_shared<TAnimTrack>(); 		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf( m_Parser.m_pBuffer, _T("%d%f%f%f%f%f%f%f"), 
			&pTrack->iTick,
			&pTrack->vVector.x,
			&pTrack->vVector.y,
			&pTrack->vVector.z,
			&pTrack->qRotate.x,
			&pTrack->qRotate.y,
			&pTrack->qRotate.z,
			&pTrack->qRotate.w );	

		pPrevTrack = SetDoublyLinkedList( pTrack.get(), pPrevTrack );
		pTrackList.push_back(pTrack);	
	}
	return true;
}
void TTbsObj::Interpolate( tTbsMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick )
{
	// TM		= AnimMat * ParentTM;
	// AaniMat	= TM * Inverse(ParentTM)
	D3DXQUATERNION qR, qS;	
	D3DXMatrixIdentity(&pMesh->m_matCalculation);		
	D3DXMATRIX matAnim, matPos, matRotate, matScale;
	
	matRotate	= pMesh->m_matWorldRotate;
	matPos		= pMesh->m_matWorldTrans;
	matScale	= pMesh->m_matWorldScale;
	
	D3DXQuaternionRotationMatrix(  &qR,  &matRotate );
	D3DXQuaternionRotationMatrix(  &qS,  &matScale );

	// fFrameTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame + CurFame;
	float fStartTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;
	float fEndTick   = 0.0f;

	TAnimTrack* pStartTrack = NULL;
	TAnimTrack* pEndTrack   = NULL;
	if( pMesh->m_pRotTrack.size() ) 
	{
		// pStartTrack�� ã���� ������
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pRotTrack, &pStartTrack, &pEndTrack )  ) 
		{
			qR = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fEndTick = pEndTrack->iTick;
			D3DXQuaternionSlerp(&qR, &qR, &pEndTrack->qRotate, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );							
		}		
		D3DXMatrixRotationQuaternion(&matRotate, &qR );	
	}
	
	pStartTrack = NULL;
	pEndTrack   = NULL;

	D3DXVECTOR3 Trans(matPos._41, matPos._42, matPos._43 );
	if( pMesh->m_pPosTrack.size() ) 
	{
		// pStartTrack�� ã���� ������
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pPosTrack, &pStartTrack, &pEndTrack )  ) 
		{
			Trans = pStartTrack->vVector; 
			fStartTick = pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp( &Trans, &Trans,&pEndTrack->vVector, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );							
		}	
		
		D3DXMatrixTranslation(&matPos, Trans.x, Trans.y, Trans.z);	
	}


	pStartTrack = NULL;
	pEndTrack   = NULL;

	D3DXMATRIX matScaleRot, matInvScaleRot;
	D3DXVECTOR3 vScale(matScale._11, matScale._22, matScale._33 );
	if( pMesh->m_pSclTrack.size() ) 
	{
		// pStartTrack�� ã���� ������
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pSclTrack, &pStartTrack, &pEndTrack )  ) 
		{
			vScale = pStartTrack->vVector; 
			qS = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp( &vScale, &vScale,&pEndTrack->vVector, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );							
			D3DXQuaternionSlerp(&qS, &qS, &pEndTrack->qRotate, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );
		}			
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);	
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS );
		D3DXMatrixInverse( &matInvScaleRot, NULL, &matScaleRot );
		matScale = matInvScaleRot  * matScale * matScaleRot;
	}

	pStartTrack = NULL;
	pEndTrack   = NULL;

	float fCurAlpha, fNextAlpha, fOffSet;
	fCurAlpha = 0.0f; 	
	fNextAlpha= 0.0f;
	if( pMesh->m_pVisTrack.size() ) 
	{
		// pStartTrack�� ã���� ������
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pVisTrack, &pStartTrack, &pEndTrack )  ) 
		{
			fCurAlpha = pStartTrack->vVector.x;  
			fStartTick = pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fNextAlpha = pEndTrack->vVector.x;
			fEndTick = pEndTrack->iTick;

			fOffSet = (fFrameTick - fStartTick)/(fEndTick - fStartTick );
			fNextAlpha = (fNextAlpha - fCurAlpha)*fOffSet;											
		}			
		pMesh->m_fVisibility = (fCurAlpha + fNextAlpha);	
	}
	else
	{		
		pMesh->m_fVisibility = 1.0f;
	}
	
	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);
	matAnim._41 = matPos._41;
	matAnim._42 = matPos._42;
	matAnim._43 = matPos._43;
	// ���� ���̸��̼� ����� �ϼ��Ѵ�.	
	D3DXMatrixMultiply(&pMesh->m_matCalculation,&matAnim, matParents );

	// �ι��� ��Ʈ���� Ȯ�� �ڵ�.
	D3DXVECTOR3 v0,v1,v2,v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if( D3DXVec3Dot( &v3, &v0 ) < 0.0f)
	{
		D3DXMATRIX matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply( &pMesh->m_matCalculation, &pMesh->m_matCalculation, &matW );
	}
}
// pEndTrack Ʈ���� ������ flase ����( ������ ����� ���� �� )
bool TTbsObj::GetAnimationTrack(float fFrame, vector<shared_ptr<TAnimTrack>> pTrackList, TAnimTrack** pStartTrack, TAnimTrack** pEndTrack )
{
	for( DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++ ) 
	{
		TAnimTrack *pTrack = pTrackList[dwTrack].get();		
		_ASSERT( pTrack );		
		// fFrame ���� ū Tick Ʈ���� �ִٸ� ���� Ʈ���� �Ѱ� �־�� �ϱ� ������ break�Ѵ�.
		if( pTrack->iTick > fFrame )	
		{			
			*pEndTrack = pTrack;
			break;
		}		
		// �������Ӻ��� ū���� ���ٸ�. ���� �ð������ ������ ���� ����Ѵ�.
		*pStartTrack	= pTrack;		
	}
	return ( *pStartTrack != NULL ) ? true : false;
}
bool TTbsObj::Init()
{
	return true;
}
bool TTbsObj::Frame()
{
	m_fElapseTime += g_fSecPerFrame * m_Scene.iFrameSpeed * m_Scene.iTickPerFrame;		
	if( m_fElapseTime >= m_Scene.iLastFrame* m_Scene.iTickPerFrame ) 
	{				
		m_fElapseTime =  m_Scene.iFirstFrame * m_Scene.iTickPerFrame;
	}

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	for( DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++) 
	{
		tTbsMesh* pMesh = m_pMesh[dwObject].get();
		if( pMesh->m_bUsed == false ) continue;	
		if( pMesh->m_pParent)
			Interpolate( pMesh, &pMesh->m_pParent->m_matCalculation,m_fElapseTime );
		else
			Interpolate( pMesh, &mat,m_fElapseTime);					
	}	
	// ��Ӱ����� ���ϸ��̼� ����� ���� ���Ŀ��� �� �޽��� ������ ��� ����( �̵� �� ȸ��/���� )
	for( DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		tTbsMesh* pMesh = m_pMesh[dwObject].get();
		_ASSERT( pMesh );
		D3DXMatrixMultiply( &pMesh->m_matCalculation , &pMesh->m_matCalculation, &m_matControlWorld  );
	}
	return true;
}
bool TTbsObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}
bool TTbsObj::Release()
{
	return true;
}
bool TTbsObj::Convert(ID3D11Device* pd3dDevice)
{	
	if( !SetMaterial() )
	{
		return false;
	}
	if( !InheriteCollect() ) 
	{
		return false;
	}
	return true;
}
TAnimTrack* TTbsObj::SetDoublyLinkedList( TAnimTrack* pCurrentTrack, TAnimTrack* pPrev )
{	
	if( pPrev )
	{
		pPrev->pNext	= pCurrentTrack;
		pCurrentTrack->pPrev	= pPrev;		
	}
	return pCurrentTrack;
}


int	TTbsObj::GetMapID( TMtrl* pMtrl, int iTexMapType )
{
	_ASSERT( pMtrl );
	if( pMtrl->m_TexMaps.size() <= 0 ) return -1;

	for( int iTexmap = 0; iTexmap < pMtrl->m_TexMaps.size(); iTexmap++ )
	{
		if( pMtrl->m_TexMaps[iTexmap].m_dwType == iTexMapType )
		{
			return pMtrl->m_TexMaps[iTexmap].m_dwIndex;					
		}
	}	
	return -1;
}

bool TTbsObj::ResetResource()
{
	return true;
}

bool TTbsObj::InheriteCollect()
{
	D3DXMATRIX m_matInverse;
	D3DXQUATERNION qR;
	D3DXVECTOR3 vTrans, vScale;
	for( DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++) 
	{		
		if( !m_pMesh[dwObject]->m_strParentName.empty() 
			&& ( m_pMesh[dwObject]->m_strParentName != _T("(null)") ) )
		{
			tTbsMesh* pParentNode = SearchToCollects( m_pMesh[dwObject]->m_strParentName );
			if(pParentNode)
			{
				m_pMesh[dwObject]->m_pParent = pParentNode;
				D3DXMATRIX matInverse= m_pMesh[dwObject]->m_matWorld * pParentNode->m_matInverse;					
				D3DXMatrixDecompose( &vScale, &qR, &vTrans, &matInverse );
				D3DXMatrixScaling( &m_pMesh[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z );
				D3DXMatrixTranslation( &m_pMesh[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z );
				D3DXMatrixRotationQuaternion( &m_pMesh[dwObject]->m_matWorldRotate, &qR );
				
				pParentNode->m_pChildMesh.push_back(m_pMesh[dwObject].get());
			}			
		}
		else
		{
			D3DXMatrixDecompose( &vScale, &qR, &vTrans, &m_pMesh[dwObject]->m_matWorld );
			D3DXMatrixScaling( &m_pMesh[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z );
			D3DXMatrixTranslation( &m_pMesh[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z );
			D3DXMatrixRotationQuaternion( &m_pMesh[dwObject]->m_matWorldRotate, &qR );		
		}
	}	
	// ������ ���� ������Ʈ ����
	for( DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++) 
	{
		if( m_pMesh[dwObject]->m_pChildMesh.size() <= 0 && 
			(m_pMesh[dwObject]->m_ClassType == CLASS_BONE || m_pMesh[dwObject]->m_ClassType == CLASS_DUMMY))
		{
			m_pMesh[dwObject]->m_bUsed = false;
		}
	}
	return true;
}
tTbsMesh* TTbsObj::SearchToCollects( T_STR	m_strParentName )
{
	for( DWORD dwGroup = 0; dwGroup < m_pMesh.size(); dwGroup++ ) 
	{		
		if( m_pMesh[dwGroup]->m_strNodeName == m_strParentName )
		{
			return m_pMesh[dwGroup].get();
		}
	}	
	return NULL;	
}
bool TTbsObj::SetMaterial()
{
	for( DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{		
		tTbsMesh* pMesh = m_pMesh[dwObject].get();
		if( pMesh->m_iNumFace <= 0 || pMesh->m_iMtrlRef < 0 ) continue;

		if( pMesh->m_pSubMesh.size() > 0 )
		{
			for( int iSubMesh =0; iSubMesh < pMesh->m_pSubMesh.size(); iSubMesh++ )
			{
				tTbsMesh* pSubMesh = pMesh->m_pSubMesh[iSubMesh].get();
				if( pSubMesh )
				{
					pSubMesh->m_iDiffuseTex = GetMapID( &m_Material[pMesh->m_iMtrlRef].m_SubMaterial[pSubMesh->m_iMtrlRef], ID_TBASIS_DI );
				}	
			}
		}		
		else
		{
			pMesh->m_iDiffuseTex = GetMapID( &m_Material[pMesh->m_iMtrlRef], ID_TBASIS_DI );			
		}		
	}
	return true;
}
HRESULT TTbsObj::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_Object.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice,
		m_Object.g_pVSBlob.Get()->GetBufferSize(),
		m_Object.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
bool TTbsObj::UpdateBuffer()
{
	if (!CombineBuffer(m_Object.g_pVertexBuffer.Get(), m_Object.g_pIndexBuffer.Get()))
	{
		return false;
	}
	return true;
}
bool TTbsObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
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

				g_pImmediateContext->UpdateSubresource(pVB, 0, &pMesh->m_Object.m_BoxVB, (void*)&pMesh->m_VertexArray.at(0), 0, 0);

				pMesh->m_Object.m_iBeginVB = vbOffset;
				vbOffset += pMesh->m_Object.m_iNumVertex;
				dstOffset = pMesh->m_Object.m_BoxVB.right;

				pMesh->m_Object.m_iNumIndex = pMesh->m_IndexArray.size();
				pMesh->m_Object.m_BoxIB.left = dstibOffset;
				pMesh->m_Object.m_BoxIB.right = dstibOffset + pMesh->m_Object.m_iNumIndex*sizeof(DWORD);
				pMesh->m_Object.m_BoxIB.top = 0; pMesh->m_Object.m_BoxIB.bottom = 1;
				pMesh->m_Object.m_BoxIB.front = 0; pMesh->m_Object.m_BoxIB.back = 1;

				g_pImmediateContext->UpdateSubresource(pIB, 0, &pMesh->m_Object.m_BoxIB, (void*)&pMesh->m_IndexArray.at(0), 0, 0);
				pMesh->m_Object.m_iBeginIB = ibOffset;
				ibOffset += pMesh->m_Object.m_iNumIndex;
				dstibOffset = pMesh->m_Object.m_BoxIB.right;

				//texture
				if (pMesh->m_iDiffuseTex < 0) continue;
				pMesh->m_Object.g_pTextureSRV = I_Texture.GetPtr(pMesh->m_iDiffuseTex)->m_pTextureRV;
			}
		}
	}
	return true;
}
bool TTbsObj::Draw(ID3D11DeviceContext*    pContext, TModel* pParent)
{
	//CStopwatch stopwatch;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		pParent->SetMatrix(&pMesh->m_matCalculation, &pParent->m_matView, &pParent->m_matProj);
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
TTbsObj::TTbsObj(void)
{
}

TTbsObj::~TTbsObj(void)
{
}
