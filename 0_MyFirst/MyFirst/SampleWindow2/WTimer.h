#pragma once
#include "WBase.h"
class WTimer : public WBase
{
public:
	bool     Init(); // �ʱ�ȭ
	bool     Frame();// ���
	bool     Render();// ��ο�
	bool     Release();// ����, �Ҹ�
public:
	WTimer();
	virtual ~WTimer();
};

