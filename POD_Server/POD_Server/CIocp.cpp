#include "stdafx.h"

CIocp::CIocp(VOID)
{
	mIocpHandle = NULL; // IOCP�� �����ϴ� �ڵ� ��
	mWorkerThreadCount = 0; // ����� WorkerThread ����
	mStartupEventHandle = NULL; // ������ �����ϴ� �̺�Ʈ
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
	// mIocpHandle ���� �ʱ�ȭ
	mIocpHandle = NULL;

	// ���� �ý����� ������ ������
	// ������ �ý��� �������� CPU �������� ���
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	// CPU ������ 2�踸ŭ�� WorkerThread�� ����
	mWorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	// IocpHandle�� ����
	// ���� IOCP �ڵ��� �����̹Ƿ� ��� �Ķ���� ���� 0 �Ǵ� NULL
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// ������ �����ߴٸ� ����
	if (!mIocpHandle)
		return FALSE;

	// ������ �����ϴ� �̺�Ʈ�� ����
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// cpu������ 2�踸ŭ�� workerThread�� �����ؼ� mWorkerThreadVector�� ����
	for (DWORD i = 0; i < mWorkerThreadCount; i++)
	{
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mWorkerThreadVector.push_back(WorkerThread);
		// �ϳ��� �����帶�� ������ �Ϸ�� ������ ���
		WaitForSingleObject(mStartupEventHandle, INFINITE);
	}
	return TRUE;
}

// ��ü ����
BOOL CIocp::End(VOID)
{
	// Iocp�� �̿��ؼ� �� WorkerThread���� ���Ḧ ��û
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)
		// IOCP�� ���������� ��ȣ�� ������ �Լ�
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);

	// �� WorkerThread�� ����Ǳ⸦ ��ٷ� �ڵ��� ����ݴϴ�.
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)
	{
		// �� WorkerThread�� ������ ����� ������ ���
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);

		CloseHandle(mWorkerThreadVector[i]);
	}

	// IOCP �ڵ鵵 ����
	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	// workerThread�� �����ϴ� ���͸� ������
	mWorkerThreadVector.clear();
	// ������ �����ϴ� �̺�Ʈ�� ����

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

// IOCP�� �����̳� ���� �ڵ��� ����ϴ� �Լ�
BOOL CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{

	//���� �ڵ��̳� Ű�� �߸��Ǿ��� ��쿡�� ��ϵ��� ����
	if (!socket || !completionKey)
		return FALSE;

	//Begin���� �����ߴ� m_IocpHandle�� �߰��� ���� �ڵ� �� Ű�� ���
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return FALSE;

	return TRUE;

}

// WorkerThread
VOID CIocp::WorkerThreadCallback(VOID)
{
	// ����, ���� ������ �Ǵ��ϴ� ����
	BOOL Successed = FALSE;
	DWORD NumberofByteTransfered = 0; // �� ����Ʈ�� IO�� �߻��ߴ��� Ȯ��
	VOID *Completionkey = NULL; // Ű ��
	OVERLAPPED * Overlapped = NULL; // Overlapped�� ��
	OVERLAPPED_EX * OverlappedEx = NULL;
	VOID *Object = NULL; // ���� �ڵ鿡 ��ġ�Ǵ� ��ü ������ ��

	while (TRUE)
	{
		// ���� WorkerThread�� ������ �� �ִٰ� �˷��ִ� �̺�Ʈ
		// Begin �Լ������� �� �̺�Ʈ�� �߻��ؾ߸� ���������� ���� ��ƾ���� �Ѿ
		SetEvent(mStartupEventHandle);

		// PostQueueCompletionStatus�� IOCP�� ��ϵ� ���� �� ������ Read, Write ���� �Լ��� ���¸� �޾ƿ��� �Լ�
		Successed = GetQueuedCompletionStatus(
			mIocpHandle, // IOCP �ڵ� ��
			&NumberofByteTransfered, // IO�� �̿�� ������ ũ��
			(LPDWORD)&Completionkey, // �����̳� ������ �ڵ� ��
			&Overlapped, // Read, Write � ���� Overlapped ��
			INFINITE); // ��ȣ�� �߻��� ������ ���������� ��ٷ���.

		// Ű�� NULL�� ��� ���� �̰��� End���� PostQueueCompletionStauts�� Ű�� NULL�� �־��� ������ ����
		// �Ǵ� �ڵ��� ������ �����Ͱ� �߸� �Ѿ�� ��� �ɷ��ִ� ����
		if (!Completionkey)
			return;

		// Overlapped �����͸� �޾� OverlappedEx�� �����͸� Ȯ��
		OverlappedEx = (OVERLAPPED_EX*)Overlapped;
		// �� �κ��� CNetworkSession ���� WSARecv, WSASend ���� �� �� �Ѱ��� OVERLAPPED_EX�� �״�� ����
		// CNetworkSession�� Begin �Լ��� ���� OverlappedEx->object�� this�� �Է����־� ��� ��ü���� Ȯ�� �Ҽ� �ְ� ��

		Object = OverlappedEx->Object;

		// GetQueuedCompletionStatus�� ����� �����̰ų� ���������� IO�� ���� ũ�Ⱑ 0�� ����
		if (!Successed || (Successed && !NumberofByteTransfered))
		{
			// Ŭ���̾�Ʈ�� AcceotEX�� ���ؼ� Accept�� ����
			if (OverlappedEx->IoType == IO_ACCEPT)
				OnIoConnected(Object);
			else // Ŭ���̾�Ʈ�� ������ ����� ���
				OnIoDisconnected(Object);

			// �ٽ� ������ �ʱ�ȭ
			continue;
		}

		// �׸��� AcceptEX���� IO_TYPE�� IO_ACCEPT�� ������ OverlappedEX�� WSARecv���� IO_TYPE�� IO_READ�� ����
		// WSASend���� IO_TYPE�� IO_WRITE�� ������ OverlappedEx�� �־���
		switch (OverlappedEx->IoType)
		{
			// �����͸� �޾��� ���(WSARecv, WSARecvFrom)
		case IO_READ:
			OnIoRead(Object, NumberofByteTransfered);
			break;
			// �����͸� ���� ���(WSASend, WSASendTo)
		case IO_WRITE:
			OnIoWrote(Object, NumberofByteTransfered);
			break;
		}
	}
}

