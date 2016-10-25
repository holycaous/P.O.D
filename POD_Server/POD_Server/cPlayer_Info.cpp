#include "stdafx.h"

extern CIocpServer IocpServer; 
extern CTerrain_Server* g_pTerrain;


void cPlayer_Info::Update(float _fDTime)
{
	if (m_HP <= 0)
		m_blive = FALSE;
	// ������ ����.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, }; 

	memcpy(pData, &m_nClientID, sizeof(UINT)); //�÷��̾� ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //�÷��̾� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //�÷��̾� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //�÷��̾� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //�÷��̾� Hp
	
	// ����.
	for (int i = 0; i < ServerMGR->GetTerrainServer()->GetTree()->Get_Index(); i++)
	{  
		cout << ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_nNum << endl;
		IocpServer.GetConnectUserManager()->WriteCulling(CPLAYER_POS, pData, size, 
														 ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_nNum);
	}
	ServerMGR->GetTerrainServer()->GetTree()->Reset_QudeIndex(); //Ʈ������ε��� �ʱ�ȭ
	//IocpServer.GetConnectUserManager()->WriteCulling(CPLAYER_POS, pData, size, m_NodeNum);
	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CPLAYER_POS, pData, size);

}
void cPlayer_Info::PlayerInit(int ID, float pos_x, float pos_y, float pos_z, float hp)
{
	m_posx = pos_x;
	//m_posx = pos_x + (rand() % 100);
	m_posy = pos_y;
	m_posz = pos_z;
	//m_posz = pos_z +(rand() % 200);
	m_HP = hp;
	m_nClientID = ID;
	
	 //������ ����.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT)); //�÷��̾� ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //�÷��̾� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //�÷��̾� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //�÷��̾� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //�÷��̾� Hp

	IocpServer.GetConnectUserManager()->WriteSOLO(CSTART_INFO, pData, size, m_nClientID);
	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CSTART_INFO, pData, size);
}

void cPlayer_Info::PlayerInfo()
{
	if (m_HP <= 0) 
		m_blive = FALSE;

 	// ������ ����.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT)); //�÷��̾� ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //�÷��̾� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //�÷��̾� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //�÷��̾� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //�÷��̾� Hp

	// ����.
	IocpServer.GetConnectUserManager()->WriteGameClientAll(CPLAYER_INFO, pData, size);
}
VOID	cPlayer_Info::Move(int state)
{
	switch (state)
	{
	case 0:
		cout << "��Ű" << endl;
		m_posx = m_posx + 20;
		break;
	case 1:
		cout << "�ٿ�Ű" << endl;
		break;
	case 2:
		cout << "����ƮŰ" << endl;
		break;
	case 3:
		cout << "����ƮŰ" << endl;
		break;
	}
}
void	cPlayer_Info::PlayerCollison()
{
	float Monster_x;
	float Monster_y;
	float Monster_z;

	float SerchRange = 10; //����Ž������
	float AttackRange = 2; //���Ͱ��ݹ���


	//*1.���Ͱ� �÷��̾����  �����Ÿ��� ���� ����  MOVE���·� ����  2.���Ͱ� �÷��̾ �����Ҽ��ִ� �Ÿ��� ���� ATTACK���·� ����
	for (int i = 0; i < MAX_MONSTER; i++)
	{
		Monster_x=MonsterMGR->GetMonsterInfo(i)->GetPositionX()-m_posx;
		Monster_y=MonsterMGR->GetMonsterInfo(i)->GetPositionY()-m_posy; 
		Monster_z=MonsterMGR->GetMonsterInfo(i)->GetPositionZ()-m_posz;

		float Distance = sqrt((Monster_x*Monster_x) + (Monster_y*Monster_y) + (Monster_z*Monster_z)); //�÷��̾�� ���Ͱ��� �Ÿ� 
		
		 if (AttackRange >= Distance)
		{
			//������ ���¸� ATTACK ���·� �����Ͽ� �÷��̾ �����ϰ� �����.
			MonsterMGR->GetMonsterInfo(i)->SetMonsterState(MonsterState::ATTACK);
		}
		 else if (SerchRange >= Distance)
		 {
			 cout << "���� ����" << endl;
			 //������ ���¸� MOVE�� �����Ͽ� �÷��̾ �i�ư��� �����.
			 MonsterMGR->GetMonsterInfo(i)->SetMonsterState(MonsterState::MOVE);
		 }
		else
		{
			//������ ���¸� IDLE ���·� ����
			MonsterMGR->GetMonsterInfo(i)->SetMonsterState(MonsterState::IDLE);
		}
		
	}
} 