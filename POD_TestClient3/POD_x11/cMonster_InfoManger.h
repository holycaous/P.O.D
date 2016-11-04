#pragma once
#include "stdafx.h"

// cMonster_Info을 관리하는 클래스
class cMonster_InfoManger
{
private:
	static cMonster_InfoManger* m_pInstance;

public:
	static cMonster_InfoManger* GetInstance();   //유일한 접근점	
	void	Destroy(void); //인스턴스 해제함수

	vector<cMonster_Info*>& GetConnetUserVector(){ return m_vMonster; } //몬스터 관리 벡터
	cMonster_Info* GetConnectMonster(int _Index) { return m_vMonster[_Index]; } //몬스터 벡터의 인덱스를 가져온다.

	/*void SetMonsterInitFlag(bool i) { m_Monster_Initflag = i; }
	bool GetMonsterInitFlag() { return m_Monster_Initflag; }*/

private:
	// 생성된 cMonster_Info개체의 포인터를 관리하는 Vector
	std::vector<cMonster_Info*> m_vMonster;
	//bool Monster_Initflag = false; //몬스터 초기화 변수 

public:
	cMonster_InfoManger(VOID);
	~cMonster_InfoManger(VOID);
	void Begin(VOID);
	void End(VOID);
	void Init(UINT Type, UINT Num, FLOAT Pos_x, FLOAT Pos_y, FLOAT Pos_z, FLOAT HP);

	// m_Monster_Initflag; //몬스터가 벡터에 입력되었는지 확인하는 변수
	int MonsterCount; //몬스터 할당 변수
};
