#pragma once
#include "TBasis.h"

enum SET_DATA_TYPES
{
	// Gameplay semantics
	NULL_DATA	= 0,
	BOOL_DATA,
	INT_DATA,
	FLOAT_DATA,
	STRING_DATA,
	VERTEX_DATA,
	VECTOR_DATA,
	INDEX_DATA,
	FACE_DATA,
	MATRIX_DATA,
};

class TParser
{
public:

public:
	FILE*	m_pStream;
public:
	TCHAR	m_pBuffer[256];
	TCHAR	m_pString[256];
public:
	BOOL	ComparisonData		(	TCHAR* pDestString, TCHAR* pSrcString = NULL );		
	bool	ErrorCloseStream	(	TCHAR* pMsg		);		
	BOOL	OpenStream			(	const TCHAR* pFileName );
	BOOL    CloseStream()		{	fclose( m_pStream); return TRUE; };

	
	BOOL	GetData( VOID* pData, INT DataType);
	// ��ü���Ͽ��� �ش� ���ڿ��� ã���� ���� �˻��ϰ� ���������͸� ������������ �ű��.
	BOOL	GetDataFromFileLoop	(	TCHAR* pString = NULL, VOID* pData = NULL,INT DataType = NULL_DATA);
	// ���������� ������� ���忭�� ã�� ������������ ���������͸� �ű��.
	BOOL	GetDataFromFileNext	(	TCHAR* pString = NULL, VOID* pData = NULL,INT DataType = NULL_DATA);
	// ���ϴ� ���ڿ��� ã�� ã�� ���ڿ� ������ġ�� �ű��
	BOOL	GetDataFromFileString	(	TCHAR* pString = NULL, VOID* pData = NULL,INT DataType = NULL_DATA  );
	// ���������� ������� �ش繮�ڿ��� ã�� �˻��� �����ϸ� ���� �������� ���������͸� �ű��.
	BOOL	GetDataFromFile		(	TCHAR* pString = NULL, VOID* pData = NULL,INT DataType = NULL_DATA);
	BOOL    GetNodeName			(   TCHAR* pString = NULL, VOID* pData = NULL,INT DataType = NULL_DATA);

public:
	//-----------------------------------------------------------------------------
	// UNICODE support for converting between CHAR, TCHAR, and WCHAR strings
	//-----------------------------------------------------------------------------
	void	ConvertString( TCHAR_STRING& szReturn, TCHAR* szSource )
	{
		CHAR strDest[256] = {0,};
		WCHAR strConvert[256] = _T("");
		wcstombs( strDest, szSource, 256 );		
		ConvertAnsiStringToWideCch( strConvert, strDest, 256 );
		szReturn = strConvert;
	}
	HRESULT ConvertAnsiStringToWideCch( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar );
	HRESULT ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar );
	HRESULT ConvertGenericStringToAnsiCch( CHAR* strDestination, const TCHAR* tstrSource, int cchDestChar );
	HRESULT ConvertGenericStringToWideCch( WCHAR* wstrDestination, const TCHAR* tstrSource, int cchDestChar );
	HRESULT ConvertAnsiStringToGenericCch( TCHAR* tstrDestination, const CHAR* strSource, int cchDestChar );
	HRESULT ConvertWideStringToGenericCch( TCHAR* tstrDestination, const WCHAR* wstrSource, int cchDestChar );
	HRESULT ConvertAnsiStringToWideCb( WCHAR* wstrDestination, const CHAR* strSource, int cbDestChar );
	HRESULT ConvertWideStringToAnsiCb( CHAR* strDestination, const WCHAR* wstrSource, int cbDestChar );
	HRESULT ConvertGenericStringToAnsiCb( CHAR* strDestination, const TCHAR* tstrSource, int cbDestChar );
	HRESULT ConvertGenericStringToWideCb( WCHAR* wstrDestination, const TCHAR* tstrSource, int cbDestChar );
	HRESULT ConvertAnsiStringToGenericCb( TCHAR* tstrDestination, const CHAR* strSource, int cbDestChar );
	HRESULT ConvertWideStringToGenericCb( TCHAR* tstrDestination, const WCHAR* wstrSource, int cbDestChar );
	HRESULT DXUtil_ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 		int cchDestChar );
	//-----------------------------------------------------------------------------
	// GUID to String converting 
	//-----------------------------------------------------------------------------
	HRESULT ConvertGUIDToStringCch( const GUID* pGuidSrc, TCHAR* strDest, int cchDestChar );
	HRESULT ConvertGUIDToStringCb( const GUID* pGuidSrc, TCHAR* strDest, int cbDestChar );
	HRESULT ConvertStringToGUID( const TCHAR* strIn, GUID* pGuidOut );
public:
	TParser(void);
	virtual ~TParser(void);
};
