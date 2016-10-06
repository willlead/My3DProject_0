#include "TTextureMgr.h"

int TTextureMgr::Load( T_STR fileName )
{
	// �ߺ� ����
	TCHAR Dirve[MAX_PATH] = {0, };
	TCHAR Dir[MAX_PATH] = {0, };
	TCHAR Name[MAX_PATH] = {0, };
	TCHAR Ext[MAX_PATH] = {0, };
	TCHAR szFileName[MAX_PATH] = {0, };

	if( !fileName.empty() )
	{
		_tsplitpath_s( fileName.c_str(),
			Dirve, Dir, Name, Ext );
		_stprintf_s( szFileName, _T("%s%s"),
			Name, Ext );
		
		TTexture* pImage;
		for( TItor itor = m_Map.begin(); 
		 itor != m_Map.end();
		 itor++ )
		{
			pImage = (TTexture*)(*itor).second;
			
			if( !_tcsicmp(pImage->m_strName.c_str(),
				szFileName ))
			{
				return (*itor).first;
			}
		}


	}
	TTexture* pImage = new TTexture;
	if( pImage == NULL ) return -1;
	pImage->Init();
	pImage->m_strName = szFileName;
	pImage->Load(fileName );

	m_Map.insert( make_pair(++m_iCurIndex,
						    pImage) );
	return m_iCurIndex;

}
TTexture* TTextureMgr::GetPtr( int iIndex )
{
	TItor itor = m_Map.find( iIndex );
	if( itor == m_Map.end() ) return NULL;
	TTexture* pImage = (*itor).second;
	return pImage;
}
TTextureMgr::TTextureMgr(void)
{
	m_iCurIndex = 0;
}

bool	TTextureMgr::Init()
{
	return true;
}

bool	TTextureMgr::Frame(){
	return true;
}
bool	TTextureMgr::Render(){
	return true;
}
bool	TTextureMgr::Release(){
	TTexture* pImage;
	for( TItor itor = m_Map.begin(); 
		 itor != m_Map.end();
		 itor++ )
	{
		pImage = (TTexture*)(*itor).second;
		pImage->Release();
		delete pImage;
	}
	m_Map.clear();
	return true;
}
TTextureMgr::~TTextureMgr(void)
{
}
