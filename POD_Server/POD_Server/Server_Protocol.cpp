//***************************************************************************************
//
//	Protocol.cpp : ���� -> Ŭ�� ������ ���� �������� ���� �ҽ� ����.
//
//  <<�Լ������͹迭�� ����>> 
//   *�Լ������� �迭�� �������� �߰� �� �� �������� ���� ��ų��.
//
//***************************************************************************************

#include "stdafx.h"
extern CTerrain_Server* g_pTerrain;
extern CIocpServer IocpServer;
cPlayer_Info info;
UINT ClientID; //������ Ŭ���̾�Ʈ�� ���̵�
namespace ServerProtocol_Func{
	Func g_Protocol_Func[Server_Protocol::SPROTOCOL_FULL] =
	{
		ServerProtocol_Func::Connect_Complete_Func,
		ServerProtocol_Func::DisConnect,
		ServerProtocol_Func::Player_Push_W,
		ServerProtocol_Func::Player_Push_S,
		ServerProtocol_Func::Player_Push_A,
		ServerProtocol_Func::Player_Push_D
	}; 

	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{

		printf("Ŭ���̾�Ʈ �ε� �Ϸ� \n");
		/*ServerMGR->TerrainWidth = *(float*)(_pPacket);
		ServerMGR->TerrainDepth = *(float*)(_pPacket+sizeof(float));
		printf("Map Size : %f, %f\n", ServerMGR->TerrainWidth, ServerMGR->TerrainDepth);*/

		for (INT i = MAX_SESSION - 1; i >= 0; --i){
			if (IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetConnected()&&IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetUserStaus() == GAME_END)  //���� ĳ���� ������ �ȵǾ�������
			{
				IocpServer.GetConnectUserManager()->GetConnectUser(i)->Player_Init();//�÷��̾� ���� �� ����
				IocpServer.GetConnectUserManager()->GetConnectUser(i)->SetUserStaus(GAME_ING); //���ӻ��·� ������ش�.
				//ServerMGR->bGame = true; // ���� ���� ��ū
			}
		}
		//MonsterMGR->INIT(); //���� ����
		//IocpServer.GetConnectUserManager()->WriteAll(CPlAYER_INFO, (BYTE*)ClientID, sizeof(UINT)); //��� Ŭ���̾�Ʈ���� ������ Ŭ���� ���̵�(Ŭ���̾�Ʈ ��)�� �����ش�.

	}
	void DisConnect(BYTE* _pPacket, DWORD& _Length)
	{
		int PlayerID = 0;
		PlayerID = (*_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(PlayerID)->SetUserStaus(GAME_END); //����������·� ������ش�.
		delete IocpServer.GetConnectUserManager()->GetConnectUser(PlayerID); //�÷��̾� ������Ʈ ����
		printf("Ŭ���̾�Ʈ ���� ���� \n");

	}
	void Player_Push_W(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::UP);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::UP);
	}
	void Player_Push_S(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::DOWN);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::DOWN);
	}
	void Player_Push_A(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::LEFT);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::LEFT);
	}
	void Player_Push_D(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::RIGHT);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::RIGHT);
	}
}





/****************************** End of File "CSProcess.cpp" ******************************/