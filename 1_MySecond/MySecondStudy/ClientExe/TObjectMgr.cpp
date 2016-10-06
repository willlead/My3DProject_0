#include "TObjectMgr.h"

bool		TObjectMgr::Init()
{
	return true;
}
bool		TObjectMgr::Frame()
{
	return true;
}
bool		TObjectMgr::Render()
{
	return true;
}
bool		TObjectMgr::Release()
{
	return true;
}

bool TObjectMgr::Collision(TObject& pSrc, TObject& pDesc)
{
	return true;
}
bool TObjectMgr::Collision(TObject& pSrc, POINT ptPos)
{
	return true;
}

TObjectMgr::TObjectMgr()
{
}


TObjectMgr::~TObjectMgr()
{
}
