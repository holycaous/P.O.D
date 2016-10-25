#pragma once
#include "stdafx.h"
// cMonsterMGR�� �����ϴ� Ŭ����

class cMonsterMGR 
{
private:
	static cMonsterMGR* m_pInstance;  //	������ �ν��Ͻ�

public:
	static cMonsterMGR* GetInstance();   //������ ������	
	void	Destroy(void); //�ν��Ͻ� �����Լ�

	cMonsterMGR(VOID);
	virtual ~cMonsterMGR(VOID);
	// �����ϴ� �Լ�
	BOOL Begin();
	// ���� ����z
	VOID INIT();
	// ���� ������Ʈ 
	VOID Update(float dt);
	// �����ϴ� �Լ�
	VOID End(VOID);

	vector<cMonster_Info*>&GetMonserInfoVector(){ return m_vMonsterInfo; }
	cMonster_Info* GetMonsterInfo(int _Index) { return m_vMonsterInfo[_Index]; }


private:
	// ������ cMonsterMGR��ü�� �����͸� �����ϴ� Vector
	std::vector<cMonster_Info*> m_vMonsterInfo;

};

