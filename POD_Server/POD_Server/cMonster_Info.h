#pragma once
#include "stdafx.h"
enum MonsterState
{
	MOVE,
	ATTACK,
	DEATH,
	DAMAGED,
<<<<<<< HEAD
	IDLE,
=======
	STAND,
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
};

enum MonsterType
{
	MONSTERZERO,
	MONSTER_1,
	MONSTER_2,
<<<<<<< HEAD
}; 
=======
};
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
class cMonster_Info : public CObject
{
public :
	cMonster_Info(){};
	cMonster_Info(UINT Type, float HP, float posX, float posY, float posZ);
<<<<<<< HEAD
	virtual ~cMonster_Info();  
=======
	virtual ~cMonster_Info();
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
	
protected :
	UINT m_Type;
	UINT m_State;

public :
	virtual void Monster_Init(int num) = 0;
	virtual void Update(float dt,int num) = 0;
	virtual void AI_Update(float dt) = 0;
	void SetMonsterType(UINT _mType) { m_Type = _mType; }
	UINT GetMonsterType() { return m_Type; }
<<<<<<< HEAD
	void SetMonsterState(UINT _mState) { m_State = _mState; }
	UINT GetMonsterState() { return m_State; }
=======
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e

};
