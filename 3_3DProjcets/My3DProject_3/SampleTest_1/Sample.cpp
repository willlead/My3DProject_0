#include "Sample.h"

bool Sample::Open(char* file)
{
	fopen_s(&m_pFile,file, "rt");
	// t는 유니인지 멀티인지 판단해서 자동으로 넘겨줌

	return true;
}

bool Sample::Open(TCHAR* file)
{
	//fp = _tfopen(file, _T("rt"));
	_tfopen_s(&m_pFile,file, _T("rt"));
	// t는 유니인지 멀티인지 판단해서 자동으로 넘겨줌

	_fgetts(m_szBuffer, MAX_PATH, m_pFile);
	// 한줄 읽어 오기 
	//TCHAR	m_szBuffer[MAX_PATH];

	_stscanf(m_szBuffer, _T("%s"), m_szWord);
	// 그 줄에서 첫번째 문자를 읽어서 szWord에 저장

	//m_pFile = m_pFile + 1;
	_fgetts(m_szBuffer, MAX_PATH, m_pFile);
	_stscanf(m_szBuffer, _T("%s"), m_szWord);
	// 그 줄에서 첫번째 문자를 읽어서 szWord에 저장


	return true;
}
bool Sample::Init()
{
	Open("../../data/obj/box2.txt");
	//Open(L"../../data/obj/box2.txt");
	Open(_T("../../data/fpTest_0.txt"));

	return true;
}
bool Sample::Frame()
{
	return true;
}
bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{
	return true;
}

Sample::Sample()
{
}


Sample::~Sample()
{
}

TBASIS_RUN(L"AseBox_Render_1")
