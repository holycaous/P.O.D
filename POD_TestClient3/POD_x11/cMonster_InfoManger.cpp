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
	// 몬스터1 최대수 만큼 생성
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		cMonster_1 *pMonster_1 = new cMonster_1(MONSTER_1,0,0,0,0);
		// 생성된 개체를 개체를 관리하는 m_vMonster 벡터에 입력.
		m_vMonster.push_back(pMonster_1);
	}

}
VOID cMonster_InfoManger::End(VOID)
{
	//몬스터 삭제
	for (int i = 0; i<m_vMonster.size(); i++)
	{
		delete m_vMonster[i];
	}
}
VOID cMonster_InfoManger::Init(UINT Type, UINT Num, FLOAT Pos_x, FLOAT Pos_y, FLOAT Pos_z, FLOAT HP)
{

	if (GetConnectMonster(Num)->GetObjectInitFlag()!= TRUE) //아직 몬스터 벡터에 집어넣기 전이라면
	{
		cout << Num << "번째 몬스터 데이터 벡터에 입력" << endl;
		GetConnectMonster(Num)->Monster_Init(Type, HP, Pos_x, Pos_y, Pos_z); //서버에서 전송받은 몬스터데이터를 몬스터벡터에 넣어준다.
		GetConnectMonster(Num)->SetObjectInitFlag(true); //몬스터패킷을 몬스터백터에 집어넣었다는 플래그 ON
		GetConnectMonster(Num)->SetDrawFlag(true); //몬스터를 클라이언트에게 그리라고 명령하는 플래그 ON
		MonsterCount = Num; //몬스터 할당변수에 현재까지 벡터에 입력한 몬스터 수 입력
	}
	else //몬스터백터에 있는 몬스터라면 데이터를 입력해준다.
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