#include "stdafx.h"

cMonster_InfoManger* cMonster_InfoManger::m_pInstance = NULL;

// cMonster_InfoManger()
cMonster_InfoManger::cMonster_InfoManger()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ~cMonster_InfoManger()
cMonster_InfoManger::~cMonster_InfoManger()
{

}
cMonster_InfoManger*	cMonster_InfoManger::GetInstance(void)
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new cMonster_InfoManger;
	}

	return m_pInstance;
}
void cMonster_InfoManger::Destroy(void) 
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
	}
}

VOID cMonster_InfoManger::Begin(VOID)
{
	// ����1 �ִ�� ��ŭ ����
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		cMonster_1 *pMonster_1 = new cMonster_1(MONSTER_1,0,0,0,0);
		// ������ ��ü�� ��ü�� �����ϴ� m_vMonster ���Ϳ� �Է�.
		m_vMonster.push_back(pMonster_1);
	}

}
VOID cMonster_InfoManger::End(VOID)
{
	//���� ����
	for (int i = 0; i<m_vMonster.size(); i++)
	{
		delete m_vMonster[i];
	}
}
VOID cMonster_InfoManger::Init(UINT Type, UINT Num, FLOAT Pos_x, FLOAT Pos_y, FLOAT Pos_z, FLOAT HP)
{

	if (GetConnectMonster(Num)->GetObjectInitFlag()!= TRUE) //���� ���� ���Ϳ� ����ֱ� ���̶��
	{
		cout << Num << "��° ���� ������ ���Ϳ� �Է�" << endl;
		GetConnectMonster(Num)->Monster_Init(Type, HP, Pos_x, Pos_y, Pos_z); //�������� ���۹��� ���͵����͸� ���ͺ��Ϳ� �־��ش�.
		GetConnectMonster(Num)->SetObjectInitFlag(true); //������Ŷ�� ���͹��Ϳ� ����־��ٴ� �÷��� ON
		GetConnectMonster(Num)->SetDrawFlag(true); //���͸� Ŭ���̾�Ʈ���� �׸���� ����ϴ� �÷��� ON
		MonsterCount = Num; //���� �Ҵ纯���� ������� ���Ϳ� �Է��� ���� �� �Է�
	}
	else //���͹��Ϳ� �ִ� ���Ͷ�� �����͸� �Է����ش�.
	{
		Monster_MGR->GetConnectMonster(Num)->SetClientID(Type);
		Monster_MGR->GetConnectMonster(Num)->Set_Posx(Pos_x);
		Monster_MGR->GetConnectMonster(Num)->Set_Posy(Pos_y);
		Monster_MGR->GetConnectMonster(Num)->Set_Posz(Pos_z);
		Monster_MGR->GetConnectMonster(Num)->Set_HP(HP);
	}
	/*if (MonsterCount == MAX_MONSTER - 1)
	{
		Monster_MGR->SetMonsterInitFlag(true);
	}*/


}