#pragma once
#include "stdafx.h"

class CNetworkSession : public CMultiThreadSync < CNetworkSession >
{
public:
	CNetworkSession(void);
	virtual ~CNetworkSession(void);
private:
	OVERLAPPED_EX mAcceptOverlapped; //Accept관련 구조체
	OVERLAPPED_EX mReadOverlapped;   //Read관련 구조체
	OVERLAPPED_EX mWriteOverlapped;  //Write관련 구조체

	BYTE		  mReadBuffer[MAX_BUFFER_LENGTH]; //Read버퍼

	SOCKET		  mSocket; //소켓
public:
	BOOL		  Begin(void);
	BOOL		  End(void);
	BOOL		  TcpBind(void);
	BOOL		  Listen(USHORT port, INT backLog);
	BOOL		  Connect(LPSTR address, USHORT port);
	BOOL		  Accept(SOCKET listenSocket);
	SOCKET		  GetSocket(VOID);
	BOOL		  InitializeReadForIocp(VOID);
	BOOL	      ReadForIocp(BYTE *data, DWORD &datalength);
	BOOL		  ReadForEventSelect(BYTE *data, DWORD &dataLEngth);
	BOOL		  Write(BYTE *data, DWORD dataLenght);
};