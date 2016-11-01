#include "stdafx.h"

CClientSession::CClientSession()
{
	m_bStart = FALSE;
	m_nClientID = 0;

	m_Protocol = 0;
	ZeroMemory(m_Packet, sizeof(BYTE)* MAX_BUFFER_LENGTH);
	m_PacketLength = 0;
}

CClientSession::~CClientSession()
{

}

// ������ ��� �ּҿ� ��Ʈ�� �Ķ���ͷ� �Է�
BOOL CClientSession::BeginTcp(LPSTR remoteAddress, USHORT remotePort)
{
	// �������� �Ķ�������� üũ
	if (!remoteAddress || remotePort <= 0)
		return FALSE;
	 
	// ����� ���� ��ü�� �ʱ�ȭ
	// �����δ� CPacketSession�� Begin �Լ��� ȣ���ϴ� ��
	if (!mSession.Begin())
	{
		End();

		return FALSE;
	}

	// ���� ��ü�� TCP�� ������ ���� ������
	if (!mSession.TcpBind())
	{
		End();

		return FALSE;
	}

	// EventSelect ������� �ش� ������ �̺�Ʈ�� �޾ƿ��� ���� �����
	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	// ������ �ּҿ� ����
	if (!mSession.Connect(remoteAddress, remotePort))
	{
		End();

		return FALSE;
	}

	return TRUE;
}

// ������ �����ϰų� �� �̻� ������� ���� �� ȣ��
BOOL CClientSession::End(VOID)
{
	// ���� ��ü�� ����
	mSession.End();

	// EventSelect��ü�� ����
	CEventSelect::End();

	return TRUE;
}

// TCP �����͸� �����ϴ� �Լ�
// �Ķ���ͷ� ����� ��������, ��Ŷ, ���̸� �־���
BOOL CClientSession::WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	// ������ ���� �Լ��� ȣ��
	if (!mSession.WritePacket(protocol, packet, packetLength))
		return FALSE;

	// �ٷ� WriteComplete�� ����
	// �ֳ��ϸ� CEventSelect�� ��� FD_WRITE�� �� ������ ���� ���� �߻����� ���� ���� �ֱ� ����
	if (!mSession.WriteComplete())
		return FALSE;

	return TRUE;
}

// �����Ͱ� �߻����� �� ȣ��Ǵ� ���� �Լ�
VOID CClientSession::OnIoRead(VOID)
{
	// ��Ŷ�� �����ϴ� ����
	BYTE Packet[MAX_BUFFER_LENGTH] {0, };
	// ��Ŷ�� ���̸� �����ϴ� ����
	DWORD PacketLength = 0;

	// ���������� �����ϴ� ����
	DWORD Protocol = 0;

	// NetWorkSession���� PacketSession���� �����͸� ����
	if (mSession.ReadPacketForEventSelect()){

		// ��Ŷ�� �̾Ƽ�
		while (mSession.GetPacket(Protocol, Packet, PacketLength))
			// ť�� ����
			mReadPacketQueue.Push(this, Protocol, Packet, PacketLength);

	}
}

VOID CClientSession::OnIoConnected(VOID)
{
}
VOID CClientSession::OnIoDisconnected(VOID)
{

}

// ��Ŷ �� ���� �޾ƿ�
BOOL CClientSession::ReadPacket(DWORD &protocol, BYTE *packet, DWORD &packetLEngth)
{
	VOID *Object = NULL;

	// ť���� ��Ŷ �� ���� �޾ƿ�
	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLEngth);

}