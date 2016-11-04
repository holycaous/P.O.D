#pragma once
#include "stdafx.h"

// cMonster_Info�� �����ϴ� Ŭ����
class cMonster_InfoManger
{
private:
	static cMonster_InfoManger* m_pInstance;

public:
	static cMonster_InfoManger* GetInstance();   //������ ������	
	void	Destroy(void); //�ν��Ͻ� �����Լ�

	vector<cMonster_Info*>& GetConnetUserVector(){ return m_vMonster; } //���� ���� ����
	cMonster_Info* GetConnectMonster(int _Index) { return m_vMonster[_Index]; } //���� ������ �ε����� �����´�.

	/*void SetMonsterInitFlag(bool i) { m_Monster_Initflag = i; }
	bool GetMonsterInitFlag() { return m_Monster_Initflag; }*/

private:
	// ������ cMonster_Info��ü�� �����͸� �����ϴ� Vector
	std::vector<cMonster_Info*> m_vMonster;
	//bool Monster_Initflag = false; //���� �ʱ�ȭ ���� 

public:
	cMonster_InfoManger(VOID);
	~cMonster_InfoManger(VOID);
	void Begin(VOID);
	void End(VOID);
	void Init(UINT Type, UINT Num, FLOAT Pos_x, FLOAT Pos_y, FLOAT Pos_z, FLOAT HP);

	// m_Monster_Initflag; //���Ͱ� ���Ϳ� �ԷµǾ����� Ȯ���ϴ� ����
	int MonsterCount; //���� �Ҵ� ����
};
