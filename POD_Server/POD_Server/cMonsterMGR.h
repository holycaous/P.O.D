#pragma once
#include "stdafx.h"
// cMonsterMGR을 관리하는 클래스

class cMonsterMGR 
{
private:
	static cMonsterMGR* m_pInstance;  //	유일한 인스턴스

public:
	static cMonsterMGR* GetInstance();   //유일한 접근점	
	void	Destroy(void); //인스턴스 해제함수

	cMonsterMGR(VOID);
	virtual ~cMonsterMGR(VOID);
	// 시작하는 함수
	BOOL Begin();
	// 몬스터 생성z
	VOID INIT();
	// 몬스터 업데이트 
	VOID Update(float dt);
	// 종료하는 함수
	VOID End(VOID);

	vector<cMonster_Info*>&GetMonserInfoVector(){ return m_vMonsterInfo; }
	cMonster_Info* GetMonsterInfo(int _Index) { return m_vMonsterInfo[_Index]; }


private:
	// 생성된 cMonsterMGR개체의 포인터를 관리하는 Vector
	std::vector<cMonster_Info*> m_vMonsterInfo;

};

