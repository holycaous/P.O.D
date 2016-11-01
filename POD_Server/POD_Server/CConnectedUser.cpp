#include "stdafx.h"

extern CIocpServer IocpServer;

CConnectedUser::CConnectedUser(VOID)
{
	CThreadSync Sync;

	// 접속 되었는지 판별하는 변수
	m_bConnected = FALSE;

	m_PlayerInfo = new cPlayer_Info(); //플레이어 생성
	//ZeroMemory(&m_Object_Player, sizeof(CObject));
}

CConnectedUser::~CConnectedUser(VOID)
{
}

BOOL CConnectedUser::Restart(SOCKET hListenSocket)
{
	// 동기화 개체
	CThreadSync Sync;

	// 개체를 종료해 줍니다.
	End();

	// 개체를 다시 시작하고 Accept상태로 만들어 줍니다.
	// Accept를 하기위해서는 반드시 hListenSocket 값이 필요합니다.
	// 이 값은 CIocpServer의 멤버인 m_pListen의 GetSocket으로 받아올 수 있습니다.
	return Begin() && Accept(hListenSocket);
}

void CConnectedUser::Update(float _fDTime)
{
	// 동기화 개체
	CThreadSync Sync;
	POINT p;

	p.x = m_PlayerInfo->GetPositionX(); 
	p.y = m_PlayerInfo->GetPositionZ();
	
	float radius = 10; //임시 반지름

	QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //쿼드트리에 속한 트리노드를 알아낸다.
	m_NodeNum = pQuad->s_NodeNum;//현재 플레이어위치의 트리노드번호를 입력
	ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsCollision(p,radius);//캐릭터와 인접한 (충돌된) 노드들을 배열에 담는다.
	
	//cout << "ID: " << m_PlayerInfo->GetClientID() << "=" << "pQuad->s_NodeNum::" << endl;
	//m_NodeNum = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p)->s_NodeNum;//현재 플레이어위치의 트리노드번호를 입력
	m_PlayerInfo->SetNodeNum(m_NodeNum); //플레이어백터에도 트리노드번호입력

	p.x = m_PlayerInfo->GetPositionX();
	p.y = m_PlayerInfo->GetPositionZ();
	m_PlayerInfo->Update(_fDTime); //플레이어 패킷 보내기
	m_PlayerInfo->PlayerCollison(); //플레이어 충돌체크 AI
	
}
int sum_X=100;
void CConnectedUser::Player_Init()
{
	CThreadSync Sync;
	int z=100;
	sum_X = sum_X+20;

	m_PlayerInfo->PlayerInit(ServerMGR->PlayerID, sum_X, 150, 200, 100);

}

void CConnectedUser::ComUpdate(float _fDTime)
{
	//CThreadSync Sync;

	//if (m_Object_InfoList[0] != NULL) m_Object_InfoList[0]->ComUpdate(_fDTime);

}