#pragma once
#include "stdafx.h"

class cMonster_1 : public cMonster_Info
{
public:
	cMonster_1(UINT Type, float HP, float posX, float posY, float posZ);
	virtual ~cMonster_1();
protected:
	char m_Name[50];
	//몬스터 스피드, 공격속도,같은 기본정보

private:

public:
	virtual void Update(float dt,int num);
	virtual void AI_Update(float dt); 
	virtual void Monster_Init(int num);
	VOID Skill1(float dt);
	VOID Skill2(float dt);

};