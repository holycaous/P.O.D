#pragma once
#include "stdafx.h"

class CIocpServer : public CIocp
{
public:
	CIocpServer(VOID);
	virtual ~CIocpServer(VOID);

	// 개체를 초기화 하고 시작하기 위한 함수
	BOOL Begin(VOID);

	// 개체를 종료하기 위한 함수
	VOID End(VOID);

	// KeepAlive Thread Callback 함수
	VOID KeepThreadCallback(VOID);

protected:
	// CIocp 상속 가상함수들을 재선언
	VOID OnIoRead(VOID *pObject, DWORD dwDataLength);
	VOID OnIoWrote(VOID *pObject, DWORD dwDataLength);
	VOID OnIoConnected(VOID *pObject);
	VOID OnIoDisconnected(VOID *pObject);

private:
	// 서버의 Listen을 담당할 개체
	CNetworkSession *m_pListen;
	CConnectedUserManager m_oConnectedSessionManager;

	// KeepAlive Thread를 관리하는 핸들값
	HANDLE m_hKeepThread;
	// KeepAlive Thread를 종료할때 사용하는 이벤트
	HANDLE m_hKeepThreadDestroyEvent;

public:
	CConnectedUserManager* GetConnectUserManager(){ return &m_oConnectedSessionManager; }
};