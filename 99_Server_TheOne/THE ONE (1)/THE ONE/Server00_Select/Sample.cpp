#include "Select.h"

int main()
{
	I_Select.Set(10000);
	if (I_Select.Init() == false)
	{
		printf("���� �ʱ�ȭ ����\n");
		return 0;
	}
	if (I_Select.Run() == false)
	{
		printf("������ ������ ����Ǿ����ϴ�.\n");
		return 0;
	}
	printf("������ ���� ����Ǿ����ϴ�.\n");
	I_Select.Release();

	return 0;
}