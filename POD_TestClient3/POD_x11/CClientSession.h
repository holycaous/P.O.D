#pragma once
#include "stdafx.h"

class CClientSession : public CEventSelect
{
private:
	CPacketSession mSession;
	CCircularQueue mReadPacketQueue;

public:
	CClientSession(VOID);
	virtual ~CClientSession(VOID);
	BOOL	BeginTcp(LPSTR remoteAddress, USHORT remotePort);
	BOOL	End(VOID);

	BOOL	WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength);

	BOOL    ReadPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength);

	BOOL	m_bStart;

	int m_nClientID; //Ŭ���̾�Ʈ ID
	int m_nClientCnt; //���� Ŭ���̾�Ʈ ���� ��

	DWORD m_Protocol;
	BYTE m_Packet[MAX_BUFFER_LENGTH];
	DWORD m_PacketLength;


	virtual VOID	OnIoRead(VOID);
	virtual VOID	OnIoConnected(VOID);
	virtual VOID	OnIoDisconnected(VOID);
};