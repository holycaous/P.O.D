#include "stdafx.h"

CEventSelect::CEventSelect(VOID)
{
	mSelectEventHandle = NULL; // ���� �̺�Ʈ�� ����
	mDestroyEventHandle = NULL; // �����带 ������ �� ����ϴ� �̺�Ʈ
	mSelectThreadHandle = NULL; // ���� �̺�Ʈ�� �����ϴ� ������ �ڵ� ��
	mStartupEventHandle = NULL; // ���� �̺�Ʈ�� �����ϴ� ������ ������ üũ�ϴ� �̺�Ʈ

	mSocket = NULL;	// ����� ���� �ڵ�
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

// ���� �̺�Ʈ�� �����ϴ� ������
void CEventSelect::SelectThreadCallback(VOID)
{
	// ��Ʈ��ũ �̺�Ʈ�� �����ϴ� ����
	WSANETWORKEVENTS NetworkEvents;
	DWORD EventID = 0;
	DWORD Result = 0;
	// �����忡 ���������� ������ ��ġ�� �̺�Ʈ�� ����� ��Ʈ��ũ �̺�Ʈ
	HANDLE ThreadEvnets[2] = { mDestroyEventHandle, mSelectEventHandle };

	while (TRUE)
	{
		// �����尡 ���������� ���۵Ǿ��ٴ� ���� Begin �Լ��� �˷���
		SetEvent(mStartupEventHandle);

		// ���⼭�� �������� ���۰� ���Ḧ ����
		// ��� �̺�Ʈ�� �߻��� ������ ��� ���
		EventID = ::WaitForMultipleObjects(2, ThreadEvnets, FALSE, INFINITE);
		switch (EventID)
		{
			// ������ ���� �̺�Ʈ�� �߻����� ���
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
			// ���� �̺�Ʈ�� �߻����� �� �޾ƿ�
			// mSelectEventHandle���� ��Ʈ��ũ �̺�Ʈ�� �̾ƿ�
			EventID = WSAEnumNetworkEvents(mSocket, mSelectEventHandle, &NetworkEvents);

			// ���� EvnetID�� SOCKET_ERROR�� ��� ������ �߸��� ���̹Ƿ� �����带 ����
			// ������ ������ WSAGetLastError �Լ��� ���ؼ� Ȯ��
			if (EventID == SOCKET_ERROR)
				return;
			else
			{
				// Connet�̺�Ʈ�� �߻����� ���
				if (NetworkEvents.lNetworkEvents & FD_CONNECT)
				{
					// ���� �Լ��� ȣ��
					OnIoConnected();

					if (NetworkEvents.iErrorCode[FD_CONNECT_BIT])
						return;
				}
				// ������� ����
				else if (NetworkEvents.lNetworkEvents & FD_WRITE)
				{
					// ONIoWrote();
					// FD_WRITE�� �Ѱ� �ƴѵ� FD_WRITE�� �߻��Ǵ� 3���� ���
					// 1. ������ ������ �������� �� (Ŭ�� EventSelect �� ���)
					// 2. Ŭ���̾�Ʈ�� ������ ���� �� ��� (������ EventSelect �� ���)
					// 3. �۽� ���ۿ� ������ ��� WSAWouldBlock�� ���ϵǾ� �۽Ź��ۿ� ������ ��������
				}
				// Read �̺�Ʈ�� �߻����� ���
				else if (NetworkEvents.lNetworkEvents & FD_READ)
				{
					// ���� �Լ��� ȣ��
					OnIoRead();
				}
				//DisConnect �̺�Ʈ�� �߻����� ���
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE)
				{
					// ���� �Լ��� ȣ��
					OnIoDisconnected();
					// ����
					return;
				}
			}
			break;
		default:
			return;
		}
	}
}

// ��ü ����
BOOL CEventSelect::Begin(SOCKET socket)
// ������ �� ���� �ڵ� ���� �޾ƿ�, �̰��� CNetworkSession�� GetSocket�� ���� Ȯ�� ����
{
	// ���� �Ķ���Ͱ� �߸��Ǿ��� ��� �������� ����
	if (!socket)
		return FALSE;

	// ��� ���� ������ �̹� NULL�� �ƴ� ��� �ߺ� �������� �����ϰ� �������� ����
	if (mSocket)
		return FALSE;

	// ��� ������ �Է�
	mSocket = socket;

	mSelectEventHandle = WSACreateEvent(); // ���� �̺�Ʈ�� ������ �̺�Ʈ�� ����
	if (mSelectEventHandle == WSA_INVALID_EVENT)
	{
		End();

		return FALSE;
	}

	mDestroyEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// ������ ���� �̺�Ʈ�� ����
	if (mDestroyEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// ������ ���� �̺�Ʈ�� ����
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// Connect, Read, Write, Close �̺�Ʈ�� �����ϴ� ������ Ȯ��
	DWORD Result = WSAEventSelect(socket, mSelectEventHandle, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (Result == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// ���� �̺�Ʈ�� ������ �����带 ����
	DWORD SelectThreadID = 0;
	mSelectThreadHandle = CreateThread(NULL, 0, ::SelectThreadCallback, this, 0, &SelectThreadID);

	if (!mSelectThreadHandle)
	{
		End();

		return FALSE;
	}

	// �����尡 ���� �Ϸ�� ������ ���
	// �����尡 Wait�� ������ �ð��� ������
	// �����尡 �����Ǹ鼭 mStartupEventHandle �̺�Ʈ�� �߻��ϸ� �Լ��� ����
	WaitForSingleObject(mStartupEventHandle, INFINITE);

	return TRUE;
}

// ��ü ����
BOOL CEventSelect::End(VOID)
{
	// ���� ����� �̹� NULL�� ��� �������� �ʽ��ϴ�.
	if (!mSocket)
		return FALSE;

	if (mSelectThreadHandle) // �����尡 ���� ���� �� ����
	{
		// �����带 �����ϴ� �̺�Ʈ�� �߻�
		SetEvent(mDestroyEventHandle);

		// �����尡 ������ ����� ������ ��ٸ��ϴ�.
		WaitForSingleObject(mSelectThreadHandle, INFINITE);

		// ����Ǿ��� ��� ������ �ڵ��� �ʱ�ȭ
		CloseHandle(mSelectThreadHandle);
	}

	// �̺�Ʈ ����
	if (mSelectEventHandle)
		CloseHandle(mSelectEventHandle);

	if (mDestroyEventHandle)
		CloseHandle(mDestroyEventHandle);

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

