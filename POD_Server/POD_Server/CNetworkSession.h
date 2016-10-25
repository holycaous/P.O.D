#pragma once
#include "stdafx.h"

class CNetworkSession : public CMultiThreadSync < CNetworkSession >
{
public:
	CNetworkSession(void);
	virtual ~CNetworkSession(void);
private:
	OVERLAPPED_EX mAcceptOverlapped; //Accept���� ����ü
	OVERLAPPED_EX mReadOverlapped;   //Read���� ����ü
	OVERLAPPED_EX mWriteOverlapped;  //Write���� ����ü

	BYTE		  mReadBuffer[MAX_BUFFER_LENGTH]; //Read����

	SOCKET		  mSocket; //����
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