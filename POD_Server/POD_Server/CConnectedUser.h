#pragma once
#include "stdafx.h"

// CPacketSession을 상속받은 CConnectedSession
class CConnectedUser : public CPacketSession
{
public:
	CConnectedUser(VOID);
	virtual ~CConnectedUser(VOID);

	// 개체를 종료하고 다시 Accept상태로 만들어주는 함수
	// CServerIocp에서 OnIoDisconnected가 되었을때 개체를 종료하고 다시 초기화 할때 사용하게 됩니다.
	BOOL Restart(SOCKET hListenSocket);

	void Player_Init(); //플레이어 생성및 초기화, 클라이언트에게 패킷전송.
	void Update(float _fDTime);
	void ComUpdate(float _fDTime);

	// m_bConnected의 값을 설정하고 가져오는 함수
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

	//충돌체크배열 넘버
	/*void SetColisonNum(UINT _CollisonNum) { m_CollisonNum = _CollisonNum; }
	UINT GetColisonNum() { return m_CollisonNum; }
*/
	cPlayer_Info* Get_Playerinfo() { return m_PlayerInfo;}

private:
	// 접속되었는지 판별하는 변수
	BOOL m_bConnected;

	//현재 유저의 씬위치
	int  m_UserStaus;

	//아이디 
	int m_ID;

	UINT m_nCharacter;
	
	UINT m_NodeNum; //현재클라이언트의  플레이어 트리노드 번호

	//UINT m_CollisonNum; //현재클라이언트의 플레이어 트리노드 충돌넘버

	cPlayer_Info* m_PlayerInfo; //플레이어 정보 


	//캐릭터 선택했는지 확인 함수
	BOOL m_bCharSetOk;
};

