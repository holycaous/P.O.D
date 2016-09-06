#include "stdafx.h"

CEventSelect::CEventSelect(VOID)
{
	mSelectEventHandle = NULL; // 소켓 이벤트를 관리
	mDestroyEventHandle = NULL; // 쓰레드를 종료할 때 사용하는 이벤트
	mSelectThreadHandle = NULL; // 소켓 이벤트를 관리하는 쓰레드 핸들 값
	mStartupEventHandle = NULL; // 소켓 이벤트를 관리하는 쓰레드 시작을 체크하는 이벤트

	mSocket = NULL;	// 사용할 소켓 핸들
}

CEventSelect::~CEventSelect(VOID)
{

}

DWORD WINAPI SelectThreadCallback(LPVOID parameter)
{
	CEventSelect *Owner = (CEventSelect*)parameter;
	Owner->SelectThreadCallback();

	return 0;
}

// 소켓 이벤트를 관리하는 쓰레드
void CEventSelect::SelectThreadCallback(VOID)
{
	// 네트워크 이벤트를 관리하는 변수
	WSANETWORKEVENTS NetworkEvents;
	DWORD EventID = 0;
	DWORD Result = 0;
	// 쓰레드에 직접적으로 영향을 미치는 이벤트는 종료와 네트워크 이벤트
	HANDLE ThreadEvnets[2] = { mDestroyEventHandle, mSelectEventHandle };

	while (TRUE)
	{
		// 쓰레드가 정상적으로 시작되었다는 것을 Begin 함수에 알려줌
		SetEvent(mStartupEventHandle);

		// 여기서는 쓰레드의 시작과 종료를 관리
		// 어떠한 이벤트가 발생할 때까지 계속 대기
		EventID = ::WaitForMultipleObjects(2, ThreadEvnets, FALSE, INFINITE);
		switch (EventID)
		{
			// 쓰레드 종료 이벤트가 발생했을 경우
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
			// 소켓 이벤트가 발생했을 때 받아옴
			// mSelectEventHandle에서 네트워크 이벤트를 뽑아옴
			EventID = WSAEnumNetworkEvents(mSocket, mSelectEventHandle, &NetworkEvents);

			// 만약 EvnetID가 SOCKET_ERROR일 경우 무엇이 잘못된 것이므로 쓰레드를 종료
			// 에러의 내용은 WSAGetLastError 함수를 통해서 확인
			if (EventID == SOCKET_ERROR)
				return;
			else
			{
				// Connet이벤트가 발생했을 경우
				if (NetworkEvents.lNetworkEvents & FD_CONNECT)
				{
					// 가상 함수를 호출
					OnIoConnected();

					if (NetworkEvents.iErrorCode[FD_CONNECT_BIT])
						return;
				}
				// 사용하지 않음
				else if (NetworkEvents.lNetworkEvents & FD_WRITE)
				{
					// ONIoWrote();
					// FD_WRITE를 한게 아닌데 FD_WRITE가 발생되는 3가지 경우
					// 1. 서버랑 접속이 성공했을 때 (클라가 EventSelect 일 경우)
					// 2. 클라이언트가 접속이 성공 된 경우 (서버가 EventSelect 일 경우)
					// 3. 송신 버퍼에 여유가 없어서 WSAWouldBlock이 리턴되어 송신버퍼에 여유가 생겼을때
				}
				// Read 이벤트가 발생했을 경우
				else if (NetworkEvents.lNetworkEvents & FD_READ)
				{
					// 가상 함수를 호출
					OnIoRead();
				}
				//DisConnect 이벤트가 발생했을 경우
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE)
				{
					// 가상 함수를 호출
					OnIoDisconnected();
					// 종료
					return;
				}
			}
			break;
		default:
			return;
		}
	}
}

// 개체 시작
BOOL CEventSelect::Begin(SOCKET socket)
// 시작할 때 소켓 핸들 값을 받아옴, 이것은 CNetworkSession의 GetSocket을 통해 확인 가능
{
	// 소켓 파라미터가 잘못되었을 경우 실행하지 않음
	if (!socket)
		return FALSE;

	// 멤버 변수 소켓이 이미 NULL이 아닐 경우 중복 실행으로 생각하고 실행하지 않음
	if (mSocket)
		return FALSE;

	// 멤버 변수에 입력
	mSocket = socket;

	mSelectEventHandle = WSACreateEvent(); // 소켓 이벤트를 관리할 이벤트를 생성
	if (mSelectEventHandle == WSA_INVALID_EVENT)
	{
		End();

		return FALSE;
	}

	mDestroyEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// 쓰레드 종료 이벤트를 생성
	if (mDestroyEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// 쓰레드 시작 이벤트를 생성
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// Connect, Read, Write, Close 이벤트를 관리하는 것으로 확인
	DWORD Result = WSAEventSelect(socket, mSelectEventHandle, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (Result == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// 소켓 이벤트를 관리할 쓰레드를 생성
	DWORD SelectThreadID = 0;
	mSelectThreadHandle = CreateThread(NULL, 0, ::SelectThreadCallback, this, 0, &SelectThreadID);

	if (!mSelectThreadHandle)
	{
		End();

		return FALSE;
	}

	// 쓰레드가 생성 완료될 때까지 대기
	// 쓰레드가 Wait될 때까지 시간을 벌어줌
	// 쓰레드가 생성되면서 mStartupEventHandle 이벤트가 발생하면 함수가 종료
	WaitForSingleObject(mStartupEventHandle, INFINITE);

	return TRUE;
}

// 개체 종료
BOOL CEventSelect::End(VOID)
{
	// 소켓 멤버가 이미 NULL일 경우 실행하지 않습니다.
	if (!mSocket)
		return FALSE;

	if (mSelectThreadHandle) // 쓰레드가 실행 중일 때 종료
	{
		// 쓰레드를 종료하는 이벤트를 발생
		SetEvent(mDestroyEventHandle);

		// 쓰레드가 완전히 종료될 때까지 기다립니다.
		WaitForSingleObject(mSelectThreadHandle, INFINITE);

		// 종료되었을 경우 쓰레드 핸들을 초기화
		CloseHandle(mSelectThreadHandle);
	}

	// 이벤트 종료
	if (mSelectEventHandle)
		CloseHandle(mSelectEventHandle);

	if (mDestroyEventHandle)
		CloseHandle(mDestroyEventHandle);

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

