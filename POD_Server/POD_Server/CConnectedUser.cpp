#include "stdafx.h"

extern CIocpServer IocpServer;

CConnectedUser::CConnectedUser(VOID)
{
	CThreadSync Sync;

	// ���� �Ǿ����� �Ǻ��ϴ� ����
	m_bConnected = FALSE;

	m_PlayerInfo = new cPlayer_Info(); //�÷��̾� ����
	//ZeroMemory(&m_Object_Player, sizeof(CObject));
}

CConnectedUser::~CConnectedUser(VOID)
{
}

BOOL CConnectedUser::Restart(SOCKET hListenSocket)
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// ��ü�� ������ �ݴϴ�.
	End();

	// ��ü�� �ٽ� �����ϰ� Accept���·� ����� �ݴϴ�.
	// Accept�� �ϱ����ؼ��� �ݵ�� hListenSocket ���� �ʿ��մϴ�.
	// �� ���� CIocpServer�� ����� m_pListen�� GetSocket���� �޾ƿ� �� �ֽ��ϴ�.
	return Begin() && Accept(hListenSocket);
}

void CConnectedUser::Update(float _fDTime)
{
	// ����ȭ ��ü
	CThreadSync Sync;
	POINT p;

<<<<<<< HEAD
	p.x = m_PlayerInfo->GetPositionX(); 
	p.y = m_PlayerInfo->GetPositionZ();
	
	float radius = 10; //�ӽ� ������

	QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //����Ʈ���� ���� Ʈ����带 �˾Ƴ���.
	m_NodeNum = pQuad->s_NodeNum;//���� �÷��̾���ġ�� Ʈ������ȣ�� �Է�
	ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsCollision(p,radius);//ĳ���Ϳ� ������ (�浹��) ������ �迭�� ��´�.
	
	//cout << "ID: " << m_PlayerInfo->GetClientID() << "=" << "pQuad->s_NodeNum::" << endl;
	//m_NodeNum = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p)->s_NodeNum;//���� �÷��̾���ġ�� Ʈ������ȣ�� �Է�
	m_PlayerInfo->SetNodeNum(m_NodeNum); //�÷��̾���Ϳ��� Ʈ������ȣ�Է�
=======
	p.x = m_PlayerInfo->GetPositionX();
	p.y = m_PlayerInfo->GetPositionZ();
	QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //����Ʈ���� ���� Ʈ����带 �˾Ƴ���.
	m_NodeNum = pQuad->s_NodeNum;//���� �÷��̾���ġ�� Ʈ������ȣ�� �Է�
//	cout << "ID: " << m_PlayerInfo->GetClientID() << "=" << "pQuad->s_NodeNum::" << endl;
	//m_NodeNum = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p)->s_NodeNum;//���� �÷��̾���ġ�� Ʈ������ȣ�� �Է�
	//m_PlayerInfo->SetNodeNum(m_NodeNum); //�÷��̾���Ϳ��� Ʈ������ȣ�Է�
	
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
	m_PlayerInfo->Update(_fDTime); //�÷��̾� ��Ŷ ������
	
}
int sum_X=0;
void CConnectedUser::Player_Init()
{
	CThreadSync Sync;
	int z=100;
	sum_X = sum_X+20;

<<<<<<< HEAD

	m_PlayerInfo->PlayerInit(ServerMGR->PlayerID, sum_X, 100, 200, 100);

}
=======
	m_PlayerInfo->PlayerInit(ServerMGR->PlayerID, sum_X, 100, 200, 100);


}

>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
void CConnectedUser::ComUpdate(float _fDTime)
{
	//CThreadSync Sync;

	//if (m_Object_InfoList[0] != NULL) m_Object_InfoList[0]->ComUpdate(_fDTime);

}