#pragma once
#include "WStd.h"
class WBase
{
public:
	virtual bool     Init() = 0; // �ʱ�ȭ
	virtual bool     Frame() = 0;// ���
	virtual bool     Render() = 0;// ��ο�
	virtual bool     Release() = 0;// ����, �Ҹ�
public:
	WBase();
	virtual ~WBase();
};

