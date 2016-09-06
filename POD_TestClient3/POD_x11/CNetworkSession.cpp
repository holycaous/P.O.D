#include "stdafx.h"

// NetworkEngine 생성자
CNetworkSession::CNetworkSession(void)
{
	// Accept Overlapped 구조체
	ZeroMemory(&mAcceptOverlapped, sizeof(mAcceptOverlapped));
	// Read Overlapped 구조체
	ZeroMemory(&mReadOverlapped, sizeof(mReadOverlapped));
	// write Overlapped 구조체
	ZeroMemory(&mWriteOverlapped, sizeof(mWriteOverlapped));
	// Read사용 버퍼
	ZeroMemory(&mReadBuffer, sizeof(mReadBuffer));

	mSocket = NULL; // 소켓 핸들 값을 저장

	mAcceptOverlapped.IoType = IO_ACCEPT;
	mReadOverlapped.IoType = IO_READ;
	mWriteOverlapped.IoType = IO_WRITE;

	mAcceptOverlapped.Object = this;
	mReadOverlapped.Object = this;
	mWriteOverlapped.Object = this;
}
CNetworkSession::~CNetworkSession(void)
{

}
// 개체 시작
BOOL CNetworkSession::Begin(void)
{
	CThreadSync Sync; // 다중 쓰레드 동기화
	if (mSocket) // 이미 실행된 소켓이 있을 경우
		return FALSE;

	// 변수 초기화
	ZeroMemory(mReadBuffer, sizeof(mReadBuffer));

	// 앞으로 계속 사용하게 될 소켓 핸들.
	mSocket = NULL;

	return TRUE;
}

// 개체 종료
BOOL CNetworkSession::End(void)
{
	CThreadSync Sync; // 다중 쓰레드 동기화
	if (!mSocket) // 이미 실행된 소켓이 있을 경우
		return FALSE;

	shutdown(mSocket,SD_BOTH); // 해당 소켓의 READ/WRITE를 멈춥.
	// Shutdown 역할 - 데이터를 보내거나 받는 도중 갑자기 통신 개체가 
	//                 사라져 데이터가 갈 곳이 없는 상태가 되는 것을 방지

	closesocket(mSocket);		 // 소켓 닫기

	mSocket = NULL;

	return TRUE;
}

// TCP로 사용할 경우 호출
BOOL CNetworkSession::TcpBind(void)
{
	CThreadSync Sync; // 동기화 개체

	// mSocket이 NULL이 아닐 경우 이미 소켓이 생성되어 Bind 되어있는 상태이므로 다시 TcpBind 함수를 실행하지 않음.
	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // TCP 형태의 소켓 생성

	int snd_buf = 2000;
	int rcv_buf = 2000;

	setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof(rcv_buf));

	setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf, sizeof(snd_buf));

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	// TCP_NODELAY 옵션 (Nagle 알고리즘 해제)
	BOOL NoDelay = TRUE;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));

	return TRUE;
}

// Listen
BOOL CNetworkSession::Listen(USHORT port, INT backLog)
{
	CThreadSync Sync; // 동기화 개체

	if (port <= 0 || backLog <= 0) // 만약 포트가 0보다 작거나 BackLog가 0보다 작으면 실패
		return FALSE;

	if (!mSocket)
		return FALSE;

	// Listen을 할 정보를 저장하는 변수
	SOCKADDR_IN ListenSocketInfo;

	ListenSocketInfo.sin_family = AF_INET;
	ListenSocketInfo.sin_port = htons(port); // Listen할 포트
	ListenSocketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // Listen할 주소는 전체 주소

	// 위에서 정의한 주소정보로 Bind
	if (bind(mSocket, (struct sockaddr*) &ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// Listen
	if (listen(mSocket, backLog) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// LINGER 관련 옵션 처리 - 현재 보내고 있거나 받는 데이터가 있을 경우 그 데이터를 모두 처리 후 소켓 close
	LINGER Linger;
	Linger.l_onoff = 1;
	Linger.l_linger = 0;

	if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

// Connect - 대용량 서버를 제작할 때 서버간의 통신에서도 이용 가능
BOOL CNetworkSession::Connect(LPSTR address, USHORT port)
// 여기서는 TCHAR 형태로 주소를 받지 않음
{
	CThreadSync sync;  // 동기화 개체
	if (!address || port <= 0) //파라미터 체크
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo; // 접속할 주소의 정보를 저장하는 구조체

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_port = htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(address);

	// 위에서 설정된 주소로 접속을 시도
	// 여기서는 WSAConnect를 이용해서 접속했지만 일반 버클리 소켓 함수를 사용할 경우 connect 함수를 이용해서 접속 가능
	if (WSAConnect(mSocket, (LPSOCKADDR)&RemoteAddressInfo, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		//실패이지만 대기상태라고 결과가 나왔을 경우는 성공으로 인식
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

// Accept
BOOL CNetworkSession::Accept(SOCKET listenSocket)
{
	CThreadSync Sync; // 동기화 개체

	if (!listenSocket)
		return FALSE;

	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // Accpet에 사용할 소켓을 미리 생성

	if (mSocket == INVALID_SOCKET)
	{
		End();

		return FALSE;
	}

	BOOL NoDelay = TRUE;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));

	// AcceptEX를 사용 - 갑자기 많은 접속이 몰릴 경우 실패 방지 하기 위해 사용. 소켓을 미리 만들어두고 Accpet 요청이 있을때 만들어진 소켓 사용
	if (!AcceptEx(listenSocket,
		mSocket,
		mReadBuffer,				// 버퍼는 정의해 주지만
		0,							// 크기를 0으로 잡아서 받지 않음
		sizeof(sockaddr_in)+16,	// 16바이트 더 커야 함
		sizeof(sockaddr_in)+16,	// 16바이트 더 커야 함
		NULL,
		&mAcceptOverlapped.Overlapped))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

SOCKET CNetworkSession::GetSocket(VOID)
{
	CThreadSync Sync;

	return	mSocket;
}

// IOCP를 사용할 때 사용하는 초기 받기
BOOL CNetworkSession::InitializeReadForIocp(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf; // WSABUF buf, len
	DWORD ReadBytes = 0;
	DWORD ReadFlag = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer;	// 멤버로 선언된 ReadBuffer 포인터 넘겨줌
	WsaBuf.len = MAX_BUFFER_LENGTH;		// 최대 버퍼 길이


	// WSARecv 호출
	INT ReturnValue = WSARecv(mSocket, &WsaBuf, // 받은 데어트를 저장할 버퍼
		1,						 // 원형 버퍼를 사용할 때 사용하는 버퍼 개수
		&ReadBytes,                // 동기 소켓에서는 받은 데이터의 크기
		&ReadFlag,                 // 포인터 값 
		&mReadOverlapped.Overlapped, // 사용하는 overlapped 구조체
		NULL                       // CompletionRoutine
		);

	// SOCKET_ERROR 이지만 WSA_IO_PENDING 이거나 WSAEWOULDBLOCK이면 정상적으로 진행
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

// 받은 데이터 가져오기
BOOL CNetworkSession::ReadForIocp(BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	CopyMemory(data, mReadBuffer, dataLength);
	// WSARecv를 통해서 받아온 데이터가 들어있는 mReadBuffer에서 데이터를 복사

	return TRUE;
}

// EventSelect 방식을 사용할 때의 Read
BOOL CNetworkSession::ReadForEventSelect(BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	//	if( !mSocket)
	//		return FALSE:

	WSABUF WsaBuf;
	DWORD ReadBytes = 0; // IOCP에서는 사용하지 않았지만 여기서는 사용
	DWORD ReadFlag = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer; // 버퍼 포인터를 입력
	WsaBuf.len = MAX_BUFFER_LENGTH; // 버퍼의 크기를 입력

	// EventSelect 방식을 이용할 때의 WSARecv는 IOCP일 때의 그것과 트림
	INT ReturnValue = WSARecv(mSocket, &WsaBuf, 1, &ReadBytes, &ReadFlag, &mReadOverlapped.Overlapped, NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	// 받은 내용을 복사
	CopyMemory(data, mReadBuffer, ReadBytes);
	// 받은 데이터 길이 넣어줌
	dataLength = ReadBytes;

	return TRUE;
}

// Write 함수
BOOL CNetworkSession::Write(BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	WSABUF WsaBuf;
	DWORD WriteBytes = 0;
	DWORD WriteFlag = 0;
	WsaBuf.buf = (CHAR*)data; // 보내는 데이터 포인터를 입력함
	WsaBuf.len = dataLength;

	INT ReturnValue = WSASend(mSocket, &WsaBuf, 1, &WriteBytes, WriteFlag, &mWriteOverlapped.Overlapped, NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}