#include "stdafx.h"

// NetworkEngine ������
CNetworkSession::CNetworkSession(void)
{
	// Accept Overlapped ����ü
	ZeroMemory(&mAcceptOverlapped, sizeof(mAcceptOverlapped));
	// Read Overlapped ����ü
	ZeroMemory(&mReadOverlapped, sizeof(mReadOverlapped));
	// write Overlapped ����ü
	ZeroMemory(&mWriteOverlapped, sizeof(mWriteOverlapped));
	// Read��� ����
	ZeroMemory(&mReadBuffer, sizeof(mReadBuffer));

	mSocket = NULL; // ���� �ڵ� ���� ����

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
// ��ü ����
BOOL CNetworkSession::Begin(void)
{
	CThreadSync Sync; // ���� ������ ����ȭ
	if (mSocket) // �̹� ����� ������ ���� ���
		return FALSE;

	// ���� �ʱ�ȭ
	ZeroMemory(mReadBuffer, sizeof(mReadBuffer));

	// ������ ��� ����ϰ� �� ���� �ڵ�.
	mSocket = NULL;

	return TRUE;
}

// ��ü ����
BOOL CNetworkSession::End(void)
{
	CThreadSync Sync; // ���� ������ ����ȭ
	if (!mSocket) // �̹� ����� ������ ���� ���
		return FALSE;

	shutdown(mSocket,SD_BOTH); // �ش� ������ READ/WRITE�� ����.
	// Shutdown ���� - �����͸� �����ų� �޴� ���� ���ڱ� ��� ��ü�� 
	//                 ����� �����Ͱ� �� ���� ���� ���°� �Ǵ� ���� ����

	closesocket(mSocket);		 // ���� �ݱ�

	mSocket = NULL;

	return TRUE;
}

// TCP�� ����� ��� ȣ��
BOOL CNetworkSession::TcpBind(void)
{
	CThreadSync Sync; // ����ȭ ��ü

	// mSocket�� NULL�� �ƴ� ��� �̹� ������ �����Ǿ� Bind �Ǿ��ִ� �����̹Ƿ� �ٽ� TcpBind �Լ��� �������� ����.
	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // TCP ������ ���� ����

	int snd_buf = 2000;
	int rcv_buf = 2000;

	setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof(rcv_buf));

	setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf, sizeof(snd_buf));

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	// TCP_NODELAY �ɼ� (Nagle �˰��� ����)
	BOOL NoDelay = TRUE;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));

	return TRUE;
}

// Listen
BOOL CNetworkSession::Listen(USHORT port, INT backLog)
{
	CThreadSync Sync; // ����ȭ ��ü

	if (port <= 0 || backLog <= 0) // ���� ��Ʈ�� 0���� �۰ų� BackLog�� 0���� ������ ����
		return FALSE;

	if (!mSocket)
		return FALSE;

	// Listen�� �� ������ �����ϴ� ����
	SOCKADDR_IN ListenSocketInfo;

	ListenSocketInfo.sin_family = AF_INET;
	ListenSocketInfo.sin_port = htons(port); // Listen�� ��Ʈ
	ListenSocketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // Listen�� �ּҴ� ��ü �ּ�

	// ������ ������ �ּ������� Bind
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

	// LINGER ���� �ɼ� ó�� - ���� ������ �ְų� �޴� �����Ͱ� ���� ��� �� �����͸� ��� ó�� �� ���� close
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

// Connect - ��뷮 ������ ������ �� �������� ��ſ����� �̿� ����
BOOL CNetworkSession::Connect(LPSTR address, USHORT port)
// ���⼭�� TCHAR ���·� �ּҸ� ���� ����
{
	CThreadSync sync;  // ����ȭ ��ü
	if (!address || port <= 0) //�Ķ���� üũ
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo; // ������ �ּ��� ������ �����ϴ� ����ü

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_port = htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(address);

	// ������ ������ �ּҷ� ������ �õ�
	// ���⼭�� WSAConnect�� �̿��ؼ� ���������� �Ϲ� ��Ŭ�� ���� �Լ��� ����� ��� connect �Լ��� �̿��ؼ� ���� ����
	if (WSAConnect(mSocket, (LPSOCKADDR)&RemoteAddressInfo, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		//���������� �����¶�� ����� ������ ���� �������� �ν�
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
	CThreadSync Sync; // ����ȭ ��ü

	if (!listenSocket)
		return FALSE;

	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // Accpet�� ����� ������ �̸� ����

	if (mSocket == INVALID_SOCKET)
	{
		End();

		return FALSE;
	}

	BOOL NoDelay = TRUE;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));

	// AcceptEX�� ��� - ���ڱ� ���� ������ ���� ��� ���� ���� �ϱ� ���� ���. ������ �̸� �����ΰ� Accpet ��û�� ������ ������� ���� ���
	if (!AcceptEx(listenSocket,
		mSocket,
		mReadBuffer,				// ���۴� ������ ������
		0,							// ũ�⸦ 0���� ��Ƽ� ���� ����
		sizeof(sockaddr_in)+16,	// 16����Ʈ �� Ŀ�� ��
		sizeof(sockaddr_in)+16,	// 16����Ʈ �� Ŀ�� ��
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

// IOCP�� ����� �� ����ϴ� �ʱ� �ޱ�
BOOL CNetworkSession::InitializeReadForIocp(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf; // WSABUF buf, len
	DWORD ReadBytes = 0;
	DWORD ReadFlag = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer;	// ����� ����� ReadBuffer ������ �Ѱ���
	WsaBuf.len = MAX_BUFFER_LENGTH;		// �ִ� ���� ����


	// WSARecv ȣ��
	INT ReturnValue = WSARecv(mSocket, &WsaBuf, // ���� ����Ʈ�� ������ ����
		1,						 // ���� ���۸� ����� �� ����ϴ� ���� ����
		&ReadBytes,                // ���� ���Ͽ����� ���� �������� ũ��
		&ReadFlag,                 // ������ �� 
		&mReadOverlapped.Overlapped, // ����ϴ� overlapped ����ü
		NULL                       // CompletionRoutine
		);

	// SOCKET_ERROR ������ WSA_IO_PENDING �̰ų� WSAEWOULDBLOCK�̸� ���������� ����
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

// ���� ������ ��������
BOOL CNetworkSession::ReadForIocp(BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	CopyMemory(data, mReadBuffer, dataLength);
	// WSARecv�� ���ؼ� �޾ƿ� �����Ͱ� ����ִ� mReadBuffer���� �����͸� ����

	return TRUE;
}

// EventSelect ����� ����� ���� Read
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
	DWORD ReadBytes = 0; // IOCP������ ������� �ʾ����� ���⼭�� ���
	DWORD ReadFlag = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer; // ���� �����͸� �Է�
	WsaBuf.len = MAX_BUFFER_LENGTH; // ������ ũ�⸦ �Է�

	// EventSelect ����� �̿��� ���� WSARecv�� IOCP�� ���� �װͰ� Ʈ��
	INT ReturnValue = WSARecv(mSocket, &WsaBuf, 1, &ReadBytes, &ReadFlag, &mReadOverlapped.Overlapped, NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	// ���� ������ ����
	CopyMemory(data, mReadBuffer, ReadBytes);
	// ���� ������ ���� �־���
	dataLength = ReadBytes;

	return TRUE;
}

// Write �Լ�
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
	WsaBuf.buf = (CHAR*)data; // ������ ������ �����͸� �Է���
	WsaBuf.len = dataLength;

	INT ReturnValue = WSASend(mSocket, &WsaBuf, 1, &WriteBytes, WriteFlag, &mWriteOverlapped.Overlapped, NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}