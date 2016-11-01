#include "stdafx.h"

CConnectedUserManager::CConnectedUserManager(VOID)
{
}

CConnectedUserManager::~CConnectedUserManager(VOID)
{
}

// CConnectedSessionManager를 시작하는 함수
BOOL CConnectedUserManager::Begin(SOCKET hListenSocket)
{
	// 동기화 개체
	CThreadSync Sync;

	// 파라미터로 Listen개체의 소켓을 받아야 한다
	// 값이 NULL일 경우 실패
	if (!hListenSocket)
		return FALSE;

	// 최대 접속자만큼의 개체를 미리 생성
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		CConnectedUser *pConnectedUser = new CConnectedUser();
		// 생성된 개체를 개체를 관리하는 m_vConnectedSessions 벡터에 입력합니다.
		m_vConnectedUser.push_back(pConnectedUser);
	}

	// 생성한 개체들을 초기화하고 Accept상태로 만든다
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		GetConnectUser(i)->SetUserStaus(GAME_END); //유저 스테이터스를 게임 시작하기전 상태로 바꾼다.
		// 개체 초기화중 실패할 경우 종료시킵니다.
		if (!m_vConnectedUser[i]->Begin())
		{
			CConnectedUserManager::End();
			return FALSE;
		}

		// 개체를 Accept상태로 변경하다 문제가 있으면 종료
		if (!m_vConnectedUser[i]->Accept(hListenSocket))
		{
			CConnectedUserManager::End();
			return FALSE;
		}
	}

	return TRUE;
}

// ConnectedSessionManager를 종료하는 함수
VOID CConnectedUserManager::End(VOID)
{
	// 동기화 개체
	CThreadSync Sync;

	// 최대 접속자만큼의 개체를 삭제
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// 개체 종료
		m_vConnectedUser[i]->End();

		delete m_vConnectedUser[i];
	}

	// 관리하는 벡터의 내용을 모두 지워준다
	m_vConnectedUser.clear();
}

// 접속된 모든 Session에 데이터를 전송하는 함수
VOID CConnectedUserManager::WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength)
{
	// 동기화 개체
	CThreadSync Sync; 

	// 접속된 모든 개체에 WritePacket을 해 줍니다.
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// 접속되어 있는 개체일 경우만 WritePacket을 해 줍니다.
		if (m_vConnectedUser[i]->GetConnected())
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength);
	}
}

VOID CConnectedUserManager::WriteAllExcept(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID) // 자기 자신 제외하고 전송
{
	// 동기화 개체
	CThreadSync Sync;

	// 접속된 모든 개체에 WritePacket을 해 줍니다.
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// 접속되어 있는 개체일 경우만 WritePacket을 해 줍니다.
		if (m_vConnectedUser[i]->GetConnected() && (m_vConnectedUser[i]->GetUserID()) != ID){
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength);
		}
	}
}

VOID CConnectedUserManager::WriteSOLO(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID) // 자기 자신 한테만 전송
{
	// 동기화 개체
	CThreadSync Sync;

	// 접속된 모든 개체를 먼저 검색
	for (DWORD i = 0; i < m_vConnectedUser.size(); i++)
	{
		if (m_vConnectedUser[i]->GetConnected() && (m_vConnectedUser[i]->GetUserID()) == ID)//접속된 개체중 자기 ID를 찾는다.
		{
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength); //찾으면 패킷전송
		}
	}
}
VOID CConnectedUserManager::WriteCulling(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int NodeNum) // 자기자신과 같은 쿼드트리 노드안에 있는 오브젝트한테만 전송
{
	// 동기화 개체
	CThreadSync Sync;

	// 접속된 모든 개체를 먼저 검색
	for (DWORD i = 0; i < m_vConnectedUser.size(); i++)
	{
		if (m_vConnectedUser[i]->GetConnected() && (m_vConnectedUser[i]->GetNodeNum()) == NodeNum)//접속된 개체중 같은 트리노드에 있는 유저를 찾는다.
		{
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength); //찾으면 패킷전송
		}
	}
}

// 게임중인 모든 클라이언트에게 데이터를 전송하는 함수
VOID CConnectedUserManager::WriteGameClientAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength)
{
	// 동기화 개체
	CThreadSync Sync;

	// 접속된 모든 개체에 WritePacket을 해 줍니다.
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// 접속되어 있는 개체일 경우만 WritePacket을 해 줍니다.
		if ((m_vConnectedUser[i]->GetConnected() && m_vConnectedUser[i]->GetUserStaus() == GAME_ING))
		{
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength);
		}

	}
}
