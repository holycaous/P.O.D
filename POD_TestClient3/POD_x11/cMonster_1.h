#include "stdafx.h"

class cMonster_1 : public cMonster_Info
{
public:
	cMonster_1(UINT Type, float HP, float posX, float posY, float posZ);
	virtual ~cMonster_1();
protected:
	char m_Name[50];
	//���� ���ǵ�, ���ݼӵ�,���� �⺻���� 
public:
	virtual void Update(float dt);
	virtual void AI_Update(float dt);
	virtual void Monster_Init(UINT Type, float HP, float posX, float posY, float posZ);
	VOID Skill1(float dt);
	VOID Skill2(float dt);
};