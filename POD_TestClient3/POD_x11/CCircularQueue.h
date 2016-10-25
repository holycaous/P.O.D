#pragma once
#include "stdafx.h"


typedef struct _QUEUE_DATA
{
	VOID	*Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;



class CCircularQueue : public CMultiThreadSync < CCircularQueue >
{
private:
	QUEUE_DATA		mQueue[MAX_QUEUE_LENGTH]; // 실제 큐에 들어가는 데이터가 저장되는 메모리 영역
	DWORD			mQueueHead; // HEAD 
	DWORD			mQueueTail; // TAIL

public:
	CCircularQueue(VOID);
	~CCircularQueue(VOID);

public:
	BOOL    Begin(VOID);
	BOOL    End(VOID);

	BYTE*	Push(VOID *object, BYTE *data, DWORD dataLength);
	BYTE*	Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength);

	BOOL    Pop(VOID);
	BOOL	Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength);
};