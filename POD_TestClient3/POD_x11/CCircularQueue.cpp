#include "stdafx.h"

CCircularQueue::CCircularQueue(VOID)
{
	ZeroMemory(mQueue, sizeof(mQueue));
	mQueueHead = 0;
	mQueueTail = 0;
}


CCircularQueue::~CCircularQueue(VOID)
{
}


BOOL CCircularQueue::Begin(VOID)
{
	ZeroMemory(mQueue, sizeof(mQueue));
	mQueueHead = 0;
	mQueueTail = 0;

	return TRUE;
}

BOOL CCircularQueue::End(VOID)
{
	return TRUE;
}

BYTE* CCircularQueue::Push(VOID *object, BYTE *data, DWORD dataLength) // 데이터 입력
{
	CThreadSync Sync;

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH; // 새로운 데이터를 입력할 위치저장

	if (TempTail == mQueueHead) // HEAD와 TAIL이 같을 경우(큐에 데이터가 가득 찼을때)
		return FALSE;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;

	CopyMemory(mQueue[TempTail].Data, data, dataLength); // 데이터를 입력
	mQueueTail = TempTail; // 사용한 TempTail의 값을 실제 TAIL 값이 들어있는 mQueueTail에 입력

	return mQueue[TempTail].Data;
}

BYTE* CCircularQueue::Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].Protocol = protocol;

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;



	return mQueue[TempTail].Data;
}

BOOL CCircularQueue::Pop(VOID) // 데이터 출력
{
	CThreadSync Sync;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object = mQueue[TempHead].Object;
	protocol = mQueue[TempHead].Protocol;

	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;


	return TRUE;
}
