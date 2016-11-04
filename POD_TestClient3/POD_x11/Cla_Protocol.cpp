//***************************************************************************************
 //
//	Protocol.cpp : Ŭ�� -> ���� ������ ���� �������� ���� �ҽ� ����.
//
//  <<�Լ������͹迭�� ����>> 
//   *�Լ������� �迭�� �������� �߰� �� �� �������� ���� ��ų��.
//
//***************************************************************************************

#include "stdafx.h"

namespace ClientProtocol_Func{
	Func g_Protocol_Func[Client_Protocol::CPROTOCOL_FULL] =
	{
		ClientProtocol_Func::Connect_Complete_Func,
		ClientProtocol_Func::DisConnect,
		ClientProtocol_Func::Loading_Start_Func,
		ClientProtocol_Func::Player_Count,
		ClientProtocol_Func::Player_Info,
		ClientProtocol_Func::Player_Pos,
		ClientProtocol_Func::Monster_Init,
		ClientProtocol_Func::Monster_Pos,
		ClientProtocol_Func::Monster_Move,
	};
	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{
		printf("������ ���� \n");
		Player_MGR->MyID = *_pPacket;
		Player_MGR->GetConnectUser(Player_MGR->MyID)->SetClientID(Player_MGR->MyID);//�������� ���� �÷��̾� ID ���
		BYTE Byte = 1;
		Cla_ServerMGR->ClientNet()->WritePacket(SCONNET_COMPLETE, &Byte, 1); //�������� �������� ��Ŷ����
		return;
	}
	void DisConnect(BYTE* _pPacket, DWORD& _Length)
	{
		BYTE ID;
		ID = Player_MGR->GetConnectUser(Player_MGR->MyID)->GetClientID();//�������� ���� �÷��̾� ID ��������
		Cla_ServerMGR->ClientNet()->WritePacket(SONIODISCONNECT, &ID, 1); //Ŭ���̾�Ʈ���� �������� ��Ŷ ����
		printf("Ŭ���̾�Ʈ ���� ���� \n");

	}
	void Loading_Start_Func(BYTE* _pPacket, DWORD& _Length)
	{

		/*UINT Player_ID = *(UINT*)(_pPacket);
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT));
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));*/
		

		UINT Player_ID = *(UINT*)(_pPacket);
		UINT Unique_Code = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));


		cout << Player_ID<<" -- [�÷��̾� ����]  ";
		cout << "����ũ �ڵ�--" << Unique_Code << endl;
	
		if (Player_MGR->MyID == 0) Player_MGR->MyID = Player_ID; //���� �÷��̾� ID ���� 
		       		
		//PlayerInfo[] ���Ϳ� �÷��̾� ���� �Է�
		Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);//�÷��̾� ID �Է�
		Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);//�÷��̾� Code �Է�
		Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);//�÷��̾� x��ǥ �Է�
		Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);//�÷��̾� y��ǥ �Է�
		Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);//�÷��̾� z��ǥ �Է�
		Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);//�÷��̾� HP�Է�
	
		//Player_MGR->GetConnectUser(Player_ID)->SetObjectInitFlag(true); //�÷��̾���Ŷ�� �÷��̾���Ϳ� ����־��ٴ� �÷��� ON
		Player_MGR->GetConnectUser(Player_ID)->SetDrawFlag(true); //�÷��̾ Ŭ���̾�Ʈ���� �׸���� ����ϴ� �÷��� ON

		return;
	}
	void Player_Count(BYTE* _pPacket, DWORD& _Length)
	{
		Cla_ServerMGR->ClientNet()->m_nClientCnt = *_pPacket;
		printf("���� �����ο� :%d�� \n", Cla_ServerMGR->ClientNet()->m_nClientCnt);
		
		return;
	}
	void Player_Info(BYTE* _pPacket, DWORD& _Length)
	{
		/*	UINT Player_ID = *(UINT*)(_pPacket);
			FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT));
			FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT));
			FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
			FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));*/
		
		    UINT Player_ID = *(UINT*)(_pPacket);
		    UINT Unique_Code = *(UINT*)(_pPacket + sizeof(UINT));
		    FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		    FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
	     	FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
	     	FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));
			
			//PlayerInfo[] ���Ϳ� �÷��̾� ���� �Է�
			Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);//�÷��̾� ID �Է�
			Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);//�÷��̾� Code �Է�
			Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);//�÷��̾� x��ǥ �Է�
			Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);//�÷��̾� y��ǥ �Է�
			Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);//�÷��̾� z��ǥ �Է�
			Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);//�÷��̾� HP�Է� 
			
			if (Player_MGR->MyID != Player_ID)
			{
				cout << Player_ID << " -- [�÷��̾� ����]" << endl;
			}
			

	}
	void Player_Pos(BYTE* _pPacket, DWORD& _Length)
	{
		/*UINT Player_ID = *(UINT*)(_pPacket);
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)); 
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));*/

		UINT Player_ID = *(UINT*)(_pPacket);
		UINT Unique_Code = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));
		
		//cout << Player_ID << " -- [�÷��̾� ����]" << "X��ǥ --" << Player_x << endl;
		if (Player_MGR->GetConnectUser(Player_ID)->GetObjectInitFlag() != TRUE) //���� �÷��̾� ���Ϳ� ����ֱ� ���̶��
		{
			cout << Player_ID<< "��° �÷��̾� ������ ���Ϳ� �Է�" << endl;
			Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);
			Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);
			Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);

			//Player_MGR->GetConnectUser(Player_ID)->SetObjectInitFlag(true); //�÷��̾���Ŷ�� �÷��̾���Ϳ� ����־��ٴ� �÷��� ON
			Player_MGR->GetConnectUser(Player_ID)->SetDrawFlag(true); //�÷��̾ Ŭ���̾�Ʈ���� �׸���� ����ϴ� �÷��� ON
		}
		else //�÷��̾���Ϳ� �ִ� �÷��̾��� �����͸� �Է����ش�.
		{
			Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);
			Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);
			Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);
		}


		
	}
	void Monster_Init(BYTE* _pPacket, DWORD& _Length)
	{
		cout << "���� �̴�" << endl;
		UINT Monster_Type = *(UINT*)(_pPacket);
		UINT Monster_Num = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Monster_ID_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Monster_ID_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Monster_ID_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Monster_ID_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));
		//cout << "���� �ʱ�ȭ  ����������-- " << Monsyer_Num << " ��° ����--" << endl;
		//Monster_MGR->Init(Monster_Type, Monster_Num, Monster_ID_x, Monster_ID_y,Monster_ID_z, Monster_ID_HP);

	}
	void Monster_Pos(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Monster_Type = *(UINT*)(_pPacket);
		UINT Monster_Num = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Monster_ID_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Monster_ID_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Monster_ID_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Monster_ID_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));

		//���� ���� �Է�
		Monster_MGR->Init(Monster_Type, Monster_Num, Monster_ID_x, Monster_ID_y, Monster_ID_z, Monster_ID_HP);
	
		
	}
	void Monster_Move(BYTE* _pPacket, DWORD& _Length)
	{
		//cout << "���� ����"<<endl;
		UINT Player_Num = *(UINT*)(_pPacket); //���� �÷��̾� ��ȣ
		UINT Monster_Num = *(UINT*)(_pPacket + sizeof(UINT)); //���� ��ȣ

		Monster_MGR->GetConnectMonster(Monster_Num)->SetMonsterState(MonsterState::MOVE, Player_Num);
		
	}

	
}





/****************************** End of File "CSProcess.cpp" ******************************/