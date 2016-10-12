#pragma once
#include "GBasisStd.h"

enum SET_DATA_TYPES
{
	// Gameplay semantics
	NULL_DATA = 0,
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

class GParser
{
public:
	FILE*	m_pStream;
	TCHAR	m_pBuffer[256];
	TCHAR	m_pString[256];
public:
	BOOL	ComparisonData(TCHAR* pDestString, TCHAR* pSrcString = NULL);
	bool	ErrorCloseStream(TCHAR* pMsg);
	BOOL	OpenStream(const TCHAR* pFileName);
	BOOL    CloseStream() { fclose(m_pStream); return TRUE; };
	BOOL	GetData(VOID* pData, INT DataType);
	// ��ü���Ͽ��� �ش� ���ڿ��� ã���� ���� �˻��ϰ� ���������͸� ������������ �ű��.
	BOOL	GetDataFromFileLoop(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	// ���������� ������� ���忭�� ã�� ������������ ���������͸� �ű��.
	BOOL	GetDataFromFileNext(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	// ���ϴ� ���ڿ��� ã�� ã�� ���ڿ� ������ġ�� �ű��
	BOOL	GetDataFromFileString(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	// ���������� ������� �ش繮�ڿ��� ã�� �˻��� �����ϸ� ���� �������� ���������͸� �ű��.
	BOOL	GetDataFromFile(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	BOOL    GetNodeName(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
public:
	GParser(void);
	virtual ~GParser(void);
};
