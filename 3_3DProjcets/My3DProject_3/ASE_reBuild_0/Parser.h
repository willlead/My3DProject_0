#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TBasisStd.h"
#include "TModel.h"
//#include "TMesh.h"

static TCHAR* AseSections[] = 
{ 
	_T("SCENE"), 
	_T("MATERIAL_LIST"), 
	_T("GEOMOBJECT"),
	_T("HELPEROBJECT"), 
};
enum AseSectionType{ SCENE =0, MATERIALLIST, GEOMOBJECT, HELPEROBJECT,};

static TCHAR* AseMaterialTokens[] = 
{ 
	_T("NUMSUMBMTLS"),
	_T("MAP_"), 
	_T("}"), 
};
enum AseMaterialType { NUM_SUBMTLS = 0, MAP, };

static TCHAR* AseTextureTokens[] = 
{ 
	_T("BITMAP"), 
	_T("}"), 
};
enum AseTexterType {BITMAP_FILE  = 0,};

static TCHAR* AseGeomObjectTokens[] =
{
	_T("NODE_PARENT"),
	_T("NODE_TM"),
	_T("MESH {"),
	_T("TM_ANIMATION"),
	_T("NODE_VISIBILITY_TRACK"),
	_T("MATERIAL_REF"),

	_T("GEOMOBJECT"),
	_T("HELPEROBJECT"),
};
enum AseGeomObjectType 
{
	NODE_PARENT = 0, MODE_TM,
	MESH, TM_ANIMATION, TM_VISIBILITY, MATERIAL_REF,
};

static TCHAR* AseHelperObjectTokens[] =
{
	_T("NODE_PARNET"),
	_T("HELPER_CLASS"),
	_T("NODE_TM"),
	_T("BOUNDINGBOX_MIN"),
	_T("TM_ANIMATION"),

	_T("GEOMOBJECT"),
	_T("HELPEROBJECT"),
};
enum AseHelperObjectType
{
	HELPER_NODE_PARENT = 0, HELPER_CLASS, HELPER_MODE_TM,
	BOUNDINGBOX_MIN, HELPER_TM_ANIMATIOIN,
};

enum AseTrackType
{
	POS_SAMPLE_TRACK = 40, ROT_SAMPLE_TRACK, SCL_SAMPLE_TRACK, VIS_SAMPLE_TRACK
};


class Parser
{
public:
	Parser();
	virtual ~Parser();

public:
	FILE*	m_pFile;
	TCHAR	m_szBuffer[MAX_PATH]; // ¶ç¾î¾²±â , ÅÇ 
	TCHAR	m_szWord[MAX_PATH]; // ¿öµå
public:
	bool	Init();
	bool	Open(TCHAR* szFileName);
	bool	Close();

	bool	FindWord(TCHAR* szFindWord);
	INT	FindWord2(TCHAR* szFindWord, INT index);
	bool	FindWordSection();
public:
	virtual	bool	Read() = 0;
};

