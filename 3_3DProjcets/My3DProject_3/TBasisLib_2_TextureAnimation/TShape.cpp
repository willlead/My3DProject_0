#include "TShape.h"
void TShape::SetTechnique(ID3DX11EffectTechnique* pTechnique)
{
	if( !pTechnique ||  pTechnique->IsValid() == FALSE ) 
	{
		return ;
	}
	m_pTechniqueApply	= pTechnique;
}
void TShape::SetAmbientColor( float fR, float fG, float fB, float fA )
{
	m_pAmbientColor->SetFloatVector( ( float* )&D3DXVECTOR4( fR, fG, fB, fA ) );
}
void TShape::SetMatrix( D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj )
{
	if( pWorld != NULL )
	{
		m_matWorld	= *pWorld;
		m_vCenterPos.x = pWorld->_41;
		m_vCenterPos.y = pWorld->_42;
		m_vCenterPos.z = pWorld->_43;
	}
	if( pView != NULL )
	{
		m_matView	= *pView;
	}
	if( pProj != NULL )
	{
		m_matProj	= *pProj;
	}		
}

bool TShape::Create( ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext, 
						TCHAR* pLoadTextureString,
						TCHAR* pLoadShaderString )
{
	m_pd3dDevice = pDevice;
	m_pImmediateContext = pImmediateContext;

	if( FAILED( LoadEffectFile(pLoadShaderString) ) )
	{
		MessageBox( 0, _T("LoadEffectFile 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}
	if( FAILED( SetInputLayout() ) )
	{
		MessageBox( 0, _T("SetInputLayout 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}
	if( FAILED(	InitBufferData() ))
	{
		MessageBox( 0, _T("InitBufferData 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}
	if( FAILED( CreateVertexBuffer() ) )
	{
		MessageBox( 0, _T("CreateVertexBuffer 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}
	if( FAILED( CreateIndexBuffer() ) )
	{
		MessageBox( 0, _T("CreateVertexBuffer 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}
	if( FAILED( CreateResource() ) )
	{
		MessageBox( 0, _T("CreateResource 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}

	if( FAILED( TextureLoad(pDevice, pLoadTextureString) ) )
	{
		MessageBox( 0, _T("CreateResource 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}
	
	return Init();
}
HRESULT TShape::InitBufferData()
{
	return S_OK;
}
HRESULT	TShape::TextureLoad( ID3D11Device* pDevice, TCHAR* pLoadTextureString)
{
	if( pLoadTextureString != NULL)
	{
		if( FAILED( m_Texture.Load(pDevice , m_pImmediateContext, pLoadTextureString) ) )
		{
			MessageBox( 0, _T("m_Texture.Load 실패"), _T("Fatal error"), MB_OK );
			return 0;
		}
	}
	return S_OK;
}
// 수정됨.
HRESULT TShape::LoadEffectFile(TCHAR* pLoadShaderString)
{	
	HRESULT hr;
	if( pLoadShaderString == 0 )
	{
		if( FAILED( hr = m_Shader.Load( m_pd3dDevice,  L"../../data/shader/Shape.fx") ))
		{
			return hr;
		}	
	}
	else
	{
		if( FAILED( hr = m_Shader.Load( m_pd3dDevice,  pLoadShaderString ) ))
		{
			return hr;
		}
	}
	GetEffectVariable();
	return S_OK;
}
void TShape::GetEffectVariable()
{
	 // Obtain the technique
	m_pTechniqueColor		= m_Shader.GetEffect()->GetTechniqueByName( "ColorRender" );
	m_pTechniqueOnlyAmbientColor	= m_Shader.GetEffect()->GetTechniqueByName( "AmbientColorRender" );	
	m_pTechnique			= m_Shader.GetEffect()->GetTechniqueByName( "Render" );
	SetTechnique(m_pTechnique);
	 // Obtain the variables
	m_pWorldVariable		= m_Shader.GetEffect()->GetVariableByName( "g_matWorld" )->AsMatrix();
    m_pViewVariable			= m_Shader.GetEffect()->GetVariableByName( "g_matView" )->AsMatrix();
    m_pProjVariable			= m_Shader.GetEffect()->GetVariableByName( "g_matProj" )->AsMatrix();
	m_pTextureVariable		= m_Shader.GetEffect()->GetVariableByName( "g_txDiffuse" )->AsShaderResource();
	m_pAmbientColor			= m_Shader.GetEffect()->GetVariableByName( "g_vAmbientColor" )->AsVector();
}
HRESULT TShape::SetInputLayout()
{	
	return S_OK;
}
HRESULT TShape::CreateVertexBuffer()
{
	return S_OK;
}
HRESULT TShape::CreateIndexBuffer()
{	
	return S_OK;
}
bool TShape::ReLoadVertexBuffer()
{
	return true;
}
ID3D11Buffer* TShape::GetVB()
{
	return m_BufferVB.m_pBuffer;
}
ID3D11Buffer* TShape::GetIB()
{
	return m_BufferIB.m_pBuffer;
}
bool TShape::Init()
{
    D3DXMatrixIdentity( &m_matWorld );    
	D3DXMatrixIdentity( &m_matView ); 
	D3DXMatrixIdentity( &m_matProj ); 
	return true;
}
bool TShape::Render()
{	
	return true;
}
bool TShape::Render(ID3D11DeviceContext* pContext)
{
	return true;
}
bool TShape::Release()
{
	m_DxState.Release();
	m_InputLayout.Release();
	m_BufferVB.Release();
	m_BufferIB.Release();	
	m_Texture.Release();
	m_Shader.Release();
	return true;
}
bool TShape::Frame()
{	
	return true;
}
bool TShape::Draw(	D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 dwColor  ) 
{
	return true;
};
void TShape::CreateSphere() {};
void TShape::CreatePlane() {};

void TShape::CreateOBBBox(	float fExtentX,float fExtentY,float fExtentZ,
						  D3DXVECTOR3 vCenter, 
						  D3DXVECTOR3 vDirX,
						  D3DXVECTOR3 vDirY,
						  D3DXVECTOR3 vDirZ )
{	
	m_Box.fExtent[0]	= fExtentX;
	m_Box.fExtent[1]	= fExtentY;
	m_Box.fExtent[2]	= fExtentZ;
	m_Box.vCenter		= vCenter;

	m_Box.vAxis[0] = m_Box.fExtent[0] * vDirX;
	m_Box.vAxis[1] = m_Box.fExtent[1] * vDirY;
	m_Box.vAxis[2] = m_Box.fExtent[2] * vDirZ;      

	m_Box.vPos[0] = vCenter - m_Box.vAxis[0] - m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[1] = vCenter - m_Box.vAxis[0] + m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[2] = vCenter + m_Box.vAxis[0] + m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[3] = vCenter + m_Box.vAxis[0] - m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[4] = vCenter - m_Box.vAxis[0] - m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[5] = vCenter - m_Box.vAxis[0] + m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[6] = vCenter + m_Box.vAxis[0] + m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[7] = vCenter + m_Box.vAxis[0] - m_Box.vAxis[1] + m_Box.vAxis[2];

	m_Box.vMax = m_Box.vPos[0];
	m_Box.vMin = m_Box.vPos[0];

	for( int iPoint = 1; iPoint < 8; iPoint++ )
	{
		if( m_Box.vMax.x < m_Box.vPos[iPoint].x )
		{
			m_Box.vMax.x = m_Box.vPos[iPoint].x;
		}
		if( m_Box.vMax.y < m_Box.vPos[iPoint].y )
		{
			m_Box.vMax.y = m_Box.vPos[iPoint].y;
		}
		if( m_Box.vMax.z < m_Box.vPos[iPoint].z )
		{
			m_Box.vMax.z = m_Box.vPos[iPoint].z;
		}

		if( m_Box.vMin.x > m_Box.vPos[iPoint].x )
		{
			m_Box.vMin.x = m_Box.vPos[iPoint].x;
		}
		if( m_Box.vMin.y > m_Box.vPos[iPoint].y )
		{
			m_Box.vMin.y = m_Box.vPos[iPoint].y;
		}
		if( m_Box.vMin.z > m_Box.vPos[iPoint].z )
		{
			m_Box.vMin.z = m_Box.vPos[iPoint].z;
		}
	}
	D3DXVec3Normalize( &m_Box.vAxis[0], &m_Box.vAxis[0] );	
	D3DXVec3Normalize( &m_Box.vAxis[1], &m_Box.vAxis[1] );	
	D3DXVec3Normalize( &m_Box.vAxis[2], &m_Box.vAxis[2] );	

}
void TShape::CreateAABBBox(	D3DXVECTOR3 max, D3DXVECTOR3 min )
{	
	m_Box.vMax = max;
	m_Box.vMin = min;
	m_Box.vPos[0] = D3DXVECTOR3( min.x, min.y, min.z );
	m_Box.vPos[1] = D3DXVECTOR3( min.x, max.y, min.z );
	m_Box.vPos[2] = D3DXVECTOR3( max.x, max.y, min.z );
	m_Box.vPos[3] = D3DXVECTOR3( max.x, min.y, min.z );

	m_Box.vPos[4] = D3DXVECTOR3( min.x, min.y, max.z );
	m_Box.vPos[5] = D3DXVECTOR3( min.x, max.y, max.z );
	m_Box.vPos[6] = D3DXVECTOR3( max.x, max.y, max.z );
	m_Box.vPos[7] = D3DXVECTOR3( max.x, min.y, max.z );

	m_Box.vCenter = ( max + min ) / 2.0f;
	m_Box.vAxis[0] = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	m_Box.vAxis[1] = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_Box.vAxis[2] = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
}	
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TShape::CreateResource()
{
	m_DxState.SetRasterizerState( m_pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
	m_DxState.SetPrimitive(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return S_OK;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TShape::DeleteResource()
{
	SAFE_DELETE_ARRAY(m_pVertexList);
	SAFE_DELETE_ARRAY(m_pIndexList);
	m_DxState.Release();
	return S_OK;
}
// x,y는 시작 스크린 위치, w,h 가로 폭, 세로 폭, vScreen은 윈도우 가로 및 세로 크기
bool TShape::SetScreenVertex(	float x, 
								float y, 
								float w, 
								float h,
								D3DXVECTOR2 vScreen )
{
	m_bScreenVertex = true;
	SAFE_NEW_ARRAY( m_pVertexList, PCT_VERTEX, 4 );
	return true;
}
TShape::TShape(void)
{
	m_pd3dDevice			= NULL;
	m_pTechnique			= NULL;	
	m_pTechniqueColor		= NULL;
	m_pTechniqueOnlyAmbientColor = NULL;
	m_pWorldVariable		= NULL;
	m_pViewVariable			= NULL;
	m_pProjVariable			= NULL;
	m_pTextureVariable		= NULL;
	m_pVertexList			= NULL;
	m_pIndexList			= NULL;
	m_vCenterPos			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vMoveDirection		= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	m_fRadius				= 0.0f;
	m_iNumVertex			= 0;
	m_bScreenVertex			= false;
}

TShape::~TShape(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TLineShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TLineShape::SetInputLayout()
{
	HRESULT hr;
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	if( FAILED( hr = m_InputLayout.Create( m_pd3dDevice, layout, numElements, &m_pTechniqueColor, 0  ) ) )
	{
		MessageBox( 0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK );
		return hr;
	}
	return S_OK;
}
HRESULT TLineShape::CreateVertexBuffer()
{
	HRESULT hr;
	
	PC_VERTEX vertices[] =
    {
        { D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { D3DXVECTOR3( 1.0f, 0.0f, 0.0f ), D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) },		
    };

	if( FAILED( hr = m_BufferVB.Create(	m_pd3dDevice,
												sizeof( PC_VERTEX), 
												COUNTOF(vertices), 
												vertices, 
												D3D11_BIND_VERTEX_BUFFER, 
												D3D11_USAGE_DEFAULT,
												NULL ) ))
	{
		return hr;
	}    
	return S_OK;
}
HRESULT TLineShape::CreateIndexBuffer()
{
	HRESULT hr;
	  // Create index buffer
    DWORD indices[] =
    {
		0,1,
    };

	if( FAILED( hr = m_BufferIB.Create(	m_pd3dDevice,
											sizeof( DWORD), 
											COUNTOF(indices), 
											indices, 
											D3D11_BIND_INDEX_BUFFER,
											D3D11_USAGE_DYNAMIC,
											NULL) ))
	{
		return hr;
	}	
	return hr;
}
bool TLineShape::Render()
{
	m_DxState.ApplyRasterizer( m_pd3dDevice, m_pImmediateContext);
	//--------------------------------------------------------------------------------------
	// 인풋레이아웃 바인딩
	//--------------------------------------------------------------------------------------
	m_InputLayout.Apply( m_pImmediateContext );
	//--------------------------------------------------------------------------------------
	// 버텍스 및 인덱스 버퍼 바인딩
	//--------------------------------------------------------------------------------------
	UINT stride = sizeof( PC_VERTEX );
    UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_BufferVB.m_pBuffer, &stride, &offset );
	m_pImmediateContext->IASetIndexBuffer( m_BufferIB.m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
	//--------------------------------------------------------------------------------------
	// 월드 행렬 세팅
	//--------------------------------------------------------------------------------------
	if(m_pWorldVariable)	m_pWorldVariable->SetMatrix( ( float* )&m_matWorld );	
	if(m_pViewVariable)		m_pViewVariable->SetMatrix( ( float* )&m_matView );			// 뷰  행렬
	if(m_pProjVariable)		m_pProjVariable->SetMatrix( ( float* )&m_matProj );// 투영행렬
	// 테크닉 정보 
	D3DX11_TECHNIQUE_DESC techDesc;				
    m_pTechniqueColor->GetDesc( &techDesc );	
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        m_pTechniqueColor->GetPassByIndex( p )->Apply( 0,m_pImmediateContext);
        m_pImmediateContext->DrawIndexed( 2, 0, 0 ); 
    }
	return true;
}
bool TLineShape::Draw( D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor )
{
	PC_VERTEX vertices[2];
	vertices[0].p = vStart;
	vertices[0].c = vColor;
	vertices[1].p = vEnd;
	vertices[1].c = vColor;

	// 동적 리소스 갱신 방법 1 : D3D11_USAGE_DEFAULT 사용
	m_pImmediateContext->UpdateSubresource( GetVB(), 0, NULL, &vertices, 0, 0); 

	// 동적 리소스 갱신 방법 2 : D3D11_USAGE_DYNAMIC 사용
	/*HRESULT hr;
	PC_VERTEX*		pVB = NULL;
	DWORD*			pIB = NULL;
	if( SUCCEEDED( hr= GetVB()->Map(  D3D11_MAP_WRITE_DISCARD, 0, ( void** )&pVB ) ) )
	{		
		pVB[0].p	= vStart;
		pVB[1].p	= vEnd;		
		pVB[0].c    = vColor;
		pVB[1].c    = vColor;			
		GetVB()->Unmap();	
	}
	if( hr == D3D10_MAP_FLAG_DO_NOT_WAIT  )
	{
		MessageBox( 0, _T("D3D10_MAP_FLAG_DO_NOT_WAIT"), _T("Fatal error"), MB_OK );
		return false;
	}*/
	return Render();
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TLineShape::CreateResource()
{
	m_DxState.SetRasterizerState( m_pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
	m_DxState.SetPrimitive(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	return S_OK;
}
TLineShape::TLineShape(void)
{
}

TLineShape::~TLineShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TBoxShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TBoxShape::SetInputLayout()
{
	HRESULT hr;
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

	if( FAILED( hr = m_InputLayout.Create( m_pd3dDevice, layout, COUNTOF(layout), &m_pTechniqueApply, 0  ) ) )
	{
		MessageBox( 0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK );
		return hr;
	}
	return S_OK;
}
HRESULT TBoxShape::InitBufferData()
{
	PCT_VERTEX vertices[] =
    {
		// 앞면
        { D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, 1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,1.0f, 0.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, -1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f)  },
        { D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR4( 1.0f,1.0f, 0.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f)  },

		// 뒷면
        { D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR4( 1.0f,1.0f, 0.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f)  },
        { D3DXVECTOR3( -1.0f, 1.0f, 1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f)  },
        { D3DXVECTOR3( -1.0f, -1.0f, 1.0f ),  D3DXVECTOR4( 0.0f,1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f)  },
        { D3DXVECTOR3( 1.0f, -1.0f, 1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 0.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f)  },

		// 왼쪽
        { D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, 1.0f, 1.0f ),  D3DXVECTOR4( 0.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, -1.0f, 1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f)  },
        { D3DXVECTOR3( 1.0f, -1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f)  },

		// 오른쪽면
        { D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR4( 0.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f)  },
        { D3DXVECTOR3( -1.0f, 1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f)  },
        { D3DXVECTOR3( -1.0f, -1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f)  },
        { D3DXVECTOR3( -1.0f, -1.0f, 1.0f ),  D3DXVECTOR4( 1.0f,1.0f, 0.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f)  },

		// 윗면
        { D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR4( 0.0f,1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, 1.0f, 1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, 1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,1.0f, 0.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f)  },
        { D3DXVECTOR3( -1.0f, 1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f)  },

		// 아랫면
        { D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, -1.0f, -1.0f ),  D3DXVECTOR4( 1.0f,1.0f, 0.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f)  },
        { D3DXVECTOR3( 1.0f, -1.0f, 1.0f ),  D3DXVECTOR4( 1.0f,0.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f)  },
        { D3DXVECTOR3( -1.0f, -1.0f, 1.0f ),  D3DXVECTOR4( 0.0f,1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f)  },

        
    };

	SAFE_NEW_ARRAY( m_pVertexList, PCT_VERTEX, 24 );
	memcpy( m_pVertexList, vertices, sizeof(PCT_VERTEX) * 24);

	SAFE_NEW_ARRAY( m_pIndexList, DWORD, 36 );
	  // Create index buffer
    DWORD indices[] =
    {
		// Box
        0,1,2,		0,2,3,		
		4,5,6,		4,6,7,
		8,9,10,		8,10,11,
		12,13,14,	12,14,15,
		16,17,18,	16,18,19,
		20,21,22,	20,22,23
    };
	memcpy( m_pIndexList, indices, sizeof(DWORD) * 36);
	return true;	
}
bool TBoxShape::ReLoadVertexBuffer()
{
	m_BufferVB.Release();
	m_pImmediateContext->UpdateSubresource( 
		m_BufferVB.m_pBuffer, 0, NULL, m_pVertexList, 0, 0 );	
	return true;
}
HRESULT TBoxShape::CreateVertexBuffer()
{
	HRESULT hr;	
	if( FAILED( hr = m_BufferVB.Create(	m_pd3dDevice,	sizeof( PCT_VERTEX), 	24, 
										m_pVertexList, 	D3D11_BIND_VERTEX_BUFFER ) ))
	{
		return hr;
	} 
	return S_OK;
}
HRESULT TBoxShape::CreateIndexBuffer()
{
	HRESULT hr;
	if( FAILED( hr = m_BufferIB.Create(	m_pd3dDevice,	sizeof( DWORD), 36, 
										m_pIndexList, 	D3D11_BIND_INDEX_BUFFER) ))
	{
		return hr;
	}	
	return hr;
}
bool TBoxShape::Render()
{
	//--------------------------------------------------------------------------------------
	// 인풋레이아웃 바인딩
	//--------------------------------------------------------------------------------------
	m_InputLayout.Apply( m_pImmediateContext );
	//--------------------------------------------------------------------------------------
	// 버텍스 및 인덱스 버퍼 바인딩
	//--------------------------------------------------------------------------------------
	UINT stride = sizeof( PCT_VERTEX );
    UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_BufferVB.m_pBuffer, &stride, &offset );
	m_pImmediateContext->IASetIndexBuffer( m_BufferIB.m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
	//--------------------------------------------------------------------------------------
	// 월드 행렬 세팅
	//--------------------------------------------------------------------------------------
	if(m_pWorldVariable)	m_pWorldVariable->SetMatrix( ( float* )&m_matWorld );	
	if(m_pViewVariable)		m_pViewVariable->SetMatrix( ( float* )&m_matView );			// 뷰  행렬
	if(m_pProjVariable)		m_pProjVariable->SetMatrix( ( float* )&m_matProj );// 투영행렬

	ID3DX11EffectTechnique*		pTechnique = m_pTechniqueApply;
	if( !m_Texture.m_pTextureRV && m_pTechniqueOnlyAmbientColor )
	{	
		//m_pImmediateContext->PSSetShaderResources(0, 1, &m_Texture.m_pTextureRV);
		pTechnique = m_pTechniqueOnlyAmbientColor;
		if(m_pTextureVariable)	m_pTextureVariable->SetResource( NULL ); 
	}
	else
	{
		if(m_pTextureVariable)	m_pTextureVariable->SetResource( m_Texture.m_pTextureRV ); 
	}

	// 테크닉 정보 
	D3DX11_TECHNIQUE_DESC techDesc;				
    pTechnique->GetDesc( &techDesc );	
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        pTechnique->GetPassByIndex( p )->Apply( 0,m_pImmediateContext);		
        m_pImmediateContext->DrawIndexed( 36, 0, 0 ); 
    }
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TBoxShape::CreateResource()
{
	m_DxState.SetRasterizerState( m_pd3dDevice, D3D11_CULL_BACK, D3D11_FILL_SOLID);
	m_DxState.SetPrimitive(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return S_OK;
}
TBoxShape::TBoxShape(void)
{
}

TBoxShape::~TBoxShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TPlaneShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool TPlaneShape::SetScreenVertex(	float x, 
								float y, 
								float w, 
								float h,
								D3DXVECTOR2 vScreen )
{
	m_bScreenVertex = true;
	SAFE_NEW_ARRAY( m_pVertexList, PCT_VERTEX, 4 );

	D3DXVECTOR2 vPoint;
	// 0 ~ 1
	vPoint.x = x / vScreen.x;
	vPoint.y = y / vScreen.y;
	// 0~1 => -1 ~ +1
	vPoint.x = vPoint.x *  2.0f - 1.0f;
	vPoint.y = -(vPoint.y *  2.0f - 1.0f);
	
	D3DXVECTOR2 vOffset;
	// 0 ~ 1
	vOffset.x = (x+w) / vScreen.x;
	vOffset.y = (y+h) / vScreen.y;
	// 0~1 => -1 ~ +1
	vOffset.x = ( vOffset.x *  2.0f - 1.0f ) - vPoint.x;
	vOffset.y = vPoint.y + ( vOffset.y *  2.0f - 1.0f );

	m_pVertexList[0].p.x = vPoint.x;
	m_pVertexList[0].p.y = vPoint.y;
	m_pVertexList[0].p.z = 0.5f;

	m_pVertexList[1].p.x = vPoint.x + vOffset.x;
	m_pVertexList[1].p.y = vPoint.y;
	m_pVertexList[1].p.z = 0.5f;

	m_pVertexList[2].p.x = vPoint.x + vOffset.x;
	m_pVertexList[2].p.y = vPoint.y - vOffset.y;
	m_pVertexList[2].p.z = 0.5f;

	m_pVertexList[3].p.x = vPoint.x;
	m_pVertexList[3].p.y = vPoint.y - vOffset.y;
	m_pVertexList[3].p.z = 0.5f;
	return true;
}
HRESULT TPlaneShape::SetInputLayout()
{
	HRESULT hr;
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
        { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
    };

	if( FAILED( hr = m_InputLayout.Create( m_pd3dDevice, layout, COUNTOF(layout), &m_pTechnique, 0  ) ) )
	{
		MessageBox( 0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK );
		return hr;
	}
	return S_OK;
}

HRESULT TPlaneShape::InitBufferData()
{
	PCT_VERTEX vertices[] =
    {
		{ D3DXVECTOR3( -1.0f, 1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f) },
        { D3DXVECTOR3( 1.0f, 1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f) },
        { D3DXVECTOR3( 1.0f, -1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f) },
        { D3DXVECTOR3( -1.0f, -1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f) },
    };

	if( m_bScreenVertex )
	{
		vertices[0].p = m_pVertexList[0].p;
		vertices[1].p = m_pVertexList[1].p;
		vertices[2].p = m_pVertexList[2].p;
		vertices[3].p = m_pVertexList[3].p;
	}
	else
	{
		SAFE_NEW_ARRAY( m_pVertexList, PCT_VERTEX, 4 );		
	}		
	memcpy( m_pVertexList, vertices, sizeof(PCT_VERTEX) * 4);

	SAFE_NEW_ARRAY( m_pIndexList, DWORD, 6 );
	m_pIndexList[0] =	0;
	m_pIndexList[1] =	1;
	m_pIndexList[2] =	3;
	m_pIndexList[3] =	1;
	m_pIndexList[4] =	2;
	m_pIndexList[5] =	3;
	return true;	
}

HRESULT TPlaneShape::CreateVertexBuffer()
{
	HRESULT hr;
	
	if( FAILED( hr = m_BufferVB.Create(	m_pd3dDevice,
												sizeof( PCT_VERTEX), 
												4, 
												m_pVertexList, 
												D3D11_BIND_VERTEX_BUFFER) ))
	{
		return hr;
	} 
	return S_OK;
}
HRESULT TPlaneShape::CreateIndexBuffer()
{
	HRESULT hr;
	if( FAILED( hr = m_BufferIB.Create(	m_pd3dDevice,	sizeof( DWORD), 6, 
										m_pIndexList, 	D3D11_BIND_INDEX_BUFFER ) ))
	{
		return hr;
	}	
	return hr;
}
bool TPlaneShape::Render()
{
	//--------------------------------------------------------------------------------------
	// 인풋레이아웃 바인딩
	//--------------------------------------------------------------------------------------
	m_InputLayout.Apply( m_pImmediateContext );
	//--------------------------------------------------------------------------------------
	// 버텍스 및 인덱스 버퍼 바인딩
	//--------------------------------------------------------------------------------------
	UINT stride = sizeof( PCT_VERTEX );
    UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_BufferVB.m_pBuffer, &stride, &offset );
	m_pImmediateContext->IASetIndexBuffer( m_BufferIB.m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
	//--------------------------------------------------------------------------------------
	// 월드 행렬 세팅
	//--------------------------------------------------------------------------------------
    if(m_pWorldVariable)	m_pWorldVariable->SetMatrix( ( float* )&m_matWorld );	
	if(m_pViewVariable)		m_pViewVariable->SetMatrix( ( float* )&m_matView );			// 뷰  행렬
    if(m_pProjVariable)		m_pProjVariable->SetMatrix( ( float* )&m_matProj );// 투영행렬

	ID3DX11EffectTechnique*		pTechnique = m_pTechniqueApply;
	if( !m_Texture.m_pTextureRV && m_pTechniqueOnlyAmbientColor )
	{	
		pTechnique = m_pTechniqueOnlyAmbientColor;
		if(m_pTextureVariable)	m_pTextureVariable->SetResource( NULL ); 
	}
	else
	{
		if(m_pTextureVariable)	m_pTextureVariable->SetResource( m_Texture.m_pTextureRV ); 
	}

	// 테크닉 정보 
	D3DX11_TECHNIQUE_DESC techDesc;				
    pTechnique->GetDesc( &techDesc );	
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        pTechnique->GetPassByIndex( p )->Apply( 0,m_pImmediateContext);
        m_pImmediateContext->DrawIndexed( 6, 0, 0 ); 
    }
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TPlaneShape::CreateResource()
{
	m_DxState.SetRasterizerState( m_pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
	m_DxState.SetPrimitive(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return S_OK;
}
TPlaneShape::TPlaneShape(void)
{
}

TPlaneShape::~TPlaneShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TLineShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TDirectionLineShape::SetInputLayout()
{
	HRESULT hr;
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	if( FAILED( hr = m_InputLayout.Create( m_pd3dDevice, layout, COUNTOF(layout), &m_pTechniqueColor, 0  ) ) )
	{
		MessageBox( 0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK );
		return hr;
	}
	return S_OK;
}
HRESULT TDirectionLineShape::CreateVertexBuffer()
{
	HRESULT hr;
	PC_VERTEX vertices[] =
    {
        { D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR4( 1.0f,0.0f, 0.0f, 1.0f ) },
        { D3DXVECTOR3( 100.0f, 0.0f, 0.0f ), D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) },
		{ D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR4( 0.0f,1.0f, 0.0f, 1.0f ) },
        { D3DXVECTOR3( 0.0f, 100.0f, 0.0f ), D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f ) },
		{ D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR4( 0.0f,0.0f, 1.0f, 1.0f ) },
        { D3DXVECTOR3( 0.0f, 0.0f, 100.0f ), D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 1.0f ) },        
    };

	if( FAILED( hr = m_BufferVB.Create(	m_pd3dDevice,	sizeof( PC_VERTEX), COUNTOF(vertices), 
										vertices, 	D3D11_BIND_VERTEX_BUFFER ) ))
	{
		return hr;
	}    
	return S_OK;
}
HRESULT TDirectionLineShape::CreateIndexBuffer()
{
	HRESULT hr;
	  // Create index buffer
    DWORD indices[] =
    {
		0,1,2,3,4,5,
    };

	if( FAILED( hr = m_BufferIB.Create(	m_pd3dDevice,	sizeof( DWORD), COUNTOF(indices), 
										indices, D3D11_BIND_INDEX_BUFFER) ))
	{
		return hr;
	}	
	return hr;
}
bool TDirectionLineShape::Render()
{
	m_DxState.ApplyRasterizer( m_pd3dDevice, m_pImmediateContext);
	//--------------------------------------------------------------------------------------
	// 인풋레이아웃 바인딩
	//--------------------------------------------------------------------------------------
	m_InputLayout.Apply( m_pImmediateContext );
	//--------------------------------------------------------------------------------------
	// 버텍스 및 인덱스 버퍼 바인딩
	//--------------------------------------------------------------------------------------
	UINT stride = sizeof( PC_VERTEX );
    UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_BufferVB.m_pBuffer, &stride, &offset );
	m_pImmediateContext->IASetIndexBuffer( m_BufferIB.m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
	//--------------------------------------------------------------------------------------
	// 월드 행렬 세팅
	//--------------------------------------------------------------------------------------
	if(m_pWorldVariable)	m_pWorldVariable->SetMatrix( ( float* )&m_matWorld );	
	if(m_pViewVariable)		m_pViewVariable->SetMatrix( ( float* )&m_matView );			// 뷰  행렬
	if(m_pProjVariable)		m_pProjVariable->SetMatrix( ( float* )&m_matProj );// 투영행렬
	// 테크닉 정보 
	D3DX11_TECHNIQUE_DESC techDesc;				
    m_pTechniqueColor->GetDesc( &techDesc );	
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        m_pTechniqueColor->GetPassByIndex( p )->Apply( 0,m_pImmediateContext);
        m_pImmediateContext->DrawIndexed( 6, 0, 0 ); 
    }
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TDirectionLineShape::CreateResource()
{
	m_DxState.SetRasterizerState( m_pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
	m_DxState.SetPrimitive(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	return S_OK;
}
TDirectionLineShape::TDirectionLineShape(void)
{
}

TDirectionLineShape::~TDirectionLineShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TSphereShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TSphereShape::SetInputLayout()
{
	HRESULT hr;
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

	if( FAILED( hr = m_InputLayout.Create( m_pd3dDevice, layout, COUNTOF(layout), &m_pTechnique, 0  ) ) )
	{
		MessageBox( 0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK );
		return hr;
	}
	return S_OK;
}
HRESULT TSphereShape::InitBufferData()
{
	CreateSphere(40);
	/*PCT_VERTEX vertices[] =
    {
		{ D3DXVECTOR3( -1.0f, 1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 0.0f) },
        { D3DXVECTOR3( 1.0f, 1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 0.0f) },
        { D3DXVECTOR3( 1.0f, -1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 1.0f, 1.0f) },
        { D3DXVECTOR3( -1.0f, -1.0f, 0.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ),D3DXVECTOR2( 0.0f, 1.0f) },
    };*/
	//SAFE_NEW_ARRAY( m_pVertexList, PNCT_VERTEX, 4 );
	//memcpy( m_pVertexList, vertices, sizeof(PNCT_VERTEX) * 4);

	/*SAFE_NEW_ARRAY( m_pIndexList, DWORD, 6 );
	m_pIndexList[0] =	0;
	m_pIndexList[1] =	1;
	m_pIndexList[2] =	3;
	m_pIndexList[3] =	1;
	m_pIndexList[4] =	2;
	m_pIndexList[5] =	3;*/
	return true;
	
}
void TSphereShape::MakeSpherePoint(std::vector<D3DXVECTOR3> &spherePoints,
							UINT Slices,
							double fRadius, D3DXVECTOR3 center)
{
    
    spherePoints.clear();
	spherePoints.resize(Slices* Slices);

	for (DWORD j= 0; j < Slices; j ++)
    {
        FLOAT theta = (D3DX_PI*j)/(Slices);
        for( DWORD i=0; i< Slices; i++ )
        {
            int  iPos = j*Slices+i;
            FLOAT phi = (2*D3DX_PI*i)/(Slices);			
            spherePoints[iPos].x = fRadius * (float)(sin(theta)*cos(phi))	+ center.x;
            spherePoints[iPos].z = fRadius * (float)(sin(theta)*sin(phi))	+ center.y;
            spherePoints[iPos].y = fRadius * (float)(cos(theta))			+ center.z;		
        }
    }
}


void TSphereShape::CreateSphere(  UINT Slices, float fRadius)
{
	const double PI = 3.141592653589793238462643383279502884197;
    int iPos = 0;
	vector<D3DXVECTOR3> VertexArray;	
	MakeSpherePoint( VertexArray, Slices, fRadius, D3DXVECTOR3(0,0,0));  

    int iNext = 0;
    for (DWORD j= 0; j < Slices; j ++)
    { 
        for( DWORD i=0; i<Slices; i++ )
        {
            if (i == Slices - 1)         iNext = 0;
            else iNext = i +1;
			PNCT_VERTEX v0, v1,v2, v3, v4,v5;
            iPos = (j*Slices*6)+(i*6);

            v0.p = VertexArray[j*Slices+i];
            v1.p = VertexArray[j*Slices+iNext];

            if (j != Slices -1) v2.p = VertexArray[((j+1)*Slices)+i];
            else                v2.p = D3DXVECTOR3( 0, -1, 0); 
		
			v3.p = v2.p;
            v4.p = v1.p;

            if (j != Slices - 1) v5.p = VertexArray[((j+1)*Slices)+iNext];
            else                 v5.p = D3DXVECTOR3( 0,-1,0);

			D3DXVec3Normalize(&v0.n, &v0.p);
			D3DXVec3Normalize(&v1.n, &v1.p);
			D3DXVec3Normalize(&v2.n, &v2.p);
            D3DXVec3Normalize(&v3.n, &v3.p);
			D3DXVec3Normalize(&v4.n, &v4.p);
			D3DXVec3Normalize(&v5.n, &v5.p);

			v0.c = D3DXVECTOR4( v0.n.x, v0.n.y, v0.n.z,1.0f );
			v1.c = D3DXVECTOR4( v1.n.x, v1.n.y, v1.n.z,1.0f );
			v2.c = D3DXVECTOR4( v2.n.x, v2.n.y, v2.n.z,1.0f );
			v3.c = D3DXVECTOR4( v3.n.x, v3.n.y, v3.n.z,1.0f );
			v4.c = D3DXVECTOR4( v4.n.x, v4.n.y, v4.n.z,1.0f );
			v5.c = D3DXVECTOR4( v5.n.x, v5.n.y, v5.n.z,1.0f );

			// 구 텍스처 좌표
			v0.t = D3DXVECTOR2( atan2(v0.n.z, v0.n.x)/(PI*2.0f)  + 0.5f,  asinf(-v0.n.y)/PI + 0.5f );
			v1.t = D3DXVECTOR2( atan2(v1.n.z, v1.n.x)/(PI*2.0f)  + 0.5f,  asinf(-v1.n.y)/PI + 0.5f );
			v2.t = D3DXVECTOR2( atan2(v2.n.z, v2.n.x)/(PI*2.0f)  + 0.5f,  asinf(-v2.n.y)/PI + 0.5f );
			v3.t = D3DXVECTOR2( atan2(v3.n.z, v3.n.x)/(PI*2.0f)  + 0.5f,  asinf(-v3.n.y)/PI + 0.5f );
			v4.t = D3DXVECTOR2( atan2(v4.n.z, v4.n.x)/(PI*2.0f)  + 0.5f,  asinf(-v4.n.y)/PI + 0.5f );
			v5.t = D3DXVECTOR2( atan2(v5.n.z, v5.n.x)/(PI*2.0f)  + 0.5f,  asinf(-v5.n.y)/PI + 0.5f );
			
			// 구형 환경 맵( 전방만 정상적임 )
		/*	v0.t = D3DXVECTOR2( (v0.n.x + 1) / 2.0f,  (-v0.n.y+1) / 2.0f );
			v1.t = D3DXVECTOR2( (v1.n.x + 1) / 2.0f,  (-v1.n.y+1) / 2.0f);
			v2.t = D3DXVECTOR2( (v2.n.x + 1) / 2.0f,  (-v2.n.y+1) / 2.0f);
			v3.t = D3DXVECTOR2( (v3.n.x + 1) / 2.0f,  (-v3.n.y+1) / 2.0f);
			v4.t = D3DXVECTOR2( (v4.n.x + 1) / 2.0f,  (-v4.n.y+1) / 2.0f);
			v5.t = D3DXVECTOR2( (v5.n.x + 1) / 2.0f,  (-v5.n.y+1) / 2.0f);
		*/
			//////////////////////////////////////
			//   V0.t.x( 0.94f) ---- V1.t.x(0.0f) 일 경우에 
			//   v1.t.x= 1.0f으로 조정한다.
			//////////////////////////////////////
			if( fabs(v1.t.x - v0.t.x) > 0.9f)
			{
				if( fabs(v1.t.x) <= 0.01f )
				{
					v1.t.x = 1.0f;
				}
				if( fabs(v0.t.x) <= 0.01f )
				{
					v0.t.x = 1.0f;
				}
			}
			if( fabs(v2.t.x - v0.t.x) > 0.9f)
			{
				if( fabs(v2.t.x) <= 0.01f )
				{
					v2.t.x = 1.0f;
				}
				if( fabs(v0.t.x) <= 0.01f )
				{
					v0.t.x = 1.0f;
				}
			}
			if( fabs(v4.t.x - v3.t.x) > 0.9f)
			{
				if( fabs(v4.t.x) <= 0.01f )
				{
					v4.t.x = 1.0f;
				}
				if( fabs(v3.t.x) <= 0.01f )
				{
					v3.t.x = 1.0f;
				}
			}
			if( fabs(v5.t.x - v3.t.x) > 0.9f)
			{
				if( fabs(v5.t.x) <= 0.01f )
				{
					v5.t.x = 1.0f;
				}
				if( fabs(v3.t.x) <= 0.01f )
				{
					v3.t.x = 1.0f;
				}
			}
			m_VertexList.push_back(v0);
			m_VertexList.push_back(v1);
			m_VertexList.push_back(v2);
			m_VertexList.push_back(v3);
			m_VertexList.push_back(v4);
			m_VertexList.push_back(v5);          
        }
    }
	VertexArray.clear();
}

HRESULT TSphereShape::CreateVertexBuffer()
{
	HRESULT hr;
	
	if( FAILED( hr = m_BufferVB.Create(	m_pd3dDevice,
												sizeof( PNCT_VERTEX), 
												m_VertexList.size(), 
												&m_VertexList.at(0), 
												D3D11_BIND_VERTEX_BUFFER) ))
	{
		return hr;
	} 
	m_iNumVertex = m_VertexList.size();
	m_VertexList.clear();
	return S_OK;
}
HRESULT TSphereShape::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	/*if( FAILED( hr = m_BufferIB.Create(	m_pd3dDevice,	sizeof( DWORD), 6, 
										m_pIndexList, 	D3D11_BIND_INDEX_BUFFER ) ))
	{
		return hr;
	}	*/
	return hr;
}
bool TSphereShape::Render()
{
	//--------------------------------------------------------------------------------------
	// 인풋레이아웃 바인딩
	//--------------------------------------------------------------------------------------
	m_InputLayout.Apply( m_pImmediateContext );
	//--------------------------------------------------------------------------------------
	// 버텍스 및 인덱스 버퍼 바인딩
	//--------------------------------------------------------------------------------------
	UINT stride = sizeof( PNCT_VERTEX );
    UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_BufferVB.m_pBuffer, &stride, &offset );
	//m_pImmediateContext->IASetIndexBuffer( m_BufferIB.m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
	//--------------------------------------------------------------------------------------
	// 월드 행렬 세팅
	//--------------------------------------------------------------------------------------
	if(m_pWorldVariable)	m_pWorldVariable->SetMatrix( ( float* )&m_matWorld );	
	if(m_pViewVariable)		m_pViewVariable->SetMatrix( ( float* )&m_matView );			// 뷰  행렬
	if(m_pProjVariable)		m_pProjVariable->SetMatrix( ( float* )&m_matProj );// 투영행렬

	ID3DX11EffectTechnique*		pTechnique = m_pTechniqueApply;
	if( !m_Texture.m_pTextureRV && m_pTechniqueOnlyAmbientColor )
	{	
		pTechnique = m_pTechniqueOnlyAmbientColor;
		if(m_pTextureVariable)	m_pTextureVariable->SetResource( NULL ); 
	}
	else
	{
		if(m_pTextureVariable)	m_pTextureVariable->SetResource( m_Texture.m_pTextureRV ); 
	}

	// 테크닉 정보 
	D3DX11_TECHNIQUE_DESC techDesc;				
    pTechnique->GetDesc( &techDesc );	
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        pTechnique->GetPassByIndex( p )->Apply( 0,m_pImmediateContext);
        //m_pImmediateContext->DrawIndexed( 6, 0, 0 ); 
		m_pImmediateContext->Draw( m_iNumVertex, 0 ); 
    }
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TSphereShape::CreateResource()
{
	m_DxState.SetRasterizerState( m_pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
	m_DxState.SetPrimitive(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return S_OK;
}
TSphereShape::TSphereShape(void)
{
}

TSphereShape::~TSphereShape(void)
{
}