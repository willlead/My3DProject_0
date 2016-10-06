#include ".\tregister.h"


//1>c:\00_tbasis\tbasissample10.root\tbasis3d10\tbasislib_0\tregister.cpp(86) : warning C4996: 'wcsncpy': This function or variable may be unsafe. Consider using wcsncpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
//1>        c:\program files\microsoft visual studio 9.0\vc\include\string.h(275) : 'wcsncpy' 선언을 참조하십시오.

VOID TRegister::ReadSettings()
{
	HKEY hkey;
	if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, BASIS_APP_KEY, 
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
	{
		// TODO: change as needed

		// Read the stored window width/height.  This is just an example,
		// of how to use Read*() functions.
		ReadIntRegKey( hkey, TEXT("Width"), &m_dwCreationWidth, m_dwCreationWidth );
		ReadIntRegKey( hkey, TEXT("Height"), &m_dwCreationHeight, m_dwCreationHeight );		
		ReadStringRegKeyCch( hkey, TEXT("BASIS_INFO"), m_szDefaultResPath, MAX_PATH, TEXT("\0") );

		TCHAR* strLastSlash = NULL;
		strLastSlash = _tcsrchr( m_szDefaultResPath, TEXT('\\') );
		if( strLastSlash )
			*strLastSlash = 0;
		lstrcat( m_szDefaultResPath, TEXT("\\") );
		lstrcat( m_szDefaultResPath, TEXT("Data") );
		RegCloseKey( hkey );
	}
}




//-----------------------------------------------------------------------------
// Name: WriteSettings()
// Desc: Write the app settings to the registry
//-----------------------------------------------------------------------------
VOID TRegister::WriteSettings()
{
	HKEY hkey;	
	if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, BASIS_APP_KEY, 
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
	{
		// TODO: change as needed

		// Write the window width/height.  This is just an example,
		// of how to use Write*() functions.
		WriteIntRegKey( hkey, TEXT("Width"), m_rcWindowClient.right + m_rcWindowClient.left );
		WriteIntRegKey( hkey, TEXT("Height"),m_rcWindowClient.bottom + m_rcWindowClient.top );			
		RegCloseKey( hkey );
	}
}

//-----------------------------------------------------------------------------
// Name: WriteSettings()
// Desc: Write the app settings to the registry
//-----------------------------------------------------------------------------
VOID TRegister::WritePathSettings( TCHAR* strPath )
{
	HKEY hkey;

	if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, BASIS_APP_KEY, 
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
	{			
		if( SUCCEEDED(WriteStringRegKey( hkey, _T("BASIS_INFO"), strPath )))
		{
			INT KKK = 0;
		}
		RegCloseKey( hkey );
	}
}


//-----------------------------------------------------------------------------
// Name: ReadStringRegKeyCch()
// Desc: Helper function to read a registry key string
//       cchDest is the size in TCHARs of strDest.  Be careful not to 
//       pass in sizeof(strDest) on UNICODE builds.
//-----------------------------------------------------------------------------
HRESULT TRegister::ReadStringRegKeyCch( HKEY hKey, TCHAR* strRegName, TCHAR* strDest, 
										DWORD cchDest, TCHAR* strDefault )
{
	DWORD dwType;
	DWORD cbDest = cchDest * sizeof(TCHAR);

	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(BYTE*)strDest, &cbDest ) )
	{
		//wcsncpy_s
		_tcsncpy( strDest, strDefault, cchDest );
		strDest[cchDest-1] = 0;

		if( dwType != REG_SZ )
			return E_FAIL;

		return S_OK;
	}

	return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: WriteStringRegKey()
// Desc: Helper function to write a registry key string
//-----------------------------------------------------------------------------
HRESULT TRegister::WriteStringRegKey( HKEY hKey, TCHAR* strRegName,
									  TCHAR* strValue )
{
	if( NULL == strValue )
		return E_INVALIDARG;

	DWORD cbValue = ((DWORD)_tcslen(strValue)+1) * sizeof(TCHAR);

	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_SZ, 
		(BYTE*)strValue, cbValue ) )
		return E_FAIL;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReadIntRegKey()
// Desc: Helper function to read a registry key int
//-----------------------------------------------------------------------------
HRESULT TRegister::ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwDest, 
								  DWORD dwDefault )
{
	DWORD dwType;
	DWORD dwLength = sizeof(DWORD);

	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(BYTE*)pdwDest, &dwLength ) )
	{
		*pdwDest = dwDefault;
		if( dwType != REG_DWORD )
			return E_FAIL;

		return S_OK;
	}

	return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: WriteIntRegKey()
// Desc: Helper function to write a registry key int
//-----------------------------------------------------------------------------
HRESULT TRegister::WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue )
{
	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
		(BYTE*)&dwValue, sizeof(DWORD) ) )
		return E_FAIL;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReadBoolRegKey()
// Desc: Helper function to read a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT TRegister::ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbDest, 
								   BOOL bDefault )
{
	DWORD dwType;
	DWORD dwLength = sizeof(BOOL);

	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(BYTE*)pbDest, &dwLength ) )
	{
		*pbDest = bDefault;
		if( dwType != REG_DWORD )
			return E_FAIL;

		return S_OK;
	}

	return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: WriteBoolRegKey()
// Desc: Helper function to write a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT TRegister::WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue )
{
	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
		(BYTE*)&bValue, sizeof(BOOL) ) )
		return E_FAIL;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReadGuidRegKey()
// Desc: Helper function to read a registry key guid
//-----------------------------------------------------------------------------
HRESULT TRegister::ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidDest, 
								   GUID& guidDefault )
{
	DWORD dwType;
	DWORD dwLength = sizeof(GUID);

	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(LPBYTE) pGuidDest, &dwLength ) )
	{
		*pGuidDest = guidDefault;
		if( dwType != REG_BINARY )
			return E_FAIL;

		return S_OK;
	}

	return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: WriteGuidRegKey()
// Desc: Helper function to write a registry key guid
//-----------------------------------------------------------------------------
HRESULT TRegister::WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue )
{
	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_BINARY, 
		(BYTE*)&guidValue, sizeof(GUID) ) )
		return E_FAIL;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: BYTE helper functions
// Desc: cchDestChar is the size in BYTEs of strDest.  Be careful not to 
//       pass use sizeof() if the strDest is a string pointer.  
//       eg.
//       TCHAR* sz = new TCHAR[100]; // sizeof(sz)  == 4
//       TCHAR sz2[100];             // sizeof(sz2) == 200
//-----------------------------------------------------------------------------
HRESULT TRegister::ConvertAnsiStringToWideCb( WCHAR* wstrDestination, const CHAR* strSource, int cbDestChar )
{
	return ConvertAnsiStringToWideCch( wstrDestination, strSource, cbDestChar / sizeof(WCHAR) );
}

HRESULT TRegister::ConvertWideStringToAnsiCb( CHAR* strDestination, const WCHAR* wstrSource, int cbDestChar )
{
	return ConvertWideStringToAnsiCch( strDestination, wstrSource, cbDestChar / sizeof(CHAR) );
}

HRESULT TRegister::ConvertGenericStringToAnsiCb( CHAR* strDestination, const TCHAR* tstrSource, int cbDestChar )
{
	return ConvertGenericStringToAnsiCch( strDestination, tstrSource, cbDestChar / sizeof(CHAR) );
}

HRESULT TRegister::ConvertGenericStringToWideCb( WCHAR* wstrDestination, const TCHAR* tstrSource, int cbDestChar )
{
	return ConvertGenericStringToWideCch( wstrDestination, tstrSource, cbDestChar / sizeof(WCHAR) );
}

HRESULT TRegister::ConvertAnsiStringToGenericCb( TCHAR* tstrDestination, const CHAR* strSource, int cbDestChar )
{
	return ConvertAnsiStringToGenericCch( tstrDestination, strSource, cbDestChar / sizeof(TCHAR) );
}

HRESULT TRegister::ConvertWideStringToGenericCb( TCHAR* tstrDestination, const WCHAR* wstrSource, int cbDestChar )
{
	return ConvertWideStringToGenericCch( tstrDestination, wstrSource, cbDestChar / sizeof(TCHAR) );
}

HRESULT TRegister::ReadStringRegKeyCb( HKEY hKey, TCHAR* strRegName, TCHAR* strDest, DWORD cbDest, TCHAR* strDefault )
{
	return ReadStringRegKeyCch( hKey, strRegName, strDest, cbDest / sizeof(TCHAR), strDefault );
}

HRESULT TRegister::ConvertGUIDToStringCb( const GUID* pGuidSrc, TCHAR* strDest, int cbDestChar )
{
	return ConvertGUIDToStringCch( pGuidSrc, strDest, cbDestChar / sizeof(TCHAR) );
}
HRESULT TRegister::ConvertStringToGUID( const TCHAR* strSrc, GUID* pGuidDest )
{
	UINT aiTmp[10];

	if( _stscanf( strSrc, TEXT("{%8X-%4X-%4X-%2X%2X-%2X%2X%2X%2X%2X%2X}"),
		&pGuidDest->Data1, 
		&aiTmp[0], &aiTmp[1], 
		&aiTmp[2], &aiTmp[3],
		&aiTmp[4], &aiTmp[5],
		&aiTmp[6], &aiTmp[7],
		&aiTmp[8], &aiTmp[9] ) != 11 )
	{
		ZeroMemory( pGuidDest, sizeof(GUID) );
		return E_FAIL;
	}
	else
	{
		pGuidDest->Data2       = (USHORT) aiTmp[0];
		pGuidDest->Data3       = (USHORT) aiTmp[1];
		pGuidDest->Data4[0]    = (BYTE) aiTmp[2];
		pGuidDest->Data4[1]    = (BYTE) aiTmp[3];
		pGuidDest->Data4[2]    = (BYTE) aiTmp[4];
		pGuidDest->Data4[3]    = (BYTE) aiTmp[5];
		pGuidDest->Data4[4]    = (BYTE) aiTmp[6];
		pGuidDest->Data4[5]    = (BYTE) aiTmp[7];
		pGuidDest->Data4[6]    = (BYTE) aiTmp[8];
		pGuidDest->Data4[7]    = (BYTE) aiTmp[9];
		return S_OK;
	}
}




//-----------------------------------------------------------------------------
// Name: DXUtil_ConvertGUIDToStringCch()
// Desc: Converts a GUID to a string 
//       cchDestChar is the size in TCHARs of strDest.  Be careful not to 
//       pass in sizeof(strDest) on UNICODE builds
//-----------------------------------------------------------------------------
HRESULT TRegister::ConvertGUIDToStringCch( const GUID* pGuidSrc, TCHAR* strDest, int cchDestChar )
{
	int nResult = _sntprintf( strDest, cchDestChar, TEXT("{%0.8X-%0.4X-%0.4X-%0.2X%0.2X-%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X}"),
		pGuidSrc->Data1, pGuidSrc->Data2, pGuidSrc->Data3,
		pGuidSrc->Data4[0], pGuidSrc->Data4[1],
		pGuidSrc->Data4[2], pGuidSrc->Data4[3],
		pGuidSrc->Data4[4], pGuidSrc->Data4[5],
		pGuidSrc->Data4[6], pGuidSrc->Data4[7] );

	if( nResult < 0 )
		return E_FAIL;
	return S_OK;
}

HRESULT TRegister::ConvertWideStringToGenericCch( TCHAR* tstrDestination, const WCHAR* wstrSource, 
												  int cchDestChar )
{
	if( tstrDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

#ifdef _UNICODE
	wcsncpy( tstrDestination, wstrSource, cchDestChar );
	tstrDestination[cchDestChar-1] = L'\0';    
	return S_OK;
#else
	return ConvertWideStringToAnsiCch( tstrDestination, wstrSource, cchDestChar );
#endif
}

HRESULT TRegister::ConvertAnsiStringToGenericCch( TCHAR* tstrDestination, const CHAR* strSource, 
												  int cchDestChar )
{
	if( tstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

#ifdef _UNICODE
	return ConvertAnsiStringToWideCch( tstrDestination, strSource, cchDestChar );
#else
	strncpy( tstrDestination, strSource, cchDestChar );
	tstrDestination[cchDestChar-1] = '\0';
	return S_OK;
#endif    
}


//-----------------------------------------------------------------------------
// Name: DXUtil_ConvertWideStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. 
//       cchDestChar is the size in TCHARs of strDestination
//-----------------------------------------------------------------------------
HRESULT TRegister::DXUtil_ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
										  int cchDestChar )
{
	if( strDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
		cchDestChar*sizeof(CHAR), NULL, NULL );
	strDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}

HRESULT TRegister::ConvertGenericStringToAnsiCch( CHAR* strDestination, const TCHAR* tstrSource, 
												  int cchDestChar )
{
	if( strDestination==NULL || tstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

#ifdef _UNICODE
	return DXUtil_ConvertWideStringToAnsiCch( strDestination, tstrSource, cchDestChar );
#else
	strncpy( strDestination, tstrSource, cchDestChar );
	strDestination[cchDestChar-1] = '\0';
	return S_OK;
#endif   
}




//-----------------------------------------------------------------------------
// Name: DXUtil_ConvertGenericStringToWide()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       WCHAR string. 
//       cchDestChar is the size in TCHARs of wstrDestination.  Be careful not to 
//       pass in sizeof(strDest) 
//-----------------------------------------------------------------------------
HRESULT TRegister::ConvertGenericStringToWideCch( WCHAR* wstrDestination, const TCHAR* tstrSource, 
												  int cchDestChar )
{
	if( wstrDestination==NULL || tstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

#ifdef _UNICODE
	wcsncpy( wstrDestination, tstrSource, cchDestChar );
	wstrDestination[cchDestChar-1] = L'\0';
	return S_OK;
#else
	return ConvertAnsiStringToWideCch( wstrDestination, tstrSource, cchDestChar );
#endif    
}







HRESULT TRegister::ConvertAnsiStringToWideCch( WCHAR* wstrDestination, const CHAR* strSource, 
											   int cchDestChar )
{
	if( wstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
		wstrDestination, cchDestChar );
	wstrDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DXUtil_ConvertWideStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. 
//       cchDestChar is the size in TCHARs of strDestination
//-----------------------------------------------------------------------------
HRESULT TRegister::ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
											   int cchDestChar )
{
	if( strDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
		cchDestChar*sizeof(CHAR), NULL, NULL );
	strDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}

 bool TRegister::Init()
 {
	 return true;
 }
 bool TRegister::Frame()
 {
	 return true;
 }
 bool TRegister::Render()
 {
	 return true;
 }
 bool TRegister::Release()
 {
	 return true;
 }	
 bool TRegister::ResetResource()
 {
	 return true;
 }
 bool TRegister::ResetDevice()
 {
	 return true;
 }

TRegister::TRegister(void)
{
	Init();
	_tcscpy( m_szDefaultResPath, TEXT("") );
	m_dwCreationWidth   = 1024;
	m_dwCreationHeight  = 768;
	TCHAR strPath[256];
	GetCurrentDirectory(256,strPath);
	WritePathSettings(strPath);
	ReadSettings();
}

TRegister::~TRegister(void)
{
	WriteSettings();
}
