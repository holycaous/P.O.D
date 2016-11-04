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
	//���� ���� (���� ��)
	POINT p; //���� ��ġ��
	
	//���� ���� (���� ��)
	for (int i = 0; i < MAX_MONSTER; i++)
	{
		int num = 0;
		int sum = num + 20;
		//cMonster_1(UINT Type, float HP, float posX, float posY, float posZ);
		cMonster_1 *pMonster_1 = new cMonster_1(MonsterType::MONSTER_1, 100 + sum, 100, 150,100,0,0,1);
		//������ ���� ���Ϳ� ����ֱ� 
		m_vMonsterInfo.push_back(pMonster_1);
		//������ ���� ��ġ�� ���� (�������� ����)
		GetMonsterInfo(i)->Monster_Init(i);
		p.x = m_vMonsterInfo[i]->GetPositionX();
		p.y = m_vMonsterInfo[i]->GetPositionY();
		QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //����Ʈ���� ���� Ʈ����带 �˾Ƴ���.
		//cout << pQuad->s_NodeNum << endl;
		m_vMonsterInfo[i]->SetNodeNum(pQuad->s_NodeNum); //���� ������ġ�� Ʈ������ȣ�� �Է�
	}
	////���� ���� (Ŭ���̾�Ʈ�� ����)
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
	//���� ����
	for (int i = 0; i<m_vMonsterInfo.size(); i++)
	{
		delete m_vMonsterInfo[i];
	}

	// �����ϴ� ������ ������ ��� �����ش�
	m_vMonsterInfo.clear();
}

VOID	cMonsterMGR::INIT(void)
{
 
}

VOID	cMonsterMGR::Update(float dt)
{
	float radius = 10; //�ӽ� ������(Ž������)
	
		//QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //����Ʈ���� ���� Ʈ����带 �˾Ƴ���.
		////cout << pQuad->s_NodeNum << endl; 
		//m_vMonsterInfo[i]->SetNodeNum(pQuad->s_NodeNum); //���� ������ġ�� Ʈ������ȣ�� �Է�

		POINT p;
		for (int i = 0; i < MAX_MONSTER; i++)
		{
			p.x = m_vMonsterInfo[i]->GetPositionX();
			p.y = m_vMonsterInfo[i]->GetPositionY();
			QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //����Ʈ���� ���� Ʈ����带 �˾Ƴ���.
			//cout << pQuad->s_NodeNum << endl; 
			m_vMonsterInfo[i]->SetNodeNum(pQuad->s_NodeNum); //���� ������ġ�� Ʈ������ȣ�� �Է�
			//GetMonsterInfo(i)->AI_Update(dt);
			
			ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsCollision(p, radius);//ĳ���Ϳ� ������ (�浹��) ������ �迭�� ��´�.
			
			GetMonsterInfo(i)->AI_Update(dt,i,pQuad);
			GetMonsterInfo(i)->Update(dt, i);
		}

}