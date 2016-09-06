#include "stdafx.h"

// KeepAlive Thread Callback �Լ�
DWORD WINAPI KeepThreadCallback(LPVOID pParameter)
{
	CIocpSever *pOwner = (CIocpSever*)pParameter;

	pOwner->KeepThreadCallback();
	
	return 0;
}

VOID CIocpSever::KeepThreadCallback(VOID)
{
	// KeepAlive ��ȣ
	DWORD dwKeepAlive = 0xFFFF;
	while (TRUE)
	{
		// 30�ʵ��� ���� �̺�Ʈ�� �߻����� ������ �Ѿ��
		DWORD dwResult = WaitForSingleObject(m_hKeepThreadDestroyEvent, 30000);

		// ���� �̺�Ʈ�� �߻�������� Thread�� ����
		if (dwResult == WAIT_OBJECT_0) return;

		// ������ �ִ� ��� Session�� ��Ŷ�� ����
		m_oConnectedSessionManager.WriteAll(0x3000000, (BYTE*)&dwKeepAlive, sizeof(DWORD));
	}
}

CIocpSever::CIocpSever(VOID)
{

}

CIocpSever::~CIocpSever(VOID)
{

}

// CIocp�� �����Լ���
VOID CIocpSever::OnIoConnected(VOID *pObject)
{
	// ���� �����Լ����� �Ѿ�� pObject�� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedUser���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	CConnectedUser *pConnectedSession = reinterpret_cast<CConnectedUser*>(pObject);

	printf("Ŭ���̾�Ʈ�� ���� %x \n", pConnectedSession);

	SGT_OBJECT->ClientCnt++;
	
	
	pConnectedSession->SetUserID(SGT_OBJECT->ClientCnt);

	//Ŭ�󿡰� ������ ��ٰ� �˷��ش�
	BYTE ID = pConnectedSession->GetUserID();
	pConnectedSession->WritePacket(SCONNET_COMPLETE, &ID, 1);

	// ������ ��ü�� IO�� IOCP�� ���ؼ� �ޱ����ؼ� IOCP�� ����ϴ� ������ ��ġ�� �ȴ�
	// ���⼭ GetSocket�� �̿��ؼ� ������ ����ϰ� �ش� Ű�� ��ü�� �����͸� �̿�
	if (!CIocp::RegisterSocketToIocp(pConnectedSession->GetSocket(), reinterpret_cast<ULONG_PTR>(pConnectedSession)))
		return;

	// IOCP �ʱ� �ޱ⸦ ������ �ݴϴ�. 
	if (!pConnectedSession->InitializeReadForIocp())
	{
		// ���� �������� ��� ��ü�� ������� �ݴϴ�.
		pConnectedSession->Restart(m_pListen->GetSocket());
		return;
	}

	pConnectedSession->SetConnected(TRUE);
}

VOID CIocpSever::OnIoDisconnected(VOID *pObject)
{
	// ���� �����Լ����� �Ѿ�� pObject�� OnIoConnected�� ���������� ������ ����� ��ü�� �Ѿ���� �ȴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedUser���� ����ȯ�� �� �־� �ް� �ȴ�.
	// ����ȯ�� reinterpret_cast�� ���
	CConnectedUser *pConnectedUser = reinterpret_cast<CConnectedUser*>(pObject);

	int ID = pConnectedUser->GetUserID();

	printf("%d ������ ���� �����Ͽ����ϴ� \n", ID);

	m_oConnectedSessionManager.WriteAllExcept(SONIODISCONNECT, (BYTE*)&ID, sizeof(int), ID);


	// ������ ���� ���� �ʱ�ȭ	
	//SGT_OBJECT->SGT_Charector--;
		

	pConnectedUser->SetCharSetOk(false);

//	SGT_OBJECT->ClientCnt--; // Ŭ���̾�Ʈ ���� �ϳ� ����



	// ������ �����Ͽ��� ������ ��ü�� �����
	pConnectedUser->Restart(m_pListen->GetSocket());

	pConnectedUser->SetConnected(FALSE);

}

VOID CIocpSever::OnIoRead(VOID *pObject, DWORD dataLength)
{
	CConnectedUser* pConnectedUser = reinterpret_cast<CConnectedUser*>(pObject);

	DWORD dwProtocol = 0, dwPacketLength = 0;

	//��Ŷ�� �����ϴ� ����
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	//CNetworkSession���� CPacketSession���� �����͸� ������.
	if (pConnectedUser->ReadPacketForIocp(dataLength)){
		//CPakcetSession���� ��Ŷ�� �̾Ƴ�
		while (pConnectedUser->GetPacket(dwProtocol, Packet, dataLength))
		{
			//�������ݿ� ���� Switch ��
			switch (dwProtocol)
			{
				//����� ��� ��������
			case CLIENT_START:
			{
								  printf("���� ����\n", pConnectedUser);
								  UINT Cnt = SGT_OBJECT->ClientCnt;
								  // Ŭ���̾�Ʈ ĳ���͸� ������ ��ü ����

								  UINT mCharector = 0;
								  int UserID = 1;

								  PLAYER_INFO* pCharacterList = new PLAYER_INFO[MAX_SESSION];
								  memset(pCharacterList, 0, sizeof(PLAYER_INFO)* MAX_SESSION);

								  int j = 0;
								  for (INT i = MAX_SESSION - 1; i >= 0; --i, ++j){ // ������ ������ ĳ���� ����
									  //--------------------------------------------------------------------------------
									  // ���� ����.
									  //--------------------------------------------------------------------------------
									  if (m_oConnectedSessionManager.GetConnectUser(i)->GetConnected()){
										  // ���� ����ü�� �� �ֱ�
										  pCharacterList[j].Character = m_oConnectedSessionManager.GetConnectUser(i)->GetCharacter(); // ĳ���� ���� �˾ƿ�
										  pCharacterList[j].ClientID = m_oConnectedSessionManager.GetConnectUser(i)->GetUserID();

										 //(*m_oConnectedSessionManager.GetConnectUser(i)->GetObject_InfoList(0))->SetClientID(UserID++);
					  
									  }
									 
								  }
								  m_oConnectedSessionManager.WriteAll(SLODING_START, (BYTE*)pCharacterList, sizeof(PLAYER_INFO)* MAX_SESSION);

								  delete[] pCharacterList;

								  break;
			}

			}

		}

		if (!pConnectedUser->InitializeReadForIocp())
			pConnectedUser->Restart(m_pListen->GetSocket());
	}

}

VOID CIocpSever::OnIoWrote(VOID *pObject, DWORD dataLength)
{
	// ��ü�� ����ϱ� ���ؼ� �� ��ȯ�� ����
	CConnectedUser *Session = reinterpret_cast<CConnectedUser*>(pObject);

	// ���� �Ϸᰡ �� ������ ����ξ��� �����͸� ����
	Session->WriteComplete();
}

BOOL CIocpSever::Begin(VOID)
{
	// ��� ���� IOCP ��ü�� �����մϴ�.
	if (!CIocp::Begin()) return FALSE;

	// Listen�� ����� ��ü�� �����մϴ�.
	m_pListen = new CNetworkSession();

	// ��ü�� �����մϴ�.
	if (!m_pListen->Begin())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpSever::End();

		return FALSE;
	}

	// TCP�� ����� ���� �����մϴ�.
	if (!m_pListen->TcpBind())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpSever::End();

		return FALSE;
	}

	// ��Ʈ 1820���� Listen�� �ϰ� �ִ� 10������ �����մϴ�.
	if (!m_pListen->Listen(DEFAULT_PORT, MAX_SESSION))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpSever::End();
		return FALSE;
	}

	// IOCP�� Listen ������ ��Ͻ��� �ݴϴ�. �׸��� Ű�δ� �ش� ��ü�� �����͸� �����մϴ�.
	if (!CIocp::RegisterSocketToIocp(m_pListen->GetSocket(), reinterpret_cast<ULONG_PTR>(m_pListen)))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpSever::End();

		return FALSE;
	}

	// CConnectedSessionManager�� �����մϴ�.
	// ���� �Լ������� CConnectedSession�� �����ϰ� Accept���·� ����� �ڵ尡 �߰��Ǿ� �ֽ��ϴ�.
	if (!m_oConnectedSessionManager.Begin(m_pListen->GetSocket()))
	{
		CIocpSever::End();

		return FALSE;
	}

	// KeepAlive ���� �̺�Ʈ�� �����մϴ�.
	m_hKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hKeepThreadDestroyEvent)
	{
		// ����������� �����մϴ�.
		CIocpSever::End();
		return FALSE;
	}

	// KeepAlive Thread�� �����մϴ�.
	m_hKeepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!m_hKeepThread)
	{
		// ���� �������� ��� �����մϴ�.
		CIocpSever::End();
		return FALSE;
	}

	return TRUE;
}

VOID CIocpSever::End(VOID)
{
	// m_hKeepThread�� NULL�� �ƴϸ� KeepAlive Thread�� ������ �ݴϴ�.
	if (m_hKeepThread)
	{
		// ���� �̺�Ʈ�� �߻���ŵ�ϴ�.
		SetEvent(m_hKeepThreadDestroyEvent);

		// Thread�� ����� ������ ����մϴ�.
		WaitForSingleObject(m_hKeepThread, INFINITE);

		// �ڵ鰪�� �ݾ��ݴϴ�.
		CloseHandle(m_hKeepThread);
		m_hKeepThread = NULL;
	}

	// KeepAlive ���� �̺�Ʈ�� NULL�� �ƴϸ�
	if (m_hKeepThreadDestroyEvent)
	{
		// �̺�Ʈ�� �ݾ��ݴϴ�.
		CloseHandle(m_hKeepThreadDestroyEvent);
		m_hKeepThreadDestroyEvent = NULL;
	}

	// IOCP�� �����մϴ�
	CIocp::End();

	// CConnectedSessionManager�� �����մϴ�.
	m_oConnectedSessionManager.End();

	// �����ߴ� Listen�� ��ü�� �����ϰ� ����
	if (m_pListen)
	{
		m_pListen->End();

		delete m_pListen;
	}
}



