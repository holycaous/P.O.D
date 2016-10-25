#pragma once
#include "stdafx.h"

// cPlayer_Info�� �����ϴ� Ŭ����
class cPlayer_InfoManager
{
private:
	static cPlayer_InfoManager* m_pInstance;


public:
	static cPlayer_InfoManager* GetInstance();   //������ ������	
	void	Destroy(void); //�ν��Ͻ� �����Լ�

	vector<cPlayer_Info*>& GetConnetUserVector(){ return m_vPlayerdUser; }
	cPlayer_Info* GetConnectUser(int _Index) { return m_vPlayerdUser[_Index]; }

private:
	// ������ cPlayer_Info��ü�� �����͸� �����ϴ� Vector
	std::vector<cPlayer_Info*> m_vPlayerdUser;

public:
	cPlayer_InfoManager(VOID);
	virtual ~cPlayer_InfoManager(VOID);
	void Begin(VOID);
	void End(VOID);

	void Update(float dt);
	INT MyID; //���� Ŭ���̾�Ʈ ID Ȯ�κ���
	//INT PlayerID; //���� ID 

};
