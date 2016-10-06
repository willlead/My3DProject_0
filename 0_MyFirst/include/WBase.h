#pragma once
#include "WStd.h"
class WBase
{
public:
	virtual bool     Init() = 0; // 초기화
	virtual bool     Frame() = 0;// 계산
	virtual bool     Render() = 0;// 드로우
	virtual bool     Release() = 0;// 삭제, 소멸
public:
	WBase();
	virtual ~WBase();
};

