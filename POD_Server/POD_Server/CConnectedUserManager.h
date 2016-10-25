#pragma once
#include "stdafx.h"

// ConnectedSession�� �����ϴ� Ŭ����
// ���� ������ ȯ�濡�� �����ϰ� ����ϱ� ���ؼ� CMultiThreadSync�� ��ӹ޽��ϴ�.
class CConnectedUserManager : public CMultiThreadSync<CConnectedUserManager>
{
public:
	CConnectedUserManager(VOID);
	virtual ~CConnectedUserManager(VOID);

	// �����ϴ� �Լ�
	BOOL Begin(SOCKET hListenSocket);
	// �����ϴ� �Լ�
	VOID End(VOID);

	// ���ӵ� ��� ConnectedSession�� ��Ŷ�� �����ϴ� �Լ�
	VOID WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength);
	VOID WriteAllExcept(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID);
	VOID WriteSOLO(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID);
	VOID WriteCulling(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int NodeNum);
	VOID WriteGameClientAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength);


	//�÷��̾���� �Լ�


	vector<CConnectedUser*>& GetConnetUserVector(){ CMultiThreadSync sync; return m_vConnectedUser; }
	CConnectedUser* GetConnectUser(int _Index) { CMultiThreadSync sync; return m_vConnectedUser[_Index]; }
	//INT UserNumber(); ���� �ο��� �޾ƿ��� �Լ�
private:
	// ������ ConnectedSession��ü�� �����͸� �����ϴ� Vector
	std::vector<CConnectedUser*> m_vConnectedUser;
};
