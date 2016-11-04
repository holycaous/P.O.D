#include "stdafx.h"

cMonsterMGR*		cMonsterMGR::m_pInstance = NULL;
extern CIocpServer IocpServer;

cMonsterMGR::cMonsterMGR(void)
{
}
cMonsterMGR::~cMonsterMGR(void)
{
}

cMonsterMGR*	cMonsterMGR::GetInstance(void)
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new cMonsterMGR;
	}

	return m_pInstance;
}

void		cMonsterMGR::Destroy(void)
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
	}
}
BOOL	cMonsterMGR::Begin(void)
{
	//몬스터 생성 (서버 내)
	POINT p; //몬스터 위치값
	
	//몬스터 생성 (서버 내)
	for (int i = 0; i < MAX_MONSTER; i++)
	{
		int num = 0;
		int sum = num + 20;
		//cMonster_1(UINT Type, float HP, float posX, float posY, float posZ);
		cMonster_1 *pMonster_1 = new cMonster_1(MonsterType::MONSTER_1, 100 + sum, 100, 150,100,0,0,1);
		//생성한 몬스터 백터에 집어넣기 
		m_vMonsterInfo.push_back(pMonster_1);
		//생성한 몬스터 위치값 지정 (랜덤으로 지정)
		GetMonsterInfo(i)->Monster_Init(i);
		p.x = m_vMonsterInfo[i]->GetPositionX();
		p.y = m_vMonsterInfo[i]->GetPositionY();
		QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //쿼드트리에 속한 트리노드를 알아낸다.
		//cout << pQuad->s_NodeNum << endl;
		m_vMonsterInfo[i]->SetNodeNum(pQuad->s_NodeNum); //현재 몬스터위치의 트리노드번호를 입력
	}
	////몬스터 생성 (클라이언트에 전송)
	//for (int i = 0; i < m_vMonsterInfo.size(); i++)
	//{
	//	GetMonsterInfo(i)->Monster_Init(i);
	//}
	/*POINT p;
	for (int i = 0; i < MAX_MONSTER; i++)
	{
		p.x = m_vMonsterInfo[i]->GetPositionX();
		p.y = m_vMonsterInfo[i]->GetPositionY();
		CTree* Tree = ServerMGR->GetTerrainServer()->GetTree();
		Tree->GetTopNode()->IsIn(p)->Monster_arr.push_back(m_vMonsterInfo[i]);
	}*/

	return TRUE;
}
VOID	cMonsterMGR::End(void)
{
	//몬스터 삭제
	for (int i = 0; i<m_vMonsterInfo.size(); i++)
	{
		delete m_vMonsterInfo[i];
	}

	// 관리하는 벡터의 내용을 모두 지워준다
	m_vMonsterInfo.clear();
}

VOID	cMonsterMGR::INIT(void)
{
 
}

VOID	cMonsterMGR::Update(float dt)
{
	float radius = 10; //임시 반지름(탐색범위)
	
		//QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //쿼드트리에 속한 트리노드를 알아낸다.
		////cout << pQuad->s_NodeNum << endl; 
		//m_vMonsterInfo[i]->SetNodeNum(pQuad->s_NodeNum); //현재 몬스터위치의 트리노드번호를 입력

		POINT p;
		for (int i = 0; i < MAX_MONSTER; i++)
		{
			p.x = m_vMonsterInfo[i]->GetPositionX();
			p.y = m_vMonsterInfo[i]->GetPositionY();
			QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //쿼드트리에 속한 트리노드를 알아낸다.
			//cout << pQuad->s_NodeNum << endl; 
			m_vMonsterInfo[i]->SetNodeNum(pQuad->s_NodeNum); //현재 몬스터위치의 트리노드번호를 입력
			//GetMonsterInfo(i)->AI_Update(dt);
			
			ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsCollision(p, radius);//캐릭터와 인접한 (충돌된) 노드들을 배열에 담는다.
			
			GetMonsterInfo(i)->AI_Update(dt,i,pQuad);
			GetMonsterInfo(i)->Update(dt, i);
		}

}