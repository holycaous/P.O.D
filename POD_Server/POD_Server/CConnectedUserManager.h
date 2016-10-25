#pragma once
#include "stdafx.h"

// ConnectedSession을 관리하는 클래스
// 다중 스레드 환경에서 안전하게 사용하기 위해서 CMultiThreadSync를 상속받습니다.
class CConnectedUserManager : public CMultiThreadSync<CConnectedUserManager>
{
public:
	CConnectedUserManager(VOID);
	virtual ~CConnectedUserManager(VOID);

	// 시작하는 함수
	BOOL Begin(SOCKET hListenSocket);
	// 종료하는 함수
	VOID End(VOID);

	// 접속된 모든 ConnectedSession에 패킷을 전송하는 함수
	VOID WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength);
	VOID WriteAllExcept(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID);
	VOID WriteSOLO(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID);
	VOID WriteCulling(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int NodeNum);
	VOID WriteGameClientAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength);


	//플레이어관련 함수


	vector<CConnectedUser*>& GetConnetUserVector(){ CMultiThreadSync sync; return m_vConnectedUser; }
	CConnectedUser* GetConnectUser(int _Index) { CMultiThreadSync sync; return m_vConnectedUser[_Index]; }
	//INT UserNumber(); 접속 인원수 받아오는 함수
private:
	// 생성된 ConnectedSession개체의 포인터를 관리하는 Vector
	std::vector<CConnectedUser*> m_vConnectedUser;
};
