// POD_Server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <ctime>
#define S_FPS 1.0f/60.0f

CIocpServer IocpServer;

int _tmain(int argc, _TCHAR* argv[])
{
	// winsock�� ����ϱ� ���� DLL �ε�

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ServerMGR->TerrainInit(&(ServerMGR->Terrain_info)); //�� �ʱ�ȭ
	MonsterMGR->Begin(); //���� �ʱ�ȭ

	DWORD ET = timeGetTime(); //����ð�
	DWORD DT = timeGetTime();//����ð�
	float fDt = 0.f;
	float fAccdt = 0.f;
	srand((unsigned)time(NULL));
	// ���� ����.
	if (IocpServer.Begin())
	{ 
		ServerMGR->Init();//���� �ʱ�ȭ
		while (1)
		{
			DT = timeGetTime() - ET; //���߽ð�- ����ð� =����ð�
			ET = timeGetTime();  //����ð� �ʱ�ȭ
			fDt = DT / 1000.0f; 
			fAccdt += fDt; //�ð� ������

			if (fAccdt > S_FPS){ 				
			/*	if (ServerMGR->bGame){
					for (int i = MAX_SESSION - 1; i >= 0; --i) {
						if (IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetConnected())
							IocpServer.GetConnectUserManager()->GetConnectUser(i)->Update(fAccdt);
					}*/
					for (int i = MAX_SESSION - 1; i >= 0; --i) {
						if (IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetUserStaus() == GAME_ING)
							IocpServer.GetConnectUserManager()->GetConnectUser(i)->Update(fAccdt);
					}
					MonsterMGR->Update(fAccdt); //���� ������Ʈ
				 
				fAccdt = 0.0f; 
			}
		}
		getchar();
		IocpServer.End();
	}
		ServerMGR->MemoryDelete(); //�����Ҵ� ����
		MonsterMGR->End(); //���� �����Ҵ� ����

		// ����� DLL�� ��ε� ��ŵ�ϴ�.
		WSACleanup();

		CoUninitialize();
		return 0;
	

}
