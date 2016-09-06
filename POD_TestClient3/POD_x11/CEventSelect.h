#pragma once
#include "stdafx.h"

class CEventSelect
{
private:
	HANDLE	mSelectEventHandle;
	HANDLE	mStartupEventHandle;
	HANDLE	mDestroyEventHandle;
	HANDLE	mSelectThreadHandle;
	SOCKET	mSocket;
public:
	CEventSelect(VOID);
	virtual ~CEventSelect(VOID);

	BOOL  Begin(SOCKET socket);
	BOOL  End(VOID);

	VOID  SelectThreadCallback(VOID);

	virtual VOID	OnIoConnected(VOID) = 0;
	virtual VOID	OnIoRead(VOID) = 0;
	virtual VOID	OnIoDisconnected(VOID) = 0;
};