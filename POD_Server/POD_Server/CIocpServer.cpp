#include "stdafx.h"

// KeepAlive Thread Callback �Լ�
DWORD WINAPI KeepThreadCallback(LPVOID pParameter)
{
	CIocpServer *pOwner = (CIocpServer*)pParameter;

	pOwner->KeepThreadCallback();
	
	return 0;
}

VOID CIocpServer::KeepThreadCallback(VOID)
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

CIocpServer::CIocpServer(VOID)
{

}

CIocpServer::~CIocpServer(VOID)
{

}

// CIocp�� �����Լ���
VOID CIocpServer::OnIoConnected(VOID *pObject)
{  
	// ���� �����Լ����� �Ѿ�� pObject�� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedUser���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	CConnectedUser *pConnectedSession = reinterpret_cast<CConnectedUser*>(pObject);

	printf("Ŭ���̾�Ʈ�� ���� %x \n", pConnectedSession);

	ServerMGR->ClientCnt++; //Ŭ���̾�Ʈ ���� �ο� ����
	ServerMGR->UniqueCode++; //����ũ�ڵ� ����
	ServerMGR->PlayerID--; //�÷��̾� ���̵� ���� [����- Ŀ��Ʈ���Ͱ� �Ųٷκ��� ��]
	pConnectedSession->SetUserID(ServerMGR->PlayerID);//Ŭ���̾�Ʈ ID����
	
	//Ŭ�󿡰� ������ ��ٰ� �˷��ش�
	BYTE ID = pConnectedSession->GetUserID();  // ���� Ŭ���̾�Ʈ�� ID�� ������
	pConnectedSession->WritePacket(CCONNET_COMPLETE, &ID, 1); //����Ŭ���̾�Ʈ�� ID�� Ŭ���̾�Ʈ���� ������


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

VOID CIocpServer::OnIoDisconnected(VOID *pObject)
{
	// ���� �����Լ����� �Ѿ�� pObject�� OnIoConnected�� ���������� ������ ����� ��ü�� �Ѿ���� �ȴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedUser���� ����ȯ�� �� �־� �ް� �ȴ�.
	// ����ȯ�� reinterpret_cast�� ���
	CConnectedUser *pConnectedUser = reinterpret_cast<CConnectedUser*>(pObject);

	int ID = pConnectedUser->GetUserID();

	printf("%d ������ ���� �����Ͽ����ϴ� \n", ID);

	m_oConnectedSessionManager.WriteAllExcept(SONIODISCONNECT, (BYTE*)&ID, sizeof(int), ID);
	// ������ ���� ���� �ʱ�ȭ	
	ServerMGR->ClientCnt--; //Ŭ���̾�Ʈ ���� �ο� ����
	ServerMGR->UniqueCode--; //����ũ�ڵ� ����
	ServerMGR->PlayerID++; //����Ʈ���� ���Ͱ����� ������ش�.
	//*Ŭ���̾�Ʈ �������� ����:{1,2,3}���������� �� 1��IDŬ�������� ������  ����Ŭ���̾�Ʈ�� �����ϸ� ID�ߺ����� �Ͼ
	//�̸� ���� int[]�迭�� �����ϰ� ID�� �̿��� �����ϰ� ������ ��ų��.

	pConnectedUser->SetCharSetOk(false);

	// ������ �����Ͽ��� ������ ��ü�� �����
	pConnectedUser->Restart(m_pListen->GetSocket());

	pConnectedUser->SetConnected(FALSE);
	pConnectedUser->SetUserStaus(GAME_END);
	
}

VOID CIocpServer::OnIoRead(VOID *pObject, DWORD dataLength)
{
	CConnectedUser* pConnectedUser = reinterpret_cast<CConnectedUser*>(pObject);

	DWORD dwProtocol = 0; //��������
	DWORD dwPacketLength = 0;  //��Ŷ����
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };  //��Ŷ ���� ���

	//CNetworkSession���� CPacketSession���� �����͸� ������.
	if (pConnectedUser->ReadPacketForIocp(dataLength)){
		//CPakcetSession���� ��Ŷ�� �̾Ƴ�
		while (pConnectedUser->GetPacket(dwProtocol, Packet, dataLength))
		{
			ServerProtocol_Func::g_Protocol_Func[dwProtocol](Packet, dataLength);
		}

		if (!pConnectedUser->InitializeReadForIocp())
			pConnectedUser->Restart(m_pListen->GetSocket());
	}

}

VOID CIocpServer::OnIoWrote(VOID *pObject, DWORD dataLength)
{
	// ��ü�� ����ϱ� ���ؼ� �� ��ȯ�� ����
	CConnectedUser *Session = reinterpret_cast<CConnectedUser*>(pObject);

	// ���� �Ϸᰡ �� ������ ����ξ��� �����͸� ����
	Session->WriteComplete();
}

BOOL CIocpServer::Begin(VOID)
{
	// ��� ���� IOCP ��ü�� �����մϴ�.
	if (!CIocp::Begin()) return FALSE;

	// Listen�� ����� ��ü�� �����մϴ�.
	m_pListen = new CNetworkSession();

	// ��ü�� �����մϴ�.
	if (!m_pListen->Begin())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpServer::End();

		return FALSE;
	}

	// TCP�� ����� ���� �����մϴ�.
	if (!m_pListen->TcpBind())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpServer::End();

		return FALSE;
	}

	// ��Ʈ 1820���� Listen�� �ϰ� �ִ� 10������ �����մϴ�.
	if (!m_pListen->Listen(DEFAULT_PORT, MAX_SESSION))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpServer::End();
		return FALSE;
	}

	// IOCP�� Listen ������ ��Ͻ��� �ݴϴ�. �׸��� Ű�δ� �ش� ��ü�� �����͸� �����մϴ�.
	if (!CIocp::RegisterSocketToIocp(m_pListen->GetSocket(), reinterpret_cast<ULONG_PTR>(m_pListen)))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CIocpServer::End();

		return FALSE;
	}

	// CConnectedSessionManager�� �����մϴ�.
	// ���� �Լ������� CConnectedSession�� �����ϰ� Accept���·� ����� �ڵ尡 �߰��Ǿ� �ֽ��ϴ�.
	if (!m_oConnectedSessionManager.Begin(m_pListen->GetSocket()))
	{
		CIocpServer::End();

		return FALSE;
	}

	// KeepAlive ���� �̺�Ʈ�� �����մϴ�.
	m_hKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hKeepThreadDestroyEvent)
	{
		// ����������� �����մϴ�.
		CIocpServer::End();
		return FALSE;
	}

	// KeepAlive Thread�� �����մϴ�.
	m_hKeepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!m_hKeepThread)
	{
		// ���� �������� ��� �����մϴ�.
		CIocpServer::End();
		return FALSE;
	}

	return TRUE;
}

VOID CIocpServer::End(VOID)
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



