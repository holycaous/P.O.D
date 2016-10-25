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

// 접속할 상대 주소와 포트를 파라미터로 입력
BOOL CClientSession::BeginTcp(LPSTR remoteAddress, USHORT remotePort)
{
	// 정상적인 파라미터인지 체크
	if (!remoteAddress || remotePort <= 0)
		return FALSE;
	 
	// 사용할 접속 개체를 초기화
	// 실제로는 CPacketSession의 Begin 함수를 호출하는 것
	if (!mSession.Begin())
	{
		End();

		return FALSE;
	}

	// 접속 개체를 TCP로 선언할 것을 선언함
	if (!mSession.TcpBind())
	{
		End();

		return FALSE;
	}

	// EventSelect 방식으로 해당 소켓의 이벤트를 받아오는 것을 등록함
	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	// 접속할 주소에 접속
	if (!mSession.Connect(remoteAddress, remotePort))
	{
		End();

		return FALSE;
	}

	return TRUE;
}

// 접속을 종료하거나 더 이상 사용하지 않을 때 호출
BOOL CClientSession::End(VOID)
{
	// 연결 개체를 종료
	mSession.End();

	// EventSelect개체를 종료
	CEventSelect::End();

	return TRUE;
}

// TCP 데이터를 전송하는 함수
// 파라미터로 사용할 프로토콜, 패킷, 길이를 넣어줌
BOOL CClientSession::WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	// 데이터 전송 함수를 호출
	if (!mSession.WritePacket(protocol, packet, packetLength))
		return FALSE;

	// 바로 WriteComplete를 해줌
	// 왜냐하면 CEventSelect의 경우 FD_WRITE가 꼭 보내기 성공 이후 발생하지 않을 수도 있기 때문
	if (!mSession.WriteComplete())
		return FALSE;

	return TRUE;
}

// 데이터가 발생했을 때 호출되는 가상 함수
VOID CClientSession::OnIoRead(VOID)
{
	// 패킷을 저장하는 버퍼
	BYTE Packet[MAX_BUFFER_LENGTH] {0, };
	// 패킷의 길이를 저장하는 변수
	DWORD PacketLength = 0;

	// 프로토콜을 저장하는 변수
	DWORD Protocol = 0;

	// NetWorkSession에서 PacketSession으로 데이터를 복사
	if (mSession.ReadPacketForEventSelect()){

		// 패킷을 뽑아서
		while (mSession.GetPacket(Protocol, Packet, PacketLength))
			// 큐에 저장
			mReadPacketQueue.Push(this, Protocol, Packet, PacketLength);

	}
}

VOID CClientSession::OnIoConnected(VOID)
{
}
VOID CClientSession::OnIoDisconnected(VOID)
{

}

// 패킷 한 개를 받아옴
BOOL CClientSession::ReadPacket(DWORD &protocol, BYTE *packet, DWORD &packetLEngth)
{
	VOID *Object = NULL;

	// 큐에서 패킷 한 개를 받아옴
	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLEngth);

}