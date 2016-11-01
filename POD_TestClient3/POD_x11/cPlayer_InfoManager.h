#pragma once
#include "stdafx.h"

// cPlayer_Info을 관리하는 클래스
class cPlayer_InfoManager
{
private:
	static cPlayer_InfoManager* m_pInstance;


public:
	static cPlayer_InfoManager* GetInstance();   //유일한 접근점	
	void	Destroy(void); //인스턴스 해제함수

	vector<cPlayer_Info*>& GetConnetUserVector(){ return m_vPlayerdUser; }
	cPlayer_Info* GetConnectUser(int _Index) { return m_vPlayerdUser[_Index]; }

private:
	// 생성된 cPlayer_Info개체의 포인터를 관리하는 Vector
	std::vector<cPlayer_Info*> m_vPlayerdUser;

public:
	cPlayer_InfoManager(VOID);
	virtual ~cPlayer_InfoManager(VOID);
	void Begin(VOID);
	void End(VOID);

	void Update(float dt);
	INT MyID; //본인 클라이언트 ID 확인변수
	//INT PlayerID; //유저 ID 

};
