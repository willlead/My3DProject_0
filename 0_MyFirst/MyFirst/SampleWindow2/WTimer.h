#pragma once
#include "WBase.h"
class WTimer : public WBase
{
public:
	bool     Init(); // 초기화
	bool     Frame();// 계산
	bool     Render();// 드로우
	bool     Release();// 삭제, 소멸
public:
	WTimer();
	virtual ~WTimer();
};

