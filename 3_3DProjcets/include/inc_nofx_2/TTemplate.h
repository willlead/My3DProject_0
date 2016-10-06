#pragma once
#include "TDefine.h"

namespace TBASIS_BASE {

template< typename T > void Swap( T* a, T* b )
{
	T Temp;
	Temp = *a;
	*a = *b;
	*b = Temp;
}

template<class T> class TSingleton
{		
public:
	static T& GetInstance() 
	{		
		static T theSingleInstance;  
		return theSingleInstance; 
	}
	static T* GetInstancePtr() 
	{
		static T theSingleInstance; 
		return &theSingleInstance; 
	}
};
template<typename TContainer>
inline void stl_wipe(TContainer& container)
{
	for( TContainer::iterator i = container.begin() ; i != container.end() ; ++i )
		delete *i;
	container.clear();
}

template<typename T>
void stl_wipe_vector(vector<T>& rVector)
{
	vector<T> emptyData;
	rVector.swap( emptyData );
}

template<class T> class TSINGLETON
{
private:
	static T* m_pInstance;
public:
	TSINGLETON()	{	m_pInstance = (T*)this; }
	~TSINGLETON()	{	m_pInstance = 0; }
	static T& GetInstance()		{	return (*m_pInstance ); }
	static T* GetInstancePtr()	{	return m_pInstance;		}
};
template <class T> T* TSINGLETON<T>::m_pInstance = 0;


////////////////////////////////////////////////////////
// �ش� ��ü�� sdl::map���� �����Ѵ�
////////////////////////////////////////////////////////
template< class Child > 
class TTemplateMap 
{
	public:	
		typedef map <int, Child*>					TemplateMap;
		typedef typename TemplateMap::iterator		TemplateMapItor;	
		TemplateMapItor								TItor;
		TemplateMap									TMap;
		int											m_iCurIndex;
	public:
		// �ʱ�ȭ �Ѵ�.
		virtual bool		Init();		
		// �ε����� ���Ͽ� ��ü�� ���Ϲ޴´�.
		virtual Child*		GetPtr( int index );
		// ��ü�̸����� ��ü�� ���Ϲ޴´�.
		virtual Child*		GetPtr( const TCHAR* szName );
		// ��ü �����ͷ� ����� �ε����� ���Ϲ޴´�.
		virtual int			GetID( Child* );
		// ��ü�̸����� ����� �ε����� ���Ϲ޴´�.
		virtual int			GetID( const TCHAR* szName );	
		// ��ü ����� ������ ��´�.
		virtual int			Count();	
		// ��ü ����� ��ü�� �����Ѵ�.
		virtual bool		Release();	
		// �ε����� �ش��ϴ� ��ü�� �����Ѵ�.
		virtual bool		Delete(int iIndex );
		// pPoint�� �ش��ϴ� ��ü�� �����Ѵ�.
		virtual bool		Delete(Child *pPoint );	
		virtual bool		Frame();
		virtual bool		Render();			
		
	public:
		TTemplateMap();
		virtual ~TTemplateMap();
};

template < class Child > 
int TTemplateMap< Child >::GetID( Child* pChild )
{
	int iIndex = -1;
	for( TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++) 
	{
		Child *pPoint = (Child *)(*itor).second;
		if( pChild == pPoint )  
		{
			iIndex =  (*itor).first;
			break;
		}
	}
	return iIndex;
}
template < class Child > 
int TTemplateMap< Child >::GetID( const TCHAR* szName )
{
	int iIndex = -1;
	for( TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++) 
	{
		Child *pPoint = (Child *)(*itor).second;		
		if( !_tcsicmp( pPoint->m_szName.c_str(), szName ) )
		{		
			iIndex =  (*itor).first;
			break;
		}
	}
	return iIndex;
}

template < class Child > 
bool TTemplateMap< Child >::Init()
{
	m_iCurIndex = 0;	
	TMap.clear();
	return true;
}
template < class Child > 
int TTemplateMap< Child >::Count()
{
	return (int)TMap.size();
}

template < class Child > 
bool TTemplateMap< Child >::Frame()
{
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++ ) 
	{
		Child *pPoint = (Child *)(*itor).second;
		if( pPoint )
			pPoint->Frame();
	}
	return true;
}
template < class Child > 
bool TTemplateMap< Child >::Render()
{
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++ ) 
	{
		Child *pPoint = (Child *)(*itor).second;
		if( pPoint )
			pPoint->Render();
	}
	return true;
}
template < class Child > 
bool TTemplateMap< Child >::Release()
{
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++ ) 
	{
		Child *pPoint = (Child *)(*itor).second;
		if( pPoint )
			pPoint->Release();
		else
			return false;
		delete pPoint;
	}
	TMap.clear();
	m_iCurIndex = 0;
	return true;
}
template < class Child > 
Child *TTemplateMap< Child >::GetPtr( int iIndex )
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return NULL;
	Child *pPoint = (*itor).second;	
	return pPoint;
}
template < class Child > 
Child* TTemplateMap< Child >::GetPtr( const TCHAR* szName )
{
	for( TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++) 
	{
		Child *pPoint = (Child *)(*itor).second;
		if( !_tcsicmp( pPoint->m_szName.c_str(), szName ) ) 
		{
			return pPoint;
		}
	}
	return NULL;
}
template < class Child > 
bool TTemplateMap< Child >::Delete(int iIndex )
{
	Child *pPoint = GetPtr( iIndex );
	if( pPoint )
	{
		pPoint->Release();	
		TMap.erase(iIndex);
	}
	return true;
}
template < class Child > 
bool TTemplateMap< Child >::Delete(Child *pPoint )
{	
	if( pPoint )
	{
		pPoint->Release();
		TMap.erase(GetID(pPoint));
	}
	return true;
}
template < class Child > 
TTemplateMap< Child >::TTemplateMap()
{
	m_iCurIndex = 0;	
	TMap.clear();
}

template < class Child > 
TTemplateMap< Child >::~TTemplateMap()
{
	Release();	
}
}