#include "stdafx.h"

extern CIocpServer IocpServer;

<<<<<<< HEAD
=======

>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
cMonster_1::cMonster_1(UINT Type, float HP, float posX, float posY, float posZ)
:cMonster_Info()
{
	m_Type = Type;
	m_HP = HP;
	m_posx = posX;
	m_posy = posY;
	m_posz = posZ;

}
void cMonster_1::Monster_Init(int num)
{
<<<<<<< HEAD
	/*m_posx = m_posx;
	m_posy = m_posy;
	m_posz = m_posz;*/
=======
	m_posx = m_posx + (rand() % 100);
	m_posy = m_posy;
	m_posz = m_posz + (rand() % 200);
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e


	// ������ ����.
	//int size = sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	//BYTE pData[24] = { 0, };

	//memcpy(pData, &m_Type, sizeof(UINT)); //���� Type
	//memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //���� �迭�ѹ�
	//memcpy(pData + sizeof(UINT)+sizeof(UINT), &m_posx, sizeof(FLOAT));  //���� ��ġ x��ǥ
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //���� ��ġ y��ǥ
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //���� ��ġ z��ǥ
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //���� Hp

	// ����.
	//IocpServer.GetConnectUserManager()->WriteSOLO(CMONSTER_INIT, pData, size, ServerMGR->ClientCnt);
}
cMonster_1::~cMonster_1(void)
{

}
VOID cMonster_1::Skill1(float dt)
{
	//cout << "��ų1 �ߵ�" << endl;
}

VOID cMonster_1::Skill2(float dt)
{ 
	cout << "��ų2 �ߵ�" << endl;
}
VOID cMonster_1::Update(float dt, int num)
{
	//cout << "���� ������Ʈ" << endl;
	if (m_HP < 50)
	{ 
		DT = timeGetTime() - ET; //����ð��� ���Ѵ�
		ET = timeGetTime(); // ����ð� �ʱ�ȭ
		fDt = DT / 1000.0f;
		fAccdt += fDt; //�ð� ������
		if (fAccdt>1.0f / 300.0f) //5�ʴ� �ѹ� �θ���.
			Skill1(fAccdt);
	}
	//������ ����.
	int size = sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[24] = { 0, };

	memcpy(pData, &m_Type, sizeof(UINT)); //���� Type
	memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //���� �迭�ѹ�
	memcpy(pData + sizeof(UINT)+sizeof(UINT), &m_posx, sizeof(FLOAT));  //���� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //���� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //���� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //���� Hp

	// ����.
	IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_POS, pData, size, m_NodeNum);
	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CMONSTER_POS, pData, size);
<<<<<<< HEAD
}
VOID cMonster_1::AI_Update(float dt)
{
	POINT p;
	p.x = m_posx;
	p.y = m_posz;
	QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //����Ʈ���� ���� Ʈ����带 �˾Ƴ���.
	//cout << pQuad->s_P->x <<"----"<< pQuad->s_P->y << endl;

	if (GetMonsterState() == MOVE) //���Ͱ� �÷��̾ �ĺ��Ͽ� �̵�
	{
		BYTE pData[4] = { 0, };
		int size = sizeof(UINT);
		// ����. 
		IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_ATTACK, pData, size,m_NodeNum);
	}
	
	if (GetMonsterState() == ATTACK) //���Ͱ� �÷��̾�� �����Ҽ� �ִ� ������ ���� ����.
	{
		BYTE pData[4] = { 0, };
		int size = sizeof(UINT);
		// Ŭ���̾�Ʈ���� �����϶�� ������ ����.
		IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_MOVE, pData, size, m_NodeNum);
	}


	if (GetMonsterState() == IDLE) //���� �ֺ��� �÷��̾ ������� �������� �̵�.
	{
		switch (rand() % 4)
		{
		case 0:
		{
				  if (m_posx > pQuad->s_P[1].x || m_posx< pQuad->s_P[2].x)
					  cout << "m_posx++" << endl; m_posx++;
				  break;
		}
		case 1:
		{
				  if (m_posx > pQuad->s_P[1].x || m_posx < pQuad->s_P[2].x)
					  cout << "m_posx--" << endl; m_posx--;
				  break;
		}
		case 2:
		{
				  if (m_posy > pQuad->s_P[1].y || m_posy < pQuad->s_P[2].y)
					  cout << "m_posz++" << endl; m_posz++;
				  break;
		}
		case 3:
		{
				  if (m_posy > pQuad->s_P[1].y || m_posy < pQuad->s_P[2].y)
					  cout << "m_posz--" << endl; m_posz--;
				  break;
		}

		}
=======

}
VOID cMonster_1::AI_Update(float dt)
{
	switch (rand() % 4)
	{
	case 0:cout << "m_posx++" << endl; m_posx++; break;
	case 1:cout << "m_posx--" << endl; m_posx--; break;
	case 2:cout << "m_posz++" << endl; m_posz++; break;
	case 3:cout << "m_posz--" << endl; m_posz--; break;
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
	}
}