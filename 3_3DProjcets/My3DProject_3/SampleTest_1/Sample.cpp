#include "Sample.h"

bool Sample::Open(char* file)
{
	fopen_s(&m_pFile,file, "rt");
	// t�� �������� ��Ƽ���� �Ǵ��ؼ� �ڵ����� �Ѱ���

	return true;
}

bool Sample::Open(TCHAR* file)
{
	//fp = _tfopen(file, _T("rt"));
	_tfopen_s(&m_pFile,file, _T("rt"));
	// t�� �������� ��Ƽ���� �Ǵ��ؼ� �ڵ����� �Ѱ���

	_fgetts(m_szBuffer, MAX_PATH, m_pFile);
	// ���� �о� ���� 
	//TCHAR	m_szBuffer[MAX_PATH];

	_stscanf(m_szBuffer, _T("%s"), m_szWord);
	// �� �ٿ��� ù��° ���ڸ� �о szWord�� ����

	//m_pFile = m_pFile + 1;
	_fgetts(m_szBuffer, MAX_PATH, m_pFile);
	_stscanf(m_szBuffer, _T("%s"), m_szWord);
	// �� �ٿ��� ù��° ���ڸ� �о szWord�� ����


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
