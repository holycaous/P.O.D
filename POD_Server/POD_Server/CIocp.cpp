#include "stdafx.h"

CIocp::CIocp(VOID)
{
	mIocpHandle = NULL; // IOCP를 관리하는 핸들 값
	mWorkerThreadCount = 0; // 사용할 WorkerThread 개수
	mStartupEventHandle = NULL; // 시작을 관리하는 이벤트
}

CIocp::~CIocp(VOID)
{

}

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	CIocp *Owner = (CIocp*)parameter;
	Owner->WorkerThreadCallback();

	return 0;
}

BOOL CIocp::Begin(VOID)
{
	// mIocpHandle 값의 초기화
	mIocpHandle = NULL;

	// 서버 시스템의 정보를 가져옴
	// 가져온 시스템 정보에서 CPU 정보만을 사용
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	// CPU 개수의 2배만큼의 WorkerThread를 만듬
	mWorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	// IocpHandle을 생성
	// 메인 IOCP 핸들의 생성이므로 모든 파라미터 값이 0 또는 NULL
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// 생성이 실패했다면 종료
	if (!mIocpHandle)
		return FALSE;

	// 시작을 관리하는 이벤트도 생성
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// cpu개수의 2배만큼의 workerThread를 생성해서 mWorkerThreadVector로 관리
	for (DWORD i = 0; i < mWorkerThreadCount; i++)
	{
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mWorkerThreadVector.push_back(WorkerThread);
		// 하나의 쓰레드마다 생성이 완료될 때까지 대기
		WaitForSingleObject(mStartupEventHandle, INFINITE);
	}
	return TRUE;
}

// 개체 종료
BOOL CIocp::End(VOID)
{
	// Iocp를 이용해서 각 WorkerThread마다 종료를 요청
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)
		// IOCP에 강제적으로 신호를 보내는 함수
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);

	// 각 WorkerThread가 종료되기를 기다려 핸들을 담아줍니다.
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)
	{
		// 한 WorkerThread가 완전히 종료될 때까지 대기
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);

		CloseHandle(mWorkerThreadVector[i]);
	}

	// IOCP 핸들도 종료
	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	// workerThread를 관리하는 벡터를 지워줌
	mWorkerThreadVector.clear();
	// 시작을 관리하는 이벤트를 종료

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

// IOCP에 소켓이나 파일 핸들을 등록하는 함수
BOOL CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{

	//소켓 핸들이나 키가 잘못되었을 경우에는 등록되지 않음
	if (!socket || !completionKey)
		return FALSE;

	//Begin에서 생성했던 m_IocpHandle에 추가로 소켓 핸들 및 키를 등록
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return FALSE;

	return TRUE;

}

// WorkerThread
VOID CIocp::WorkerThreadCallback(VOID)
{
	// 성공, 실패 유무를 판단하는 변수
	BOOL Successed = FALSE;
	DWORD NumberofByteTransfered = 0; // 몇 바이트의 IO가 발생했는지 확인
	VOID *Completionkey = NULL; // 키 값
	OVERLAPPED * Overlapped = NULL; // Overlapped의 값
	OVERLAPPED_EX * OverlappedEx = NULL;
	VOID *Object = NULL; // 소켓 핸들에 매치되는 개체 포인터 값

	while (TRUE)
	{
		// 이제 WorkerThread를 시작할 수 있다고 알려주는 이벤트
		// Begin 함수에서는 이 이벤트가 발생해야만 정상적으로 다음 루틴으로 넘어감
		SetEvent(mStartupEventHandle);

		// PostQueueCompletionStatus나 IOCP에 등록된 소켓 및 파일의 Read, Write 관련 함수의 상태를 받아오는 함수
		Successed = GetQueuedCompletionStatus(
			mIocpHandle, // IOCP 핸들 값
			&NumberofByteTransfered, // IO에 이용된 데이터 크기
			(LPDWORD)&Completionkey, // 소켓이나 파일의 핸들 값
			&Overlapped, // Read, Write 등에 사용된 Overlapped 값
			INFINITE); // 신호가 발생될 때까지 무제한으로 기다려줌.

		// 키가 NULL일 경우 종료 이것은 End에서 PostQueueCompletionStauts의 키를 NULL로 넣어준 것으로 동작
		// 또는 코딩의 문제로 데이터가 잘못 넘어올 경우 걸려주는 역할
		if (!Completionkey)
			return;

		// Overlapped 포인터를 받아 OverlappedEx의 포인터를 확인
		OverlappedEx = (OVERLAPPED_EX*)Overlapped;
		// 이 부분은 CNetworkSession 에서 WSARecv, WSASend 등을 할 때 넘겨준 OVERLAPPED_EX를 그대로 받음
		// CNetworkSession의 Begin 함수를 보면 OverlappedEx->object에 this를 입력해주어 어느 개체인지 확인 할수 있게 함

		Object = OverlappedEx->Object;

		// GetQueuedCompletionStatus의 결과가 실패이거나 성공이지만 IO에 사용된 크기가 0일 경우는
		if (!Successed || (Successed && !NumberofByteTransfered))
		{
			// 클라이언트가 AcceotEX를 통해서 Accept한 경우와
			if (OverlappedEx->IoType == IO_ACCEPT)
				OnIoConnected(Object);
			else // 클라이언트가 접속이 종료된 경우
				OnIoDisconnected(Object);

			// 다시 루프를 초기화
			continue;
		}

		// 그리고 AcceptEX에는 IO_TYPE를 IO_ACCEPT로 설정한 OverlappedEX를 WSARecv에는 IO_TYPE를 IO_READ로 설정
		// WSASend에는 IO_TYPE을 IO_WRITE로 설정한 OverlappedEx를 넣어줌
		switch (OverlappedEx->IoType)
		{
			// 데이터를 받았을 경우(WSARecv, WSARecvFrom)
		case IO_READ:
			OnIoRead(Object, NumberofByteTransfered);
			break;
			// 데이터를 보낼 경우(WSASend, WSASendTo)
		case IO_WRITE:
			OnIoWrote(Object, NumberofByteTransfered);
			break;
		}
	}
}

