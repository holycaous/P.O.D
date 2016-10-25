#pragma once
#include "stdafx.h"
enum MonsterState
{
	MOVE,
	ATTACK,
	DEATH,
	DAMAGED,
	IDLE,
};

enum MonsterType
{
	MONSTERZERO,
	MONSTER_1,
	MONSTER_2,
}; 
class cMonster_Info : public CObject
{
public :
	cMonster_Info(){};
	cMonster_Info(UINT Type, float HP, float posX, float posY, float posZ);
	virtual ~cMonster_Info();  
	
protected :
	UINT m_Type;
	UINT m_State;

public :
	virtual void Monster_Init(int num) = 0;
	virtual void Update(float dt,int num) = 0;
	virtual void AI_Update(float dt) = 0;
	void SetMonsterType(UINT _mType) { m_Type = _mType; }
	UINT GetMonsterType() { return m_Type; }
	void SetMonsterState(UINT _mState) { m_State = _mState; }
	UINT GetMonsterState() { return m_State; }

};
