#include "stdafx.h"


cPlayer_InfoManager* cPlayer_InfoManager::m_pInstance = NULL;

// cPlayer_InfoManager()
cPlayer_InfoManager::cPlayer_InfoManager()
{
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// ~cPlayer_InfoManager()
cPlayer_InfoManager::~cPlayer_InfoManager()
{

}
cPlayer_InfoManager*	cPlayer_InfoManager::GetInstance(void)
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new cPlayer_InfoManager;
	}

	return m_pInstance;
}


VOID cPlayer_InfoManager::Begin(VOID)
{
	// �ִ� �����ڸ�ŭ�� ��ü�� �̸� �����մϴ�.
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		cPlayer_Info *pPlayer_Info = new cPlayer_Info();
		// ������ ��ü�� ��ü�� �����ϴ� m_vPlayerdUser ���Ϳ� �Է�.
		m_vPlayerdUser.push_back(pPlayer_Info);
	}
	MyID = 0; //���� ���̵� �ʱ�ȭ

}
VOID cPlayer_InfoManager::End(VOID)
{
	// �ִ� �����ڸ�ŭ�� ��ü�� �����մϴ�.
	for (DWORD i = 0; i<m_vPlayerdUser.size(); i++)
	{
		delete m_vPlayerdUser[i];
	}

	// �����ϴ� ������ ������ ��� ����
	m_vPlayerdUser.clear();
}
void cPlayer_InfoManager::Destroy(void)
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
	}
}
void cPlayer_InfoManager::Update(float dt)
{

}