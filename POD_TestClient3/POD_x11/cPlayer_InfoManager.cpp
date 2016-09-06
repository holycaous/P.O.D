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
	// 최대 접속자만큼의 개체를 미리 생성합니다.
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		cPlayer_Info *pPlayer_Info = new cPlayer_Info();
		// 생성된 개체를 개체를 관리하는 m_vPlayerdUser 벡터에 입력.
		m_vPlayerdUser.push_back(pPlayer_Info);
	}
	MyID = 0; //나의 아이디 초기화

}
VOID cPlayer_InfoManager::End(VOID)
{
	// 최대 접속자만큼의 개체를 삭제합니다.
	for (DWORD i = 0; i<m_vPlayerdUser.size(); i++)
	{
		delete m_vPlayerdUser[i];
	}

	// 관리하는 벡터의 내용을 모두 지워
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