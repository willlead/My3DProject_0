#include "WCore.h"
bool     WCore::Init()
{
	return true;
}; // �ʱ�ȭ
bool     WCore::Frame() {
	return true;
};
bool     WCore::Render() {
	return true;
};
bool     WCore::Release() {
	return true;
}
void WCore::MsgEvent(MSG msg)
{
}
bool WCore::GameInit() {
	Init();
	return true;
}
bool WCore::GameRun() {
	GameFrame();
	GameRender();
	return true;
}
bool WCore::GameFrame() {
	Frame();
	return true;
}
bool WCore::GameRender() {
	Render();
	return true;
}
bool WCore::GameRelease() {
	Release();
	return true;
}
bool WCore::Run()
{
	GameInit();
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		// �޼��� ť���� �޼��� 1���� �����´�.
		// �޼����� �������� ���� ���� ����.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// Ű �ؼ�
			TranslateMessage(&msg);
			// ������ �޼����� ���ν����� ����.
			DispatchMessage(&msg);

			MsgEvent(msg);
		}
		else {
			//Sleep(1);
			GameRun();
		}
	}
	GameRelease();
	return true;
}


WCore::WCore()
{
}


WCore::~WCore()
{
}
