#include "stdafx.h"

// KeepAlive Thread Callback 함수
DWORD WINAPI KeepThreadCallback(LPVOID pParameter)
{
	CIocpServer *pOwner = (CIocpServer*)pParameter;

	pOwner->KeepThreadCallback();
	
	return 0;
}

VOID CIocpServer::KeepThreadCallback(VOID)
{
	// KeepAlive 신호
	DWORD dwKeepAlive = 0xFFFF;
	while (TRUE)
	{
		// 30초동안 종료 이벤트가 발생하지 않으면 넘어간다
		DWORD dwResult = WaitForSingleObject(m_hKeepThreadDestroyEvent, 30000);

		// 종료 이벤트가 발생했을경우 Thread를 종료
 		if (dwResult == WAIT_OBJECT_0) return;

		// 접속해 있는 모든 Session에 패킷을 전송
		m_oConnectedSessionManager.WriteAll(0x3000000, (BYTE*)&dwKeepAlive, sizeof(DWORD));
	}
}

CIocpServer::CIocpServer(VOID)
{

}

CIocpServer::~CIocpServer(VOID)
{

}

// CIocp의 가상함수들
VOID CIocpServer::OnIoConnected(VOID *pObject)
{  
	// 여기 가상함수에서 넘어온 pObject는 접속을 담당한 개체가 넘어오게 됩니다.
	// 이것을 여기서 클라이언트를 관리할 CConnectedUser으로 형변환을 해 주어 받게 됩니다.
	// 형변환은 reinterpret_cast를 사용합니다.
	CConnectedUser *pConnectedSession = reinterpret_cast<CConnectedUser*>(pObject);

	printf("클라이언트가 접속 %x \n", pConnectedSession);

	ServerMGR->ClientCnt++; //클라이언트 접속 인원 증가
	ServerMGR->UniqueCode++; //유니크코드 증가
	ServerMGR->PlayerID--; //플레이어 아이디 설정 [이유- 커넥트백터가 거꾸로부터 들어감]
	pConnectedSession->SetUserID(ServerMGR->PlayerID);//클라이언트 ID설정
	
	//클라에게 접속이 됬다고 알려준다
	BYTE ID = pConnectedSession->GetUserID();  // 접속 클라이언트의 ID를 가져옴
	pConnectedSession->WritePacket(CCONNET_COMPLETE, &ID, 1); //접속클라이언트의 ID를 클라이언트에게 보내줌


	// 접속한 개체의 IO를 IOCP를 통해서 받기위해서 IOCP에 등록하는 과정을 거치게 된다
	// 여기서 GetSocket을 이용해서 소켓을 등록하고 해당 키는 개체의 포인터를 이용
	if (!CIocp::RegisterSocketToIocp(pConnectedSession->GetSocket(), reinterpret_cast<ULONG_PTR>(pConnectedSession)))
		return;

	// IOCP 초기 받기를 실행해 줍니다. 
	if (!pConnectedSession->InitializeReadForIocp())
	{
		// 만일 실패했을 경우 개체를 재시작해 줍니다.
		pConnectedSession->Restart(m_pListen->GetSocket());
		return;
	}
	pConnectedSession->SetConnected(TRUE);
}

VOID CIocpServer::OnIoDisconnected(VOID *pObject)
{
	// 여기 가상함수에서 넘어온 pObject는 OnIoConnected와 마찬가지로 접속을 담당한 개체가 넘어오게 된다.
	// 이것을 여기서 클라이언트를 관리할 CConnectedUser으로 형변환을 해 주어 받게 된다.
	// 형변환은 reinterpret_cast를 사용
	CConnectedUser *pConnectedUser = reinterpret_cast<CConnectedUser*>(pObject);

	int ID = pConnectedUser->GetUserID();

	printf("%d 유저가 접속 종료하였습니다 \n", ID);

	m_oConnectedSessionManager.WriteAllExcept(SONIODISCONNECT, (BYTE*)&ID, sizeof(int), ID);
	// 접속한 유저 정보 초기화	
	ServerMGR->ClientCnt--; //클라이언트 접속 인원 차감
	ServerMGR->UniqueCode--; //유니크코드 차감
	ServerMGR->PlayerID++; //컨넥트유저 벡터공간을 만들어준다.
	//*클라이언트 접속차감 문제:{1,2,3}유저가있을 때 1번ID클라유저가 나가고  새로클라이언트가 접속하면 ID중복현상 일어남
	//이를 위해 int[]배열로 선언하고 ID를 이용해 삭제하고 삽입을 시킬것.

	pConnectedUser->SetCharSetOk(false);

	// 접속을 종료하였기 때문에 개체를 재시작
	pConnectedUser->Restart(m_pListen->GetSocket());

	pConnectedUser->SetConnected(FALSE);
	pConnectedUser->SetUserStaus(GAME_END);
	
}

VOID CIocpServer::OnIoRead(VOID *pObject, DWORD dataLength)
{
	CConnectedUser* pConnectedUser = reinterpret_cast<CConnectedUser*>(pObject);

	DWORD dwProtocol = 0; //프로토콜
	DWORD dwPacketLength = 0;  //패킷길이
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };  //패킷 버퍼 장소

	//CNetworkSession에서 CPacketSession으로 데이터를 가져옴.
	if (pConnectedUser->ReadPacketForIocp(dataLength)){
		//CPakcetSession에서 패킷을 뽑아냄
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
	// 개체로 사용하기 위해서 형 변환을 해줌
	CConnectedUser *Session = reinterpret_cast<CConnectedUser*>(pObject);

	// 전송 완료가 될 때까지 살려두었던 데이터를 삭제
	Session->WriteComplete();
}

BOOL CIocpServer::Begin(VOID)
{
	// 상속 받은 IOCP 개체를 시작합니다.
	if (!CIocp::Begin()) return FALSE;

	// Listen에 사용할 개체를 생성합니다.
	m_pListen = new CNetworkSession();

	// 개체를 시작합니다.
	if (!m_pListen->Begin())
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		CIocpServer::End();

		return FALSE;
	}

	// TCP로 사용할 것을 선언합니다.
	if (!m_pListen->TcpBind())
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		CIocpServer::End();

		return FALSE;
	}

	// 포트 1820으로 Listen을 하고 최대 10명으로 설정합니다.
	if (!m_pListen->Listen(DEFAULT_PORT, MAX_SESSION))
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		CIocpServer::End();
		return FALSE;
	}

	// IOCP에 Listen 소켓을 등록시켜 줍니다. 그리고 키로는 해당 개체의 포인터를 설정합니다.
	if (!CIocp::RegisterSocketToIocp(m_pListen->GetSocket(), reinterpret_cast<ULONG_PTR>(m_pListen)))
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		CIocpServer::End();

		return FALSE;
	}

	// CConnectedSessionManager를 시작합니다.
	// 시작 함수내에는 CConnectedSession을 생성하고 Accept상태로 만드는 코드가 추가되어 있습니다.
	if (!m_oConnectedSessionManager.Begin(m_pListen->GetSocket()))
	{
		CIocpServer::End();

		return FALSE;
	}

	// KeepAlive 종료 이벤트를 생성합니다.
	m_hKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hKeepThreadDestroyEvent)
	{
		// 실패했을경우 종료합니다.
		CIocpServer::End();
		return FALSE;
	}

	// KeepAlive Thread를 생성합니다.
	m_hKeepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!m_hKeepThread)
	{
		// 생성 실패했을 경우 종료합니다.
		CIocpServer::End();
		return FALSE;
	}

	return TRUE;
}

VOID CIocpServer::End(VOID)
{
	// m_hKeepThread가 NULL이 아니면 KeepAlive Thread를 종료해 줍니다.
	if (m_hKeepThread)
	{
		// 종료 이벤트를 발생시킵니다.
		SetEvent(m_hKeepThreadDestroyEvent);

		// Thread가 종료될 때까지 대기합니다.
		WaitForSingleObject(m_hKeepThread, INFINITE);

		// 핸들값을 닫아줍니다.
		CloseHandle(m_hKeepThread);
		m_hKeepThread = NULL;
	}

	// KeepAlive 종료 이벤트가 NULL이 아니면
	if (m_hKeepThreadDestroyEvent)
	{
		// 이벤트를 닫아줍니다.
		CloseHandle(m_hKeepThreadDestroyEvent);
		m_hKeepThreadDestroyEvent = NULL;
	}

	// IOCP를 종료합니다
	CIocp::End();

	// CConnectedSessionManager를 종료합니다.
	m_oConnectedSessionManager.End();

	// 생성했던 Listen용 개체를 종료하고 삭제
	if (m_pListen)
	{
		m_pListen->End();

		delete m_pListen;
	}
}



