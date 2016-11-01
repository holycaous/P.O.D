#pragma once
#include "stdafx.h"
class CIocp
{
private:
	HANDLE    mIocpHandle;
	DWORD     mWorkerThreadCount;
	vector<HANDLE> mWorkerThreadVector;
	HANDLE    mStartupEventHandle;
protected:
	virtual VOID  OnIoRead(VOID *object, DWORD dataLength) = 0;
	virtual VOID  OnIoWrote(VOID *object, DWORD dataLength) = 0;
	virtual VOID  OnIoConnected(VOID *object) = 0;
	virtual VOID  OnIoDisconnected(VOID *object) = 0;
public:
	CIocp(VOID);
	virtual ~CIocp(VOID);
	BOOL  Begin(VOID);
	BOOL  End(VOID);
	BOOL  RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);
	VOID  WorkerThreadCallback(VOID);

};