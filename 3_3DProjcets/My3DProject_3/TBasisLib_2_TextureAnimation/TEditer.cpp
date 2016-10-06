#include "TEditer.h"

int TEditer::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
        case WM_TIMER:        
			{
				Update();				
			}
            break;
		case WM_KEYDOWN:
        {
            switch( wParam )
            {				
                case VK_F12:
					{
						Create(m_strFileName.c_str(), hWnd);
					}break;
			}
		}break;
	}
	return -1;
}

bool TEditer::Create(TCHAR_STRING strLoadFile, HWND hWnd )
{
	if( m_NotePad_PI.hProcess != NULL ) 
	{
		return false;
	}
	m_hWnd = hWnd;

	TCHAR WindowPath[MAX_PATH];
	TCHAR CurrentPath[MAX_PATH];
	TCHAR ExecutePath[MAX_PATH];
	GetWindowsDirectory(WindowPath,MAX_PATH); 	
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	_tcscat( CurrentPath, L"\\" );
	_tcscat( CurrentPath, strLoadFile.c_str() );
	_tcscpy( ExecutePath, L"notepad.exe " );
	_tcscat( ExecutePath, CurrentPath );

	//WinExec("notepad.exe", SW_SHOW );
	//ShellExecute(gSample.m_hWnd,L"open",L"notepad",L"HLSL_0.fx",L"",SW_SHOWNORMAL);

	BOOL bRet;
	STARTUPINFO         si;
	DWORD               dwCode  =   0;
	ZeroMemory  (   &si,    sizeof  (   STARTUPINFO));
	si.cb           =   sizeof  (   STARTUPINFO);
	si.dwFlags      =   STARTF_USESIZE | STARTF_USEPOSITION | STARTF_USESHOWWINDOW;
	si.dwX			= 0;
	si.dwY			= 0;
	si.dwXSize		= 300;
	si.dwYSize		= 200;
	si.wShowWindow  =   SW_SHOWNORMAL;

	//Process를 실행시키는 명령어
	CreateProcess(NULL,	//lpApplicationName
				ExecutePath, //lpCommandLine
				NULL,	
				NULL,				
				TRUE, 
				DETACHED_PROCESS,
				NULL,
				WindowPath,
				&si,	
				&m_NotePad_PI);	
	//  차일드 프로세스가 초기화 될 때 까지 대기한다.
	WaitForInputIdle(m_NotePad_PI.hProcess, INFINITE ); 
	HWND hMemo= FindWindow( L"Notepad", NULL );
	RECT rt;
	GetWindowRect( hWnd, &rt );
	MoveWindow(hMemo, rt.left+30, rt.bottom-300, 
					  rt.right-rt.left-60, 
					  300, FALSE );

	m_strFileName = strLoadFile;
	GetFileAttributesEx( m_strFileName.c_str(), GetFileExInfoStandard, (void*)&m_fileAttData);	
	SetTimer( hWnd, 1, 1000, NULL );
	return true;
}
bool  TEditer::Update()
{
	_WIN32_FILE_ATTRIBUTE_DATA	fileAttData;
	GetFileAttributesEx( m_strFileName.c_str(), GetFileExInfoStandard, (void*)&fileAttData);				

	if( CompareFileTime(&fileAttData.ftLastWriteTime, &m_fileAttData.ftLastWriteTime) != 0 )
	{
		m_bReload = true;
		m_fileAttData = fileAttData;
		return true;
	}
	return false;	
}
bool TEditer::IsReload()
{
	if( m_bReload )
	{
		m_bReload = false;
		return true;
	}
	return false;
}
bool TEditer::Init()
{
	return true;
}

bool TEditer::Frame()
{
	if( WAIT_TIMEOUT != WaitForSingleObject( m_NotePad_PI.hProcess, 1 ) )
	{
		memset( &m_NotePad_PI, 0, sizeof(PROCESS_INFORMATION) );
	}
	return true;
}
bool TEditer::Release()
{
	KillTimer( m_hWnd, 1 );
	SafeTerminateProcess( m_NotePad_PI.hProcess, 0 );
	return true;
}
BOOL TEditer::SafeTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
  DWORD dwTID, dwCode, dwErr = 0;
  HANDLE hProcessDup = INVALID_HANDLE_VALUE;
  HANDLE hRT = NULL;
  HINSTANCE hKernel = GetModuleHandle(L"Kernel32");
  
  BOOL bSuccess = FALSE;
  BOOL bDup = DuplicateHandle(GetCurrentProcess(), 
                                  hProcess, 
                                  GetCurrentProcess(), 
                                  &hProcessDup, 
                                  PROCESS_ALL_ACCESS, 
                                  FALSE, 
                                  0);
  if ( GetExitCodeProcess((bDup) ? hProcessDup : hProcess, &dwCode) 
       && (dwCode == STILL_ACTIVE) )
  {
    FARPROC pfnExitProc;
    pfnExitProc = GetProcAddress(hKernel, "ExitProcess");
    hRT = CreateRemoteThread((bDup) ? hProcessDup : hProcess, 
                            NULL, 
                            0, 
                            (LPTHREAD_START_ROUTINE)pfnExitProc,
                            (PVOID)uExitCode, 0, &dwTID);
    if ( hRT == NULL ) dwErr = GetLastError();
  }
  else 
  {
       dwErr = ERROR_PROCESS_ABORTED;
   }
   if ( hRT )
  {
      WaitForSingleObject((bDup) ? hProcessDup : hProcess, INFINITE);
      CloseHandle(hRT);
      bSuccess = TRUE;
  }
  if ( bDup ) 
  CloseHandle(hProcessDup);
  if ( !bSuccess )
  SetLastError(dwErr);
  
return bSuccess;
}
TEditer::TEditer(void)
{
	m_bReload = false;
	memset( &m_NotePad_PI, 0, sizeof(PROCESS_INFORMATION) );
}

TEditer::~TEditer(void)
{
}
