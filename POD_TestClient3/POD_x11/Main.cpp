#include "stdafx.h"
#include "Main.h"

// ���� �ν��Ͻ�
cCam gCam;

// �� ����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// ��Ÿ�� �޸� üũ(����� ���)
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//��Ʈ��ũ �ʱ�ȭ 
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// �޸� ���� �˻�
	//_CrtSetBreakAlloc(154);

	// ���� Ŭ���� �ν��Ͻ� ����
	cEngine Application(hInstance);

	// ���� Ŭ���� �ʱ�ȭ
	if (!Application.Init())
		return 0;

	// ���� Ŭ���� ����
	return Application.Run();
}
