#include "stdafx.h"

CConnectedUserManager::CConnectedUserManager(VOID)
{
}

CConnectedUserManager::~CConnectedUserManager(VOID)
{
}

// CConnectedSessionManager�� �����ϴ� �Լ�
BOOL CConnectedUserManager::Begin(SOCKET hListenSocket)
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// �Ķ���ͷ� Listen��ü�� ������ �޾ƾ� �Ѵ�
	// ���� NULL�� ��� ����
	if (!hListenSocket)
		return FALSE;

	// �ִ� �����ڸ�ŭ�� ��ü�� �̸� ����
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		CConnectedUser *pConnectedUser = new CConnectedUser();
		// ������ ��ü�� ��ü�� �����ϴ� m_vConnectedSessions ���Ϳ� �Է��մϴ�.
		m_vConnectedUser.push_back(pConnectedUser);
	}

	// ������ ��ü���� �ʱ�ȭ�ϰ� Accept���·� �����
	for (DWORD i = 0; i<MAX_SESSION; i++)
	{
		GetConnectUser(i)->SetUserStaus(GAME_END); //���� �������ͽ��� ���� �����ϱ��� ���·� �ٲ۴�.
		// ��ü �ʱ�ȭ�� ������ ��� �����ŵ�ϴ�.
		if (!m_vConnectedUser[i]->Begin())
		{
			CConnectedUserManager::End();
			return FALSE;
		}

		// ��ü�� Accept���·� �����ϴ� ������ ������ ����
		if (!m_vConnectedUser[i]->Accept(hListenSocket))
		{
			CConnectedUserManager::End();
			return FALSE;
		}
	}

	return TRUE;
}

// ConnectedSessionManager�� �����ϴ� �Լ�
VOID CConnectedUserManager::End(VOID)
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// �ִ� �����ڸ�ŭ�� ��ü�� ����
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// ��ü ����
		m_vConnectedUser[i]->End();

		delete m_vConnectedUser[i];
	}

	// �����ϴ� ������ ������ ��� �����ش�
	m_vConnectedUser.clear();
}

// ���ӵ� ��� Session�� �����͸� �����ϴ� �Լ�
VOID CConnectedUserManager::WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength)
{
	// ����ȭ ��ü
	CThreadSync Sync; 

	// ���ӵ� ��� ��ü�� WritePacket�� �� �ݴϴ�.
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// ���ӵǾ� �ִ� ��ü�� ��츸 WritePacket�� �� �ݴϴ�.
		if (m_vConnectedUser[i]->GetConnected())
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength);
	}
}

VOID CConnectedUserManager::WriteAllExcept(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID) // �ڱ� �ڽ� �����ϰ� ����
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// ���ӵ� ��� ��ü�� WritePacket�� �� �ݴϴ�.
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// ���ӵǾ� �ִ� ��ü�� ��츸 WritePacket�� �� �ݴϴ�.
		if (m_vConnectedUser[i]->GetConnected() && (m_vConnectedUser[i]->GetUserID()) != ID){
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength);
		}
	}
}

VOID CConnectedUserManager::WriteSOLO(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int ID) // �ڱ� �ڽ� ���׸� ����
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// ���ӵ� ��� ��ü�� ���� �˻�
	for (DWORD i = 0; i < m_vConnectedUser.size(); i++)
	{
		if (m_vConnectedUser[i]->GetConnected() && (m_vConnectedUser[i]->GetUserID()) == ID)//���ӵ� ��ü�� �ڱ� ID�� ã�´�.
		{
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength); //ã���� ��Ŷ����
		}
	}
}
VOID CConnectedUserManager::WriteCulling(DWORD dwProtocol, BYTE *pData, DWORD dwLength, int NodeNum) // �ڱ��ڽŰ� ���� ����Ʈ�� ���ȿ� �ִ� ������Ʈ���׸� ����
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// ���ӵ� ��� ��ü�� ���� �˻�
	for (DWORD i = 0; i < m_vConnectedUser.size(); i++)
	{
		if (m_vConnectedUser[i]->GetConnected() && (m_vConnectedUser[i]->GetNodeNum()) == NodeNum)//���ӵ� ��ü�� ���� Ʈ����忡 �ִ� ������ ã�´�.
		{
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength); //ã���� ��Ŷ����
		}
	}
}

// �������� ��� Ŭ���̾�Ʈ���� �����͸� �����ϴ� �Լ�
VOID CConnectedUserManager::WriteGameClientAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength)
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// ���ӵ� ��� ��ü�� WritePacket�� �� �ݴϴ�.
	for (DWORD i = 0; i<m_vConnectedUser.size(); i++)
	{
		// ���ӵǾ� �ִ� ��ü�� ��츸 WritePacket�� �� �ݴϴ�.
		if ((m_vConnectedUser[i]->GetConnected() && m_vConnectedUser[i]->GetUserStaus() == GAME_ING))
		{
			m_vConnectedUser[i]->WritePacket(dwProtocol, pData, dwLength);
		}

	}
}
