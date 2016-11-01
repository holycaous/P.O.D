#pragma once
#include "stdafx.h"

class CIocpServer : public CIocp
{
public:
	CIocpServer(VOID);
	virtual ~CIocpServer(VOID);

	// ��ü�� �ʱ�ȭ �ϰ� �����ϱ� ���� �Լ�
	BOOL Begin(VOID);

	// ��ü�� �����ϱ� ���� �Լ�
	VOID End(VOID);

	// KeepAlive Thread Callback �Լ�
	VOID KeepThreadCallback(VOID);

protected:
	// CIocp ��� �����Լ����� �缱��
	VOID OnIoRead(VOID *pObject, DWORD dwDataLength);
	VOID OnIoWrote(VOID *pObject, DWORD dwDataLength);
	VOID OnIoConnected(VOID *pObject);
	VOID OnIoDisconnected(VOID *pObject);

private:
	// ������ Listen�� ����� ��ü
	CNetworkSession *m_pListen;
	CConnectedUserManager m_oConnectedSessionManager;

	// KeepAlive Thread�� �����ϴ� �ڵ鰪
	HANDLE m_hKeepThread;
	// KeepAlive Thread�� �����Ҷ� ����ϴ� �̺�Ʈ
	HANDLE m_hKeepThreadDestroyEvent;

public:
	CConnectedUserManager* GetConnectUserManager(){ return &m_oConnectedSessionManager; }
};