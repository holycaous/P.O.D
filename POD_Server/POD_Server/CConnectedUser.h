#pragma once
#include "stdafx.h"

// CPacketSession�� ��ӹ��� CConnectedSession
class CConnectedUser : public CPacketSession
{
public:
	CConnectedUser(VOID);
	virtual ~CConnectedUser(VOID);

	// ��ü�� �����ϰ� �ٽ� Accept���·� ������ִ� �Լ�
	// CServerIocp���� OnIoDisconnected�� �Ǿ����� ��ü�� �����ϰ� �ٽ� �ʱ�ȭ �Ҷ� ����ϰ� �˴ϴ�.
	BOOL Restart(SOCKET hListenSocket);

	void Player_Init(); //�÷��̾� ������ �ʱ�ȭ, Ŭ���̾�Ʈ���� ��Ŷ����.
	void Update(float _fDTime);
	void ComUpdate(float _fDTime);

	// m_bConnected�� ���� �����ϰ� �������� �Լ�
	VOID SetConnected(BOOL bConnected) { CThreadSync Sync; m_bConnected = bConnected; }
	BOOL GetConnected(VOID) { CThreadSync Sync; return m_bConnected; }

	
	VOID SetUserStaus(int _UserStaus) { CThreadSync Sync; m_UserStaus = _UserStaus; }
	int  GetUserStaus(VOID) { CThreadSync Sync; return m_UserStaus; }

	VOID SetUserID(int _id){ CThreadSync Sync; m_ID = _id; }
	int  GetUserID(VOID){ CThreadSync Sync; return m_ID; }

	
	VOID SetCharacter(int _index) { CThreadSync Sync; m_nCharacter = _index; }
	int GetCharacter(VOID) { CThreadSync Sync; return m_nCharacter; }

	VOID SetCharSetOk(BOOL bCharSetOk) { CThreadSync Sync; m_bCharSetOk = bCharSetOk; }
	BOOL GetCharSetOk(VOID) { CThreadSync Sync; return m_bCharSetOk; }

	void SetNodeNum(UINT _NodeNum) { m_NodeNum = _NodeNum; }
	UINT GetNodeNum() { return m_NodeNum; }

	//�浹üũ�迭 �ѹ�
	/*void SetColisonNum(UINT _CollisonNum) { m_CollisonNum = _CollisonNum; }
	UINT GetColisonNum() { return m_CollisonNum; }
*/
	cPlayer_Info* Get_Playerinfo() { return m_PlayerInfo;}

private:
	// ���ӵǾ����� �Ǻ��ϴ� ����
	BOOL m_bConnected;

	//���� ������ ����ġ
	int  m_UserStaus;

	//���̵� 
	int m_ID;

	UINT m_nCharacter;
	
	UINT m_NodeNum; //����Ŭ���̾�Ʈ��  �÷��̾� Ʈ����� ��ȣ

	//UINT m_CollisonNum; //����Ŭ���̾�Ʈ�� �÷��̾� Ʈ����� �浹�ѹ�

	cPlayer_Info* m_PlayerInfo; //�÷��̾� ���� 


	//ĳ���� �����ߴ��� Ȯ�� �Լ�
	BOOL m_bCharSetOk;
};

